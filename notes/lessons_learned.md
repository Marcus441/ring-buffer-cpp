# Lessons Learned

## Dynamically Allocated Arrays

- `new int[n]` vs `new int[n]{}`          
  - `new int[n]` will not initialize the elements and `new int[n]{}` will.
- `delete[]` does two things — and one thing it does NOT do to the pointer
  - `delete[]` will release the memory of the array to the allocator 
  - `delete[]` will also call any destructors in the array if present
  - `delete[]` does not set the ptr to `nullptr`
- the one-arrow invariant: 0 owners = leak, 2 owners = double free, 1 = ok
- dangling vs null:
  - `delete[]` leaves behind a dangling pointer. It is still mapped, still
    readable garbage (allocator kept the pages), which is why use-after-free
    "works" instead of crashing.
  - `if (p)` cannot detect if the pointer is dangling as the pointer is
    still there, its just pointing to freed memory.
  - `delete[]` is safe to be called on a `nullptr`, this comes up in move
    semantics.
- p = q copies the address, not the block
- heap overflow: writing past `new int[2]`
  - Every test stays green: no bounds checking exists in C++, and the store 
    lands in still-mapped process memory, so nothing traps and no assertion 
    reads the clobbered bytes.
  - Memory corruption would appear in whatever code touches the clobbered bytes.
  - -fsanitize=address catches it on first write as it places poisoned red zones
    around allocations to detect the cause of corruption.
- UB is a promise to the COMPILER: UB cannot happen. 
  - guard before % (my Put): zero-capacity paths exit early so the compiler's 
    `capacity != 0` inference is correct.
  - guard after %: inference already holds at the guard therefore the condition is 
    provably false, the guard is then deleted as dead code by the optimizer, then 
    the zero-capacity caller reaches the UB unfenced.

## The Importance of Rule of Three

- The rule of three consists of:
  - Destructor, Copy Constructor, Copy Assignment Operator 
- Rule of Zero applies when every member manages its own resource (`vector`,
`unique_ptr`, `string`). Rule of Three/Five applies only when you hold a 
raw owning handle. The rule tracks who does the managing, not whether memory 
exists.
- The compiler-generated destructor destroys members, and destroying a raw pointer frees nothing, therefore the block is orphaned at scope exit which leaks memory.
- If default copy constructor exists, we would have a double free.
  - to avoid this we need to deeply copy the memory from a, so we need to allocate
    a new block for b to use, and copy the values from a into that block of memory
```cpp
int main() {
  RingBuffer a{10};
  // a's ptr to memory exists in memory here
  RingBuffer b{a}; // when copied, the pointer to the dynamic memory in b 
                   // is just reseated to where a's memory is
                   // a and b are pointing to the same location in memory

  return 0;
} // when both go out of scope, the destructor on both a and b are run,  
  // and since a and b are pointing to the same block, double free 
```
- If the default copy assignment operator exists, we would have a memory leak,
  and a double free
```cpp
int main() {
  // here a and b own two distinct blocks of memory
  RingBuffer a{10};
  RingBuffer b{20};
  b = a; // with the default copy assignment operator, b's pointer to its 
         // existing memory will be shallowly reseated to a's memory (b's old 
         // block is now orphaned and has now leaked)
         
  return 0;
} // when they both go out of scope, a and b will be pointing to the same block,
  // their destructors will both be run on their memory, which also leads to a 
  // double free
```

## Ring Buffer Mechanics

- Fundamentally, a ring buffer is a queue data structure with a fixed capacity,
  a read index, and a write index.
- Uses FIFO
  - If an element is removed, it is the oldest element in the queue
- When an element is removed, the read index is incremented
  - Removed elements are still in the buffer, they are just waiting to be
    overwritten by newer elements.
- If it is full and a write is performed, the oldest data is overwritten
  - on full write, either reject or overwrite oldest (lossy/freshest data wins;
    my choice).
- The key logic around the ring buffer read and write index is the wrap around 
  mechanism
  - If the index is equal to capacity, we need to wrap to 0, hence when a 
    ring buffer cursor is incremented, we use modulo arithmetic 
    `(idx + 1) % capacity`
- The tracked size of the ring buffer must not be negative, or greater than the 
  capacity.
  - In the put logic, size is only incremented if not `Full()`.
  - Size is always decremented on `Get()`, this is only possible due to the early
    return on empty (so that size does not go negative).
- When full, readIdx and writeIdx are both equal and are pointing to the oldest 
  element. Overwriting when full advances both in lockstep with one another. So
  the equality and readIdx's claim on the oldest survivor is preserved through 
  every overwrite. 
  - I originally wrote a bug where readIdx would not be incremented, which made 
    it point to the newest element. 

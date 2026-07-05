# Suggested Reading — Ring Buffer Checkpoint (No STL)

## Priority 1 — Dynamic Memory (Ch. 19)
- [19.1 Dynamic memory allocation with new and delete](https://www.learncpp.com/cpp-tutorial/dynamic-memory-allocation-with-new-and-delete/)
  - the scalar mechanism, before jumping to arrays.
- [19.2 Dynamically allocating arrays](https://www.learncpp.com/cpp-tutorial/dynamically-allocating-arrays/)
  - directly answers `int* val_buf = new int[capacity]` and the matching `delete[]`.
- [19.3 Destructors](https://www.learncpp.com/cpp-tutorial/destructors/)
  - shows that for a class managing raw dynamic memory, you must explicitly `delete[]` it in a hand-written destructor, since the compiler-generated one only destroys the pointer variable, not the memory it points to.

## Priority 2 
- [14.9 Introduction to constructors](https://www.learncpp.com/cpp-tutorial/introduction-to-constructors/)
- [14.14 Introduction to the copy constructor](https://www.learncpp.com/cpp-tutorial/introduction-to-the-copy-constructor/)
  - why the compiler's default copy is dangerous once a class owns a raw pointer.
- [15.4 Introduction to destructors](https://www.learncpp.com/cpp-tutorial/introduction-to-destructors/)
  - class-specific companion to 19.3.


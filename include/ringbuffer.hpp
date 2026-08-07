#pragma once

#include <cstddef>

class RingBuffer {
 private:
  int capacity{};
  int size{};
  int writeIdx{};
  int readIdx{};
  int* val_buf;

 public:
  explicit RingBuffer(int capacity) : capacity{capacity}, val_buf{new int[capacity]{}} {}
  ~RingBuffer() { delete[] val_buf; }
  RingBuffer(const RingBuffer& rb)
      : capacity{rb.capacity},
        size{rb.size},
        writeIdx{rb.writeIdx},
        readIdx{rb.readIdx},
        val_buf{new int[rb.capacity]} {
    for (size_t i = 0; i < static_cast<size_t>(rb.capacity); ++i) {
      val_buf[i] = rb.val_buf[i];
    }
  }
  // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
  RingBuffer& operator=(this RingBuffer& self, const RingBuffer& other) {
    // allocate-before-delete ordering is
    // self-assignment-safe by construction
    int* new_buf = new int[other.capacity];
    for (size_t i = 0; i < static_cast<size_t>(other.capacity); ++i) {
      new_buf[i] = other.val_buf[i];
    }
    delete[] self.val_buf;
    self.val_buf = new_buf;
    self.capacity = other.capacity;
    self.size = other.size;
    self.writeIdx = other.writeIdx;
    self.readIdx = other.readIdx;
    return self;
  }

  int Size() const { return size; }
  int Capacity() const { return capacity; }
  bool Full() const { return size == capacity; }
  bool Empty() const { return size == 0; }

  bool Put(int item) {
    if (capacity == 0) return false;
    if (Full()) readIdx = (readIdx + 1) % capacity;
    val_buf[writeIdx] = item;
    writeIdx = (writeIdx + 1) % capacity;

    if (!Full()) {
      size++;
    }
    return true;
  }

  bool Get(int& item) {
    if (Empty()) {
      return false;
    }
    item = val_buf[readIdx];
    readIdx = (readIdx + 1) % capacity;
    size--;
    return true;
  }
};

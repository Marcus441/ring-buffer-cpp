#pragma once

class RingBuffer {
 private:
  int capacity{};
  int size{};
  int writeIdx{};
  int readIdx{};
  int *val_buf;

 public:
  RingBuffer(int capacity) : capacity{capacity}, val_buf{new int[capacity]{}} {}
  ~RingBuffer() { delete[] val_buf; }

  int Size() const { return size; }
  int Capacity() const { return capacity; }
  bool Full() const { return size == capacity; };
  bool Empty() const { return size == 0; };

  bool Put(int item) {
    if (Full()) {
      return false;
    }
    val_buf[writeIdx] = item;
    writeIdx = (writeIdx + 1) % capacity;
    size++;
    return true;
  }

  bool Get(int &item) {
    if (Empty()) {
      return false;
    }
    item = val_buf[readIdx];
    readIdx = (readIdx + 1) % capacity;
    size--;
    return true;
  }
};

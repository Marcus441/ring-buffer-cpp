#include <iostream>

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
  bool Full() const { return ((writeIdx + 1) % capacity == readIdx); };
  bool Empty() const { return (readIdx == writeIdx); };
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
    item = std::move(val_buf[readIdx]);
    readIdx = (readIdx + 1) % capacity;
    size--;
    return true;
  }
};

int main() {
  RingBuffer rb(10);
  std::cout << "ringbuffer of size: " << rb.Capacity() << " created" << '\n';
  rb.Put(10);
  std::cout << "putting 10" << '\n';
  rb.Put(10);
  std::cout << "putting 10" << '\n';
  int value{};
  rb.Get(value);
  std::cout << "getting value..." << " got " << value << '\n';
  return 0;
}

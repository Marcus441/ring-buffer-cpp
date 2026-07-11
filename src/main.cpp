#include <iostream>

#include "ringbuffer.hpp"

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

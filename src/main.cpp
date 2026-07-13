#include <iostream>

#include "ringbuffer.hpp"

int main() {
  RingBuffer rb(10);
  RingBuffer rb_2{rb};
  return 0;
}

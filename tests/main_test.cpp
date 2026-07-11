#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ringbuffer.hpp"

TEST(RingBufferTest, PutIncreasesSizeAndIsNotEmpty) {
  RingBuffer rb(10);
  rb.Put(42);
  EXPECT_FALSE(rb.Empty());
}

TEST(RingBufferTest, GetReturnsInOrder) {
  RingBuffer rb(3);
  rb.Put(1);
  rb.Put(2);
  int value{};
  rb.Get(value);
  int value2{};
  rb.Get(value2);
  EXPECT_EQ(value, 1);
  EXPECT_EQ(value2, 2);
}

TEST(RingBufferTest, EmptyAfterDrained) {
  int value{};
  RingBuffer rb(2);
  rb.Put(1);
  rb.Get(value);
  EXPECT_TRUE(rb.Empty());
}

TEST(RingBufferTest, WrapAroundOverwritesOldest) {
  RingBuffer rb(2);
  rb.Put(1);
  rb.Put(2);
  rb.Put(3);
  int value{};
  rb.Get(value);
  EXPECT_EQ(value, 2);
}

TEST(RingBufferTest, GetOnEmptyReturnsFalse) {
  int value{};
  RingBuffer rb(1);
  EXPECT_FALSE(rb.Get(value));
}

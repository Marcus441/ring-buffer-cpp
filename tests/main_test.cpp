#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ringbuffer.hpp"

TEST(RingBufferTest, PutIncreasesSizeAndIsNotEmpty) {
  RingBuffer rb(10);
  EXPECT_TRUE(rb.Put(42));
  EXPECT_FALSE(rb.Empty());
}

TEST(RingBufferTest, GetReturnsInOrder) {
  RingBuffer rb(3);
  ASSERT_TRUE(rb.Put(1));
  ASSERT_TRUE(rb.Put(2));
  int value{};
  ASSERT_TRUE(rb.Get(value));
  int value2{};
  ASSERT_TRUE(rb.Get(value2));
  EXPECT_EQ(value, 1);
  EXPECT_EQ(value2, 2);
}

TEST(RingBufferTest, EmptyAfterDrained) {
  int value{};
  RingBuffer rb(2);
  ASSERT_TRUE(rb.Put(1));
  ASSERT_TRUE(rb.Get(value));
  EXPECT_TRUE(rb.Empty());
}

TEST(RingBufferTest, WrapAroundOverwritesOldest) {
  RingBuffer rb(2);
  ASSERT_TRUE(rb.Put(1));
  ASSERT_TRUE(rb.Put(2));
  EXPECT_TRUE(rb.Put(3));
  int value{};
  ASSERT_TRUE(rb.Get(value));
  EXPECT_EQ(value, 2);
}

TEST(RingBufferTest, StaysFullAfterOverwrite) {
  RingBuffer rb(2);
  ASSERT_TRUE(rb.Put(1));
  ASSERT_TRUE(rb.Put(2));
  ASSERT_TRUE(rb.Put(3));
  EXPECT_TRUE(rb.Full());
  EXPECT_FALSE(rb.Empty());
}

TEST(RingBufferTest, GetOnEmptyReturnsFalse) {
  int value{};
  RingBuffer rb(1);
  EXPECT_FALSE(rb.Get(value));
}

TEST(RingBufferTest, ZeroCapacityPutReturnsFalse) {
  RingBuffer rb(0);
  EXPECT_FALSE(rb.Put(1));
}

TEST(RingBufferTest, ZeroCapacityIsEmptyAndFull) {
  RingBuffer rb(0);
  EXPECT_TRUE(rb.Empty());
  EXPECT_TRUE(rb.Full());
}

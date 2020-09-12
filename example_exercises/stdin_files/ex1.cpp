#include "ex1.h"
RECORD_TEST(StdInTest, 1, "get_int() should return 5", "5", {
  int actual = get_int();
  RecordProperty("actual", actual);
  EXPECT_EQ(5, actual);
})

RECORD_TEST(StdInTest, 2, "get_int() should return 10", "10", {
  int actual = get_int();
  RecordProperty("actual", actual);
  EXPECT_EQ(10, actual);
})

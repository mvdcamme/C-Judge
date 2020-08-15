#include "ex1.h"
namespace {
RECORD_TEST(StdInTest, 3, "get_int() should return 5", "42", {
  int actual = get_int();
  RecordProperty("actual", actual);
  EXPECT_EQ(42, actual);
})

RECORD_TEST(StdInTest, 4, "get_int() should return 3", "3", {
  int actual = get_int();
  RecordProperty("actual", actual);
  EXPECT_EQ(3, actual);
})

RECORD_TEST(StdInTest, 5, "get_int() should return 4", "4", {
  int actual = get_int();
  RecordProperty("actual", actual);
  EXPECT_EQ(4, actual);
})
}

#include "ex1.h"
namespace {

RECORD_TEST(FactorialTest, Negative1, "fac(-5)", "1", {
	int actual = fac(-5);
	RecordProperty("actual", actual);
	EXPECT_EQ(1, actual);
})

RECORD_TEST(FactorialTest, Negative2, "fac(-1)", "1", {
  int actual = fac(-1);
  RecordProperty("actual", actual);
  EXPECT_EQ(1, actual);
})
RECORD_TEST(FactorialTest, Negative3, "fac(-10)", "1", {
	int actual = fac(-10);
	RecordProperty("actual", actual);
	EXPECT_GT(actual, 0);
})
// Tests factorial of 0.
RECORD_TEST(FactorialTest, Zero1, "fac(0)", "1", {
  int actual = fac(0);
  RecordProperty("actual", actual);
  EXPECT_EQ(1, actual);
})
// Tests factorial of positive numbers.
RECORD_TEST(FactorialTest, Positive1, "fac(1)", "1", {
  int actual = fac(1);
  RecordProperty("actual", actual);
  EXPECT_EQ(1, actual);
})
// Tests factorial of positive numbers.
RECORD_TEST(FactorialTest, Positive2, "fac(2)", "2", {
  int actual = fac(2);
  RecordProperty("actual", actual);
  EXPECT_EQ(2, actual);
})
// Tests factorial of positive numbers.
RECORD_TEST(FactorialTest, Positive3, "fac(3)", "6", {
  int actual = fac(3);
  RecordProperty("actual", actual);
  EXPECT_EQ(6, actual);
})
// Tests factorial of positive numbers.
RECORD_TEST(FactorialTest, Positive4, "fac(8)", "40320", {
  int actual = fac(8);
  RecordProperty("actual", actual);
  EXPECT_EQ(40320, actual);
})
}

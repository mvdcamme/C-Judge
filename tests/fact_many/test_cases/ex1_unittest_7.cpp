#include "ex1.h"
RECORD_TEST(FactorialTest, Positive3, "fac(3)", "6", {
  int actual = fac(3);
  RecordProperty("actual", actual);
  EXPECT_EQ(6, actual);
})

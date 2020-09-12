#include "ex1.h"
RECORD_TEST(FactorialTest, Zero1, "fac(0)", "1", {
  int actual = fac(0);
  RecordProperty("actual", actual);
  EXPECT_EQ(1, actual);
})

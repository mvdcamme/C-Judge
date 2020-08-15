#include "ex1.h"
namespace {
RECORD_TEST(FactorialTest, Positive4, "fac(8)", "40320", {
  int actual = fac(8);
  RecordProperty("actual", actual);
  EXPECT_EQ(40320, actual);
})
}

#include "ex1.h"
namespace {
RECORD_TEST(FactorialTest, Positive2, "fac(2)", "2", {
  int actual = fac(2);
  RecordProperty("actual", actual);
  EXPECT_EQ(2, actual);
})
}

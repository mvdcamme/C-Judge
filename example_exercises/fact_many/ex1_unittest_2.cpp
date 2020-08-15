#include "ex1.h"
namespace {
RECORD_TEST(FactorialTest, Negative2, "fac(-1)", "1", {
  int actual = fac(-1);
  RecordProperty("actual", actual);
  EXPECT_EQ(1, actual);
})
}

#include "ex1.h"
namespace {
RECORD_TEST(FactorialTest, Negative3, "fac(-10)", "1", {
	int actual = fac(-10);
	RecordProperty("actual", actual);
	EXPECT_GT(actual, 0);
})
}

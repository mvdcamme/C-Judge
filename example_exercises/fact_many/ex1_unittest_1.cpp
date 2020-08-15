#include "ex1.h"
namespace {
RECORD_TEST(FactorialTest, Negative1, "fac(-5)", "1", {
	int actual = fac(-5);
	RecordProperty("actual", actual);
	EXPECT_EQ(1, actual);
})
}

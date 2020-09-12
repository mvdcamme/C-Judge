#include "ex1.h"
RECORD_TEST(CountCTest, 1, "count_c(\"file_1\")", "1", {
  int actual = count_c("file_1");
  RecordProperty("actual", actual);
  EXPECT_EQ(1, actual);
})

RECORD_TEST(CountCTest, 2, "count_c(\"file_2\")", "16", {
  int actual = count_c("file_2");
  RecordProperty("actual", actual);
  EXPECT_EQ(16, actual);
})

RECORD_TEST(CountCTest, 3, "count_c(\"file_3\")", "0", {
  int actual = count_c("file_3");
  RecordProperty("actual", actual);
  EXPECT_EQ(0, actual);
})

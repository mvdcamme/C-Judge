#include "ex1.h"
namespace {
RECORD_TEST(StdInTest, 1, "print_hello_world() should print \"hello world\"", "\"hello world\"", {
  testing::internal::CaptureStdout();
  print_hello_world();
  std::string actual = testing::internal::GetCapturedStdout();
  RecordProperty("actual", actual);
  EXPECT_EQ("hello world 42", actual);
})
}

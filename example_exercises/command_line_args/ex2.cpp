#include "ex1.h"
namespace {
int g_argc;
char ** g_argv;

RECORD_TEST(CommandLineArgTest, 2, "main(10 X 7)", "70", {
  int actual = __submission_main__(g_argc, g_argv);
  RecordProperty("actual", actual);
  EXPECT_EQ(70, actual);
})
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
	g_argc = argc;
	g_argv = argv;
    return RUN_ALL_TESTS();
}

#include "ex1.h"

int g_argc;
char ** g_argv;

RECORD_TEST(CommandLineArgTest, 1, "main(1 + 2)", "3", {
  int actual = __submission_main__(g_argc, g_argv);
  RecordProperty("actual", actual);
  EXPECT_EQ(3, actual);
})


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
	g_argc = argc;
	g_argv = argv;
    return RUN_ALL_TESTS();
}

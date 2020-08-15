#include "ex1.h"
namespace {

RECORD_TEST(CountNodesTest, 1, "should return 3", "3", {
  Tnode *root = NULL;
  root = insert(root, 1);
  root = insert(root, 2);
  root = insert(root, 3);
  int actual = count_nodes(root);
  RecordProperty("actual", actual);
  EXPECT_EQ(3, actual);
})

RECORD_TEST(CountNodesTest, 2, "should return 5", "5", {
  Tnode *root = NULL;
  root = insert(root, 1);
  root = insert(root, 2);
  root = insert(root, 3);
  root = insert(root, 4);
  root = insert(root, 5);
  int actual = count_nodes(root);
  RecordProperty("actual", actual);
  EXPECT_EQ(5, actual);
})
}

#include <stdlib.h>

#include "binary_tree.h"

int count_nodes(Tnode *tree) {
	if (tree == NULL) {
		return 0;
	} else {
		int nr_of_childs_left = count_nodes(tree->left);
		int nr_of_childs_right = count_nodes(tree->right);
		return nr_of_childs_left + nr_of_childs_right + 1;
		
	}
}

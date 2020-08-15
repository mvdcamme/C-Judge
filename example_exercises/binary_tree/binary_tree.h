#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdlib.h>

typedef struct tnode {
	int value;
	struct tnode *left, *right;
} Tnode;

enum tside {RIGHT,LEFT};

Tnode *readinttree();
void printinttree(Tnode *);
Tnode *lookup(Tnode *, int);
Tnode *insert(Tnode *, int);
int get_int(); /* as before */
Tnode *insert_v2(Tnode *tree, int i);

#endif  // BINARY_TREE_H

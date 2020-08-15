//
//  main.c
//  binaryTree
//
//  Created by Elisa Gonzalez Boix on 07/11/16.
//  Copyright © 2016 Elisa Gonzalez Boix. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"

//int main(void)
//{
//	int i;
//	Tnode *temp, *root;
//
//	root = readinttree();
//	// input: 4 7 2 8 6 3 1 9 5 0
//	printinttree(root);
//	printf("\n");
//
//	i = get_int();
//	// input: 7
//	temp = lookup(root,i);
//	if (temp == NULL)
//		printf("%d not in tree\n",i);
//	else {
//		printf("%d found at ",i);
//		printinttree(temp);
//		printf("\n");
//	}
//	return 0;
//}

Tnode *readinttree()
{
	int i;
	Tnode *root = NULL;
	while ((i = get_int()) != 0){
		root = insert(root,i);
	}
	return root;
}

void printinttree(Tnode *tree)
{
	if (tree == NULL)
		return;
	printf("(");
	printinttree(tree->left);
	printf(" %d ", tree->value);
	printinttree(tree->right);
	printf(")");
}

Tnode *lookup(Tnode *tree, int i)
{
	while (tree){
		if (tree->value == i)
			return tree;
		if (tree->value < i)
			tree = tree->right;
		else
			tree = tree->left;
	}
	return NULL;
}

Tnode *insert(Tnode *tree, int i)
{
	Tnode *neww, *rest, *prev;
	int side;
	
	neww = (Tnode *) malloc(sizeof(Tnode));
	neww->value = i;
	neww->left = NULL; neww->right = NULL;
	
	if (tree == NULL)
		return neww;
	
	rest = tree;
	while (rest){
		if (rest->value == i){
			free(neww);
			return tree;
		}
		if (rest->value < i) {
			side = RIGHT; prev = rest;
			rest = rest->right;
		} else {
			side = LEFT; prev = rest;
			rest = rest->left;
		}
	}
	if (side)
		prev->left = neww;
	else prev->right = neww;
	
	return tree;
}
Tnode *insert_v2(Tnode *tree, int i)
{
	Tnode *neww, **rest;
	
	neww = (Tnode *) malloc(sizeof(Tnode));
	neww->value = i; neww->left = NULL; neww->right = NULL;
	
	if (tree == NULL)
		return (neww);
	
	rest = &tree;
	while (*rest) {
		if ((*rest)->value == i){
			free(neww);
			return tree;
		}
		if ((*rest)->value < i)
			rest = &((*rest)->right);
		else
			rest = &((*rest)->left);
	}
	*rest = neww;
	return tree;
	
}

int get_int()
{
	int i;
	scanf("%d", &i);
	return i;
}

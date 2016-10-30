

#include <stdbool.h>

#ifndef DCOMPILER_BINARYSEARCHTREE_H
#define DCOMPILER_BINARYSEARCHTREE_H

#endif //DCOMPILER_BINARYSEARCHTREE_H


typedef void *tree;

void create(tree *t);

void delete(tree *t);

bool isEmpty(tree t);

char* getData(tree t);

int getComponentByLex(tree t, char* d);

tree left(tree t);

tree right(tree t);

void insert(tree *t, char* d, int comp);

void printTree(tree t);

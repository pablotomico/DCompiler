

#include <stdbool.h>

#ifndef DCOMPILER_BINARYSEARCHTREE_H
#define DCOMPILER_BINARYSEARCHTREE_H

#endif //DCOMPILER_BINARYSEARCHTREE_H


typedef void *tree;
typedef char *data;

void create(tree *t);

void delete(tree *t);

bool isEmpty(tree t);

data getData(tree t);

int getComponentByLex(tree t, data d);

tree left(tree t);

tree right(tree t);

void insert(tree *t, data d, int comp);

void printTree(tree t);

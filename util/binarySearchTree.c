#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char *data;
typedef struct node *tree;
struct node {
    char *data;
    int component;
    tree left;
    tree right;
};


void create(tree *t) {
    *t = NULL;
}

bool isEmpty(tree t) {
    return t == NULL;
}

data getData(tree t) {
    return t->data;
}

int getComponentByLex(tree t, data d) {

    int res = strcmp(d, t->data);
    if (res < 0) {
        if (t->left != NULL) {
            return getComponentByLex(t->left, d);
        } else {
            return 0;
        }
    } else if (res > 0) {
        if (t->right != NULL) {
            return getComponentByLex(t->right, d);
        } else {
            return 0;
        }
    } else {
        return t->component;
    }
}


void delete(tree *t) {
    tree tAux;
    tAux = *t;
    if (!isEmpty(tAux)) {
        if (tAux->left != NULL)
            delete(&(tAux->left));
        if (tAux->right != NULL)
            delete(&(tAux->right));
        free(tAux);
        *t = NULL;
    }
}

tree left(tree t) {
    return t->left;
}

tree right(tree t) {
    return t->right;
}

void insert(tree *t, data d, int comp) {

    tree tAux;
    tAux = (tree) malloc(sizeof(struct node));
    tAux->data = d;
    tAux->component = comp;
    tAux->left = NULL;
    tAux->right = NULL;

    if (isEmpty(*t)) {
        *t = tAux;
    } else {
        int res = strcmp(d, (*t)->data);

        if (res < 0) {
            insert(&((*t)->left), d, comp);
        } else {
            insert(&((*t)->right), d, comp);
        }
    }
}

void printTree(tree t) {

    if (t->left != NULL) {
        printTree(t->left);
    }

    if (t->data != NULL) {
        printf("%d -> %s\n", t->component, t->data);
    }

    if (t->right != NULL) {
        printTree(t->right);
    }
}



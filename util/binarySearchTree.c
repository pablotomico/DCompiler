#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


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

char* getData(tree t) {
    return t->data;
}

/*
 * Devuelve el componente léxico asociado al lexema de entrada
 * Si no lo encuentra devuelve 0
 */
int getComponentByLex(tree t, char* d) {

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
        if(tAux->data != NULL)
            free(tAux->data);
        free(tAux);
    }
}

tree left(tree t) {
    return t->left;
}

tree right(tree t) {
    return t->right;
}

void insert(tree *t, char* d, int comp) {

    tree tAux;
    char* data = NULL;

    if (isEmpty(*t)) {
        tAux = (tree) malloc(sizeof(struct node));
        data = (char*)malloc(sizeof(char)* (strlen(d) + 1));
        strcpy(data, d);
        data[strlen(d)] = '\0';
        tAux->data = data;
        tAux->component = comp;
        tAux->left = NULL;
        tAux->right = NULL;
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


/*
 * Recorremos el árbol imprimiendo los elementos de la forma (izquierda-raiz-derecha)
 * para mantener el orden y así que el resultado esté ordenado alfabéticamente
 */
void printTree(tree t) {

    if (t->left != NULL) {
        printTree(t->left);
    }

    if (t->data != NULL) {
        printf("      %3d  --> %s\n", t->component, t->data);
    }

    if (t->right != NULL) {
        printTree(t->right);
    }
}






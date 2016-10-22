#include "symbolTable.h"

tree tree;

void initSymbolTable(){
    create(&tree);
}

void addLexem(char *lex, int comp){
    insert(&tree, lex, comp);
}

int getComponentByLexem(char *lex){
    return getComponentByLex(tree, lex);
}

void deleteSymbolTable(){
    delete(&tree);
}

void printSymbolTable(){
    printTree(tree);
}
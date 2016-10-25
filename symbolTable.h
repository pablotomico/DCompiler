#ifndef DCOMPILER_SYMBOLTABLE_H
#define DCOMPILER_SYMBOLTABLE_H

#endif //DCOMPILER_SYMBOLTABLE_H

#include "util/binarySearchTree.h"

typedef void *symbolTable;

void createSymbolTable(symbolTable *st);

void initSymbolTable(symbolTable *st);

void addLexem(symbolTable *st, char *lex, int comp);

int getComponentByLexem(symbolTable st, char *lex);

void deleteSymbolTable(symbolTable *st);

void printSymbolTable(symbolTable st);
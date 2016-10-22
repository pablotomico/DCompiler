#ifndef DCOMPILER_SYMBOLTABLE_H
#define DCOMPILER_SYMBOLTABLE_H

#endif //DCOMPILER_SYMBOLTABLE_H

#import "util/binarySearchTree.h"

void initSymbolTable();

void addLexem(char *lex, int comp);

int getComponentByLexem(char *lex);

void deleteSymbolTable();
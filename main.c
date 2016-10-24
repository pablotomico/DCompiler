#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "scanner.h"
#include "lexAnalyzer.h"


int main(int argc, char **argv) {
    lexAnalyzer la;
    initLexAnalyzer(&la);



    symbolTable st;

    initSymbolTable(&st);

    printSymbolTable(st);
    deleteSymbolTable(&st);

    return EXIT_SUCCESS;
}
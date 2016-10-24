#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "scanner.h"
#include "lexAnalyzer.h"


int main(int argc, char **argv) {
    lexAnalyzer la;
    initLexAnalyzer(&la);
    int i = 0;
    lex l;
    for (i = 0; i < 5; i++) {
        l = getNextComponent(&la);
        printf("%d -> [%s]\n", l.component, l.lexem);


    }


/*    symbolTable st;

    initSymbolTable(&st);

    printSymbolTable(st);
    deleteSymbolTable(&st);
*/


    deleteLexAnalyzer(&la);
    return EXIT_SUCCESS;
}
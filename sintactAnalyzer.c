#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "scanner.h"
#include "lexAnalyzer.h"


int main(int argc, char **argv) {
    lexAnalyzer la;
    initLexAnalyzer(&la);
    lex l;

    /*int i;
    for(i=0;i<5;i++){
        l = getNextComponent(&la);
        printf("%3d -> [%s]\n", l.component, l.lexem);
    }*/

    do{
        l = getNextComponent(&la);
        printf("%3d -> %s\n", l.component, l.lexem);
    }while(l.component != EOF);

    printSymbols(&la);
/*    symbolTable st;

    initSymbolTable(&st);

    printSymbolTable(st);
    deleteSymbolTable(&st);
*/


    deleteLexAnalyzer(&la);
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "scanner.h"

#define FILE_PATH "/home/tomico/Documentos/GitProjects/DCompiler/regression.d"


int main(int argc, char **argv) {
    /*
    initScanner(FILE_PATH);

    char mander;
    int i = 0;
    do {
        mander = getNextChar();

        if (i < 10)
            printf("Leo %c\n", mander);

        if (i == 6) {
            printf("Reconozco [%s]\n", getLexem());
            returnChar();
        }
        i++;
    } while (mander != EOF);

    deleteScanner();
     */

    symbolTable st;

    initSymbolTable(&st);
    //addLexem("asd", 1);
    //printf("getComponentByLexem(\"dsa\") = %d\n" , getComponentByLexem("dsa"));





    printSymbolTable(st);
    deleteSymbolTable(&st);

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include "util/binarySearchTree.h"
#include "scanner.h"

#define FILE_PATH "/home/tomico/ClionProjects/DCompiler/regression.d"


int main(int argc, char **argv) {

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

    return EXIT_SUCCESS;
}
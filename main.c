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
        //printf("%c", mander);

        if(i%10 == 0){
            //printf ("%s\n", getLexem());
            getLexem();
        }
        i++;
    }while(mander !=  EOF);


    deleteScanner();




/*
    tree myTree;
    data myData = "ak47";
    data myOtherData = "bbsgratis";


    create(&myTree);

    insert(&myTree, myData, 1);

    insert(&myTree, myOtherData, 2);

    printTree(myTree);

    delete(&myTree);
*/

    return EXIT_SUCCESS;
}
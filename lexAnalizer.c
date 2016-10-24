
#include "symbolTable.h"
#include "scanner.h"

#define FILE_PATH "/home/tomico/Documentos/GitProjects/DCompiler/regression.d"

typedef struct analizer *lexAnalizer;
struct analizer {
    symbolTable st;
    Scanner s;
};



void initLexAnalizer(lexAnalizer *la){
    initScanner(&((*la)->s), FILE_PATH);

}

int getNextComponent(lexAnalizer *la) {
    char c;
    c = getNextChar(&((*la)->s));



    return 0;
}

void deleteLexAnalizer(lexAnalizer *la){
    deleteScanner(&((*la)->s));
}
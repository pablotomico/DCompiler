
#include "symbolTable.h"
#include "scanner.h"

#define FILE_PATH "/home/tomico/Documentos/GitProjects/DCompiler/regression.d"

typedef struct analyzer *lexAnalyzer;
struct analyzer {
    symbolTable st;
    Scanner s;
};



void initLexAnalyzer(lexAnalyzer *la){

    lexAnalyzer laAux;
    laAux = (lexAnalyzer) malloc(sizeof(struct analyzer));
    laAux->s = NULL;
    laAux->st = NULL;

    createScanner(&(laAux->s));
    initScanner(&(laAux->s), FILE_PATH);
    initSymbolTable(&(laAux->st));

    *la = laAux;

}

int getNextComponent(lexAnalyzer *la) {
    char c;
    c = getNextChar(&((*la)->s));



    return 0;
}

void deleteLexAnalyzer(lexAnalyzer *la){
    deleteScanner(&((*la)->s));
    deleteSymbolTable(&((*la)->st));
    free(la);
}
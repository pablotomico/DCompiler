
#include "symbolTable.h"
#include "scanner.h"

#define FILE_PATH "/home/tomico/Documentos/GitProjects/DCompiler/regression.d"

typedef struct analyzer *lexAnalyzer;
struct analyzer {
    symbolTable st;
    Scanner s;
};



void initLexAnalyzer(lexAnalyzer *la){
    initScanner(&((*la)->s), FILE_PATH);

}

int getNextComponent(lexAnalyzer *la) {
    char c;
    c = getNextChar(&((*la)->s));



    return 0;
}

void deleteLexAnalyzer(lexAnalyzer *la){
    deleteScanner(&((*la)->s));
}
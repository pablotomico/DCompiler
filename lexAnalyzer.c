#include <ctype.h>
#include "symbolTable.h"
#include "scanner.h"

#define FILE_PATH "/home/tomico/Documentos/GitProjects/DCompiler/regression.d"
#define IDENTIFIER 400
#define STRING 401
#define CHARACTER 402
#define INTEGER 403
#define FLOAT 404
#define BINARY 405

typedef struct analyzer *lexAnalyzer;
struct analyzer {
    symbolTable st;
    Scanner s;
};
typedef struct {
    char *lexem;
    int component;
} lex;

bool isIdentifierChar(char c);

bool isIdentifierInitialChar(char c);

bool isSeparator(char c);

bool isNumberChar(char c);

bool isBinaryChar(char c);

void confirmLexem(lexAnalyzer *la) {
    returnChar(&((*la)->s));
}

void initLexAnalyzer(lexAnalyzer *la) {

    lexAnalyzer laAux;
    laAux = (lexAnalyzer) malloc(sizeof(struct analyzer));
    laAux->s = NULL;
    laAux->st = NULL;

    createScanner(&(laAux->s));
    createSymbolTable(&(laAux->st));

    initScanner(&(laAux->s), FILE_PATH);
    initSymbolTable(&(laAux->st));


    *la = laAux;


}

lex getNextComponent(lexAnalyzer *la) {

    lex l;
    char c;
    char *lexem;
    int res = -1;
    enum state {
        S_INITIAL, S_IDENTIFIER, S_FINAL, S_SEPARATOR, S_NUMBER, S_BINARY
    };
    enum state s = S_INITIAL;
    c = getNextChar(&((*la)->s));


    do {
        //TODO Implement lexical analyzer
        switch (s) {
            case S_INITIAL:
                if (isIdentifierInitialChar(c)) {
                    s = S_IDENTIFIER;
                } else if (isSeparator(c)) {
                    s = S_SEPARATOR;
                } else if (isNumberChar(c)) {
                    s = S_NUMBER;
                } else {
                    s = S_FINAL;
                }


                break;
            case S_IDENTIFIER:
                do {
                    c = getNextChar(&((*la)->s));
                } while (isIdentifierChar(c));

                lexem = getLexem(&((*la)->s));
                //returnChar(&((*la)->s));
                confirmLexem(la);

                if ((res = getComponentByLexem((*la)->st, lexem)) != 0) {
                    s = S_FINAL;
                } else {
                    res = IDENTIFIER;
                    addLexem(&((*la)->st), lexem, res);
                    s = S_FINAL;
                }
                break;


            case S_SEPARATOR:
                res = c;
                s = S_FINAL;
                c = getNextChar(&((*la)->s));
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);
                break;


            case S_NUMBER:
                if (c == '0') {
                    c = getNextChar(&((*la)->s));
                    if (c == 'b') {
                        s = S_BINARY;
                        break;
                    }
                }
                while (isNumberChar(c)) {
                    c = getNextChar(&((*la)->s));
                }
                if (c == '.') {
                    //Reconocemos la segunda parte del float
                    do {
                        c = getNextChar(&((*la)->s));
                    } while (isNumberChar(c));
                    lexem = getLexem(&((*la)->s));
                    //returnChar(&((*la)->s));
                    confirmLexem(la);

                    res = FLOAT;
                    s = S_FINAL;

                } else {
                    lexem = getLexem(&((*la)->s));
                    //returnChar(&((*la)->s));
                    confirmLexem(la);

                    res = INTEGER;
                    s = S_FINAL;
                }

                break;

            case S_BINARY:
                c = getNextChar(&((*la)->s));
                if (isBinaryChar(c)) {
                    do {
                        c = getNextChar(&((*la)->s));
                    } while (isBinaryChar(c));
                    lexem = getLexem(&((*la)->s));
                    //returnChar(&((*la)->s));
                    confirmLexem(la);
                    res = BINARY;
                    s = S_FINAL;
                } else {
                    //TODO Gestionar error de binario mal formado
                    printf("BINARIO MAL FORMAO\n");
                    lexem = getLexem(&((*la)->s));
                    //returnChar(&((*la)->s));
                    confirmLexem(la);
                    res = BINARY;
                    s = S_FINAL;
                }
                break;
            case S_FINAL:
                l.component = res;
                l.lexem = lexem;
                return l;
        }

    } while (true);
}

void deleteLexAnalyzer(lexAnalyzer *la) {
    deleteScanner(&((*la)->s));
    deleteSymbolTable(&((*la)->st));
}

bool isIdentifierInitialChar(char c) {
    if (isalpha(c) || c == '_') {
        return true;
    } else {
        return false;
    }
}

bool isIdentifierChar(char c) {
    if (isalpha(c) || c == '_' || isdigit(c)) {
        return true;
    } else {
        return false;
    }
}

bool isSeparator(char c) {
    if (c == ' ' || c == ';' || c == '\n' || c == '\t' || c == ',' || c == '.') {
        return true;
    } else {
        return false;
    }
}

bool isNumberChar(char c) {
    if (isdigit(c)) {
        return true;
    } else {
        return false;
    }
}

bool isBinaryChar(char c) {
    if (c == '0' || c == '1') {
        return true;
    } else {
        return false;
    }

}

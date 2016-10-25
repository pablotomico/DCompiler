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
#define PLUSPLUS 406
#define MINUSMINUS 407
#define EQUALEQUAL 408
#define GREATEREQUAL 409
#define LESSEQUAL 410
#define PLUSEQUAL 411
#define MINUSEQUAL 412
#define MULTEQUAL 413
#define GREATERGREATER 414
#define LESSLESS 415
#define PERCENTPERCENT 416
#define ANDAND 417
#define ANDEQUAL 418
#define OROR 419
#define OREQUAL 420
#define DOCCOMMENT 421


typedef struct analyzer *lexAnalyzer;
struct analyzer {
    symbolTable st;
    Scanner s;
    int n_line;
};
typedef struct {
    char *lexem;
    int component;
} lex;


bool isIdentifierChar(char c);

bool isIdentifierInitialChar(char c);

bool isTrashChar(char c);

bool isSeparator(char c);

bool isStructureSymbol(char c);

bool isOperatorInitialChar(char c);

bool isNumberInitialChar(char c);

bool isNumberChar(char c);

bool isBinaryChar(char c);


void confirmLexem(lexAnalyzer *la) {
    returnChar(&((*la)->s));
}

void newLine(lexAnalyzer *la) {
    (*la)->n_line = (*la)->n_line + 1;
}

void initLexAnalyzer(lexAnalyzer *la) {

    lexAnalyzer laAux;
    laAux = (lexAnalyzer) malloc(sizeof(struct analyzer));
    laAux->s = NULL;
    laAux->st = NULL;
    laAux->n_line = 1;

    createScanner(&(laAux->s));
    createSymbolTable(&(laAux->st));

    initScanner(&(laAux->s), FILE_PATH);
    initSymbolTable(&(laAux->st));


    *la = laAux;
}

lex getNextComponent(lexAnalyzer *la) {

    lex l;
    char c;
    char *lexem = NULL;
    int res = -1;
    int nested_count= 0;
    enum state {
        S_INITIAL,
        S_IDENTIFIER,
        S_FINAL,
        S_SEPARATOR,
        S_NUMBER,
        S_BINARY,
        S_TRASH,
        S_STRUCTURE_SYMBOL,
        S_OPERATOR,
        S_COMMENT_ONELINE,
        S_COMMENT_MULTILINE,
        S_COMMENT_NESTED,
        S_STRING_LITERAL,
        S_CHAR_LITERAL
    };
    enum state s = S_INITIAL;
    c = getNextChar(&((*la)->s));


    do {
        //TODO Implement lexical analyzer
        switch (s) {
            case S_INITIAL:
                if (isTrashChar(c)) {
                    s = S_TRASH;
                } else if (isSeparator(c)) {
                    s = S_SEPARATOR;
                } else if (isStructureSymbol(c)) {
                    s = S_STRUCTURE_SYMBOL;
                } else if (isOperatorInitialChar(c)) {
                    s = S_OPERATOR;
                } else if (isIdentifierInitialChar(c)) {
                    s = S_IDENTIFIER;
                } else if (isNumberInitialChar(c)) {
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

            case S_STRUCTURE_SYMBOL:
                res = c;
                s = S_FINAL;
                c = getNextChar(&((*la)->s));
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);
                break;

            case S_OPERATOR:
                switch (c) {
                    case '+':
                        c = getNextChar(&((*la)->s));
                        if (c == '+') {
                            res = PLUSPLUS;
                            c = getNextChar(&((*la)->s));
                        } else if (c == '=') {
                            res = PLUSEQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '+';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '-':
                        c = getNextChar(&((*la)->s));
                        if (c == '-') {
                            res = MINUSMINUS;
                            c = getNextChar(&((*la)->s));
                        } else if (c == '=') {
                            res = MINUSEQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '-';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '*':
                        c = getNextChar(&((*la)->s));
                        if (c == '=') {
                            res = MULTEQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '*';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '>':
                        c = getNextChar(&((*la)->s));
                        if (c == '>') {
                            res = GREATERGREATER;
                            c = getNextChar(&((*la)->s));
                        } else if (c == '=') {
                            res = GREATEREQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '>';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '<':
                        c = getNextChar(&((*la)->s));
                        if (c == '<') {
                            res = LESSLESS;
                            c = getNextChar(&((*la)->s));
                        } else if (c == '=') {
                            res = LESSEQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '<';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '%':
                        c = getNextChar(&((*la)->s));
                        if (c == '%') {
                            res = PERCENTPERCENT;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '%';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '&':
                        c = getNextChar(&((*la)->s));
                        if (c == '&') {
                            res = ANDAND;
                            c = getNextChar(&((*la)->s));
                        } else if (c == '=') {
                            res = ANDEQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '&';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '|':
                        c = getNextChar(&((*la)->s));
                        if (c == '|') {
                            res = OROR;
                            c = getNextChar(&((*la)->s));
                        } else if (c == '=') {
                            res = OREQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '|';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '=':
                        c = getNextChar(&((*la)->s));
                        if (c == '=') {
                            res = EQUALEQUAL;
                            c = getNextChar(&((*la)->s));
                        } else {
                            res = '=';
                        }
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_FINAL;
                        break;
                    case '/':
                        //TODO Comments
                        c = getNextChar(&((*la)->s));
                        if (c == '/') {
                            s = S_COMMENT_ONELINE;
                        } else if (c == '*') {
                            c = getNextChar(&((*la)->s));
                            if (c == '*') {
                                c = getNextChar(&((*la)->s));
                                if (c == '/') {
                                    res = '0';

                                    //Confirmamos lexema pero no devolvemos el componente lexico
                                    getLexem(&((*la)->s));
                                    confirmLexem(la);

                                    lexem = NULL;
                                    c = getNextChar(&((*la)->s));
                                    s = S_INITIAL;
                                } else {
                                    while (s != S_FINAL) {
                                        c = getNextChar(&((*la)->s));
                                        if(c == EOF){
                                            //TODO gestionar unexpetedEOF
                                        }else if (c == '*') {
                                            c = getNextChar(&((*la)->s));
                                            if (c == '/') {
                                                res = DOCCOMMENT;
                                                c = getNextChar(&((*la)->s));
                                                lexem = getLexem(&((*la)->s));
                                                confirmLexem(la);

                                                s = S_FINAL;
                                            }
                                        }
                                    }
                                }

                            } else {
                                s = S_COMMENT_MULTILINE;
                            }
                        } else if (c == '+') {
                            s = S_COMMENT_NESTED;
                        } else {
                            res = '/';
                            lexem = getLexem(&((*la)->s));
                            confirmLexem(la);
                            s = S_FINAL;
                        }

                        break;

                    default:
                        break;
                }
                break;

            case S_COMMENT_ONELINE:
                do {
                    c = getNextChar(&((*la)->s));
                } while (c != '\n');
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);
                c = getNextChar(&((*la)->s));
                s = S_INITIAL;
                break;

            case S_COMMENT_MULTILINE:
                nested_count = 1;
                while (s != S_INITIAL) {
                    c = getNextChar(&((*la)->s));
                    if(c == EOF){
                        //TODO gestionar unexpetedEOF
                    }else if (c == '*') {
                        c = getNextChar(&((*la)->s));
                        if (c == '/') {
                            res = '0';
                            c = getNextChar(&((*la)->s));
                            lexem = getLexem(&((*la)->s));
                            confirmLexem(la);
                            s = S_INITIAL;
                        }
                    }
                }
                break;

            case S_COMMENT_NESTED:
                nested_count = 1;
                while (nested_count > 0) {
                    c = getNextChar(&((*la)->s));
                    if(c == EOF){
                        //TODO gestionar unexpetedEOF
                    }else if (c == '+') {
                        c = getNextChar(&((*la)->s));
                        if (c == '/') {
                            nested_count--;
                        }
                    }else if(c == '/'){
                        c = getNextChar(&((*la)->s));
                        if (c == '+') {
                            nested_count++;
                        }
                    }
                }
                res = '0';
                c = getNextChar(&((*la)->s));
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);

                s = S_INITIAL;
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

            //TODO implement string literal and char literal states

            case S_TRASH:
                if (c == '\n') {
                    newLine(la);
                }
                ignoreNextChar(&((*la)->s));
                c = getNextChar(&((*la)->s));
                s = S_INITIAL;
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

bool isTrashChar(char c) {
    if (c == '\n' || c == '\t' || c == ' ') {
        return true;
    } else {
        return false;
    }
}

bool isSeparator(char c) {
    if (c == ';' || c == ',' || c == '.') {
        return true;
    } else {
        return false;
    }
}

bool isStructureSymbol(char c) {
    if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
        return true;
    } else {
        return false;
    }
}

bool isOperatorInitialChar(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<' || c == '%' || c == '&' || c == '|' ||
        c == '=') {
        return true;
    } else {
        return false;
    }
}

bool isNumberInitialChar(char c) {
    if (isdigit(c)) {
        return true;
    } else {
        return false;
    }
}

bool isNumberChar(char c) {
    if (isdigit(c) || c == '_') {
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

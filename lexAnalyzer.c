#include "symbolTable.h"
#include "scanner.h"
#include "util/lexDefinitions.h"
#include "lexicAnalysisFunctions.h"
#include "errorManager.h"

#define FILE_PATH "/home/tomico/Documentos/GitProjects/DCompiler/regression.d"


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
    int res = 0;
    int nested_count = 0;
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
        S_STRING_LITERAL
    };
    enum state s = S_INITIAL;
    c = getNextChar(&((*la)->s));


    do {
        switch (s) {
            case S_INITIAL:
                if (c == EOF) {
                    lexem = "$";
                    res = EOF;
                    s = S_FINAL;
                } else if (isTrashChar(c)) {
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
                } else if (isStringLiteralInitialChar(c)) {
                    s = S_STRING_LITERAL;
                } else {
                    res = EOF;
                    lexem = "$";
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
                        c = getNextChar(&((*la)->s));
                        if(c == EOF){
                            unexpectedEOF(((*la)->n_line));
                            res = '0';
                            lexem = NULL;
                            s = S_INITIAL;
                            break;
                        }else if (c == '/') {
                            s = S_COMMENT_ONELINE;
                        } else if (c == '*') {
                            c = getNextChar(&((*la)->s));
                            if(c == EOF){
                                unexpectedEOF(((*la)->n_line));
                                res = '0';
                                lexem = NULL;
                                s = S_INITIAL;
                                break;
                            } else if (c == '*') {
                                c = getNextChar(&((*la)->s));
                                if (c == '/') {
                                    res = '0';

                                    //Confirmamos lexema pero no devolvemos el componente lexico

                                    getLexem(&((*la)->s));
                                    confirmLexem(la);
                                    c = getNextChar(&((*la)->s));
                                    lexem = NULL;

                                    s = S_INITIAL;
                                } else if (c == '\n'){
                                    newLine(la);
                                }else {
                                    while (s != S_FINAL) {
                                        c = getNextChar(&((*la)->s));
                                        if (c == EOF) {
                                            unexpectedEOF(((*la)->n_line));
                                            res = '0';
                                            lexem = NULL;
                                            s = S_INITIAL;
                                            break;
                                        } else if (c == '\n') {
                                            newLine(la);
                                        } else if (c == '*') {
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
                //newLine(la);
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);
                c = getNextChar(&((*la)->s));
                s = S_INITIAL;
                break;

            case S_COMMENT_MULTILINE:
                while (s != S_INITIAL) {
                    c = getNextChar(&((*la)->s));
                    if (c == EOF) {
                        unexpectedEOF(((*la)->n_line));
                        res = '0';
                        lexem = NULL;
                        s = S_INITIAL;
                        break;
                    } else if (c == '\n') {
                        newLine(la);
                    } else if (c == '*') {
                        c = getNextChar(&((*la)->s));
                        if (c == '/') {
                            res = '0';
                            lexem = NULL;
                            c = getNextChar(&((*la)->s));
                            getLexem(&((*la)->s));
                            confirmLexem(la);

                            c = getNextChar(&((*la)->s));


                            s = S_INITIAL;
                        }
                    }
                }
                break;

            case S_COMMENT_NESTED:
                nested_count = 1;
                while (nested_count > 0) {
                    c = getNextChar(&((*la)->s));
                    if (c == EOF) {
                        unexpectedEOF(((*la)->n_line));
                        res = '0';
                        lexem = NULL;
                        s = S_INITIAL;
                        break;
                    } else if (c == '\n') {
                        newLine(la);
                    } else if (c == '+') {
                        c = getNextChar(&((*la)->s));
                        if (c == '/') {
                            nested_count--;
                        }else if (c == '\n'){
                            newLine(la);
                        }
                    } else if (c == '/') {
                        c = getNextChar(&((*la)->s));
                        if (c == '+') {
                            nested_count++;
                        }else if (c == '\n'){
                            newLine(la);
                        }
                    }
                }
                res = '0';
                lexem = NULL;
                c = getNextChar(&((*la)->s));
                getLexem(&((*la)->s));
                confirmLexem(la);

                c = getNextChar(&((*la)->s));
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


                res = INTEGER;

                if (c == '.') {
                    //Reconocemos la segunda parte del float
                    do {
                        c = getNextChar(&((*la)->s));
                    } while (isNumberChar(c));


                    res = FLOAT;

                }
                if (c == 'e') {
                    //Pasamos a ronocer un numero en notacion cientifica
                    c = getNextChar(&((*la)->s));
                    if (c == '-' || c == '+') {
                        c = getNextChar(&((*la)->s));
                        if (isNumberChar(c)) {
                            while (isNumberChar(c)) {
                                c = getNextChar(&((*la)->s));
                            }
                        } else {
                            lexem = getLexem(&((*la)->s));
                            confirmLexem(la);
                            s = S_INITIAL;
                            malformedScientific((*la)->n_line, lexem);
                            break;
                        }
                        res = CIENTIFIC;
                    } else if (isNumberChar(c)) {
                        while (isNumberChar(c)) {
                            c = getNextChar(&((*la)->s));
                        }
                        res = CIENTIFIC;
                    } else {
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        s = S_INITIAL;
                        malformedScientific((*la)->n_line, lexem);
                        break;
                    }

                }
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);
                s = S_FINAL;

                break;

            case S_BINARY:
                c = getNextChar(&((*la)->s));
                if (isBinaryChar(c)) {
                    do {
                        c = getNextChar(&((*la)->s));
                    } while (isBinaryChar(c));
                    lexem = getLexem(&((*la)->s));
                    confirmLexem(la);
                    res = BINARY;
                    s = S_FINAL;
                } else {
                    lexem = getLexem(&((*la)->s));
                    confirmLexem(la);
                    malformedBinary((*la)->n_line, lexem);
                    res = BINARY;
                    s = S_INITIAL;
                }
                break;


            case S_STRING_LITERAL:
                while (s != S_FINAL) {
                    c = getNextChar(&((*la)->s));
                    if (c == '\\') {
                        c = getNextChar(&((*la)->s));
                    } else if (c == '\n') {
                        unexpectedEndOfLine((*la)->n_line);
                        newLine(la);
                    } else if (c == '"') {
                        c = getNextChar(&((*la)->s));
                        lexem = getLexem(&((*la)->s));
                        confirmLexem(la);
                        res = STRING;
                        s = S_FINAL;
                    } else if (c == EOF) {
                        unexpectedEOF(((*la)->n_line));
                        res = '0';
                        lexem = NULL;
                        s = S_INITIAL;
                        break;
                    }
                }
                break;

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
    lexAnalyzer laAux;
    laAux = *la;
    deleteScanner(&(laAux->s));
    deleteSymbolTable(&(laAux->st));
    free(laAux);
}

void printSymbols(lexAnalyzer *la) {
    printSymbolTable((*la)->st);
}
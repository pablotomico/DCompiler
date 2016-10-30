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

    /*
     * Definimos los estados en los que se puede encontrar nuestro analizador
     * En caso de querer añadir nuevos componentes lexicos a reconocer, podemos
     * crear fácilmente un estado.
     */
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
        /*
         * Mediante este switch representamos el autómata con los estados definidos anteriormente.
         */
        switch (s) {
            /*
             * En este estado inicial comprobaremos cual es el siguiente estado al que debemos ir,
             * ordenados por frecuencia aproximada de aparición.
             */
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
                    unknownComponent((*la)->n_line);
                    s = S_FINAL;
                }
                break;
            /*
             * Estado de reconocimiento de un identificador.
             */    
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

            /*
             * Estado de reconocimiento de un separador.
             */
            case S_SEPARATOR:
                res = c;
                s = S_FINAL;
                c = getNextChar(&((*la)->s));
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);
                break;

            /*
             * Estado de reconocimiento de un simbolo de estructura.
             */
            case S_STRUCTURE_SYMBOL:
                res = c;
                s = S_FINAL;
                c = getNextChar(&((*la)->s));
                lexem = getLexem(&((*la)->s));
                confirmLexem(la);
                break;

            /*
             * Estado de reconocimiento de un operador o un comentario.
             */
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
                            free(lexem);
                            s = S_INITIAL;
                            break;
                        }else if (c == '/') {
                            s = S_COMMENT_ONELINE;
                        } else if (c == '*') {
                            c = getNextChar(&((*la)->s));
                            if(c == EOF){
                                unexpectedEOF(((*la)->n_line));
                                res = '0';
                                free(lexem);
                                s = S_INITIAL;
                                break;
                            } else if (c == '*') {
                                c = getNextChar(&((*la)->s));
                                if (c == '/') {
                                    res = '0';

                                    //Confirmamos lexema pero no devolvemos el componente lexico

                                    lexem = getLexem(&((*la)->s));
                                    free(lexem);
                                    confirmLexem(la);
                                    c = getNextChar(&((*la)->s));
                                    s = S_INITIAL;
                                } else if (c == '\n'){
                                    newLine(la);
                                }else {
                                    while (s != S_FINAL) {
                                        c = getNextChar(&((*la)->s));
                                        if (c == EOF) {
                                            unexpectedEOF(((*la)->n_line));
                                            res = '0';
                                            free(lexem);
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

            /*
             * Estado para reconocer comentarios de una linea.
             */
            case S_COMMENT_ONELINE:
                do {
                    c = getNextChar(&((*la)->s));
                } while (c != '\n');
                //newLine(la);
                lexem = getLexem(&((*la)->s));
                free(lexem);
                confirmLexem(la);
                c = getNextChar(&((*la)->s));
                s = S_INITIAL;
                break;

            /*
             * Estado de reconocimiento de un comentario multilinea.
             */
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
                            c = getNextChar(&((*la)->s));
                            lexem = getLexem(&((*la)->s));
                            free(lexem);
                            confirmLexem(la);

                            c = getNextChar(&((*la)->s));


                            s = S_INITIAL;
                        }
                    }
                }
                break;

            /*
             * Estado que reconoce comentarios anidados.
             */
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
                c = getNextChar(&((*la)->s));
                lexem = getLexem(&((*la)->s));
                free(lexem);
                confirmLexem(la);

                c = getNextChar(&((*la)->s));
                s = S_INITIAL;
                break;

            /*
             * Estado que reconoce los tipos de numeros.
             */
            case S_NUMBER:
                //Binarios
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

                /*
                 * Si no es un binario, podemos asumir que estamos leyendo un entero a no ser que encontremos algo que
                 * demuestre lo contrario.
                 */
                res = INTEGER;

                //Si hallamos un punto, es un float
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
            /*
             * Estado de reconocimiento de binarios.
             */
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

            /*
             * Estado que reconoce las cadenas literales.
             */
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

            /*
             * Estado para reconocer elementos a ignorar.
             */
            case S_TRASH:
                if (c == '\n') {
                    newLine(la);
                }
                ignoreNextChar(&((*la)->s));
                c = getNextChar(&((*la)->s));
                s = S_INITIAL;
                break;

            /*
             * Estado final
             */
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
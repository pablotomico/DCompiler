#include <ctype.h>
#include "lexicAnalysisFunctions.h"

/*
 * Comprueba si es un caracter válido para inicio de un identificador
 */
bool isIdentifierInitialChar(char c) {
    if (isalpha(c) || c == '_') {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si es un caracter válido para un identificador
 */
bool isIdentifierChar(char c) {
    if (isalpha(c) || c == '_' || isdigit(c)) {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si se trata de un caracter que debamos ignorar en el análisis
 */
bool isTrashChar(char c) {
    if (c == '\n' || c == '\t' || c == ' ') {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si el caracter es un separador valido
 */
bool isSeparator(char c) {
    if (c == ';' || c == ',' || c == '.') {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si es un símbolo de construcción o estructura
 */
bool isStructureSymbol(char c) {
    if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si el caracter de entrada corresponde a un operador inicial válido
 */
bool isOperatorInitialChar(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<' || c == '%' || c == '&' || c == '|' ||
        c == '=') {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si es un caracter válido como inicial de un número
 */
bool isNumberInitialChar(char c) {
    if (isdigit(c)) {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si es un caracter válido para el "cuerpo" de un número
 */
bool isNumberChar(char c) {
    if (isdigit(c) || c == '_') {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si es un caracter binario
 */
bool isBinaryChar(char c) {
    if (c == '0' || c == '1') {
        return true;
    } else {
        return false;
    }
}

/*
 * Comprueba si es un caracter válido para iniciar un string literal
 */
bool isStringLiteralInitialChar(char c){
    if(c == '"'){
        return true;
    }else{
        return false;
    }
}

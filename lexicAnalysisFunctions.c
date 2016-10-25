#include <ctype.h>
#include "lexicAnalysisFunctions.h"

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

bool isStringLiteralInitialChar(char c){
    if(c == '"'){
        return true;
    }else{
        return false;
    }
}

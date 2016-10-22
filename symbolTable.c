#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util/binarySearchTree.h"

#define DEFINITIONS_PATH "/home/tomico/Documentos/GitProjects/DCompiler/util/definitions.txt"


typedef struct table *symbolTable;
struct table {
    tree t;
    FILE *file;
};

void executeParse(symbolTable *st, char *filePath);

void initSymbolTable(symbolTable *st) {
    create(&((*st)->t));
    executeParse(st, DEFINITIONS_PATH);
}

void addLexem(symbolTable *st, char *lex, int comp) {
    char *lexem = (char *) malloc (sizeof(char) * strlen(lex));
    int i = 0;
    for (i = 0; i<strlen(lex); i++){
        lexem[i] = lex[i];
    }
    insert(&((*st)->t), lexem, comp);
}

int getComponentByLexem(symbolTable st, char *lex) {
    return getComponentByLex(st->t, lex);
}

void deleteSymbolTable(symbolTable *st) {
    delete(&((*st)->t));
}

void printSymbolTable(symbolTable st) {
    printTree(st->t);
}

void executeParse(symbolTable *st, char *filePath) {

    char *line = NULL;
    char *word = NULL;
    char *lex = NULL;
    int component = 0;
    size_t len = 0;
    ssize_t read;
    int res = 0;


    (*st)->file = fopen(filePath, "r");

    if ((*st)->file == NULL) {
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, (*st)->file)) != -1) {
        //printf("%s", line);
        word = strtok(line, " ");

        res = strcmp(word, "#define");
        if (res == 0) {
            word = strtok(NULL, " ");
        }
        res = strcmp(word, "#define");
        if (res != 0 && isalpha(word[0])) {
            lex = word;
            word = strtok(NULL, " ");
            if (word != NULL && isdigit(word[0])) {
                component = atoi(word);
                addLexem(st, lex, component);
                word = NULL;
                component = 0;
            }
        }


        //printf("%s\n", word);



    }

    fclose((*st)->file);

    if (line) {
        free(line);
    }

    //printTree((*st)->t);
}
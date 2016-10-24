
#ifndef DCOMPILER_SCANNER_H
#define DCOMPILER_SCANNER_H

#endif //DCOMPILER_SCANNER_H

#include <stdlib.h>
#include <stdio.h>

typedef void *Scanner;

void createScanner(Scanner *s);

void initScanner(Scanner *s, char *filePath);

char getNextChar(Scanner *s);

void returnChar(Scanner *s);

char * getLexem(Scanner *s);

void deleteScanner(Scanner *s);

#ifndef DCOMPILER_SCANNER_H
#define DCOMPILER_SCANNER_H

#endif //DCOMPILER_SCANNER_H

#include <stdlib.h>
#include <stdio.h>


void initScanner(char *filePath);

char getNextChar();

char * getLexem();

void deleteScanner();
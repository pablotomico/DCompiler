

#include <stdio.h>

void unexpectedEOF(int lineNumber){
    printf("ERROR: unexpected EOF at line %d\n", lineNumber);
}
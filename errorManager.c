#include <stdio.h>
#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KWHT  "\x1B[37m"

void unexpectedEOF(int lineNumber){
    printf("%sERROR: unexpected EOF at line %d\n", KYEL, lineNumber);
    printf("%s", KWHT);
}

void malformedBinary(int lineNumber, char* number){
    printf("%sERROR: malformed binary at line %d: expected [0-1] sequence after \"%s\"\n", KYEL, lineNumber, number);
    printf("%s", KWHT);
}

void malformedScientific(int lineNumber, char* number){
    printf("%sERROR: malformed scientific number at line %d: expected number after \"%s\"\n", KYEL, lineNumber, number);
    printf("%s", KWHT);
}

void fatalError(char *msg){
    printf("%sFATAL ERROR: %s\n", KRED, msg);
    printf("%s", KWHT);
}


#include <stdio.h>

#define KRED  "\x1B[31m"
#define KYEL  "\x1B[33m"
#define KWHT  "\x1B[37m"

void unexpectedEOF(int lineNumber) {
    printf("%sERROR: unexpected EOF at line %d\n", KRED, lineNumber);
    printf("%s", KWHT);
}

void unexpectedEndOfLine(int lineNumber) {
    printf("%sERROR: unexpected end of line '\\n' at line %d\n", KRED, lineNumber);
    printf("%s", KWHT);
}

void unknownComponent(int lineNumber) {
    printf("%sERROR: unknown component at line %d\n", KRED, lineNumber);
    printf("%s", KWHT);
}

void malformedBinary(int lineNumber, char *number) {
    printf("%sERROR: malformed binary at line %d: expected [0-1] sequence after \"%s\"\n", KRED, lineNumber, number);
    printf("%s", KWHT);
}

void malformedScientific(int lineNumber, char *number) {
    printf("%sERROR: malformed scientific number at line %d: expected number after \"%s\"\n", KRED, lineNumber, number);
    printf("%s", KWHT);
}

void showWarning(char *msg) {
    printf("%sWARNING: %s\n", KYEL, msg);
    printf("%s", KWHT);
}

void fatalError(char *msg) {
    printf("%sFATAL ERROR: %s\n", KRED, msg);
    printf("%s", KWHT);
}


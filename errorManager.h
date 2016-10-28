#ifndef DCOMPILER_ERRORMANAGER_H
#define DCOMPILER_ERRORMANAGER_H

#endif //DCOMPILER_ERRORMANAGER_H


void unexpectedEOF(int lineNumber);

void unexpectedEndOfLine(int lineNumber);

void malformedBinary(int lineNumber, char* number);

void malformedScientific(int lineNumber, char* number);

void showWarning(char* msg);

void fatalError(char* msg);
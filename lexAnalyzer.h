#ifndef DCOMPILER_LEXANALIZER_H
#define DCOMPILER_LEXANALIZER_H

#endif //DCOMPILER_LEXANALIZER_H

typedef void *lexAnalyzer;

int getNextComponent(lexAnalyzer *la);

void initLexAnalyzer(lexAnalyzer *la);

void deleteLexAnalyzer(lexAnalyzer *la);
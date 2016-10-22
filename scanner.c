#include <sys/stat.h>
#include <stdbool.h>
#include "scanner.h"

FILE *file;
char *first;
char *end;
char *fBlock;
char *sBlock;
int blockSize;

void readFirstBlock() {
    size_t num_chars = fread(fBlock, sizeof(char), (size_t) blockSize, file);


    first = fBlock;
    end = fBlock - sizeof(char);


    *(fBlock + num_chars * sizeof(char)) = EOF;

}

void initScanner(char *filePath) {

    file = fopen(filePath, "r");

    if (file == NULL)
        exit(EXIT_FAILURE);


    struct stat bs;
    stat(filePath, &bs);
    blockSize = (int) bs.st_blksize;

    //blockSize = 16;

    fBlock = (char *) malloc((blockSize + 1) * sizeof(char));
    sBlock = (char *) malloc((blockSize + 1) * sizeof(char));

    readFirstBlock();
}

void readBlock() {


    if (end == (fBlock + blockSize * sizeof(char))) {
        //Si el puntero end sigue en el primer bloque, hay que cargar el 'segundo'
        size_t num_chars = fread(sBlock, sizeof(char), (size_t) blockSize, file);
        end = sBlock - sizeof(char);


        *(sBlock + num_chars * sizeof(char)) = EOF;


    } else {
        //Estamos en el segundo, hay que cargar el 'primero'
        size_t num_chars = fread(fBlock, sizeof(char), (size_t) blockSize, file);
        end = fBlock - sizeof(char);


        *(fBlock + num_chars * sizeof(char)) = EOF;

    }
}

void deleteScanner() {
    if (fBlock != NULL) {
        free(fBlock);
    }
    if (sBlock != NULL) {
        free(sBlock);
    }
    fclose(file);
}

char getNextChar() {

    end = end + sizeof(char);
    char character = *end;

    if (character == EOF &&
        (end == (fBlock + blockSize * sizeof(char)) || end == (sBlock + blockSize * sizeof(char)))) {

        readBlock();

        character = getNextChar();
        return character;

    }


    return character;
}

void returnChar(){
    end = end - sizeof(char);
}

char *getLexem() {
    int i = 0;
    char *lexem = NULL;


    //Comprobar en que bloque esta el inicio del lexema
    unsigned long iLength = (first - fBlock);
    unsigned long eLength = 0;
    unsigned long length = 0;
    if (iLength >= 0 && iLength <= blockSize) {
        //Esta en el primer bloque, pero ahora comprobamos donde esta el final de la misma forma
        iLength = (first - fBlock);
        eLength = (end - fBlock);

        //Hay que hacer esta comprobacion por si el puntero end esta justo una posicion detras del inicio del bloque, la
        //longitud del lexema dara una unidad por debajo de la real, por lo que nos aseguramos poniendo la longitud a 0
        if (eLength == - (sizeof(char))){
            eLength = 0;
        }

        if (eLength >= 0 && eLength <= blockSize) {
            //Estan los dos en el primero, por lo que la longitud se calcula directamente
            eLength = (end - fBlock);

            length = (eLength - iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));

            //printf("Lexema: [");
            for (i = 0; i < length; i++) {
                lexem[i] = *(first + i * sizeof(char));
                //printf("%c", lexem[i]);
            }
            lexem[length] = '\0';

            //printf("]\n");

        } else {
            //Esta el inicio en fBlock y el final en sBlock
            iLength = (blockSize * sizeof(char)) - iLength;
            eLength = (end - fBlock);
            length = (eLength + iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));
            int j = 0;
            for (i = 0; i < iLength; i++) {
                lexem[j] = *(first + i * sizeof(char));
                j++;
            }
            for (i = 0; i < eLength; i++) {
                lexem[j] = *(sBlock + i * sizeof(char));
                j++;
            }
            lexem[length] = '\0';
        }
    } else {
        //Segundo bloque
        iLength = (first - sBlock);

        eLength = (end - fBlock);

        //Hay que hacer esta comprobacion por si el puntero end esta justo una posicion detras del inicio del bloque, la
        //longitud del lexema dara una unidad por debajo de la real, por lo que nos aseguramos poniendo la longitud a 0
        if (eLength == - (sizeof(char))){
            eLength = 0;
        }


        if (eLength > 0 && eLength <= blockSize) {
            //Esta el inicio en fBlock y el final en sBlock
            eLength = (end - fBlock);
            iLength = blockSize - iLength;
            length = (eLength + iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));
            int j = 0;
            for (i = 0; i < iLength; i++) {
                lexem[j] = *(first + i * sizeof(char));
                j++;
            }
            for (i = 0; i < eLength; i++) {
                lexem[j] = *(fBlock + i * sizeof(char));
                j++;
            }
            lexem[length] = '\0';
        } else {

            //Estan los dos en el segundo, por lo que la longitud se calcula directamente
            eLength = (end - fBlock) / sizeof(char);
            length = eLength - iLength;

            lexem = (char *) malloc((length + 1) * sizeof(char));

            for (i = 0; i < length; i++) {
                lexem[i] = *(first + i * sizeof(char));
            }
            lexem[length] = '\0';
        }
    }


    first = end;
    //end = first - sizeof(char);
    return lexem;
}



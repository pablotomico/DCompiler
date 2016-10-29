#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "errorManager.h"


typedef struct scan *Scanner;
struct scan {
    FILE *file;
    char *first;
    char *end;
    char *fBlock;
    char *sBlock;
    int blockSize;
    unsigned long readChars;
    bool isFirstBlock;
};


void readFirstBlock(Scanner *s) {
    size_t num_chars = fread((*s)->fBlock, sizeof(char), (size_t) (*s)->blockSize, (*s)->file);


    (*s)->first = (*s)->fBlock;
    (*s)->end = (*s)->fBlock - sizeof(char);


    *((*s)->fBlock + num_chars * sizeof(char)) = EOF;

}

void createScanner(Scanner *s) {
    *s = NULL;
}

void initScanner(Scanner *s, char *filePath) {
    Scanner sAux;
    sAux = (Scanner) malloc(sizeof(struct scan));

    sAux->file = fopen(filePath, "r");

    if (sAux->file == NULL) {
        fatalError("Couldn't read code file!");
        exit(EXIT_FAILURE);
    }


    struct stat bs;
    stat(filePath, &bs);
    sAux->blockSize = (int) bs.st_blksize;
//    sAux->blockSize = 32;

    sAux->isFirstBlock = true;
    sAux->readChars = 0;
    sAux->fBlock = (char *) malloc((sAux->blockSize + 1) * sizeof(char));
    sAux->sBlock = (char *) malloc((sAux->blockSize + 1) * sizeof(char));
    *s = sAux;

    readFirstBlock(s);
}

void readBlock(Scanner *s) {

    (*s)->isFirstBlock = false;
    if ((*s)->end == ((*s)->fBlock + (*s)->blockSize * sizeof(char))) {
        //Si el puntero end sigue en el primer bloque, hay que cargar el 'segundo'
        size_t num_chars = fread((*s)->sBlock, sizeof(char), (size_t) (*s)->blockSize, (*s)->file);
        (*s)->end = (*s)->sBlock - sizeof(char);


        *((*s)->sBlock + num_chars * sizeof(char)) = EOF;


    } else {
        //Estamos en el segundo, hay que cargar el 'primero'
        size_t num_chars = fread((*s)->fBlock, sizeof(char), (size_t) (*s)->blockSize, (*s)->file);
        (*s)->end = (*s)->fBlock - sizeof(char);


        *((*s)->fBlock + num_chars * sizeof(char)) = EOF;

    }
}

void deleteScanner(Scanner *s) {
    Scanner sAux;
    sAux = *s;
    if (sAux->fBlock != NULL) {
        free(sAux->fBlock);
    }
    if (sAux->sBlock != NULL) {
        free(sAux->sBlock);
    }
    fclose(sAux->file);
    free(sAux);
}

char getNextChar(Scanner *s) {

    (*s)->end = (*s)->end + sizeof(char);
    char character = *((*s)->end);


    if (character == EOF &&
        ((*s)->end == ((*s)->fBlock + (*s)->blockSize * sizeof(char)) ||
         (*s)->end == ((*s)->sBlock + (*s)->blockSize * sizeof(char)))) {

        readBlock(s);

        character = getNextChar(s);
        return character;

    }else {
        (*s)->readChars++;
    }


    return character;
}

void returnChar(Scanner *s) {
    (*s)->end = (*s)->end - sizeof(char);
    (*s)->readChars--;
}

void ignoreNextChar(Scanner *s) {

    (*s)->end = (*s)->end + sizeof(char);
    (*s)->first = (*s)->end;
    returnChar(s);
    char character = *((*s)->end);
    (*s)->readChars = 0;

    if (character == EOF &&
        ((*s)->end == ((*s)->fBlock + (*s)->blockSize * sizeof(char)) ||
         (*s)->end == ((*s)->sBlock + (*s)->blockSize * sizeof(char)))) {

        readBlock(s);
    }
}


char *getLexem(Scanner *s) {
    int i = 0;
    int j = 0;
    char *lexem = NULL;


    //Comprobar en que bloque esta el inicio del lexema
    unsigned long iLength = ((*s)->first - (*s)->fBlock);
    unsigned long eLength = 0;
    unsigned long length = 0;

    if ((*s)->readChars > (*s)->blockSize) {
        /*
         * Si el tamaño de los que hemos leido supera el tamaño de bloque, avisaremos de que es posible que se produzcan
         * errores a la hora de devolver el componente
         */
        showWarning("Your request is too large, may cause problems");

        length = (*s)->readChars;

        if ((*s)->isFirstBlock) {
            lexem = (char *) malloc((length + 1) * sizeof(char));

            for (i = 0; i < length; i++) {
                lexem[i] = *((*s)->fBlock + i * sizeof(char));
            }
            lexem[length] = '\0';
        } else {
            eLength = (*s)->end - (*s)->fBlock;
            if (eLength >= 0 && eLength <= (*s)->blockSize) {
                /*
                 * END esta en el primer bloque
                 */

                //Comprobamos si podemos recuperar el inicio
                if (length < (eLength + (*s)->blockSize)){
                    //Si se puede
                    iLength = ((*s)->first - (*s)->fBlock);

                    if(iLength >= 0 && iLength <= (*s)->blockSize) {
                        /*
                         * FIRST esta en el primero
                         */
                        lexem = (char *) malloc((length + 1) * sizeof(char));
                        eLength = ((*s)->end - (*s)->first) / sizeof(char);
                        j = 0;
                        for (i = 0; i < eLength; i++) {
                            lexem[j] = *((*s)->first + i * sizeof(char));
                            j++;
                        }
                        lexem[length] = '\0';
                    }else {
                        /*
                         * FIRST esta en el segundo
                         */
                        iLength = ((*s)->first - (*s)->sBlock);
                        iLength = (*s)->blockSize - iLength;
                        lexem = (char *) malloc((length + 1) * sizeof(char));
                        eLength = ((*s)->end - (*s)->fBlock) / sizeof(char);
                        j = 0;
                        for (i = 0; i < iLength; i++) {
                            lexem[j] = *((*s)->first + i * sizeof(char));
                            j++;
                        }
                        for (i = 0; i < eLength; i++) {
                            lexem[j] = *((*s)->fBlock + i * sizeof(char));
                            j++;
                        }
                        lexem[length] = '\0';
                    }


                }else {
                    lexem = (char *) malloc((length + 1) * sizeof(char));
                    eLength = ((*s)->end - (*s)->fBlock) / sizeof(char);
                    j = 0;
                    for (i = 0; i < (*s)->blockSize; i++) {
                        lexem[j] = *((*s)->sBlock + i * sizeof(char));
                        j++;
                    }
                    for (i = 0; i < eLength; i++) {
                        lexem[j] = *((*s)->fBlock + i * sizeof(char));
                        j++;
                    }
                    lexem[length] = '\0';
                }

            } else {
                /*
                 * END esta en el segundo
                 */

                //Comprobamos si podemos recuperar el inicio
                if (length < (eLength + (*s)->blockSize) / sizeof(char)) {
                    //Si se puede
                    iLength = ((*s)->first - (*s)->fBlock);

                    if (iLength >= 0 && iLength <= (*s)->blockSize){
                        /*
                         * FIRST esta en el primero
                         */
                        iLength = (*s)->blockSize - iLength;
                        lexem = (char *) malloc((length + 1) * sizeof(char));
                        eLength = ((*s)->end - (*s)->sBlock) / sizeof(char);
                        j = 0;
                        for (i = 0; i < iLength; i++) {
                            lexem[j] = *((*s)->first + i * sizeof(char));
                            j++;
                        }
                        for (i = 0; i < eLength; i++) {
                            lexem[j] = *((*s)->sBlock + i * sizeof(char));
                            j++;
                        }
                        lexem[length] = '\0';
                    } else {
                        iLength = (*s)->first - (*s)->sBlock;
                        iLength = (*s)->blockSize - iLength;
                        lexem = (char *) malloc((length + 1) * sizeof(char));
                        eLength = ((*s)->end - (*s)->sBlock) / sizeof(char);
                        j = 0;
                        for (i = 0; i < iLength; i++) {
                            lexem[j] = *((*s)->first + i * sizeof(char));
                            j++;
                        }
                        for (i = 0; i < eLength; i++) {
                            lexem[j] = *((*s)->sBlock + i * sizeof(char));
                            j++;
                        }
                        lexem[length] = '\0';
                    }


                }else {
                    lexem = (char *) malloc((length + 1) * sizeof(char));
                    eLength = ((*s)->end - (*s)->sBlock) / sizeof(char);
                    j = 0;
                    for (i = 0; i < (*s)->blockSize; i++) {
                        lexem[j] = *((*s)->fBlock + i * sizeof(char));
                        j++;
                    }
                    for (i = 0; i < eLength; i++) {
                        lexem[j] = *((*s)->sBlock + i * sizeof(char));
                        j++;
                    }
                    lexem[length] = '\0';
                }
            }
        }


    } else if (iLength >= 0 && iLength <= (*s)->blockSize) {
        //Esta en el primer bloque, pero ahora comprobamos donde esta el final de la misma forma
        iLength = ((*s)->first - (*s)->fBlock);
        eLength = ((*s)->end - (*s)->fBlock);

        //Hay que hacer esta comprobacion por si el puntero end esta justo una posicion detras del inicio del bloque, la
        //longitud del lexema dara una unidad por debajo de la real, por lo que nos aseguramos poniendo la longitud a 0
        if (eLength == -(sizeof(char))) {
            eLength = 0;
        }

        if (eLength >= 0 && eLength <= (*s)->blockSize) {
            //Estan los dos en el primero, por lo que la longitud se calcula directamente
            eLength = ((*s)->end - (*s)->fBlock);

            length = (eLength - iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));

            //printf("Lexema: [");
            for (i = 0; i < length; i++) {
                lexem[i] = *((*s)->first + i * sizeof(char));
                //printf("%c", lexem[i]);
            }
            lexem[length] = '\0';

            //printf("]\n");

        } else {
            //Esta el inicio en fBlock y el final en sBlock
            iLength = ((*s)->blockSize * sizeof(char)) - iLength;
            eLength = ((*s)->end - (*s)->sBlock);
            length = (eLength + iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));
            j = 0;
            for (i = 0; i < iLength; i++) {
                lexem[j] = *((*s)->first + i * sizeof(char));
                j++;
            }
            for (i = 0; i < eLength; i++) {
                lexem[j] = *((*s)->sBlock + i * sizeof(char));
                j++;
            }
            lexem[length] = '\0';
        }
    } else {
        //Segundo bloque
        iLength = ((*s)->first - (*s)->sBlock);

        eLength = ((*s)->end - (*s)->fBlock);

        //Hay que hacer esta comprobacion por si el puntero end esta justo una posicion detras del inicio del bloque, la
        //longitud del lexema dara una unidad por debajo de la real, por lo que nos aseguramos poniendo la longitud a 0
        if (eLength == -(sizeof(char))) {
            eLength = 0;
        }


        if (eLength >= 0 && eLength <= (*s)->blockSize) {
            //Esta el inicio en sBlock y el final en fBlock
            eLength = ((*s)->end - (*s)->fBlock);
            iLength = (*s)->blockSize - iLength;
            length = (eLength + iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));
            j = 0;
            for (i = 0; i < iLength; i++) {
                lexem[j] = *((*s)->first + i * sizeof(char));
                j++;
            }
            for (i = 0; i < eLength; i++) {
                lexem[j] = *((*s)->fBlock + i * sizeof(char));
                j++;
            }
            lexem[length] = '\0';
        } else {

            //Estan los dos en el segundo, por lo que la longitud se calcula directamente
            eLength = ((*s)->end - (*s)->sBlock) / sizeof(char);
            length = eLength - iLength;

            lexem = (char *) malloc((length + 1) * sizeof(char));

            for (i = 0; i < length; i++) {
                lexem[i] = *((*s)->first + i * sizeof(char));
            }
            lexem[length] = '\0';
        }
    }


    (*s)->first = (*s)->end;
    (*s)->readChars = 0;
    return lexem;
}



#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct scan *Scanner;
struct scan {
    FILE *file;
    char *first;
    char *end;
    char *fBlock;
    char *sBlock;
    int blockSize;

};


void readFirstBlock(Scanner *s) {
    size_t num_chars = fread((*s)->fBlock, sizeof(char), (size_t) (*s)->blockSize, (*s)->file);


    (*s)->first = (*s)->fBlock;
    (*s)->end = (*s)->fBlock - sizeof(char);


    *((*s)->fBlock + num_chars * sizeof(char)) = EOF;

}

void createScanner(Scanner *s){
    *s = NULL;
}

void initScanner(Scanner *s, char *filePath) {
    Scanner sAux;
    sAux = (Scanner) malloc(sizeof(struct scan));

    sAux->file = fopen(filePath, "r");

    if (sAux->file == NULL)
        exit(EXIT_FAILURE);


    struct stat bs;
    stat(filePath, &bs);
    sAux->blockSize = (int) bs.st_blksize;

    //blockSize = 16;

    sAux->fBlock = (char *) malloc((sAux->blockSize + 1) * sizeof(char));
    sAux->sBlock = (char *) malloc((sAux->blockSize + 1) * sizeof(char));
    *s = sAux;

    readFirstBlock(s);
}

void readBlock(Scanner *s) {


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
    if ((*s)->fBlock != NULL) {
        free((*s)->fBlock);
    }
    if ((*s)->sBlock != NULL) {
        free((*s)->sBlock);
    }
    fclose((*s)->file);
    free(s);
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

    }


    return character;
}

void returnChar(Scanner *s) {
    (*s)->end = (*s)->end - sizeof(char);
}

char *getLexem(Scanner *s) {
    int i = 0;
    char *lexem = NULL;


    //Comprobar en que bloque esta el inicio del lexema
    unsigned long iLength = ((*s)->first - (*s)->fBlock);
    unsigned long eLength = 0;
    unsigned long length = 0;
    if (iLength >= 0 && iLength <= (*s)->blockSize) {
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
            eLength = ((*s)->end - (*s)->fBlock);
            length = (eLength + iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));
            int j = 0;
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


        if (eLength > 0 && eLength <= (*s)->blockSize) {
            //Esta el inicio en fBlock y el final en sBlock
            eLength = ((*s)->end - (*s)->fBlock);
            iLength = (*s)->blockSize - iLength;
            length = (eLength + iLength) / sizeof(char);

            lexem = (char *) malloc((length + 1) * sizeof(char));
            int j = 0;
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
            eLength = ((*s)->end - (*s)->fBlock) / sizeof(char);
            length = eLength - iLength;

            lexem = (char *) malloc((length + 1) * sizeof(char));

            for (i = 0; i < length; i++) {
                lexem[i] = *((*s)->first + i * sizeof(char));
            }
            lexem[length] = '\0';
        }
    }


    (*s)->first = (*s)->end;
    //end = first - sizeof(char);
    return lexem;
}



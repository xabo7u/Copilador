#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Manejo de código fuente
FILE *inFile;
FILE *outFile;
char srcLine[255];
int idxLine = 0;
int srcRow = 0;

//Campos relativos a la lectura de tokens
char srcChar;
int srcToktyp;
char srcToken[255];

void concatenarTokenChar();

int next_is_EOL();
int next_is_EOF();
int isAlphaUp();
int isAlphaDown();
int isNumeric();
void nextLine();
void nextChar();
void extractIdentifier();
void extractSpace();
void extractNumber();
void extractString();
void extractComment();
void nextToken();

int main(){
    inFile = fopen("input.msk", "r");
    nextLine();
    while (next_is_EOF() != 1) {
        nextToken();
        printf("%s\n", srcToken);
    }
    fclose(inFile);
    return 0;
}

//Devuelve 1 si el siguiente token corresponde a un Fin de Línea (EOL).
int next_is_EOL(){
    if(idxLine >= strlen(srcLine)){
        return 1;
    } else {
        return 0;
    }
}

//Devuelve 1 si el siguiente token corresponde a un Fin de Archivo (EOF).
int next_is_EOF(){
    if(feof(inFile)){
        if(next_is_EOL() == 1){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

//Pasa a la siguiente línea del archivo de entrada
void nextLine(){
    if(feof(inFile)){
        return;
    }
    fgets(srcLine, sizeof(srcLine), inFile);
    srcRow++;
    idxLine = 0;
}

//Incrementa "idxLine". Pasa al siguiente caracter.
void nextChar(){
    idxLine = idxLine + 1;
    if(next_is_EOL() == 1){
        srcChar = 0;
    } else {
        srcChar = srcLine[idxLine];
    }
}

//Indica si el caracter en "srcChar" es alfabético mayúscula.
int isAlphaUp(){
    if(srcChar >= 'A' && srcChar <= 'Z'){
        return 1;
    } else {
        return 0;
    }
}

//Indica si el caracter en "srcChar" es alfabético minúscula.
int isAlphaDown(){
    if (srcChar == '_'){
        return 1;
    }
    if(srcChar >= 'a' && srcChar <= 'z'){
        return 1;
    } else {
        return 0;
    }
}

//Indica si el caracter en "srcChar" es numérico.
int isNumeric(){
    if(srcChar >= '0' && srcChar <= '9'){
        return 1;
    } else {
        return 0;
    }
}

void extractIdentifier(){
    strcpy(srcToken, "");
    srcToktyp = 2;
    int isToken = 1;
    while (isToken == 1){
        concatenarTokenChar();
        nextChar();
        if(next_is_EOL() == 1){
            return;
        }
        isToken = isAlphaUp() || isAlphaDown() || isNumeric();
    }
}

void extractSpace(){
    strcpy(srcToken, "");
    srcToktyp = 1;
    int isToken = 1;
    while(isToken == 1){
        concatenarTokenChar();
        nextChar();
        if(next_is_EOL() == 1){
            return;
        }
        if (srcChar == ' '|| srcChar == '\t'){
            isToken = 1;
        } else {
            isToken = 0;
        }
    }
}

void extractNumber(){
    strcpy(srcToken, "");
    srcToktyp = 3;
    int isToken = 1;
    while(isToken == 1){
        concatenarTokenChar();
        nextChar();
        if(next_is_EOL() == 1){
            return;
        }
        isToken = isNumeric();
    }
}

void extractString(){
    strcpy(srcToken, "");
    srcToktyp = 2;
    int isToken = 1;
    while(isToken == 1){
        concatenarTokenChar();
        nextChar();
        if(next_is_EOL() == 1){
            return;
        }
        if (srcChar != '"') {
            isToken = 1; 
        } else {
            isToken = 0;
        }
    }   
    nextChar();
    strcat(srcToken, "\"");
}

void extractComment(){
    strcpy(srcToken, "");
    srcToktyp = 5;
    while(next_is_EOL() == 0){
        concatenarTokenChar();
        nextChar();
    }
}

//Lee un token y devuelve el texto en "srcToken" y el tipo en "srcToktyp".
//Mueve la posición de lectura al siguiente token.
void nextToken(){
    srcToktyp = 9;
    if (next_is_EOF() == 1){
        strcpy(srcToken, "");
        srcToktyp = 10;
        return;
    }
    if (next_is_EOL() == 1){
        strcpy(srcToken, "");
        srcToktyp = 0;
        nextLine();
    } else {
        srcChar =  srcLine[idxLine];
        if(isAlphaUp() == 1){
            extractIdentifier();
            return;
        } else if(isAlphaDown() == 1) {
            extractIdentifier();
            return;
        } else if(srcChar == '_'){
            extractIdentifier();
        } else if(isNumeric() == 1){
            extractNumber();
        } else if(srcChar == ' '){
            extractSpace();
        } else if(srcChar == '\t'){
            extractSpace();
        } else if(srcChar == '"'){
            extractString();
        } else if(srcChar == '>'){
            srcToktyp = 6;
            nextChar();
            if(srcChar ==  '='){
                strcpy(srcToken, ">=");
            } else {
                strcpy(srcToken, ">");
            }
        } else if(srcChar == '<'){
            srcToktyp = 6;
            nextChar();
            if(srcChar ==  '='){
                strcpy(srcToken, "<=");
                nextChar();
            } else if (srcChar == '>'){
                strcpy(srcToken, "<>");
                nextChar();
            } else {
                strcpy(srcToken, "<");
            }
        } else if(srcChar == '/'){
            if(srcLine[idxLine + 1] == '/'){
                extractComment();
            } else {
                strcpy(srcToken, "/");
                srcToktyp = 9;
                nextChar();
            }
        } else {
            srcToken[0] = srcChar;
            srcToken[1] = '\0';
            srcToktyp = 9;
            nextChar();
        }
    }
}

void concatenarTokenChar(){
    char cadenaTemporal[2];
    cadenaTemporal[0] = srcChar;
    cadenaTemporal[1] = '\0';
    strcat(srcToken, cadenaTemporal);
}
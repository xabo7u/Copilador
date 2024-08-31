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
char srcCharEsp[2];
int srcToktyp;
char srcToken[255];

//Error
char msjError[255];

//Funciones extras
void concatenarTokenChar();

//Archivo
int next_is_EOL();
int next_is_EOF();

//Lexer
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

//Sintaxis
void trimSpaces();
void processBlock();
void parserProgram();
void asmOut(char lin[255]);
void asmLine(char lin[255]);
void asmInt(char lin[255]);
void asmStr(char lin[255]);
void asmIntArr(char lin[255], int asmCant);
void asmStrArr(char lin[255], int asmCant);
void registerVar(char vName[255], int vType, int arrSiz);
void parserVar();
int capture(char c[255]);
int endOfBlock();
int endOfInstruction();
int endOfExpression();

//Tabla de variables
int nVars;
char varNames[255][255];
int varType[255]; //1 Int, 2 String
int varArrSiz[255];

int main(){
    inFile = fopen("input.msk", "r");
    outFile = fopen("input.asm", "w");

    strcpy(msjError, "");
    nextLine();
    nextToken();

    parserProgram();
    if(strcmp(msjError, "") != 0){
        printf("ERROR: input.msk (%d,%d): %s",srcRow, idxLine, msjError);
    }

    fclose(outFile);
    fclose(inFile);

    if(strcmp(msjError, "") == 0)
        return 0;
    return 1;
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

//>>                        Lexical Analysis                        <<//

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
        if (srcChar == ' '|| srcChar == '\n'){
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
        srcCharEsp[0] = srcLine[idxLine];
        srcCharEsp[1] = srcLine[idxLine + 1];
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
        } else if(strcmp(srcCharEsp, "\n") == 0){
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

//>>                    Syntax Analysis                     <<//

//Extrae espacios o comentarios o saltos de línea
void trimSpaces(){
    while(srcToktyp == 1 || srcToktyp == 5 || srcToktyp == 0)
        nextToken();
}

//Toma la cadena indicada. Si no la encuentra, genera mensaje de error y devuelve 0
int capture(char c[255]){
    trimSpaces();
    if(strcmp(srcToken,c) != 0) {
        sprintf(msjError, "Se esperaba: \"%s\"", c);
        return 0;
    }
    nextToken();
    return 1;
}  

//Indica si estamos en el fin de un bloque.
int endOfBlock(){
    trimSpaces();
    if(srcToktyp == 10 || strcmp(srcToken, "end") == 0 || strcmp(srcToken, "else") == 0)
        return 1;
    return 0;
}

//Indica si estamos en el fin de una instrucción.
int endOfInstruction(){
    if(endOfBlock() == 1 || strcmp(srcToken, ";") == 0)
        return 1;
    return 0;
}

int endOfExpression(){
    if(endOfInstruction() == 1 || strcmp(srcToken, ",") == 0 || strcmp(srcToken, ")") == 0 ||strcmp(srcToken, "]") == 0)
        return 1;
    return 0;
}

void processBlock(){
    while(endOfBlock() != 1){
        capture(";");
        if(strcmp(msjError, "") != 0);
            return;
    }
}

void parserProgram(){
    if(endOfBlock() == 1)
        return;
    fprintf(outFile, "section .data\n");
    while(strcmp(srcToken,"var") == 0){
        parserVar();
        if(strcmp(msjError, "") != 0)
            return;
        capture(";");
        if(strcmp(msjError, "") != 0)
            return;
        trimSpaces();
    }
    trimSpaces();
    while(strcmp(srcToken,"procedure") == 0){
        
    }
    fprintf(outFile, "_start:");
    processBlock();
}

void asmOut(char lin[255]){
    fprintf(outFile, "%s\n",lin);
}

void asmLine(char lin[255]){
    fprintf(outFile, "    %s\n",lin);
}

void asmInt(char lin[255]){
    fprintf(outFile, "    %s dd 0\n",lin);
}

void asmIntArr(char lin[255], int asmCant){
    char asmIntArrLine[255] = "dd", asmIntArrAdd[2] = " 0";
    for(int i = 0;i < asmCant; i++){
        strcat(asmIntArrLine, asmIntArrAdd);
        if (i < asmCant - 1) {
            strcat(asmIntArrLine, ",");
        }
    }
    strcat(asmIntArrLine, "\n");
    fprintf(outFile, "    %s %s", lin, asmIntArrLine);
}

void asmStr(char lin[255]){
    fprintf(outFile, "    %s db 255 dup (0)\n",lin);
}

void asmStrArr(char lin[255], int asmCant){
    char asmStrArrLine[255] = "dd ", arrCantVecesStr[20] = "", cpyLin[255];
    int arrCantVeces = 1;
    for(int i = 0;i < asmCant; i++){
        strcpy(cpyLin, lin);
        sprintf(arrCantVecesStr, "%d", arrCantVeces);
        strcat(cpyLin, arrCantVecesStr);
        asmStr(cpyLin);
        strcat(asmStrArrLine, cpyLin);
        if (i < asmCant - 1) {
            strcat(asmStrArrLine, ",");
        }
        arrCantVeces++;
    }
    strcat(asmStrArrLine, "\n");
    fprintf(outFile, "    %s %s", lin, asmStrArrLine);
}

//Registra una variable en los arreglos correspondientes.
void registerVar(char vName[255], int vType, int arrSiz){
    snprintf(varNames[nVars], sizeof(nVars), vName);
    varType[nVars] = vType;
    varArrSiz[nVars] = arrSiz;
    nVars++;
}

void parserVar(){
    char varName[255], typName[255], arrSize[255], arrSize256[255];
    int arrSizeN;
    nextToken();
    trimSpaces();
    if(srcToktyp != 2){
        strcpy(msjError, "Se esperaba un identifiacador.");
        return;
    }
    strcpy(varName, srcToken);
    nextToken();
    trimSpaces();
    if(strcmp(srcToken,"[") == 0){
        nextToken();
        trimSpaces();
        if(srcToktyp != 3){
            strcpy(msjError, "Se esperaba numero.");
            return;
        }
        strcpy(arrSize, srcToken);
        arrSizeN = atoi(srcToken);
        sprintf(arrSize256, "%d", arrSizeN*256);
        nextToken();
        capture("]");
        if(strcmp(msjError, "") != 0)
            return;
        capture(":");
        if(strcmp(msjError, "") != 0)
            return;
        trimSpaces();
        strcpy(typName, srcToken);
        if(strcmp(typName, "integer") == 0){
            nextToken();
            asmIntArr(varName, arrSizeN);
            registerVar(varName, 1, arrSizeN);
        } else if(strcmp(typName, "string") == 0){
            nextToken();
            asmStrArr(varName, arrSizeN);
            registerVar(varName, 2, arrSizeN);
        } else {
            strcpy(msjError, "Tipo desconocido");
            return;
        }
    } else if(strcmp(srcToken, ":") == 0){
        nextToken();
        trimSpaces();
        strcpy(typName, srcToken);
        if(strcmp(typName, "integer") == 0){
            nextToken();
            asmInt(varName);
            registerVar(varName,1,0);
        } else if(strcmp(typName, "string") == 0){
            nextToken();
            asmStr(varName);
            registerVar(varName,2,0);
        } else {
            strcpy(msjError, "Tipo desconocido");
            return;
        }
    } else {
        strcpy(msjError, "Se esperaba, ':' o '[]'.");
        return;
    }
}


#include "assistFunctions.h"
#include "struct.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/ioctl.h>

extern char *comands[], *pararmetrs, *userString;
extern int screenCol, screenRow, screenNumY, tabWidth, wrapMod;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;
static struct termios stored_settings;


int degree(int num, int deg){
    int temp = num;
    
    if (deg == 0){
        return 1;
    }
    else {
        while (deg > 1) {
            temp *= num;
            deg--;
        }
    }
    return temp;
}

void clrscr(void){
    printf("\033[2J");
    printf("\033[0;0f");
}

void moveTxtY(char dir){
    int rowNum = 0, colNum = 0, i;
    
    struct winsize screenSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &screenSize);
    
    if (dir == 'U'){
        for (i = 0; i < (2 * screenRow); i++) {
            if ((tmpStrPointer -> prev) != NULL) {
                tmpStrPointer = tmpStrPointer -> prev;
            }
            else break;
        }
    }
    
    while ((tmpStrPointer != NULL) && (rowNum < screenRow)) {
        resetKeypress();
        clrscr();
        if (wrapMod) {
            while ((tmpCharPointer != NULL)) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum - 1) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        else {
                            printf("\n<--->");
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum = tabWidth + 5;
                            rowNum++;
                        }
                        break;
                    }
                        
                    case '\n': {
                        printf("\n");
                        rowNum++;
                        colNum = 0;
                    }
                        
                    default: {
                        if ((screenCol - colNum) >= 2) {
                            printf("%c", tmpCharPointer -> curChar);
                            colNum++;
                        }
                        else {
                            printf("\n<--->%c", tmpCharPointer -> curChar);
                            colNum = 6;
                            rowNum++;
                        }
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
            }
            tmpStrPointer = tmpStrPointer -> next;
        }
        else {
            while (colNum < screenCol) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        break;
                    }
                        
                    case '\n': {
                        printf("\n");
                        rowNum++;
                        colNum = 0;
                    }
                        
                    default: {
                        printf("%c", tmpCharPointer -> curChar);
                        colNum++;
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
            }
            tmpStrPointer = tmpStrPointer -> next;
            tmpCharPointer = tmpStrPointer -> curString;
        }
    }
    
    setKeypress();
}

void moveTxtX(char dir){
    int i, colNum = 0, rowNum = 0;
    
    struct winsize screenSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &screenSize);
    resetKeypress();
    
    if (screenNumY == 0) {
        if (dir == 'L'){
            exit(0);
        }
    }
    
    if (dir == 'R') {
        tmpStrPointer = pointerForStrings;
        tmpCharPointer = pointerForStrings -> curString;
        for (i = 0; i < screenNumY * screenCol; i++) {
            tmpCharPointer = tmpCharPointer -> next;
        }
        while ((tmpStrPointer != NULL) || (rowNum < screenRow)) {
            while ((tmpCharPointer != NULL) || (colNum < screenCol)) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        break;
                    }
                        
                    case '\n': {
                        printf("\n");
                        rowNum++;
                        colNum = 0;
                    }
                        
                    default: {
                        printf("%c", tmpCharPointer -> curChar);
                        colNum++;
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
            }
            tmpStrPointer = tmpStrPointer -> next;
            for (i = 0; i < screenNumY * screenCol; i++) {
                tmpCharPointer = tmpCharPointer -> next;
            }
        }
        screenNumY++;
    }
    else {
        screenNumY--;
        tmpStrPointer = pointerForStrings;
        tmpCharPointer = pointerForStrings -> curString;
        for (i = 0; i < screenNumY * screenCol; i++) {
            tmpCharPointer = tmpCharPointer -> next;
        }
        while ((tmpStrPointer != NULL) && (rowNum < screenRow)) {
            while ((tmpCharPointer != NULL) && (colNum < screenCol)) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        break;
                    }
                        
                    case '\n': {
                        printf("\n");
                        rowNum++;
                        colNum = 0;
                    }
                        
                    default: {
                        printf("%c", tmpCharPointer -> curChar);
                        colNum++;
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
            }
            tmpStrPointer = tmpStrPointer -> next;
            for (i = 0; i < screenNumY * screenCol; i++) {
                tmpCharPointer = tmpCharPointer -> next;
            }
        }
    }
}

void initCmd(void){
    comands[0] = "set tabwidth";
    comands[1] = "print pages";
    comands[2] = "print range";
    comands[3] = "set wrap";
    comands[4] = "insert after";
    comands[5] = "edit string";
    comands[6] = "insert symbol";
    comands[7] = "replace substring";
    comands[8] = "delete range";
    comands[9] = "delete braces";
    comands[10] = "exit";
    comands[11] = "read";
    comands[12] = "open";
    comands[13] = "write";
    comands[14] = "set name";
    comands[15] = "help";
}

int initFile(char *fileName){
    int firstIteration = 1;
    char temp = '!';
    struct listOfStrings *tempPrevStr = NULL, *tempNextStr = NULL;
    struct listOfChars *tempPrevChar = NULL, *tempNextChar = NULL;
    FILE *inputFile;
    
    tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
    tempNextStr->prev = NULL;
    pointerForStrings = tempNextStr;
    inputFile = fopen(fileName, "rb");
    if (inputFile == NULL) {
        return 0;
    }
    
    do{
        while(temp != '\n'){
            fread(&temp, sizeof(char), 1, inputFile);
            if (feof(inputFile)) {
                break;
            }
            tempNextChar = (struct listOfChars*)malloc(sizeof(struct listOfChars));
            if (tempNextChar == NULL) {
                fprintf(stderr, "Недостаточно памяти!\n");
                exit (1);
            }
            if (firstIteration == 1){
                tempNextStr->curString = tempNextChar;
                tempNextChar->prev = NULL;
                tempNextChar->curChar = temp;
                tempPrevChar = tempNextChar;
                firstIteration = 0;
                continue;
            }
            tempNextChar->curChar = temp;
            tempNextChar->prev = tempPrevChar;
            tempPrevChar->next = tempNextChar;
            tempPrevChar = tempNextChar;
        }
        if (feof(inputFile)){
            tmpStrPointer = pointerForStrings;
            tmpCharPointer = pointerForStrings -> curString;
            tempNextStr -> next = NULL;
            fclose(inputFile);
            return 1;
        }
        temp = '!';
        firstIteration = 1;
        tempNextChar->next = NULL;
        tempPrevStr = tempNextStr;
        tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
        if (tempNextStr == NULL) {
            fprintf(stderr, "Недостаточно памяти!\n");
            
            exit (1);
        }
        tempNextStr->prev = tempPrevStr;
        tempPrevStr->next = tempNextStr;
    }while (1);
}

int readCmd(void){ /* 1 - пустая строка, 2 - нарушение сочетания кавычек, 3 - переполнение памяти*/
    char tempCur = '!', tempPrev = '!';
    int firstSymbol = 0;
    int i = 0;
    int tripleQuotes = 0;
    
    userString = NULL;
    tempCur = getchar();
    tempPrev = tempCur;
    
    if (tempCur == '\n'){
        return 1;
    }
    
    if (tempCur != ' '){
        firstSymbol = 1;
        userString = (char*)realloc(userString, (i + 1) * sizeof(char));
        if (userString == NULL){
            free(userString);
            return 3;
        }
        userString[i] = tempCur;
        i++;
    }
    
    tempCur = getchar();
    
    while (1) {
        if ((!firstSymbol) && (tempCur != ' ') && (tempPrev == ' ')) {
            firstSymbol++;
        }
        switch (tempCur) {
            case ' ': {
                if (firstSymbol == 1){
                    if (tempPrev == ' '){
                        tempCur = getchar();
                    }
                    else {
                        tempPrev = tempCur;
                        userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                        if (userString == NULL){
                            free(userString);
                            return 3;
                        }
                        userString[i] = tempCur;
                        i++;
                        tempCur = getchar();
                    }
                }
                else {
                    tempPrev = tempCur;
                    tempCur = getchar();
                }
                break;
            }
                
            case '"':{
                tempPrev = tempCur;
                tempCur = getchar();
                if (tempCur == '"'){
                    tempPrev = tempCur;
                    tempCur = getchar();
                    if (tempCur == '"'){
                        tripleQuotes++;
                        userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                        if (userString == NULL){
                            free(userString);
                            return 3;
                        }
                        userString[i] = '"';
                        i++;
                        tempPrev = tempCur;
                        tempCur = getchar();
                        while (1) {
                            if (tempCur == '\\') {
                                tempPrev = tempCur;
                                tempCur = getchar();
                            }
                            if (tempCur == '"') {
                                if (tempPrev == '\\'){
                                    tempPrev = tempCur;
                                    userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                                    if (userString == NULL){
                                        free(userString);
                                        return 3;
                                    }
                                    userString[i] = tempCur;
                                    tempCur = getchar();
                                    i++;
                                }
                                else{
                                    if (tempCur == '"') {
                                        tempCur = getchar();
                                        if (tempCur == '"') {
                                            userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                                            if (userString == NULL){
                                                free(userString);
                                                return 3;
                                            }
                                            userString[i] = '\0';
                                            return 0;
                                        }
                                        else{
                                            return 2;
                                        }
                                    }
                                    else{
                                        return 2;
                                    }
                                }
                            }
                            else {
                                tempPrev = tempCur;
                                userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                                if (userString == NULL){
                                    free(userString);
                                    return 3;
                                }
                                userString[i] = tempCur;
                                tempCur = getchar();
                                i++;
                            }
                        }
                    }
                    else{
                        return 2;
                    }
                }
                else{
                    userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                    if (userString == NULL){
                        free(userString);
                        return 3;
                    }
                    userString[i] = '"';
                    i++;
                    while (1) {
                        if (tempCur == '\\') {
                            tempPrev = tempCur;
                            tempCur = getchar();
                        }
                        if (tempCur == '"') {
                            if (tempPrev == '\\'){
                                tempPrev = tempCur;
                                userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                                if (userString == NULL){
                                    free(userString);
                                    return 3;
                                }
                                userString[i] = tempCur;
                                tempCur = getchar();
                                i++;
                            }
                            else{
                                userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                                if (userString == NULL){
                                    free(userString);
                                    return 3;
                                }
                                userString[i] = '\0';
                                return 0;
                            }
                        }
                        else {
                            if (tempCur == '\n'){
                                return 2;
                            }
                            tempPrev = tempCur;
                            userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                            if (userString == NULL){
                                free(userString);
                                return 3;
                            }
                            userString[i] = tempCur;
                            tempCur = getchar();
                            i++;
                        }
                    }
                }
            }
                
            case '#': case '\n':{
                userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                if (userString == NULL){
                    free(userString);
                    return 3;
                }
                userString[i] = '\0';
                return 0;
            }
                
            default: {
                tempPrev = tempCur;
                userString = (char*)realloc(userString, (i + 1) * sizeof(char));
                if (userString == NULL){
                    free(userString);
                    return 3;
                }
                userString[i] = tempCur;
                i++;
                tempCur = getchar();
                break;
            }
        }
    }
}

void setKeypress(void){
    struct termios new_settings;
    
    tcgetattr(0,&stored_settings);
    
    new_settings = stored_settings;
    
    new_settings.c_lflag &= (~ICANON & ~ECHO);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    
    tcsetattr(0,TCSANOW,&new_settings);
    return;
}

void resetKeypress(void){
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}

int recognizeCmd(void){ // -1 - неккоректная команда
    char temp = '!';
    int i = 0, j = 0, isCmdCorrect = 1, pararmetrsCounter = 0;
    
    if (userString == NULL) {
        return -1;
    }
    for (i = 0; i < 16; i++){
        isCmdCorrect = 1;
        while (temp != '\0') {
            temp = comands[i][j];
            if (temp != '\0') {
                if (temp != userString[j]){
                    isCmdCorrect = 0;
                    j = 0;
                    break;
                }
                j++;
            }
        }
        if (isCmdCorrect) {
            if (userString[j] == ' '){
                j++;
            }
            while (userString[j] != '\0') {
                pararmetrs = (char*)realloc(pararmetrs, (pararmetrsCounter + 1) * sizeof(char));
                pararmetrs[pararmetrsCounter] = userString[j];
                pararmetrsCounter++;
                j++;
            }
            return i;
        }
    }
    return -1;
}

void clearOutBuffer(void){
    while(getchar() != '\n'){};
}
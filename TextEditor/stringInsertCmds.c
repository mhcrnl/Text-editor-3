#include "stringInsertCmds.h"
#include "assistFunctions.h"
#include "struct.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/ioctl.h>

extern char *comands[], *parametrs, *userString;
extern char fileName[];
extern int screenCol, screenRow, screenNumY, tabWidth, wrapMod;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;


int insertAfter(void){
    int i = 0, strNum = 0, posIndicator = 1, firstIteration = 1;;
    struct listOfStrings *tempPrevStr = NULL, *tempNextStr = NULL, *begPointer = NULL, *tempStr = NULL, *tempNext;
    struct listOfChars *tempPrevChar = NULL, *tempNextChar = NULL;
    
    if (parametrs[0] != '"') {
        while ((parametrs[i] != ' ') && (parametrs[i] != '"')) {
            if (parametrs[i] == '\0') {
                fprintf(stderr, "Неккоректный параметр!\n");
                return 0;
            }
            posIndicator = 0;
            strNum += atoi(&parametrs[i]) * degree(10, i);
            i++;
        }
    }
    
    i++;
    if (parametrs[i] == '"') {
        i++;
    }
    
    tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
    tempNextStr->prev = NULL;
    begPointer = tempNextStr;
    
    do{
        while(parametrs[i] != '\n'){
            if (parametrs[i] == '\0') {
                break;
            }
            tempNextChar = (struct listOfChars*)malloc(sizeof(struct listOfChars));
            if (tempNextChar == NULL) {
                fprintf(stderr, "Недостаточно памяти!\n");
                exit(1);
            }
            if (firstIteration == 1){
                tempNextStr->curString = tempNextChar;
                tempNextChar->prev = NULL;
                tempNextChar->curChar = parametrs[i];
                tempPrevChar = tempNextChar;
                firstIteration = 0;
                i++;
                continue;
            }
            tempNextChar->curChar = parametrs[i];
            tempNextChar->prev = tempPrevChar;
            tempPrevChar->next = tempNextChar;
            tempPrevChar = tempNextChar;
            i++;
        }
        if (parametrs[i] == '\0') {
            //tempNextStr -> next = NULL;
            break;
        }
        firstIteration = 1;
        tempNextChar->next = NULL;
        tempPrevStr = tempNextStr;
        tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
        if (tempNextStr == NULL) {
            fprintf(stderr, "Недостаточно памяти!\n");
            return 1;
        }
        tempNextStr->prev = tempPrevStr;
        tempPrevStr->next = tempNextStr;
        i++;
    }while (1);
    
    tempNextStr -> next = NULL;
    tempStr = pointerForStrings;
    
    if ((tempStr -> next == NULL) && (tempStr -> curString == NULL)) {
        tempStr = begPointer;
    }else{
    
        if (posIndicator) {
            while (tempStr -> next != NULL) {
                tempStr = tempStr -> next;
            }
            tempStr -> next = begPointer;
            tempNextStr -> prev = tempStr;
        }
        else{
            if (strNum != 0) {
                for (i = 1; i < strNum; i++) {
                    if (tempStr -> next == NULL) {
                        tempStr -> next = begPointer;
                        tempNextStr -> prev = tempStr;
                        break;
                    }
                    tempStr = tempStr -> next;
                }
                tempNext = tempStr -> next;
                tempStr -> next = begPointer;
                tempNextStr -> next = tempNext;
                tempNext -> prev = tempNextStr;
            }
            else{
                tempNextStr -> next = tempStr;
                tempStr -> prev = tempNextStr;
            }
        }
    }
    
    tmpStrPointer = pointerForStrings;
    tmpCharPointer = pointerForStrings -> curString;
    return 0;
}
#include "textViewCmds.h"
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
extern struct winsize screenSize;


void printPages(void){
    char temp = '!';
    int i, rowNum = 0, colNum = 0;
   
/*
    ioctl(0, TIOCGWINSZ, &screenSize);
    screenCol = screenSize.ws_col;
    screenRow = screenSize.ws_row;
    
 */
    if (pointerForStrings == NULL) {
        return;
    }
    
    if (!isatty(1)) {
        while (tmpStrPointer!= NULL) {
            while (tmpCharPointer!= NULL) {
                printf("%c", tmpCharPointer -> curChar);
                tmpCharPointer = tmpCharPointer -> next;
            }
            tmpStrPointer = tmpStrPointer -> next;
        }
    }
    else {
        while ((tmpStrPointer != NULL) && (rowNum < screenRow)) {
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
                            if (rowNum == (screenRow - 1)){
                                rowNum++;
                                break;
                            }
                            printf("\n");
                            rowNum++;
                            colNum = 0;
                            break;
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
                if (tmpStrPointer != NULL) {
                    tmpCharPointer = tmpStrPointer -> curString;
                }
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
            }
            
        }
        
        setKeypress();
        do{
            temp = getchar();
            if ((temp != 'q') && (temp != 'Q')) {
                switch (temp) {
                        
                    case ' ': case 's': case 'S':{
                        moveTxtY('D');
                        break;
                    }
                        
                    case '<': case 'a': case 'A':{
                        if (!wrapMod) {
                            moveTxtX('L');
                        }
                        break;
                    }
                        
                    case '>': case 'd': case 'D':{
                        if (!wrapMod) {
                            moveTxtX('R');
                        }
                        break;
                    }
                    
                    case 'w': case 'W':{
                        moveTxtY('U');
                        break;
                    }
                        
                    case 27:{
                        temp = getchar();
                        if (temp == '[') {
                            temp = getchar();
                            switch (temp) {
                                case 'A':{
                                    moveTxtY('U');
                                    break;
                                }
                                    
                                case 'B':{
                                    moveTxtY('D');
                                    break;
                                }
                                    
                                case 'C':{
                                    if (!wrapMod) {
                                        moveTxtX('L');
                                    }
                                    break;
                                }
                                    
                                case 'D':{
                                    if (!wrapMod) {
                                        moveTxtX('R');
                                    }
                                    break;
                                }
                                    
                                default:{
                                    break;
                                }
                            }
                        }
                    }
                            
                    default: break;
                }
            }
        } while ((temp != 'q') && (temp != 'Q'));
        
        tmpStrPointer = pointerForStrings;
        tmpCharPointer = pointerForStrings -> curString;
        
    }
    resetKeypress();
    printf("\n");
}

void printRange(void){
    int i = 0, startR = 0, endR = 0;
    struct listOfStrings *prevStr, *nextStr, *firstStr, *lastStr;
    
    tmpStrPointer = pointerForStrings;
    
    if (parametrs == NULL) {
        printPages();
        exit (0);
    }
    
    while ((parametrs[i] != ' ') || (parametrs[i] != '\0')) {
        startR += atoi(&parametrs[i]) * degree(10, i);
        i++;
    }
    
    for (i = 1; i < startR; i++) {
        tmpStrPointer = tmpStrPointer -> next;
    }
    firstStr = tmpStrPointer;
    prevStr = tmpStrPointer -> prev;
    tmpStrPointer -> prev = NULL;
    
    if (parametrs[i] != '\0'){
        i++;
        while ((parametrs[i] != '\0')) {
            endR += atoi(&parametrs[i]) * degree(10, i);
            i++;
        }
        
        for (i = 1; i < endR; i++) {
            tmpStrPointer = tmpStrPointer -> next;
        }
        lastStr = tmpStrPointer;
        nextStr = tmpStrPointer -> next;
        tmpStrPointer -> next = NULL;
    }
    
    tmpStrPointer = firstStr;
    tmpCharPointer = firstStr -> curString;
    printPages();
    firstStr -> prev = prevStr;
    lastStr -> next = nextStr;
    tmpStrPointer = pointerForStrings;
    tmpCharPointer = pointerForStrings -> curString;
}

void setWrap(void){
    int i = 0;
    char yesM[3] = "yes";
    char noM[2] = "no";
    
    if (parametrs == NULL) {
        fprintf(stderr, "Некорректный параметр!\n");
        return;
    }
    
    while (parametrs[i] != '\0') {
        if (parametrs[i] == yesM[i]){
            i++;
            if (i == 3){
                wrapMod = 1;
                return;
            }
        }
        else break;
    }
    i = 0;
    while (parametrs[i] != '\0') {
        if (parametrs[i] == noM[i]){
            i++;
            if (i == 2){
                wrapMod = 0;
                return;
            }
        }
        else break;
    }
    fprintf(stderr, "Некорректный параметр!\n");;
}

void setTabWidth(void){
    tabWidth = atoi(parametrs);
}
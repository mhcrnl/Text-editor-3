#include "textViewCmds.h"
#include "assistFunctions.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/ioctl.h>

struct listOfChars{
    char curChar;
    struct listOfChars *next;
    struct listOfChars *prev;
};

struct listOfStrings{
    struct listOfChars *curString;
    struct listOfStrings *next;
    struct listOfStrings *prev;
};

extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern int wrapMod;
extern char *pararmetrs;
extern char *userString;
extern int tabWidth;
struct listOfStrings *pointerForStrings;
extern int screenCol, screenRow;

void printPages(void){
    char temp = '!';
    int i, rowNum = 0, colNum = 0;
    struct winsize screenSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &screenSize);
    
    changeTtyMod(1);
    
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
        
        changeTtyMod(0);
        do{
            temp = getchar();
            if ((temp != 'q') && (temp != 'Q')) {
                switch (temp) {
                        
                    case ' ':{
                        moveTxtY('D');
                        break;
                    }
                        
                    case '<':{
                        if (!wrapMod) {
                            moveTxtX('L');
                        }
                        break;
                    }
                        
                    case '>':{
                        if (!wrapMod) {
                            moveTxtX('R');
                        }
                        break;
                    }
                        
                    default:{
                        switch (temp) {
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
                        break;
                    }
                        
                }
            }
        } while ((temp != 'q') && (temp != 'Q'));
        
        tmpStrPointer = pointerForStrings;
        tmpCharPointer = pointerForStrings -> curString;
        
    }
    changeTtyMod(1);
    printf("\n");
}

void printRange(void){
    int i = 0, startR = 0, endR = 0;
    struct listOfStrings *prevStr, *nextStr, *firstStr, *lastStr;
    
    tmpStrPointer = pointerForStrings;
    
    if (pararmetrs == NULL) {
        printPages();
        exit (0);
    }
    
    while ((pararmetrs[i] != ' ') || (pararmetrs[i] != '\0')) {
        startR += atoi(&pararmetrs[i]) * degree(10, i);
        i++;
    }
    
    for (i = 1; i < startR; i++) {
        tmpStrPointer = tmpStrPointer -> next;
    }
    firstStr = tmpStrPointer;
    prevStr = tmpStrPointer -> prev;
    tmpStrPointer -> prev = NULL;
    
    if (pararmetrs[i] != '\0'){
        i++;
        while ((pararmetrs[i] != '\0')) {
            endR += atoi(&pararmetrs[i]) * degree(10, i);
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
    
    if (pararmetrs == NULL) {
        fprintf(stderr, "Некорректный параметр!\n");
        return;
    }
    
    while (pararmetrs[i] != '\0') {
        if (pararmetrs[i] == yesM[i]){
            i++;
            if (i == 3){
                wrapMod = 1;
                return;
            }
        }
        else break;
    }
    i = 0;
    while (pararmetrs[i] != '\0') {
        if (pararmetrs[i] == noM[i]){
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
    tabWidth = atoi(pararmetrs);
}
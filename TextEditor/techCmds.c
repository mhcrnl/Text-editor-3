#include "techCmds.h"
#include "struct.h"
#include "assistFunctions.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/ioctl.h>

extern char *comands[], *pararmetrs, *userString;
extern char fileName[];
extern int screenCol, screenRow, screenNumY, tabWidth, wrapMod;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;

void exitF(void){
    int i = 0, isForceExit = 0;
    char *force = "force";
    while (pararmetrs[i] != '\0'){
        if (force[i] != pararmetrs[i]) {
            isForceExit = 0;
            break;
        }
        isForceExit = 1;
        i++;
    }
    
    if (isForceExit) {
        return;
    }
    
    fprintf(stderr, "The file is not saved!\n");
    return;
}

void readF(void){
    int i = 0;
    char tempFileName[127];
    
    if (pararmetrs[i] == '"') {
        i++;
    }
    
    while (pararmetrs[i] != '\0') {
        tempFileName[i] = pararmetrs[i];
    }
    if(!initFile(tempFileName)){
        fprintf(stderr, "No such file!\n");
        return;
    }
}

void openF(void){
    int i = 0;
    
    if (pararmetrs[i] == '"') {
        i++;
    }
    
    while (pararmetrs[i] != '\0') {
        fileName[i] = pararmetrs[i];
    }
    if(!initFile(fileName)){
        fprintf(stderr, "No such file!\n");
        return;
    }
}

void setName(void){
    int i = 0;
    
    if (pararmetrs[i] == '"') {
        i++;
    }
    
    while (pararmetrs[i] != '\0') {
        fileName[i] = pararmetrs[i];
    }
}

void writeF(void){
    if (fileName[0] == '\0') {
        fprintf(stderr, "There is an associated file!\n");
        return;
    }
    
    FILE *outputFile = fopen(fileName, "wb");
    if (outputFile == NULL) {
        fprintf(stderr, "No such file!\n");
        return;
    }
    
    while (tmpStrPointer != NULL) {
        tmpCharPointer = tmpStrPointer -> curString;
        while (tmpCharPointer != NULL) {
            fwrite(&(tmpCharPointer -> curChar), sizeof(char), 1, outputFile);
            tmpCharPointer = tmpCharPointer -> next;
        }
        tmpStrPointer = tmpStrPointer -> next;
    }
}
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
extern int screenCol, screenRow, screenNumY, tabWidth, wrapMod, isSaved;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;

int exitF(void){
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
        return 1;
    }
    if (!isSaved) {
        fprintf(stderr, "Файл не сохранен!\n");
        return 0;
    }
    
    return 1;
}

void readF(void){
    int i = 0, j = 0;
    char tempFileName[127] = "!";
    
    if (pararmetrs[i] == '"') {
        i++;
    }
    
    while (pararmetrs[i] != '\0') {
        tempFileName[j] = pararmetrs[i];
        i++;
        j++;
    }
    
    if(!initFile(tempFileName)){
        fprintf(stderr, "Нет такого файла!\n");
        return;
    }
}

void openF(void){
    int i = 0, j = 0;
    
    if (pararmetrs[i] == '"') {
        i++;
    }
    
    while (pararmetrs[i] != '\0') {
        fileName[j] = pararmetrs[i];
        i++;
        j++;
    }
    if(!initFile(fileName)){
        fprintf(stderr, "Нет такого файла!\n");
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
        fprintf(stderr, "Нет ассоциированного файла!\n");
        return;
    }
    
    FILE *outputFile = fopen(fileName, "wb");
    if (outputFile == NULL) {
        fprintf(stderr, "Нет такого файла!\n");
        return;
    }
    
    tmpStrPointer = pointerForStrings;
    
    while (tmpStrPointer != NULL) {
        tmpCharPointer = tmpStrPointer -> curString;
        while (tmpCharPointer != NULL) {
            fwrite(&(tmpCharPointer -> curChar), sizeof(char), 1, outputFile);
            tmpCharPointer = tmpCharPointer -> next;
        }
        tmpStrPointer = tmpStrPointer -> next;
    }
    isSaved = 1;
}
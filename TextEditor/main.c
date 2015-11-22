#include "textViewCmds.h"
#include "techCmds.h"
#include "stringEditCmds.h"
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


#define NUMOFCMD 16

char *comands[NUMOFCMD], *pararmetrs = NULL, *userString = NULL;
char fileName[127] = "!";
int tabWidth = 8, wrapMod = 1, screenNumY = 0, isFileSaved = 0, isSaved = 0;
struct listOfStrings *pointerForStrings;
struct listOfStrings *tmpStrPointer;
struct listOfChars *tmpCharPointer;
int screenCol = 80, screenRow = 25;

int main(int argc, char *argv[]){
    int exit = 0, returningInt, i = 0;
    //char temp;
    
    if (argc == 2){
        while (argv[1][i] != '\0') {
            fileName[i] = argv[1][i];
            i++;
        }
        initFile(fileName);
    }
    
    initCmd();
    
    while (!exit) {
        fprintf(stderr, "Editor: ");
        returningInt = readCmd();
        
        //fprintf(stderr, "%s", userString);
        switch (returningInt) {
            case 1: break;
                
            case 2:{
                fprintf(stderr, "Нарушено сочетание кавычек!\n");
                break;
            }
                
            case 3:{
                fprintf(stderr, "Переполнение памяти!\n");
                return 0;
            }
                
            default:
                break;
        }
        if ((returningInt == 1) || (returningInt == 2)) {
            continue;
        }
        
        returningInt = recognizeCmd();
        
        if (returningInt == -1) {
            fprintf(stderr, "Некорректная команда!\n");
            continue;
        }
        
        switch (returningInt) {
            case 0:{
                setTabWidth();
                break;
            }
            case 1:{
                printPages();
                clearOutBuffer();
                break;
            }
            case 2:{
                printRange();
                break;
            }
            case 3:{
                setWrap();
                clearOutBuffer();
                break;
            }
                
            case 4:{
                insertAfter();
                clearOutBuffer();
                break;
            }
            
            /*case 5:{
                editString();
                break;
             }
                
            case 6:{
                insertSymbol();
                break;
            }
            
            case 7:{
                replaceSubstring();
                break;
            }
            
            case 8:{
                deleteRange();
                break;
            }
                
            case 9:{
                deleteBraces();
                break;
            }*/
             
            case 10:{
                exit = exitF();
                break;
            }
             
            case 11:{
                readF();
                clearOutBuffer();
                break;
            }
             
            case 12:{
                openF();
                clearOutBuffer();
                break;
            }
             
            case 13:{
                writeF();
                //clearOutBuffer();
                break;
            }
             
            case 14:{
                setName();
                break;
            }
             
            /*case 15:{
                helpF();
                break;
            }*/
                
            default: break;
        }
    }
    return 0;
}
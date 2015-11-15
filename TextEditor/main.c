#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include "prog_brain.h"

#define NUMOFCMD 16

char *comands[NUMOFCMD], *pararmetrs = NULL, *userString = NULL;
char fileName[127];
int tabWidth = 8, wrapMod = 1, screenNumY = 0, isFileSaved = 0;
struct listOfStrings *pointerForStrings;
struct listOfStrings *tmpStrPointer;
struct listOfChars *tmpCharPointer;

int screenCol = 80, screenRow = 25;

int main(int argc, char *argv[]){
    int exit = 0, returningInt, i = 0;
    char temp;
    
    if (argc == 2){
        while (argv[1][i] != '\0') {
            fileName[i] = argv[1][i];
            i++;
        }
        initFile(fileName);
    }
    
    initCmd();
    
    while (!exit) {
        fprintf(stderr, "editor: ");
        returningInt = readCmd();
        switch (returningInt) {
            case 1: break;
                
            case 2:{
                fprintf(stderr, "Нарушено сочетание кавычек!\n");
                break;
            }
                
            case 3:{
                return 0;
                break;
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
            break;
        }
        
        switch (returningInt) {
            case 0:{
                setTabWidth();
                break;
            }
            case 1:{
                printPages();
                break;
            }
            case 2:{
                printRange();
                break;
            }
            case 3:{
                setWrap();
                break;
            }
                /*case 4:{
                 
                 break;
                 }
                 case 5:{
                 
                 break;
                 }
                 case 6:
                 
                 break;
                 case 7:
                 
                 break;
                 case 8:
                 <#statements#>
                 break;
                 case 9:
                 <#statements#>
                 break;
                 case 10:
                 <#statements#>
                 break;
                 case 11:
                 <#statements#>
                 break;
                 case 12:
                 <#statements#>
                 break;
                 case 13:
                 <#statements#>
                 break;
                 case 14:
                 <#statements#>
                 break;
                 case 15:
                 <#statements#>
                 break;*/
            default:
                break;
        }
    }
    return 0;
}
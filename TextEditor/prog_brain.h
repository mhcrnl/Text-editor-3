#ifndef prog_brain_h
#define prog_brain_h

int printStrings(char);
void clrscr(void);
void setTabWidth(void);
void moveTxtX(char dir);
void moveTxtY(char dir);
void printPages(void);
void printRange(void);
int setWrap(void);
void initCmd(void);
int initFile(char*);
int readCmd(void);
int changeTtyMod(int);
int recognizeCmd(void);

#endif

/*do{
 while(temp != '\n'){
 fread(&temp, sizeof(char), 1, inputFile);
 if (feof(inputFile)) {
 break;
 }
 tempNextChar = (struct listOfChars*)malloc(sizeof(struct listOfChars));
 if (tempNextChar == NULL) {
 fprintf(stderr, "Not enough memory\n");
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
 pointerForStrings -> next = NULL;
 return 0;
 }
 tempNextChar->next = NULL;
 tempPrevStr = tempNextStr;
 tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
 if (tempNextStr == NULL) {
 fprintf(stderr, "Not enough memory\n");
 exit (1);
 }
 tempNextStr->prev = tempPrevStr;
 tempPrevStr->next = tempNextStr;
 }while (1);
 
 tempNextStr->next = NULL;
 
 tmpStrPointer = pointerForStrings;
 tmpCharPointer = pointerForStrings -> curString;
 */

#ifndef assistFunctions_h
#define assistFunctions_h

int degree(int, int);
void clrscr(void);
void moveTxtY(char);
void moveTxtX(char);
void initCmd(void);
int initFile(char*);
int readCmd(void);
int changeTtyMod(int);
int recognizeCmd(void);
void clearOutBuffer(void);

#endif
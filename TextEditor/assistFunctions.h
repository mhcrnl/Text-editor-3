#ifndef assistFunctions_h
#define assistFunctions_h

int degree(int, int);
void clrscr(void);
void moveTxtY(char);
void moveTxtX(char);
void initCmd(void);
int initFile(char*);
int readCmd(void);
void setKeypress(void);
void resetKeypress(void);
int recognizeCmd(void);
void clearOutBuffer(void);

#endif
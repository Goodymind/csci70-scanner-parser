// Contract for main functions of scanner.c

#include "token.h"
#define FALSE 0
#define TRUE 1
int openfile(char *filename);
struct token gettoken();
int getlinenumber();
void generatetable();
const char *errormessage(int errnum);
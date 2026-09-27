// similar format to scanner lab

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"

// character clases
#define SPACE       0       // 
#define TAB         1
#define NEWLINE     2       // \n
#define DIGIT       3       // 0123456789
#define LETTER      4       // abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
#define UNDERSCORE  5       // _
#define PERIOD      6       // .
#define E           7       // e/E
#define PLUS        8       // +
#define MINUS       9       // -
#define DOUBLEQOUTE 10      // "
#define SLASH       11      // /
#define COLON       12      // :
#define EQUAL       13      // =
#define SEMICOLON   14      // ;
#define COMMA       15      // ,
#define ASTERISK    16      // *
#define LEFTANGLE   17      // <
#define RIGHTANGLE  18      // >
#define EXCLAMATION 19      // !
#define LEFTPAREN   20      // (
#define RIGHTPAREN  21      // )
#define EOF_C         22      // eof
#define OTHER       23      // other symbols that may be printable

// refer to DFA
int delta[32][24];

void generatetable() {
    for (int s = 0; s < 32; s++)
    {
        for (int c = 0; c < 24; c++) 
        {
            delta[s][c] = ERROR;
        }
    }
    // prev_state char      next_state
    delta[0][SPACE] =       0;
    delta[0][TAB] =         0;
    delta[0][NEWLINE] =     0;
    delta[0][LETTER] =      IDENTIFIER;
    delta[0][UNDERSCORE] =  IDENTIFIER;
    delta[0][DIGIT] =       NUMBER;
    delta[0][DOUBLEQOUTE] = 1;
    delta[0][SLASH] =       DIVIDE;
    delta[0][COLON] =       COLON_S;
    delta[0][SEMICOLON] =   SEMICOLON_S;
    delta[0][PLUS] =        PLUS_S;
    delta[0][MINUS] =       MINUS_S;
    delta[0][COMMA] =       COMMA_S;
    delta[0][ASTERISK] =    MULTIPLY;
    delta[0][LEFTANGLE] =   LESSTHAN;
    delta[0][EQUAL] =       EQUAL_S;
    delta[0][RIGHTANGLE] =  GREATERTHAN;
    delta[0][EXCLAMATION] = 4;
    delta[0][RIGHTPAREN] =  RIGHTPAREN_S;
    delta[0][LEFTPAREN] =   LEFTPAREN_S;
    delta[0][EOF_C] =       EOF_S;

    delta[IDENTIFIER][LETTER] = IDENTIFIER;
    delta[IDENTIFIER][DIGIT]  = IDENTIFIER;
    delta[IDENTIFIER][UNDERSCORE] = IDENTIFIER;
    
    delta[NUMBER][DIGIT]    = NUMBER;
    delta[NUMBER][E]        = 5;
    delta[NUMBER][PERIOD]   = DECIMAL;

    // TODO: FIX TOMROW (FINALIZE DFA)
}

#define MAXLINELEN  1000
FILE *file;
static int linenum = 1;
char line[MAXLINELEN];
int len = 0;
int ptr = 1;
int pushback = FALSE;
char charread = '\0';

int openfile(char *filename)
{
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File not found.");
        exit(1);
    }

    return 0;
}

char mygetchar()
{
    if (pushback)
    {
        pushback = FALSE;
    }
    else
    {
        charread = fgetc(file);
        if (charread == '\n')
        {
            linenum++;
        }
    }
    return charread;
}

int getlinenumber()
{
    return linenum;
}

int charclass(char c)
{
    if ((c >= '0') && (c <= '9')) return DIGIT;
    if ((c == 'e') || (c == 'E')) return E;
    if ((c >= 'a') && (c <= 'z')) return LETTER;
    if ((c >= 'A') && (c <= 'Z')) return LETTER;
    switch(c)
    {
        case  ' ': return SPACE;
        case '\r':
        case '\n': return NEWLINE;
        case '\t': return TAB;
        case  EOF: return EOF_C;
        case  '=': return EQUAL;
        case  '+': return PLUS;
        case  '-': return MINUS;
        case  '*': return ASTERISK;
        case  '<': return LEFTANGLE;
        case  '>': return RIGHTANGLE;
        case  '(': return LEFTPAREN;
        case  ')': return RIGHTPAREN;
        case  '/': return SLASH;
        case  '!': return EXCLAMATION;
        case  '.': return PERIOD;
        case  '"': return DOUBLEQOUTE;
        case  ',': return COMMA;
        case  ';': return SEMICOLON;
        default  : return OTHER;
    }
}

const char *errormessage(int errnum)
{

}

struct token gettoken()
{
    int state = 0, prevstate;
    struct token temp;
    temp.lexeme[0] = '\0';
    char buf[2] = {0, 0};

    do {
        char c = mygetchar();
        int ch = charclass(c);
        prevstate = state;
        state = delta[state][ch];

        if (state == 0) {
            temp.lexeme[0] = '\0';      // still skipping whitespace
        } else if (state != ERROR) {
            buf[0] = c;
            strcat(temp.lexeme, buf);
        }
    } while (state != ERROR && state != EOF_S);

    pushback = TRUE;      // put back the char that caused ERROR/final transition
    temp.id = prevstate;  // the last *valid* state is the token type
    return temp;
}
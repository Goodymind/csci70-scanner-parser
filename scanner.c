// similar format to scanner lab

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"

// character clases
#define SPACE 0 //
#define TAB 1
#define NEWLINE 2      // \n
#define DIGIT 3        // 0123456789
#define LETTER 4       // abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
#define UNDERSCORE 5   // _
#define PERIOD 6       // .
#define E 7            // e/E
#define PLUS 8         // +
#define MINUS 9        // -
#define DOUBLEQOUTE 10 // "
#define SLASH 11       // /
#define COLON 12       // :
#define EQUAL 13       // =
#define SEMICOLON 14   // ;
#define COMMA 15       // ,
#define ASTERISK 16    // *
#define LEFTANGLE 17   // <
#define RIGHTANGLE 18  // >
#define EXCLAMATION 19 // !
#define LEFTPAREN 20   // (
#define RIGHTPAREN 21  // )
#define EOF_C 22       // eof
#define OTHER 23       // other symbols that may be printable

// refer to DFA
int delta[NUMSTATES][24];

void generatetable()
{
    for (int s = 0; s < NUMSTATES; s++)
    {
        for (int c = 0; c < OTHER + 1; c++)
        {
            delta[s][c] = ERROR;
        }
    }
    // prev_state char      next_state
    // empty
    delta[0][SPACE] = 0;
    delta[0][TAB] = 0;
    delta[0][NEWLINE] = 0;

    // identifiers
    delta[0][LETTER] = IDENTIFIER;
    delta[0][E] = IDENTIFIER;
    delta[0][UNDERSCORE] = IDENTIFIER;
    delta[IDENTIFIER][LETTER] = IDENTIFIER;
    delta[IDENTIFIER][E] = IDENTIFIER;
    delta[IDENTIFIER][DIGIT] = IDENTIFIER;
    delta[IDENTIFIER][UNDERSCORE] = IDENTIFIER;

    // numbers
    delta[0][DIGIT] = NUMBER;
    delta[NUMBER][DIGIT] = NUMBER;
    delta[NUMBER][E] = 5;
    delta[NUMBER][PERIOD] = 6;
    delta[6][DIGIT] = DECIMAL;
    delta[DECIMAL][DIGIT] = DECIMAL;
    delta[DECIMAL][E] = 5;
    delta[5][PLUS] = 7;
    delta[5][MINUS] = 7;
    delta[5][DIGIT] = EXPONENTIAL;
    delta[7][DIGIT] = EXPONENTIAL;
    delta[EXPONENTIAL][DIGIT] = EXPONENTIAL;


    // String
    delta[0][DOUBLEQOUTE] = 1;
    for (int c = 0; c < OTHER + 1; c++)
    {
        delta[1][c] = 1; // any character included in string
    }
    delta[1][DOUBLEQOUTE] = STRING;
    delta[1][NEWLINE] = ERROR; // expected ' " ', string cannot be multiline
    delta[1][EOF_C] = ERROR;   // unterminated String

    // Divide/Comments
    delta[0][SLASH] = DIVIDE;
    delta[DIVIDE][SLASH] = 2;
    for (int c = 0; c < OTHER + 1; c++)
    {
        delta[2][c] = 2; // ignore everything
    }
    // Ignore Comments instead of recognizing it as a token
    delta[2][NEWLINE] = 0;
    delta[2][EOF_C] = EOF_S;

    // Assign
    delta[0][COLON] = COLON_S;
    delta[COLON_S][EQUAL] = ASSIGN;

    // Semicolon
    delta[0][SEMICOLON] = SEMICOLON_S;

    // Plus Minus and Comma
    delta[0][PLUS] = PLUS_S;
    delta[0][MINUS] = MINUS_S;
    delta[0][COMMA] = COMMA_S;

    // Multiply/Raise
    delta[0][ASTERISK] = MULTIPLY;
    delta[MULTIPLY][ASTERISK] = RAISE;

    // Comparison Operators
    // LT, LETEQUAL
    delta[0][LEFTANGLE] = LESSTHAN;
    delta[LESSTHAN][EQUAL] = LTEQUAL;

    // GT. GTEQUAL
    delta[0][RIGHTANGLE] = GREATERTHAN;
    delta[GREATERTHAN][EQUAL] = GTEQUAL;

    // Equal  Not Equal
    delta[0][EQUAL] = EQUAL_S;
    delta[0][EXCLAMATION] = 4;
    delta[4][EQUAL] = NOTEQUAL;

    // Parenthesis
    delta[0][LEFTPAREN] = LEFTPAREN_S;
    delta[0][RIGHTPAREN] = RIGHTPAREN_S;

    // end of file
    delta[0][EOF_C] = EOF_S;

    // unexpected characters
    delta[0][OTHER] = ERROR;
}

#define MAXLINELEN 1000
FILE *file;
static int linenum = 1;
char line[MAXLINELEN];
int len = 0;
int ptr = 1;
int pushback = FALSE;
static int lastch = 0;
char *_filename;

int openfile(char *filename)
{

    if (file != NULL)
    {
        // clean up old file
        fclose(file);
        file = NULL;
    }
    
    _filename = filename;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File not found.");
        exit(1);
    }

    // reset values after opening a new file
    pushback = FALSE;
    lastch = 0;
    linenum = 0;
    return 0;
}

// get next character from input file, or return EOF if at end of file
int mygetchar()
{
    // if pushback is true, return the last character read from the file
    if (pushback)
    {
        pushback = FALSE;
    }
    // else, read the next character from the file
    else
    {
        lastch = fgetc(file);
        if (lastch == '\n')
        {
            linenum++;
        }
    }
    return lastch;
}

// helper function
int getlinenumber()
{
    return linenum;
}

// returns id of character class of c, or OTHER if c is not a recognized character
int charclass(int c)
{
    if (c == EOF)
        return EOF_C;
    if ((c >= '0') && (c <= '9'))
        return DIGIT;
    if ((c == 'e') || (c == 'E'))
        return E;
    if ((c >= 'a') && (c <= 'z'))
        return LETTER;
    if ((c >= 'A') && (c <= 'Z'))
        return LETTER;
    switch (c)
    {
    case ' ':
        return SPACE;
    case '\r':
    case '\n':
        return NEWLINE;
    case '\t':
        return TAB;
    case EOF:
        return EOF_C;
    case '=':
        return EQUAL;
    case '+':
        return PLUS;
    case '-':
        return MINUS;
    case '*':
        return ASTERISK;
    case '<':
        return LEFTANGLE;
    case '>':
        return RIGHTANGLE;
    case '(':
        return LEFTPAREN;
    case ')':
        return RIGHTPAREN;
    case '/':
        return SLASH;
    case '!':
        return EXCLAMATION;
    case '.':
        return PERIOD;
    case '"':
        return DOUBLEQOUTE;
    case ',':
        return COMMA;
    case ';':
        return SEMICOLON;
    case ':':
        return COLON;
    default:
        return OTHER;
    }
}

// Errors defined in tokkennames array, return the error message for the given error number.
const char *errormessage(int errnum)
{
    // if the error number is not in the range of defined errors, return a generic error message
    // defined errors are from states 1 to 9 in the DFA.
    // undefined errors are generic errors.
    if (errnum < 1 || errnum > 9)
    {
        return "Undefined error.";
    }
    return tokennames[errnum];
}

// returns the next token from the input file, or an error token if an error is encountered
struct token gettoken()
{
    int state = 0, prevstate;
    struct token temp;
    temp.lexeme[0] = '\0';
    char buf[2] = {0, 0};
    char lastchar = '\0';
    do
    {
        int c = mygetchar();
        lastchar = c;
        int ch = charclass(c);
        prevstate = state;
        state = delta[state][ch];

        if (state == 0 || state == 2)
        {
            temp.lexeme[0] = '\0'; // skip whitespace and comments;
        }
        else if (state < ERROR && ch != EOF_C)
        {
            // append the character to the lexeme
            buf[0] = c;
            strcat(temp.lexeme, buf);
        }
    } while (state < ERROR);

    if (prevstate == 0)
    {
        // required because a random character after state 0 would make pushback true,
        // entering an infinite loop of errors.
        buf[0] = lastchar;
        temp.lexeme[0] = buf[0]; // consume the offending characcter
        temp.lexeme[1] = '\0';
        pushback = FALSE;
        temp.id = ERROR;
    }
    else if (prevstate < 10)
    {
        // defined errors
        pushback = TRUE;
        temp.id = prevstate;
        printf("Error at line %d: %s \"%s\"\n", getlinenumber(), errormessage(temp.id), temp.lexeme);
    }
    else if (prevstate == IDENTIFIER)
    {
        pushback = TRUE;
        // check if identifier is used as a keyword, if so, 
        // return the keyword token id instead of identifier token id.
        int keyword = check_keyword(temp.lexeme);
        if (keyword == -1)
        {
            temp.id = prevstate; // just an identifier
        }
        else
        {
            temp.id = keyword; // keyword token id: IF, ELSE, ENDIF, etc...
        }
    }
    else
    {
        pushback = TRUE;
        temp.id = prevstate;
    }

    // Decimals and Exponentials are considered as numbers, not separate tokens.
    if (prevstate == DECIMAL || prevstate == EXPONENTIAL)
    {
        temp.id = NUMBER;
    }

    return temp;
}
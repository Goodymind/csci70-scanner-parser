#include <string.h>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"

void parseerror(char *message)
{
    printf("Error: %s\n", message);
    exit(0);
}

struct token currenttoken;

void match(int expected)
{
    if (currenttoken.id == expected)
    {
        currenttoken = gettoken();
    }
    else
    {
        printf("syntax error at line %d: expected %s, got id=%d (%s)",
               getlinenumber(), tokennames[expected], currenttoken.id, tokennames[currenttoken.id]);
        exit(1);
    }
}

void prg()
{
    blk();
    match(EOF_S);
}

void blk()
{
    if (currenttoken.id == IDENTIFIER || currenttoken.id == PRINT || currenttoken.id == IF)
    {
        {
            stm();
            blk();
        }
    }
}
    void stm()
    {
        switch (currenttoken.id)
        {
            case IDENTIFIER: match(IDENTIFIER); match(ASSIGN); exp(); match(SEMICOLON_S);
            case PRINT: match(LEFTPAREN_S); arg(); argfollow(); match(RIGHTPAREN_S); match(SEMICOLON_S);
            case IF: match(IF); cnd(); match(COLON_S); blk(); iffollow();
        }
    }

    void parse(char *filename)
    {
        openfile(filename);
        printf("Starting Parse...\n");
        generatetable();

        return prg();
    }
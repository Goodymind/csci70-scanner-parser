#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "scan.h"
#include "parse.h"
#include "token.h"

char *fname;
int valid;
int errors;

struct token currenttoken;

void match(int expected)
{
    if (currenttoken.id == expected)
    {
        currenttoken = gettoken();
    }
    else
    {
        // print error message and set valid to 0
        printf("Syntax error at line %d: expected %s, got id=%d (%s)\n",
               getlinenumber(), tokennames[expected], currenttoken.id, tokennames[currenttoken.id]);
        valid = 0;
        errors++;
        exit(1);
    }
}

void prg()
{
    valid = 1;
    errors = 0;
    blk();
    match(EOF_S);
    if (valid)
    {
        printf("%s is a valid SimpCalc program\n", fname);
    }
    else
    {
        printf("%s is not a valid SimpCalc program\n", fname);
    }
}

void blk()
{
    if (currenttoken.id == IDENTIFIER || currenttoken.id == PRINT || currenttoken.id == IF)
    {

        stm();
        blk();
    }
    else
    {
        // epsilon
    }
}

void stm()
{
    int before = errors;
    switch (currenttoken.id)
    {
    case IDENTIFIER:
        match(IDENTIFIER);
        match(ASSIGN);
        exp();
        match(SEMICOLON_S);
        if (errors == before)
        {
            printf("Assignment Statement Recognized\n");
        }
        break;

    case PRINT:
        match(PRINT);
        match(LEFTPAREN_S);
        arg();
        argfollow();
        match(RIGHTPAREN_S);
        match(SEMICOLON_S);
        if (errors == before)
        {
            printf("Print Statement Recognized\n");
        }
        break;

    case IF:
        printf("If Statement Begins\n");
        match(IF);
        cnd();
        match(COLON_S);
        blk();
        iffollow();
        printf("If Statement Ends\n");
        break;

    default:
        printf("Invalid Statement\n");
        valid = 0;
        errors++;
        exit(1);
        break;
    }
}

void argfollow()
{
    if (currenttoken.id == COMMA_S)
    {
        match(COMMA_S);
        arg();
        argfollow();
    }
    else
    {
        // epsilon
    }
}

void arg()
{
    if (currenttoken.id == STRING)
    {
        match(STRING);
    }
    else
    {
        exp();
    }
}

void iffollow()
{
    if (currenttoken.id == ENDIF)
    {
        match(ENDIF);
        match(SEMICOLON_S);
    }
    else if (currenttoken.id == ELSE)
    {
        int before = errors;
        match(ELSE);
        blk();
        match(ENDIF);
        match(SEMICOLON_S);
        if (errors != before)
        {
            printf("Incomplete if Statement\n");
        } 
    }
    else
    {
        printf("Incomplete if Statement\n");
        valid = 0;
        errors++;
        exit(1);
    }
}

void exp()
{
    trm();
    trmfollow();
}

void trmfollow()
{
    if (currenttoken.id == PLUS_S)
    {
        match(PLUS_S);
        trm();
        trmfollow();
    }
    else if (currenttoken.id == MINUS_S)
    {
        match(MINUS_S);
        trm();
        trmfollow();
    }
    else
    {
        // epsilon
    }
}

void trm()
{
    fac();
    facfollow();
}

void facfollow()
{
    if (currenttoken.id == MULTIPLY)
    {
        match(MULTIPLY);
        fac();
        facfollow();
    }
    else if (currenttoken.id == DIVIDE)
    {
        match(DIVIDE);
        fac();
        facfollow();
    }
    else
    {
        // epsilon
    }
}

void fac()
{
    lit();
    litfollow();
}

void litfollow()
{
    if (currenttoken.id == RAISE)
    {
        match(RAISE);
        lit();
        litfollow();
    }
    else
    {
        // epsilon
    }
}

void lit()
{
    if (currenttoken.id == MINUS_S)
    {
        match(MINUS_S);
        val();
    }
    else
    {
        val();
    }
}

void val()
{
    switch (currenttoken.id)
    {
    case IDENTIFIER:
        match(IDENTIFIER);
        break;

    case NUMBER:
        match(NUMBER);
        break;

    case SQRT:
        match(SQRT);
        match(LEFTPAREN_S);
        exp();
        match(RIGHTPAREN_S);
        break;

    case LEFTPAREN_S:
        match(LEFTPAREN_S);
        exp();
        match(RIGHTPAREN_S);
        break;

    default:
        printf("Invalid Value\n");
        valid = 0;
        errors++;
        exit(1);
        break;
    }
}

void cnd()
{
    exp();
    rel();
    exp();
}

void rel()
{
    switch (currenttoken.id)
    {
    case LESSTHAN:
        match(LESSTHAN);
        break;

    case EQUAL_S:
        match(EQUAL_S);
        break;

    case GREATERTHAN:
        match(GREATERTHAN);
        break;

    case LTEQUAL:
        match(LTEQUAL);
        break;

    case NOTEQUAL:
        match(NOTEQUAL);
        break;

    case GTEQUAL:
        match(GTEQUAL);
        break;

    default:
        printf("Missing relational operator\n");
        valid = 0;
        errors++;
        exit(1);
        break;
    }
}

void parse(char *filename)
{
    fname = filename;
    openfile(filename);
    generatetable();
    generatetokennames();
    currenttoken = gettoken();
    prg();
}
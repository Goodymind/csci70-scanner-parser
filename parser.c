#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "token.h"

char *fname;
int valid;

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
        // printf("success! %s is a %s\n", currenttoken.lexeme, tokennames[currenttoken.id]);
        currenttoken = gettoken();
    }
    else
    {
        printf("syntax error at line %d: expected %s, got id=%d (%s)\n",
               getlinenumber(), tokennames[expected], currenttoken.id, tokennames[currenttoken.id]);
        valid = 0;
        // exit(1);
    }
}

void prg()
{
    // printf("prg: over here...\n");
    valid = 1;
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
    // printf("stm: %s\n", tokennames[currenttoken.id]);
    switch (currenttoken.id)
    {
    case IDENTIFIER:
        match(IDENTIFIER);
        match(ASSIGN);
        exp();
        match(SEMICOLON_S);
        printf("Assignment Statement Recognized\n");
        break;

    case PRINT:
        match(PRINT);
        match(LEFTPAREN_S);
        arg();
        argfollow();
        match(RIGHTPAREN_S);
        match(SEMICOLON_S);
        printf("Print Statement Recognized\n");
        break;

    case IF:
        printf("If Statement Recognized\n");
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
        break;
        // exit(1);
    }
}

void argfollow()
{
    // printf("argfollow %s\n", tokennames[currenttoken.id]);
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
    // printf("arg %s\n", tokennames[currenttoken.id]);
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
    // printf("iffollow %s\n", tokennames[currenttoken.id]);
    if (currenttoken.id == ENDIF)
    {
        match(ENDIF);
        match(SEMICOLON_S);
    }
    else if (currenttoken.id == ELSE)
    {
        match(ELSE);
        blk();
        match(ENDIF);
        match(SEMICOLON_S);
    }
    else
    {
        printf("Incomplete IF Statement\n");
        valid = 0;
        // exit(1);
    }
}

void exp()
{
    // printf("exp %s\n", tokennames[currenttoken.id]);
    trm();
    trmfollow();
}

void trmfollow()
{
    // printf("trmfollow %s\n", tokennames[currenttoken.id]);
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
    // printf("trm %s\n", tokennames[currenttoken.id]);
    fac();
    facfollow();
}

void facfollow()
{
    // printf("facfollow %s\n", tokennames[currenttoken.id]);
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
    // printf("fac %s\n", tokennames[currenttoken.id]);
    lit();
    litfollow();
}

void litfollow()
{
    // printf("litfollow %s\n", tokennames[currenttoken.id]);
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
    // printf("lit %s\n", tokennames[currenttoken.id]);
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
    // printf("val %s\n", tokennames[currenttoken.id]);
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
        break;
        // exit(1);
    }
}

void cnd()
{
    // printf("cnd %s\n", tokennames[currenttoken.id]);
    exp();
    rel();
    exp();
}

void rel()
{
    // printf("rel %s\n", tokennames[currenttoken.id]);
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
        break;
        // exit(1);
    }
}

void parse(char *filename)
{
    fname = filename;
    openfile(filename);
    generatetable();
    generatetokennames();
    printf("Starting Parse...\n");
    currenttoken = gettoken();
    prg();
}
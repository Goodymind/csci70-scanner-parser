#include <string.h>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "token.h"

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

        stm();
        blk();
    }
    else
    {
        //epsilon
    }
}

void stm()
{
    switch (currenttoken.id)
    {
    case IDENTIFIER:
        match(IDENTIFIER);
        match(ASSIGN);
        exp();
        match(SEMICOLON_S);

    case PRINT:
        match(PRINT);
        match(LEFTPAREN_S);
        arg();
        argfollow();
        match(RIGHTPAREN_S);
        match(SEMICOLON_S);

    case IF:
        match(IF);
        cnd();
        match(COLON_S);
        blk();
        iffollow();
    }

    default:
        printf("Invalid Statement\n");
        exit(1);
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
        match(ELSE);
        blk();
        match(ENDIF);
        match(SEMICOLON_S);
    }
    else
    {
        printf("Incomplete IF Statement\n");
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
    if (currenttoken.id == PLUS)
    {
        match(PLUS);
        trm();
        trmfollow();
    }
    else if (currenttoken.id == MINUS)
    {
        match(MINUS);
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
    if (currenttoken.id == MINUS)
    {
        match(MINUS);
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

        case NUMBER:
            match(NUMBER);

        case SQRT:
            match(SQRT);
            match(LEFTPAREN_S);
            exp();
            match(RIGHTPAREN_S);

        case LEFTPAREN_S:
            match(LEFTPAREN_S);
            exp();
            match(RIGHTPAREN_S);

        default:
            printf("Invalid Value\n");
            exit(1);
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

        case EQUAL:
            match(EQUAL);

        case GREATERTHAN:
            match(GREATERTHAN);

        case LTEQUAL:
            match(LTEQUAL);

        case NOTEQUAL:
            match(NOTEQUAL);
            
        case GTEQUAL:
            match(GTEQUAL);

        default:
            printf("Missing relational operator\n");
            exit(1);
    }
}

void parse(char *filename)
{
    openfile(filename);
    printf("Starting Parse...\n");
    generatetable();

    return prg();
}
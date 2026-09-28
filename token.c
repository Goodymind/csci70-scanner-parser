#include "token.h"
#include "string.h"

const char *tokennames[NUMSTATES] = {NULL};

void generatetokennames() {
    tokennames[IDENTIFIER] = "Identifier";
    tokennames[NUMBER] = "Number";
    tokennames[ASSIGN] = "Assign";
    tokennames[DECIMAL] = "Decimal";
    tokennames[EXPONENTIAL] = "Exponential";
    tokennames[STRING] = "String";
    tokennames[COMMENT] = "Comment";
    tokennames[SEMICOLON_S] = "Semicolon";
    tokennames[PLUS_S] = "Plus";
    tokennames[COLON_S] = "Colon";
    tokennames[MINUS_S] = "Minus";
    tokennames[COMMA_S] = "Comma";
    tokennames[MULTIPLY] = "Multiply";
    tokennames[RAISE] = "Raise";
    tokennames[DIVIDE] = "Divide";
    tokennames[LESSTHAN] = "LessThan";
    tokennames[LTEQUAL] = "LTEqual";
    tokennames[EQUAL_S] = "Equal";
    tokennames[GREATERTHAN] = "GreaterThan";
    tokennames[GTEQUAL] = "GTEqual";
    tokennames[NOTEQUAL] = "NotEqual";
    tokennames[LEFTPAREN_S] = "LeftParen";
    tokennames[RIGHTPAREN_S] = "RightParen";
    tokennames[EOF_S] = "EOF";
    tokennames[ERROR] = "Error: Unexpected Character";
    // TODO: Finish this

    // keywords
    tokennames[IF] = "If";
    tokennames[ELSE] = "Else";
    tokennames[ENDIF] = "EndIf";
    tokennames[PRINT] = "Print";
    tokennames[SQRT] = "Sqrt";
    tokennames[AND] = "And";
    tokennames[OR] = "Or";
    tokennames[NOT] = "Not";
}

const char *keywords[] = {
    "PRINT",
    "IF",
    "ELSE",
    "ENDIF",
    "SQRT",
    "AND",
    "OR",
    "NOT"
};

int check_keyword(char *iden) {
    int length = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < length; i++ )
    {
        if (strcmp(keywords[i], iden) == 0) return IF + i;
    }
    // TODO: Check for Correctness
    return -1;
}
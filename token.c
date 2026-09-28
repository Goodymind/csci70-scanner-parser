#include "token.h"
#include "string.h"

const char *tokennames[NUMSTATES] = {NULL};

void generatetokennames() {
    tokennames[IDENTIFIER] = "Identifier";
    tokennames[NUMBER] = "Number";
    tokennames[ASSIGN] = "Assign";
    // TODO: Finish this
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

int check_identifier(char *iden) {
    int length = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < length; i++ )
    {
        if (strcmp(keywords[i], iden) == 0) return i;
    }
    // TODO: Check for Correctness
    return -1;
}
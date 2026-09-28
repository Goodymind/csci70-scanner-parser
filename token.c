#include "token.h"
#include "string.h"

const char *tokennames[NUMSTATES];

void generatetokennames() {
    strcpy(tokennames[IDENTIFIER], "IDENTIFIER");
    strcpy(tokennames[NUMBER], "NUMBER");
}
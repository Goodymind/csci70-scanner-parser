#include "token.h"
#include "string.h"

const char *tokennames[NUMSTATES];

void generatetokennames() {
    strcpy(tokennames[IDENTIFIER], "IDENTIFIER");
    strcpy(tokennames[NUMBER], "NUMBER");
    // TODO: implement token names (and call this function)
    // Not sure if this is valid in C but we'll see -Alinus
}
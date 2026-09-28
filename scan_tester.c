#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"

int main(int argc, char** argv)
{
    printf("hello?\n");
    char filename[60];
    strcpy(filename, "sample/sample1-quad-formula.txt");
    openfile(filename);
    printf("Starting...\n");
    printf("Input: %s\n", filename);
    printf("Identifier\tDiscriminant\n");
    generatetable();
    generatetokennames();
    struct token t = gettoken();
    while (t.id != EOF_S)
    {
        // TODO: Call tokennames here
        printf("%s\t\t%s\n", tokennames[t.id], t.lexeme);
        t = gettoken();
    }
    return 0;
}
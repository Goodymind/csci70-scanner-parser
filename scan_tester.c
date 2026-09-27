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
    printf("Identifier\t\tDiscriminant\n");
    generatetable();
    struct token t = gettoken();
    while (t.id != EOF_S)
    {
        printf("%d\t\t%s\n", t.id, t.lexeme);
        t = gettoken();
    }
    return 0;
}
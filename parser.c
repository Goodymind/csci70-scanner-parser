#include <string.h>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"

void prg()
{
    blk();
    return EOF;
}

void blk()
{

}

void parse(char *filename)
{
    openfile(filename);
    printf("Starting Parse...\n");
    generatetable();

    return prg();
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scan.h"
#include "dirent.h"

// helper function that checks if input in the string exists.
int find_input(char *str)
{
    char *pos = strstr(str, "input");
    if (pos == NULL)
        return -1;
    return (int)(pos - str);
}

// helper function that checks if output in the string exists.
int find_output(char *str)
{
    char *pos = strstr(str, "output");
    if (pos == NULL)
        return -1;
    return (int)(pos - str);
}

// checks if it is a txt file (ends in .txt)
int is_txt_file(char *str)
{
    size_t len = strlen(str);
    if (len <= 4)
        return 0;
    return strcmp(str + len - 4, ".txt") == 0;
}

// checks if the file is an input file by
// - validating if it is a txt file
// - it does not contain output.
int isinputfile(char *filename)
{
    size_t len = strlen(filename);
    if (len <= 4)
        return 0;
    if (find_output(filename) > -1)
        return 0;

    return is_txt_file(filename);
}

// build the output filename by replacing "input" with "scanner_output" in the input filename
char *buildoutputfilename(int inputindex, char *filename)
{
    // if the word input is in the filename, replace with scanner_output.
    if (inputindex > -1)
    {
        const char *outputstr = "scanner_output";
        const size_t in_len = 5; // strlen("input")

        size_t len = strlen(filename) + 1;    // includes '\0'
        size_t outputlen = strlen(outputstr); // NO null terminator

        char *buf = malloc(len - in_len + outputlen);
        if (buf == NULL)
            return NULL;

        memcpy(buf, filename, inputindex);
        memcpy(buf + inputindex, outputstr, outputlen);
        memcpy(buf + inputindex + outputlen,
               filename + inputindex + in_len,
               len - inputindex - in_len);

        return buf; // freed by caller
    }
    // otherwise, append scanner_output to the filename before the .txt extension.
    else
    {
        
        const char *outputstr = "_scanner_output";
        
        size_t len = strlen(filename) + 1;      // include \0
        size_t outputlen = strlen(outputstr);   // do not include \0
        
        int target = len - 5; ".txt0";

        char *buf = malloc(len + outputlen);

        if (buf == NULL) return NULL;

        memcpy(buf, filename, target);
        memcpy(buf + target, outputstr, outputlen);
        memcpy(buf + target + outputlen,
                filename + target, 5 );

        return buf;
    }
}

// run scanner module on the file.
int scan(char *filename)
{
    printf("Input: %s\n", filename);
    printf("%-40s%-40s\n", "Identifier", "Discriminant");
    openfile(filename);
    generatetable();
    generatetokennames();
    struct token t = gettoken();
    while (t.id != EOF_S)
    {
        printf("%-40s%-40s\n", tokennames[t.id], t.lexeme);
        t = gettoken();
    }
    return 0;
}

int main(int argc, char **argv)
{
    DIR *dir = opendir(".");

    if (dir == NULL)
    {
        perror("Unable to open directory");
        return 0;
    }

    struct dirent *de;

    // get all files/folders in the current directory
    while ((de = readdir(dir)) != NULL)
    {
        // check if file is an input file.
        if (isinputfile(de->d_name))
        {
            int inputstringindex = find_input(de->d_name);
            char *output = buildoutputfilename(inputstringindex, de->d_name);
            // redirect print stream to output file.
            if (freopen(output, "w", stdout) == NULL)
            {
                perror("Failed to redirect stdout");
            }

            scan(de->d_name);
            // free the output filename buffer.
            free(output);
        }
    }
    closedir(dir);
    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "dirent.h"
#include "parser.h"

int find_input(char *str)
{
    char *pos = strstr(str, "input");
    if (pos == NULL)
        return -1;
    return (int)(pos - str);
}

int find_output(char *str)
{
    char *pos = strstr(str, "output");
    if (pos == NULL)
        return -1;
    return (int)(pos - str);
}

int is_txt_file(char *str)
{
    size_t len = strlen(str);
    if (len <= 4)
        return 0;
    return strcmp(str + len - 4, ".txt") == 0;
}

int isinputfile(char *filename)
{
    size_t len = strlen(filename);
    if (len <= 4)
        return 0;
    if (find_output(filename) > -1)
        return 0;

    return is_txt_file(filename);
}

char *buildoutputfilename(int inputindex, char *filename)
{
    if (inputindex > -1)
    {
        const char *outputstr = "parser_output";
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
    else
    {

        const char *outputstr = "_parser_output";

        size_t len = strlen(filename) + 1;    // include \0
        size_t outputlen = strlen(outputstr); // do not include \0

        int target = len - 5;
        // ".txt0";

        char *buf = malloc(len + outputlen);

        if (buf == NULL)
            return NULL;

        memcpy(buf, filename, target);
        memcpy(buf + target, outputstr, outputlen);
        memcpy(buf + target + outputlen,
               filename + target, 5);

        return buf;
    }
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

    while ((de = readdir(dir)) != NULL)
    {
        if (isinputfile(de->d_name))
        {
            int inputstringindex = find_input(de->d_name);

            char *output =
                buildoutputfilename(inputstringindex, de->d_name);

            if (output == NULL)
            {
                perror("Failed to create output filename");
                continue;
            }

            if (freopen(output, "w", stdout) == NULL)
            {
                perror("Failed to redirect stdout");
                free(output);
                continue;
            }

            parse(de->d_name);

            free(output);
        }
    }

    closedir(dir);

    return 0;
}
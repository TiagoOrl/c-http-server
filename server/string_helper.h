#ifndef H_STRING_HELPER
#define H_STRING_HELPER

#include <stdbool.h>
#include <stdlib.h>

bool is_space(char c)
{
    return c == ' ';
}


bool is_newline(char c)
{
    return c == '\n';
}

char* get_substring(const char* str, unsigned int start, unsigned int end)
{
    unsigned int length = end - start;
    char* substring = (char*)calloc(length + 1, sizeof(char));

    for (int i = 0; i <= length; i++)
    {
        substring[i] = str[start + i];
    }

    return substring;
}



#endif
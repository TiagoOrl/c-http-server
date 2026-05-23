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

bool is_carriage(char c)
{
    return c == '\r';
}

char* get_substring(const char* str, int start, int end)
{
    int length = end - start + 1;

    if (length < 1)
        return NULL;
        
    char* substring = (char*)calloc(length, sizeof(char));

    for (int i = 0; i < length; i++)
    {
        substring[i] = str[start + i];
    }

    return substring;
}



#endif
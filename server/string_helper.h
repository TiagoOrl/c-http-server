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


list get_substrings(char* str, int size, char separator)
{
    list tokens = l_new();
    int i = 0;
    int start = 0;
    int end = 0;
    while (i < size)
    {
        if (str[i] == separator || str[i] == '\0')  // \0 to check if reached end of str
        {
            end = i - 1;
            char* substring = get_substring(str, start, end);
            if (substring == NULL)
            {
                i++;
                continue;
            }
            l_push(&tokens, substring, end - start + 1);
            free(substring);
            start = i + 1;
        }
        i++;
    }

    return tokens;
}



#endif
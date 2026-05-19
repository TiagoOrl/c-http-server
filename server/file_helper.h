#ifndef H_FILE_NET
#define H_FILE_NET


#include <stdio.h>
#include <stdlib.h>


char * file_read(const char* path, const char* mode)
{
    FILE *fp = fopen(path, mode);

    if (fp == NULL)
    {   
        perror("error opening file\n");
        return 0;
    }

    fseek(fp, 0L, SEEK_END);
    long buff_size = ftell(fp);
    rewind(fp);

    char* buffer = (char*) calloc(buff_size + 1, sizeof(char));

    size_t new_len = fread(buffer, sizeof(char), buff_size, fp);
    if (ferror(fp) != 0)
        fputs("error reading the file\n", fp);
    else
        buffer[new_len] = '\0';

    fclose(fp);
    return buffer;
}

#endif
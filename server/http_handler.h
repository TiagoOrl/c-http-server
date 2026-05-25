#ifndef H_HTTP_HANDLER
#define H_HTTP_HANDLER

#include "parser_http.h"
#include "file_helper.h"



void* server_client_conn_thread(void* arg)
{
    char in_buffer[1024];
    int* fd = (int*)arg;

    memset(in_buffer, 0, sizeof(in_buffer));

    size_t res_read = read(*fd, in_buffer, sizeof(in_buffer) - 1);

    if (res_read <= 0)
    {
        printf("Client disconnected.\n");
        close(*fd);
        return NULL;
    }

    struct http_req header = parse(in_buffer, strnlen(in_buffer, 1024));

    send(*fd, header.res_header_buff, header.res_header_len, 0);
    send(*fd, header.res_resource, header.res_resource_len, 0);

    close(*fd);
    free(header.res_resource);

    return NULL;
}


#endif
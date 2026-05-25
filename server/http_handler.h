#ifndef H_HTTP_HANDLER
#define H_HTTP_HANDLER

#include "parser_http.h"
#include "file_helper.h"

#define MAX_BUFFER_SIZE 25000


/**
 * handles the request and returns the file data requested in header, returns 0 if not found
 */
char* handler_handle_request(struct http_req header, char** res_data)
{
    const char* res_path_format = "./assets%s";
    char path[150];

    snprintf(path, sizeof(path), res_path_format, header.resource_loc);
    *res_data = file_read(path, "rb");
}


void* server_client_conn_thread(void* arg)
{
    char in_buffer[1024];
    int* fd = (int*)arg;
    char response_buffer[1024] = {0};
    struct http_req header;
    char* resource_data = NULL;

    memset(in_buffer, 0, sizeof(in_buffer));
    header.connection = KEEP_ALIVE;

    size_t res_read = read(*fd, in_buffer, sizeof(in_buffer) - 1);

    if (res_read <= 0)
    {
        printf("Client disconnected.\n");
        close(*fd);
        return NULL;
    }

    header = parse(in_buffer, strnlen(in_buffer, 1024));

    handler_handle_request(header, &resource_data);

    if (resource_data == NULL)
    {
        close(*fd);
        return NULL;
    }
        
    

    size_t html_len = strnlen(resource_data, MAX_BUFFER_SIZE);
    

    int header_len = snprintf(response_buffer, sizeof(response_buffer),
        http_header_res,
        200,
        "OK",
        "text/html",
        html_len
    );

    send(*fd, response_buffer, header_len, 0);
    send(*fd, resource_data, html_len, 0);

    close(*fd);
    free(resource_data);

    return NULL;
}


#endif
#ifndef H_HTTP_PARSER
#define H_HTTP_PARSER

#include <stdbool.h>
#include <string.h>
#include "list/list.h"
#include "string_helper.h"


// GET /favicon.ico HTTP/1.1
// Host: 127.0.0.1:9090
// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:150.0) Gecko/20100101 Firefox/150.0
// Accept: image/avif,image/webp,image/png,image/svg+xml,image/*;q=0.8,*/*;q=0.5
// Accept-Language: en-US,en;q=0.9
// Accept-Encoding: gzip, deflate, br, zstd
// Connection: keep-alive
// Referer: http://127.0.0.1:9090/
// Sec-Fetch-Dest: image
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Site: same-origin
// Priority: u=6


enum request {
    GET, POST, PUT, DELETE, OTHER
};

enum connection {
    UNDEF, CLOSE, KEEP_ALIVE
};

struct http_req {
    enum request request;
    enum connection connection;
    char resource_loc[80];
};



void set_request_type(const char* type_str, struct http_req* header)
{   
    header->request = OTHER;

    if (strncmp(type_str, "GET", 3) == 0)
        header->request = GET;
    
    if (strncmp(type_str, "POST", 4) == 0)
        header->request = POST;
    
    if (strncmp(type_str, "PUT", 3) == 0)
        header->request = PUT;
    
    if (strncmp(type_str, "DELETE", 6) == 0)
        header->request = DELETE;

}



/**
 * converts the http header request string into a list
 */
List* parser_convert_to_list(const char* str, int size)
{
    List* header_list = l_newList();

    int i = 0;
    int start = 0;
    int end = 0;
    while (i < size)
    {   
        if (is_carriage(str[i]) && is_newline(str[i + 1]))
        {
            end = i - 1; // i - 1 to not include control byte
            char * substring = get_substring(str, start, end);
            if (substring == NULL)
            {
                i++;
                continue;
            }
                
            l_push(header_list, substring);

            start = i + 2;
            i += 2; // jump 2 positions ahead over carriage and newline bytes
            continue;
        }
        i++;
    }

    return header_list;
}


List* get_tokens(char* str, unsigned int size)
{
    List* tokens = l_newList();
    int i = 0;
    int start = 0;
    int end = 0;
    while (i < size)
    {
        if (is_space(str[i]) || str[i] == '\0')
        {
            end = i - 1;
            char* substring = get_substring(str, start, end);
            if (substring == NULL)
            {
                i++;
                continue;
            }
            l_push(tokens, substring);
            start = i + 1;
        }
        i++;
    }

    return tokens;
}


void set_connection_type(List* list, struct http_req* header)
{
    Node* it = list->top;

    while(it != NULL)
    {
        List* key_value = get_tokens(it->data, it->size);
        Node* key = key_value->bottom;

        if (strncmp(key->data, "Connection:", key->size) == 0)
        {
            Node* value = key_value->bottom->prev;

            if (strncmp(value->data, "keep-alive", 10)  == 0)
                header->connection = KEEP_ALIVE;
            
            if (strncmp(value->data, "closed", 6)  == 0)
                header->connection = CLOSE;

            if (strncmp(value->data, "close", 5)  == 0)
                header->connection = CLOSE;

        }
        l_free_list(key_value);
        it = it->next;
    }
}


struct http_req parse(const char* str, int size)
{
    struct http_req header;
    memset(header.resource_loc, 0, sizeof(header.resource_loc));

    List* h_list = parser_convert_to_list(str, size);
    List* req_list = get_tokens(h_list->bottom->data, h_list->bottom->size);


    l_print_simple(h_list);
    
    set_request_type(req_list->bottom->data, &header);
    set_connection_type(h_list, &header);
    strncpy(header.resource_loc, req_list->bottom->prev->data, req_list->bottom->prev->size);
    l_free_list(req_list);

}


#endif
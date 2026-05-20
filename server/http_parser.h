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


enum req_types {
    GET, POST, PUT, DELETE, OTHER
};

enum conn_type {
    KEEP_ALIVE, CLOSE
};

struct http_req {
    enum req_types req_type;
    char resource_loc[100];
    enum conn_type conn_type;
};



enum req_types get_request_type(const char* type_str)
{
    if (strncmp(type_str, "GET", 3) == 0)
        return GET;
    
    if (strncmp(type_str, "POST", 4) == 0)
        return POST;
    
    if (strncmp(type_str, "PUT", 3) == 0)
        return PUT;
    
    if (strncmp(type_str, "DELETE", 6) == 0)
        return DELETE;

    return OTHER;
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
        if (is_newline(str[i]))
        {
            end = i;
            l_push(header_list, get_substring(str, start, end - 1));    // end - 1 to not include newline byte
            start = i + 1;
        }
        i++;
    }

    return header_list;
}


struct http_req parse(const char* str, int size)
{
    bool expect_req_type = true;
    bool expect_resource_loc = false;
    bool expect_conn_type = false;
    struct http_req request_header;

    List* header_list = parser_convert_to_list(str, size);

    l_print_simple(header_list);

    int i = 0;
    while (i < size)
    {
        if (expect_req_type)
        {
            char type[10] = {0};
            int j = 0;
            while(!is_space(str[i]))
            {
                type[j] = str[i];
                i++;
                j++;
            }
            request_header.req_type = get_request_type(type);
            expect_req_type = false;
            expect_resource_loc = true;
        }

        i++;
    }
}


#endif
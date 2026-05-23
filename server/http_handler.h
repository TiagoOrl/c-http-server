#ifndef H_HTTP_HANDLER
#define H_HTTP_HANDLER

#include "parser_http.h"
#include "file_helper.h"

/**
 * handles the request and returns the file data requested in header, returns 0 if not found
 */
char* handler_handle_request(struct http_req header)
{
    const char* res_path_format = "./assets%s";
    char* res_data = NULL;
    char path[150];

    // printf("%s\n", header.resource_loc);

    if (header.resource_loc[0] == '/' && header.resource_loc[1] == 0)
    {
        printf("get page.html\n");
        return file_read("./assets/page.html", "rb");
    }

    snprintf(path, sizeof(path), res_path_format, header.resource_loc);
    printf("get %s\n", path);

    res_data = file_read(path, "rb");


    return res_data;
}

#endif
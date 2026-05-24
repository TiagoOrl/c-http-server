#ifndef H_HTTP
#define H_HTTP


/**
 * Content-Type
 * text/html for html page
 * text/css for css
 * application/javascript for script
 */

const char* http_header_res = 
    "HTTP/1.1 %d %s\r\n"
    "Content-Type: %s; charset=UTF-8\r\n"
    "Content-Length: %zu\r\n"
    "Connection: closed\r\n\r\n";

#endif




#ifndef H_HTTP
#define H_HTTP

const char* http_header_res = 
    "HTTP/1.1 %d %s\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "Content-Length: %zu\r\n"
    "Connection: closed\r\n\r\n";

#endif




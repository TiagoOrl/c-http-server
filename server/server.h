#ifndef H_SERVER
#define H_SERVER

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdint.h>

#include "file_helper.h"
#include "http.h"
#include "http_parser.h"

#define MAX_BUFFER_SIZE 25000


pthread_mutex_t lock;


void net_error(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


int server_accept_new_conn(struct sockaddr_in* address, int server_fd, socklen_t * addrlen)
{
    int client_fd = accept(server_fd, (struct sockaddr*) address, addrlen);
    if (client_fd < 0) {
        if (errno == EINTR) {
            return -2; // Specialized code to tell the loop: "just try again"
        }
        return client_fd;
    }

    struct in_addr ipAddr = address->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);

    // printf("connection from: %s\n", str);

    return client_fd;
}


void* server_client_conn_thread(void* arg)
{
    char in_buffer[1024];
    int* fd = (int*)arg;
    

    memset(in_buffer, 0, sizeof(in_buffer));
    size_t res_read = read(*fd, in_buffer, sizeof(in_buffer) - 1);

    if (res_read <= 0)
    {
        printf("Client disconnected.\n");
    }

    // printf("%s", in_buffer);
    parse(in_buffer, strnlen(in_buffer, 1024));
    
    char* data = file_read("./assets/page.html", "rb");

    size_t html_len = strnlen(data, MAX_BUFFER_SIZE);
    char buffer_header[1024];

    int header_len = snprintf(buffer_header, sizeof(buffer_header),
        http_header_res,
        200,
        "OK",
        html_len
    );

    send(*fd, buffer_header, header_len, 0);
    send(*fd, data, html_len, 0);

    close(*fd);
    free(data);

    return NULL;
}



void server_start(unsigned short port)
{
    int server_fd;
    size_t res_read;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int opt = 1;
    pthread_mutex_init(&lock, NULL);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);


    if (server_fd < 0)
        net_error("socket failed on creation\n");
    

    // attach socket to port 8080
    int sock_opt = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (sock_opt)
        net_error("error on socket opt\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // bind port to socket
    int bind_res = bind(server_fd, (struct sockaddr*) &address, sizeof(address));
    if (bind_res < 0)
        net_error("error on socket bind\n");

    int listen_res = listen(server_fd, 5);
    if (listen_res < 0)
        net_error("error on socket listening\n");

    printf("HTTP server started.\n");
    
    while(1)
    {
        
        
        int client_fd = server_accept_new_conn(&address, server_fd, &addrlen);
        if (client_fd < 0)
        {
            printf("$error: on accepting new client connection: %d\n", client_fd);
            continue;
        }


        pthread_t thread;
        
        pthread_create(&thread, NULL, server_client_conn_thread, (void*)&client_fd);
        pthread_detach(thread);
    }


    pthread_mutex_destroy(&lock);
    close(server_fd);
}



#endif
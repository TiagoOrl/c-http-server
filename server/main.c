#include "server.h"


int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Specify port number.\n");
        return 0;
    }
    
    server_start(atoi(argv[1]));
    return 0;
}

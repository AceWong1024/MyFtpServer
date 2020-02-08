#ifndef SESSION_H
#define SESSION_H
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Session{
public:
    int clisocket;
    struct sockaddr_in cliaddr;
    int datasocket;
    struct sockaddr_in dataddr;
    int server_data_socket;

    char username[50];
    char directory[1024];
    bool is_logined;
    bool is_anonymous;
    bool is_pasv;
};

#endif // SESSION_H

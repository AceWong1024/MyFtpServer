#ifndef SERVER_H
#include <sys/socket.h>
#include <sys/epoll.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include "Session.h"

#define SERVER_H
class Server{
public:
    int fd;
    std::unordered_map<int,Session> session_map;
    Server(){
        
    }
    ~Server(){
        
    }
    bool start();
private:
    bool send_msg(int socket,char* msg);
    bool handle_msg(Session*,char*);
};

#endif // SERVER_H

#ifndef SERVER_H

#include <vector>
#include <unordered_map>
#include <thread>
#include <string>
#include <vector>
#include "Session.h"
#include "SocketUntil.h"
#define SERVER_H
class Server{
public:
    int fd;
    std::unordered_map<int,Session> session_map;
    std::vector<std::thread> threads;
    std::vector<struct epoll_event> events = std::vector<struct epoll_event>(16);
    struct epoll_event event;

    Server(){

    }
    ~Server(){
        
    }
    bool start();
    static void handle_msg(int,char*);
    void threads_join();
private:

};

#endif // SERVER_H

#ifndef SOCKETUNTIL_H
#define SOCKETUNTIL_H
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
static bool send_msg(int socket,std::string msg){
    ssize_t l = msg.size();
    if(l <= 0){
        std::cout << "send_msg: length too short!" << std::endl;
        return false;
    }
    ssize_t ret,n=0;
    while(n<l){
        ret = write(socket,msg.data()+n,l);
        if(ret < 0){
            std::cout << "send_msg: write error!" << std::endl;
            return false;
        }else{
            n+=ret;
        }
    }
    return true;
}
#endif // SOCKETUNTIL_H

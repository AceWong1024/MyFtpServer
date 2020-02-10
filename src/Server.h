#ifndef SERVER_H
#include <sys/socket.h>
#include <sys/epoll.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <thread>
#include <string>
#include <vector>
#include "Session.h"

#define SERVER_H
class Server{
public:
    int fd;
    std::unordered_map<int,Session> session_map;
    std::vector<std::thread> threads;
    Server(){
        
    }
    ~Server(){
        
    }
    bool start();
    static void handle_msg(int,std::string);
    void threads_join();
private:
    bool send_msg(int socket,char* msg);
    void handle_user(int conn,std::string msg);
    void handle_pass(int conn,std::string msg);
    void handle_syst(int conn);
    void handle_cwd(int conn,std::string msg);
    void handle_cdup(int conn,std::string msg);
    void handle_quit(int conn);
    void handle_port(int conn,std::string msg);
    void handle_pasv(int conn);
    void handle_type(int conn,std::string msg);
    void handle_retr(int conn,std::string msg);
    void handle_stor(int conn,std::string msg);
    void handle_appe(int conn,std::string msg);
    void handle_list(int conn,std::string msg);
    void handle_nlst(int conn,std::string msg);
    void handle_rest(int conn,std::string msg);
    void handle_abor(int conn);
    void handle_pwd(int conn);
    void handle_mkd(int conn,std::string msg);
    void handle_rmd(int conn,std::string msg);
    void handle_dele(int conn,std::string msg);
    void handle_rnfr(int conn,std::string msg);
    void handle_rnto(int conn,std::string msg);
    void handle_site(int conn,std::string msg);
    void handle_stat(int conn,std::string msg);
    void handle_noop(int conn,std::string msg);
    void handle_help(int conn);
    void handle_size(int conn,std::string msg);
};

#endif // SERVER_H

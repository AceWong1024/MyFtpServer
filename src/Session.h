#ifndef SESSION_H
#define SESSION_H
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include "SocketUntil.h"
#include <pwd.h>
#include <shadow.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "Config.h"

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
    bool is_closed;
    //ftp protoco
    void handle_user(char msg[]);
    void handle_pass(char msg[]);
    void handle_syst();
    void handle_cwd(char msg[]);
    void handle_cdup(char msg[]);
    void handle_quit();
    void handle_port(char msg[]);
    void handle_pasv();
    void handle_type(char msg[]);
    void handle_retr(char msg[]);
    void handle_stor(char msg[]);
    void handle_appe(char msg[]);
    void handle_list(char msg[]);
    void handle_nlst(char msg[]);
    void handle_rest(char msg[]);
    void handle_abor();
    void handle_pwd();
    void handle_mkd(char msg[]);
    void handle_rmd(char msg[]);
    void handle_dele(char msg[]);
    void handle_rnfr(char msg[]);
    void handle_rnto(char msg[]);
    void handle_site(char msg[]);
    void handle_stat(char msg[]);
    void handle_noop(char msg[]);
    void handle_help();
    void handle_size(char msg[]);
    void handle_msg(char msg[]);

private:
    int checkAuthority();
    const char* statbuf_get_date(struct stat *sbuf);
    const char* statbuf_get_perms(struct stat *sbuf);
    void getDataSock(Session* sess);
};
#endif // SESSION_H

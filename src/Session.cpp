#include "Session.h"

void Session::handle_msg(char msg[]){
    //参数字符串
    char *argument = nullptr;

    int i=0;
    while(*(msg+i) != '\0'){
        if(*(msg+i) == '\r' || *(msg+i) == '\n')
            *(msg+i) = '\0';
        ++i;
    }
    //查找空格
    char* space = strchr(msg,' ');
    if(space != NULL){
        *space = '\0';
        argument = ++space;
    }

    //处理命令
    if(strcmp("USER",msg) == 0){
        handle_user(argument);
    }else
    if(strcmp("PASS",msg) == 0){
        handle_pass(argument);
    }else
    if(strcmp("ACCT",msg) == 0){
        std::cout << "log: Unimplement!" << std::endl;
    }else
    if(strcmp("CWD",msg) == 0){
        handle_cwd(argument);
    }else
    if(strcmp("CDUP",msg) == 0){
        handle_cdup(argument);
    }else
    if(strcmp("QUIT",msg) == 0){
        handle_quit();
    }else
    if(strcmp("SMNT",msg) == 0){
        std::cout << "log: Unimplement!" << std::endl;
    }else
    if(strcmp("PORT",msg) == 0){
        handle_port(argument);
    }else
    if(strcmp("TYPE",msg) == 0){
        handle_type(argument);
    }else
    if(strcmp("RETR",msg) == 0){
        handle_retr(argument);
    }else
    if(strcmp("STOR",msg) == 0){
        handle_stor(argument);
    }else
    if(strcmp("APPE",msg) == 0){
        handle_appe(argument);
    }else
    if(strcmp("LIST",msg) == 0){
        handle_list(argument);
    }else if(strcmp("NLST",msg) == 0){
        handle_nlst(argument);
    }else if(strcmp("REST",msg) == 0){
        handle_rest(argument);
    }else if((strcmp("ABOR",msg) == 0) || (strcmp("\377\364\377\362ABOR",msg) == 0)){
        handle_abor();
    }else if(strcmp("PWD",msg) == 0){
        handle_pwd();
    }else if(strcmp("MKD",msg) == 0){
        handle_mkd(argument);
    }else if(strcmp("RMD",msg) == 0){
        handle_rmd(argument);
    }else if(strcmp("DELE",msg) == 0){
        handle_dele(argument);
    }else if(strcmp("RNFR",msg) == 0){
        handle_rnfr(argument);
    }else if(strcmp("RNTO",msg) == 0){
        handle_rnto(argument);
    }else if(strcmp("SITE",msg) == 0){
        handle_site(argument);
    }else if(strcmp("SYST",msg) == 0){
        handle_syst();
    }else if(strcmp("STAT",msg) == 0){
        handle_stat(argument);
    }else if(strcmp("NOOP",msg) == 0){
        handle_noop(argument);
    }else if(strcmp("HELP",msg) == 0){
        handle_help();
    }else if(strcmp("SIZE",msg) == 0){
        handle_size(argument);
    }else
    {
        std::cout << "log: Unimplement!" << msg << std::endl;
        send_msg(clisocket,"500 Unimplement!\r\n");
    }
}

void Session::handle_user(char arg[]){
    if(strcmp(arg,"anonymous") == 0){
        std::cout << "log: Accet username = " << arg << std::endl;
        strcpy(username,arg);
        send_msg(clisocket,"311 Guest login ok, send your complete e-mail address as password.\r\n");
        return;
    }
    struct passwd* user = getpwnam(arg);
    if(user == nullptr){
        send_msg(clisocket,"530 Login incorrect.\r\n");
    }else{
        strcpy(username,arg);
        send_msg(clisocket,"311 Please specify the password.\r\n");
    }
}

void Session::handle_pass(char arg[]){

    if(strcmp(username,"anonymous") == 0){
        is_logined = true;
        is_anonymous = true;
        send_msg(clisocket,"230 Anonymous Login success.\r\n");
        return;
    }
    if(arg == nullptr){
        send_msg(clisocket,"530 Login incorrect.\r\n");
        return;
    }

    if(strlen(username) == 0){
        send_msg(clisocket,"332 No Username.\r\n");
    }
    struct spwd* user = getspnam(username);
    if(user == nullptr){
        send_msg(clisocket,"530 Login incorrect.\r\n");
    }else{
        char* encrypted_passwd = crypt(arg,user->sp_pwdp);
        //std::cout << "user->sp_pwdp " << user->sp_pwdp << "   " << sizeof(user->sp_pwdp) << std::endl;
        if(encrypted_passwd == nullptr){
            send_msg(clisocket,"530 Login incorrect.\r\n");
            return ;
        }
        if(strcmp(encrypted_passwd, user->sp_pwdp) != 0){
            send_msg(clisocket,"530 Login incorrect.\r\n");
        }else{
            is_logined = true;
            send_msg(clisocket,"230 Login success.\r\n");
        }
    }
}

void Session::handle_syst(){

}
void Session::handle_cwd(char msg[]){

}
void Session::handle_cdup(char msg[]){

}
void Session::handle_quit(){

}
void Session::handle_port(char msg[]){

}
void Session::handle_pasv(){

}
void Session::handle_type(char msg[]){}
void Session::handle_retr(char msg[]){}
void Session::handle_stor(char msg[]){}
void Session::handle_appe(char msg[]){}
void Session::handle_list(char msg[]){}
void Session::handle_nlst(char msg[]){}
void Session::handle_rest(char msg[]){}
void Session::handle_abor(){}
void Session::handle_pwd(){}
void Session::handle_mkd(char msg[]){}
void Session::handle_rmd(char msg[]){}
void Session::handle_dele(char msg[]){}
void Session::handle_rnfr(char msg[]){}
void Session::handle_rnto(char msg[]){}
void Session::handle_site(char msg[]){}
void Session::handle_stat(char msg[]){}
void Session::handle_noop(char msg[]){}
void Session::handle_help(){}
void Session::handle_size(char msg[]){}

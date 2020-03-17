#include "Session.h"

const char* Session::statbuf_get_date(struct stat *sbuf)
{
    static char datebuf[64] = {0};
    const char *p_date_format = "%b %e %H:%M";
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t local_time = tv.tv_sec;
    if (sbuf->st_mtime > local_time || (local_time - sbuf->st_mtime) > 60*60*24*182)
    {
        p_date_format = "%b %e  %Y";
    }

    struct tm* p_tm = localtime(&local_time);
    strftime(datebuf, sizeof(datebuf), p_date_format, p_tm);

    return datebuf;
}

const char* Session::statbuf_get_perms(struct stat *sbuf)
{
    static char perms[] = "----------";
    perms[0] = '?';

    mode_t mode = sbuf->st_mode;
    switch (mode & S_IFMT)
    {
    case S_IFREG:
        perms[0] = '-';
        break;
    case S_IFDIR:
        perms[0] = 'd';
        break;
    case S_IFLNK:
        perms[0] = 'l';
        break;
    case S_IFIFO:
        perms[0] = 'p';
        break;
    case S_IFSOCK:
        perms[0] = 's';
        break;
    case S_IFCHR:
        perms[0] = 'c';
        break;
    case S_IFBLK:
        perms[0] = 'b';
        break;
    }

    if (mode & S_IRUSR)
    {
        perms[1] = 'r';
    }
    if (mode & S_IWUSR)
    {
        perms[2] = 'w';
    }
    if (mode & S_IXUSR)
    {
        perms[3] = 'x';
    }
    if (mode & S_IRGRP)
    {
        perms[4] = 'r';
    }
    if (mode & S_IWGRP)
    {
        perms[5] = 'w';
    }
    if (mode & S_IXGRP)
    {
        perms[6] = 'x';
    }
    if (mode & S_IROTH)
    {
        perms[7] = 'r';
    }
    if (mode & S_IWOTH)
    {
        perms[8] = 'w';
    }
    if (mode & S_IXOTH)
    {
        perms[9] = 'x';
    }
    if (mode & S_ISUID)
    {
        perms[3] = (perms[3] == 'x') ? 's' : 'S';
    }
    if (mode & S_ISGID)
    {
        perms[6] = (perms[6] == 'x') ? 's' : 'S';
    }
    if (mode & S_ISVTX)
    {
        perms[9] = (perms[9] == 'x') ? 't' : 'T';
    }

    return perms;
}

void Session::getDataSock(Session *sess){
    if(sess->is_pasv){
        std::cout << "log: Getting PASV socket." << std::endl;
        socklen_t size = sizeof(struct sockaddr);
        struct sockaddr_in data_client;
        sess->datasocket = accept(sess->server_data_socket,(struct sockaddr*)&data_client,&size);

        send_msg(sess->clisocket,"150 Connection accepted!\r\n");
        if(sess->datasocket < 0){
            std::cout << "error: accept error!!" << std::endl;
        }
    }else{
        std::cout << "log: Getting PORT socket." << std::endl;
        sess->datasocket = socket(AF_INET,SOCK_STREAM,0);
        if(sess->datasocket == -1){
            std::cout << "error: Listen on datasocket error!" << std::endl;
            return;
        }

        int ret = 0;
        ret = connect(sess->datasocket,(struct sockaddr*)&sess->dataddr,sizeof(struct sockaddr));
        send_msg(sess->clisocket,"125 Open.\r\n");
        if(ret == -1){
            std::cout << "error: Connect to client error!" << std::endl;
            return;
        }
    }
}

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

//    std::cout << "command: " << msg << " argument: " << argument << std::endl;
    printf("command: %s argument: %s\n",msg,argument);
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
        std::cout << "log: SMNT unimplement!" << std::endl;
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
    if(checkAuthority() == 0){
        send_msg(clisocket,"530 Please login first.\r\n");
        return;
    }
    std::cout << "handle_syst" << std::endl;
    send_msg(clisocket,"215 UNIX Type: L8.\r\n");
}

void Session::handle_cwd(char msg[]){
    if(checkAuthority() == 0){
        send_msg(clisocket,"530 Please login first.\r\n");
        return;
    }

    //distinguish if it is global
    bool isGlobal = false;
    if(msg[0] == '/'){
        isGlobal = true;
    }

    char temp[1024] = {0};
    if(isGlobal){
        strcpy(temp,globaldir);
        strcpy(temp,msg);
    }else{
        strcpy(temp,globaldir);
        strcpy(temp,directory);
        strcpy(temp,msg);
    }

    if(isExist(temp)){
        if(isGlobal){
            strcpy(directory,msg);
        }else{
            if(directory[strlen(directory)-2] != '/')
                strcat(directory,"/");
            strcat(directory,msg);
        }
        send_msg(clisocket,"212 Change directory success!\r\n");
    }else{
        send_msg(clisocket,"550 No such directory!\r\n");
    }
}

void Session::handle_cdup(char msg[]){
    send_msg(clisocket,"500 Inavalid Comand!\r\n");
}

void Session::handle_quit(){
    send_msg(clisocket,"221 Goodbye!\r\n");
    is_closed = true;
}

void Session::handle_port(char msg[]){
    if(checkAuthority() == 0){
        send_msg(clisocket,"530 Please login first.\r\n");
        return;
    }

    //default pasv mode
    this->is_pasv = false;
    int por[6];
    sscanf(msg,"%d,%d,%d,%d,%d,%d",&por[0],&por[1],&por[2],&por[3],&por[4],&por[5]);
    dataddr.sin_family = AF_INET;
    dataddr.sin_port = htons(por[4]*256 + por[5]);
    std::cout << "log: data port = " << dataddr.sin_port << std::endl;

    char ipaddr[20];
    sprintf(ipaddr,"%d.%d.%d.%d",por[0],por[1],por[2],por[3]);
    inet_aton(ipaddr,&dataddr.sin_addr);
    std::cout << "log:  ipaddr = " << ipaddr << std::endl;

    send_msg(clisocket,"200 PORT command successful.\r\n");
}

void Session::handle_pasv(){

}
void Session::handle_type(char msg[]){}
void Session::handle_retr(char msg[]){}
void Session::handle_stor(char msg[]){}
void Session::handle_appe(char msg[]){}

void Session::handle_list(char msg[]){
    if(checkAuthority() == 0){
        send_msg(clisocket,"530 Please login first.\r\n");
        return;
    }

    char path[1024] = {0};
    strcpy(path,globaldir);
    strcat(path,this->directory);
    strcat(path,"/");
    if(msg != nullptr){
        strcat(path,msg);
    }

    std::cout << path << std::endl;
    DIR* dir = opendir(path);
    std::cout << dir << std::endl;
    if(dir == nullptr){
        send_msg(clisocket,"553 Directory read error.\r\n");
        std::cout << "error: read path error!" << std::endl;
    }else{
        getDataSock(this);
        struct dirent* dt;
        struct stat sbuf;
        while((dt = readdir(dir))){
            if(dt->d_name[0] == '.'){
                std::cout << "log: ignore '.'!" << std::endl;
                continue;
            }

            char name[100] = {0};
            strcpy(name,path);
            strcat(name,dt->d_name);
            if (lstat(name, &sbuf) == -1)
            {
                std::cout << "continue1 errno = " << errno << std::endl;
                continue;
            }

            char buf[1024] = {0};

            const char *perms = statbuf_get_perms(&sbuf);

            int off = 0;
            off += sprintf(buf, "%s ", perms);
            off += sprintf(buf + off, " %3d %-8d %-8d ", sbuf.st_nlink, sbuf.st_uid, sbuf.st_gid);
            off += sprintf(buf + off, "%8lu ", (unsigned long)sbuf.st_size);

            const char *datebuf = statbuf_get_date(&sbuf);
            off += sprintf(buf + off, "%s ", datebuf);
            if (S_ISLNK(sbuf.st_mode))
            {
                char tmp[1024] = {0};
                readlink(dt->d_name, tmp, sizeof(tmp));
                off += sprintf(buf + off, "%s -> %s\r\n", dt->d_name, tmp);
            }else{
                off += sprintf(buf + off, "%s\r\n", dt->d_name);
            }
            std::cout << buf << std::endl;
            send_msg(datasocket,buf);
        }
        send_msg(clisocket,"226 List Finished!\r\n");
        closedir(dir);
        close(datasocket);
    }
}

void Session::handle_nlst(char msg[]){}
void Session::handle_rest(char msg[]){}
void Session::handle_abor(){}

void Session::handle_pwd(){
    if(checkAuthority() == 0){
        send_msg(clisocket,"530 Please login first.\r\n");
    }else{
        char text[1024] = "212 ";
        strcat(text,directory);
        strcat(text,"\r\n");
        send_msg(clisocket,text);
    }
}

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

int Session::checkAuthority(){
    if(!is_logined){
        return 0;
    }else{
        if(is_anonymous){
//                if(strcmp(msg,"STOR") == 0){
//                    return 2;
//                }
        }
        return 1;
    }
}

#include "Server.h"

void Server::threads_join(){
    for(auto iter = threads.begin();iter!=threads.end();iter++){
        iter->join();
    }
}

bool Server::start(){
    //创建套接字
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < -1){
        std::cout << "error: 创建套接字失败！" << std::endl;
        return false;
    }

    int sock_reuse = 1;
    int ret = setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(char*)&sock_reuse,sizeof (sock_reuse));
    if(ret < 0){
        std::cout << "error: 设置套接字复用失败！" << std::endl;
        return false;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(21);
    //绑定
    ret = bind(fd,(sockaddr*)&servaddr,sizeof (struct sockaddr));
    if(ret < 0){
        std::cout << "error: 套接字绑定失败！" << std::endl;
        return false;
    }

    ret = listen(fd,SOMAXCONN);
    if(ret < 0){
        std::cout << "error: 套接字监听失败！" << std::endl;
        return false;
    }
    std::cout << "log: Listening!" << std::endl;

    int epollfd = epoll_create1(EPOLL_CLOEXEC);
    if(epollfd < -1){
        std::cout << "error: 创建epoll套接字失败！" << std::endl;
        return false;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);

    std::vector<struct epoll_event> events(16);
    int evenum = 0;

    socklen_t clilen;
    struct sockaddr_in cliaddr;
    memset(&cliaddr,0,sizeof (cliaddr));

    while(1){
        evenum = epoll_wait(epollfd,&*events.begin(),static_cast<int>(events.size()),-1);
        if(evenum < 0){
            std::cout << "error: epoll_wait失败！" << std::endl;
            return false;
        }

        if(evenum == static_cast<int>(events.size()))
            events.resize(2*events.size());

        for(int i = 0;i < evenum;++i){
            if(events[i].data.fd == fd){
                clilen = sizeof(cliaddr);
                int con = accept(fd,(struct sockaddr*)&cliaddr,&clilen);

                Session session;
                session.clisocket = con;
                session.cliaddr = cliaddr;
                session.is_pasv = false;
                session.is_logined = false;
                session.is_anonymous = false;
                strcpy(session.directory,"/");
                session_map.insert(std::make_pair(con,session));

                event.data.fd = con;
                epoll_ctl(epollfd,EPOLL_CTL_ADD,con,&event);

                send_msg(con,"220 Service ready\r\n");
                std::cout << "log: accept client ip = " << inet_ntoa(cliaddr.sin_addr) << std::endl;
            }else if(events[i].events & EPOLLIN){
                char recvbuf[1024];
                memset(recvbuf,'\0',sizeof(recvbuf));

                ret = recv(events[i].data.fd,recvbuf,sizeof(recvbuf),0);
                if(ret == -1){
                    std::cout << "error: recv error!" << std::endl;
                    return false;
                }else if(ret == 0){
                    std::cout << "log: client close!" << std::endl;
                    epoll_ctl(epollfd,EPOLL_CTL_DEL,events[i].data.fd,&event);
                }

                //Session* sess = &session_map[events[i].data.fd];
                int eventfd = events[i].data.fd;
                threads.push_back(std::thread([this,eventfd,&recvbuf](){
                    Session sess = session_map[eventfd];
                    sess.handle_msg(recvbuf);
                }));
//                handle_msg(sess,recvbuf);
                        //&Server::handle_msg,static_cast<int>(events[i].data.fd),const_cast<char*>(recvbuf.data()))
            }
        }
    }
    return true;
}

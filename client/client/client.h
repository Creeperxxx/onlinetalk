#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>


const int SEND_BUF = 1024;
const int RECV_BUF = 1024;
const int MAX_EVETNS = 10;

class clientonlineio
{
public:
    clientonlineio(const char* serverip,int port):m_serverip(serverip),m_port(port){}
    void connectserver();
    void init_fd();
    void sendmessage();
    void recvmessage();
    char* getsendbuf(){return send_buffer;}
    char* getrecvbuf(){return recv_buffer;}
    int getsocketfd(){return m_socketfd;}
    int getepollfd(){return epoll_fd;}
    // bool& getsendall(){return send_all;}
    void epoll_setup();
    void add_to_epoll(int fd,uint32_t events);
    void set_blocking(int socketfd);
    void set_nonblocking(int socketfd);
    void clearbuf(char* buf,int len);
private:
    const char* m_serverip;
    int m_port;
    int epoll_fd;
    int m_socketfd;
    char send_buffer[SEND_BUF];
    char recv_buffer[RECV_BUF];
    // bool send_all;
};

class Client
{
public:
    Client(const char* serverip,int port):m_io(new clientonlineio(serverip,port)){}
    ~Client();
    void run();
    void event_loop();
    void send();
    void recv();
    // void stdin_send();
    void showrecvived();
    void getsendmsg(const std::string msg);
private:
    clientonlineio* m_io;
    
};

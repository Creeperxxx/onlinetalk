#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include "user.h"
#include <unordered_map>
// #include <queue>

extern const int FULL_CONNECT_LENGTH;
extern const int MAX_EVENTS;
extern const int ACCEPT_NEW_CONN;

class onlineio
{
public:
    onlineio(int port):m_port(port){}
    void send_msg(user* user);
    void recv_msg(user* user);
    int get_new_socketfd(){return m_new_socketfd;}
    int get_epoll_fd(){return epoll_fd;}
    int get_listen_fd(){return listen_fd;}
    void deal_with_newconn();
    // void Event_loop();
    void init();
private:
    // void newconnectiondeal();
    void set_nonblocking(int socket_fd);
    // void set_blocking(int socket_fd);
    void init_listen_fd(int port);
    void init_epoll();
    void add_to_epoll(int fd,uint32_t events);
    void set_newclientconn();
    void add_unlogin_user();
    // int generate_unlogin_uid();

private:
    int listen_fd;
    int epoll_fd;
    int m_port;
    int m_new_socketfd;
    // std::unordered_map<int,user*> unlogin_user;
    // int unlogin_user_num;
    // std::queue<int> socket_queue;
};
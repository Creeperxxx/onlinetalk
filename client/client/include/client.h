#include "onlineio.h"
#pragma once



class Client
{
public:
    Client(const char* serverip,int port):m_io(new clientonlineio(serverip,port))
    {
        shutdown = false;
    }
    ~Client();
    void run();
    void exit();
private:
    void event_loop();
    void send();
    void recv();
    // void stdin_send();
    void showrecvived();
    void getsendmsg(const std::string msg);
private:
    clientonlineio* m_io;
    // bool exit;
    bool shutdown;
};


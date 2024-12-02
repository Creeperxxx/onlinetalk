#pragma once
#include <QObject>
#include "client.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(const char* serverip,int port):client(new Client(serverip,port)){}
    void exit(){
        client->exit();
    }
    ~Worker(){
        delete client;
    }
public slots:
    void doWork() const {
        client->run();
    }
private:
    Client* client;
};
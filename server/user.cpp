#include "user.h"


void user::clear()
{
    close(socketfd);
}

void user::init(int socketfd,int uid = 0,std::string username = "")
{
    this->socketfd = socketfd;
    this->uid = uid;
    this->username = username;
    is_alive = 1;
}

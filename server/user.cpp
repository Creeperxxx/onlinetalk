#include "user.h"


void user::clear()
{
    close(socketfd);
}

void user::init(int socketfd,int uid)
{
    this->socketfd = socketfd;
    this->uid = uid;
    is_alive = 1;
}

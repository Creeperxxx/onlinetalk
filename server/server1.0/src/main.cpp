#include "server.h"

const int PORT = 8060;

int main()
{
    Server server(PORT);
    server.start();
    return 0;
}

//todo 如何判断某个套接字是否断开了链接，如何使用user中的is_alive

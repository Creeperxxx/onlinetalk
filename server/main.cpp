#include "server.h"

const int PORT = 8060;

int main()
{
    Server server(PORT);
    server.start();
    return 0;
}
// 
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for close()
#include <vector>
#include <memory>

#define PORT 8080

int main() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    char buffer[1024] = {0};
    const char *hello = "Hello from client";
    int valread;

    // 创建socket文件描述符
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将IP地址从字符串转换为二进制形式
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    // valread = read(sock, buffer, 1024);
    std::shared_ptr<std::vector<uint8_t>> data = std::make_shared<std::vector<uint8_t>>(1024);
    valread = recv(sock,data->data(),1024,0);
    printf("Server says: %s\n", data->data());
    close(sock);
    //
    return 0;
}
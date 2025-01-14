// // 
// #include <iostream>
// #include <cstring>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h> // for close()
// #include <vector>
// #include <memory>

// #define PORT 8080

// int main() {
//     struct sockaddr_in serv_addr;
//     int sock = 0;
//     char buffer[1024] = {0};
//     const char *hello = "Hello from client";
//     int valread;

//     // 创建socket文件描述符
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         printf("\n Socket creation error \n");
//         return -1;
//     }

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     // 将IP地址从字符串转换为二进制形式
//     if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
//         printf("\nInvalid address/ Address not supported \n");
//         return -1;
//     }

//     // 连接到服务器
//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//         printf("\nConnection Failed \n");
//         return -1;
//     }
//     send(sock, hello, strlen(hello), 0);
//     printf("Hello message sent\n");
//     // valread = read(sock, buffer, 1024);
//     std::shared_ptr<std::vector<uint8_t>> data = std::make_shared<std::vector<uint8_t>>(1024);
//     valread = recv(sock,data->data(),1024,0);
//     printf("Server says: %s\n", data->data());
//     close(sock);
//     //
//     return 0;
// }


#include <iostream>
#include <set>
using namespace std;
#include <memory>

class t
{
private:
    int a;

public:
    t(int b):a(b){}
    // bool operator<(const t& temp) const
    // {
    //     return this->a < temp.get_a();
    // }
    void set_a(int b){a=b;}
    int get_a()const {return a;}
};
struct comparet
{
    bool operator()(const shared_ptr<t>& a,const shared_ptr<t>& b) const
    {
        return a->get_a() < b->get_a();
    }
};

int main()
{
    multiset<shared_ptr<t>,comparet> s;
    s.insert(std::make_shared<t>(2));
    s.insert(std::make_shared<t>(3));
    s.insert(std::make_shared<t>(1));
    int num = 1;
    for( auto &a : s)
    {
        cout<<"第" <<num <<"个值为："<<a->get_a()<<endl;
    }
    return 0;
}



// #include <iostream>
// #include <memory>
// #include <string>
// #include <set>

// class Person {
// public:
//     std::string name;

//     explicit Person(const std::string& n) : name(n) {}

//     // 重载小于操作符
//     bool operator<(const Person& other) const {
//         return name < other.name;
//     }
// };

// int main() {
//     // 使用默认比较器创建set
//     std::set<std::shared_ptr<Person>> peopleSet;

//     // 添加元素到set中
//     peopleSet.insert(std::make_shared<Person>("Charlie"));
//     peopleSet.insert(std::make_shared<Person>("Alice"));
//     peopleSet.insert(std::make_shared<Person>("Bob"));

//     // 遍历并打印set中的元素
//     for (const auto& person : peopleSet) {
//         std::cout << person->name << "\n";
//     }

//     return 0;
// }

// #include <iostream>
// #include <memory>
// #include <string>
// #include <set>

// class Person {
// public:
//     std::string name;

//     explicit Person(const std::string& n) : name(n) {}

//     // 为了让比较器能访问name成员，这里将其设为public，
//     // 或者可以在比较器中声明为友元类/函数。
// };

// // 定义仿函数作为比较器
// struct CompareByName {
//     bool operator()(const std::shared_ptr<Person>& lhs, const std::shared_ptr<Person>& rhs) const {
//         return lhs->name < rhs->name;
//     }
// };

// int main() {
//     // 使用自定义比较器创建set
//     std::set<std::shared_ptr<Person>, CompareByName> peopleSet;

//     // 添加元素到set中
//     peopleSet.insert(std::make_shared<Person>("Charlie"));

//     peopleSet.insert(std::make_shared<Person>("Alice"));
//     peopleSet.insert(std::make_shared<Person>("Bob"));

//     // 遍历并打印set中的元素
//     for (const auto& person : peopleSet) {
//         std::cout << person->name << "\n";
//     }

//     return 0;
// }
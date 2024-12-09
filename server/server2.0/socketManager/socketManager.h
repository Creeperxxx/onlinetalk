#include "socketVec.h"
#include "unordered_map"
class socketManager
{
private:
    std::unordered_map<int,std::shared_ptr<socketVector>> socket_vecs;
public:
    void add_socket_vec(int socketfd);
    bool delete_socket_vec(int socketfd);
    void enqueue_recv_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);   
    void enqueue_send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data(int socketfd);
    std::shared_ptr<std::vector<uint8_t>> dequeue_send_data(int socketfd);
};
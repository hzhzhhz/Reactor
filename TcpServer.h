#ifndef _tcp_server_
#define _tcp_server_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <memory>
#include "EventLoop.h"

class EventLoop;
class TcpServer{
public:
    TcpServer();
    ~TcpServer();
    
    void Init(int port);
    void ConnectClient(int fd);
    void ReadData(int fd);
    void WriteData(int fd);
    void Run();
private:
    void SetNoBlocking(int fd);
    void SetReuseAddr(int sockfd);

    std::unique_ptr<EventLoop> loop;
    //std::vector<int,std::shared_ptr<HandleEvent>>handles;
};
#endif
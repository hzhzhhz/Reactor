#ifndef _event_loop_
#define _event_loop_

#include "HandleEvent.h"
#include "Epoller.h"
#include "TcpServer.h"
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <memory>
#include <unordered_map>

enum TYPE{
    READ=0,
    WRITE=1
};
class EventLoop{
public:
    EventLoop();
    ~EventLoop();
    void Init(int port);
    void SetNoBlocking(int listenfd);
    void Run();
    /*添加事件并且在epoll添加套接字
    fd   :socket fd
    type :read or write
    call :callback function
    */
    void AddEvent(int fd,TYPE type,Callback call);
    void ModEvent(int fd);
    /*删除事件和套接字*/
    void DelEvent(int fd);
private:
    bool is_running;
    //活跃的fd
    std::vector<int>fds;
    //对应的epoll类
    std::unique_ptr<Epoller>epoll_ptr;
    //注册的fd和相应事件
    std::unordered_map<int,std::shared_ptr<HandleEvent>>handle_events_;
};
#endif
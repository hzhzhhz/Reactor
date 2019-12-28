#ifndef _epoller_
#define _epoller_

#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

class Epoller{
public:
    Epoller();
    ~Epoller();
    /*
    添加监听fd 
    */
    void AddEpollfd(int fd);
    void ModEpollfd(int fd,int mask);
    //从监听中移除fd
    void DelEpollfd(int fd);
    void EpollWait(std::vector<int>&fds);
public:
    //epoll fd
    int epoll_fd;
    struct epoll_event* events;
};
#endif
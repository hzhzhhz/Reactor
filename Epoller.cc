#include "Epoller.h"
#include <string.h>

Epoller::Epoller(){
    epoll_fd=epoll_create(1024);
    events=new epoll_event[1024];
}
Epoller::~Epoller(){
    if(events!=nullptr){
        delete []events;
        events=nullptr;
    }
}
void Epoller::AddEpollfd(int fd){
    struct epoll_event event;
    bzero(&event,sizeof(event));
    
    event.events |=EPOLLIN|EPOLLET;//et模式,一次读写必须在一次内完成
    event.data.fd=fd;
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event);
}
void Epoller::ModEpollfd(int fd,int mask){
    struct epoll_event event;
    if(mask==1){
        event.events |= EPOLLOUT;
        event.data.fd=fd;
    }
    else{
        event.events |=EPOLLIN;
        event.data.fd=fd;
    }
    epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&event);
}
void Epoller::DelEpollfd(int fd){
    struct epoll_event event;
    event.events =EPOLLOUT;
    event.data.fd=fd;
    epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,&event);
}
void Epoller::EpollWait(std::vector<int>&fds){
    int epollfd=epoll_wait(epoll_fd,events,1024,-1);
    if(epollfd<0){
        printf("epoll wait failed!");
    }
    for(int i=0;i<epollfd;++i){
        fds.push_back(events[i].data.fd);
    }
}
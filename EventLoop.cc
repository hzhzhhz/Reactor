#include "EventLoop.h"
#include <memory.h>
#include <fcntl.h>
#include <algorithm>

EventLoop::EventLoop():is_running(false),epoll_ptr(new Epoller){
}
EventLoop::~EventLoop(){
    is_running=false;
}


void EventLoop::SetNoBlocking(int listenfd){
    fcntl(listenfd,F_SETFL,fcntl(listenfd, F_GETFL,0)|O_NONBLOCK);
}
void EventLoop::Run(){
    is_running=true;
    
    while(is_running){
        std::vector<int>tmpfd;
        epoll_ptr->EpollWait(fds);//epoll 监听
        fds.swap(tmpfd);
        for(int i=0;i<tmpfd.size();++i){
            handle_events_[tmpfd[i]]->Handle(tmpfd[i]);//处理事件
        }
    }
}

void EventLoop::AddEvent(int fd,TYPE type,Callback call_back_){
    
    if(type==READ){
        auto handle_ptr=std::make_shared<HandleEvent>(fd);//新增事件需要先创建
        handle_ptr->AddReadEvent(call_back_);//绑定回调函数
        handle_ptr->SetReadable(true);//可写
        handle_events_[fd]=handle_ptr;//添加至事件map
        printf("add a read fd :%d\n",fd);
        epoll_ptr->AddEpollfd(fd);//添加至epoll
    }
    else if(type==WRITE){
        handle_events_[fd]->AddWriteEvent(call_back_);
        handle_events_[fd]->SetWritable(true);
        handle_events_[fd]->SetReadable(false);
        printf("add a write fd: %d\n",fd);
        epoll_ptr->ModEpollfd(fd,type);
    }
}
void EventLoop::ModEvent(int fd){
     printf("change to read :%d\n",fd);
     handle_events_[fd]->SetWritable(false);
     handle_events_[fd]->SetReadable(true);
     epoll_ptr->ModEpollfd(fd,WRITE);
}
void EventLoop::DelEvent(int fd){
    printf("remove a fd :%d\n",fd);
    auto handle_iter=handle_events_.find(fd);
    if(handle_iter!=handle_events_.end()){
        handle_events_.erase(handle_iter);
        epoll_ptr->DelEpollfd(fd);
    }
}
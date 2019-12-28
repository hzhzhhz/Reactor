#include "TcpServer.h"
#include <string>
#include <memory.h>
#include <fcntl.h>
#include <memory>

TcpServer::TcpServer():loop(new EventLoop()){
}
TcpServer::~TcpServer(){
}


void TcpServer::SetNoBlocking(int fd){
    fcntl(fd,F_SETFL,fcntl(fd, F_GETFL,0)|O_NONBLOCK);   
}
void TcpServer::SetReuseAddr(int sockfd)
{
    int reuse=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
}
void TcpServer::Init(int port){
    int listenfd;
    struct sockaddr_in servaddr;
    if( (listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return ;
    }
    
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if( bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return ;
    }
    if( listen(listenfd,10) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return ;
    }
    //SetReuseAddr(listenfd);
    SetNoBlocking(listenfd);
    //将监听fd注册
    loop->AddEvent(listenfd,READ,[this](int listfd){
        this->ConnectClient(listfd);
    });
    
}
void TcpServer::ConnectClient(int fd){
    
    sockaddr_in addr;
	socklen_t len = sizeof(addr);
    int sockfd=accept(fd,(struct sockaddr*)&addr,&len);
    SetNoBlocking(sockfd);
    if(sockfd<0){
        printf("accept error\n");
    }
    else{
        printf("accept \n");
        //添加连接fd,并且注册读事件
        loop->AddEvent(sockfd,READ,[this](int connfd){
           this->ReadData(connfd);
       });
    }
}

void TcpServer::ReadData(int fd){
    char recv_buff[4096];
    int n_left=4096;
    int n_read=0;
    while(n_left>0){
        n_read=read(fd,recv_buff+n_read,n_left);
        if(n_read<0){
            if(errno==EINTR){
                n_read=0;
            }
            else if(errno==EAGAIN)//读取完成
            {
                printf("recv message :%s\n",recv_buff);
                
                    char send_buf[20]="hello client";
                    int send_len=sizeof send_buf;
                    while(send_len>0){
                        int len=write(fd,send_buf,send_len);
                        if(len<0){
                            if(errno==EAGAIN)//写完成
                            {
                                close(fd);
                                loop->DelEvent(fd);
                                break;
                            }
                        }
                        else if(len==0){
                            close(fd);//先关闭套接字
                            loop->DelEvent(fd);
                            return ;
                        }
                        else{
                            send_len-=len;
                        }
                    }
                
                break;
            }
        }
        else if(n_read==0){
            close(fd);
            loop->DelEvent(fd);
            break;
        }
        n_left -= n_read;    
    }
}


void TcpServer::Run(){
    loop->Run();
}
#include "TcpServer.h"

int main(){
    TcpServer tcp_server_;
    tcp_server_.Init(9999);
    tcp_server_.Run();
    return 0;
}
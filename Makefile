target:server client
.PHONY:target
cxx=g++
cflags= -g -std=c++11
src=server.cc TcpServer.cc EventLoop.cc Epoller.cc HandleEvent.cc
src_client=client.cc
server:$(src)
	$(cxx) -o server $(cflags) $(src)
client:$(src_client)
	$(cxx) -o client $(cflags) $(src_client)
.PHONY:clean
clean:
	rm server client

#ifndef _handle_event_
#define _handle_event_
#include <functional>


//function+bind(或lambda表达式)完成回调,简洁方便
using Callback=std::function<void(int)>;
//每个HandeEvent封装一个fd,fd事件注册,改变,处理均调用此类,但是生命周期不由此类管理
class HandleEvent{
public:
    
    HandleEvent(int);
    ~HandleEvent();
    //可写
    void SetReadable(bool);
    //可读
    void SetWritable(bool);
    //注册读事件
    void AddReadEvent(Callback call);
    //注册写事件
    void AddWriteEvent(Callback call);
    //处理读写事件
    void Handle(int fd);
private:
    int fd_;
    bool readable;
    bool writeable;
    Callback Read_Back;
    Callback Write_Back;
};
#endif
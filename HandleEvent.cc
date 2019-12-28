#include "HandleEvent.h"

HandleEvent::HandleEvent(int afd):fd_(afd),readable(false),writeable(false){

}
HandleEvent::~HandleEvent(){

}

void HandleEvent::SetReadable(bool flag){
    readable=flag;
}
void HandleEvent::SetWritable(bool flag){
    writeable=flag;
}

void HandleEvent::AddReadEvent(Callback call){
    Read_Back=call;
}
void HandleEvent::AddWriteEvent(Callback call){
    Write_Back=call;
}
void HandleEvent::Handle(int fd){
    if(readable&&Read_Back){
        Read_Back(fd);
    }
    else if(writeable&&Write_Back){
        Write_Back(fd);
    }
}
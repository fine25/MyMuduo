#include <stdlib.h>

#include "Poller.h"
#include "EpollPoller.h"
//基类最好不要依赖派生类，单独公共源文件写，分解了poller与epollpoller的强耦合
Poller* Poller::newDefaultPoller(EventLoop *loop)
{
    if(::getenv("MUDUO_USE_POLL"))
    {
        //生成poll实例
        return nullptr;
    }
    else
    {

        //生成epoll实例
        return new EpollPoller(loop);
    }
}
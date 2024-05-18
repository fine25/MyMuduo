#pragma once
#include "Poller.h"
#include "Timestamp.h"

#include <vector>
#include <sys/epoll.h>

class Channel;

class EpollPoller : public Poller
{

public:
    EpollPoller(EventLoop *loop);
    ~EpollPoller() override;
    // 重写基类的poller的抽象方法
    Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *Channel) override;
    void removeChannel(Channel *channel) override;

private:
    static const int kInitEventListSize = 16;


    // 填写活跃的链接
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    // 更新channel通道
    void update(int operation, Channel *channel);

    using EventList = std::vector<epoll_event>; // vector默认长度是16

    int epollfd_;
    EventList events_;
};
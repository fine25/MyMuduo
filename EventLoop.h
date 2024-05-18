#pragma once
#include <atomic>
#include <functional>
#include <vector>
#include <memory>
#include <mutex>

#include "Timestamp.h"
#include "noncopyable.h"
#include "CurrentThread.h"
#include "EpollPoller.h"

class Channel;
class Poller;
// 时间循环类，主要包含了两个大的模块channel Poller（epoll的抽象
class EventLoop : noncopyable
{
public:
    using Functor = std::function<void()>;

    EventLoop();

    ~EventLoop();
    // 开始事件循环
    void loop();
    // 退出事件循环
    void quit();

    Timestamp pollReturnTime() const { return pollReturnTime_; }
    // 在当前loop中执行cb
    void runInLoop(Functor cb);

    // 把cb放入队列中，唤醒loop所在的线程，执行cb
    void queueInLoop(Functor cb);
    
    // 用来唤醒loop所在的线程的
    void wakeup();
    // Eventloop的方法 -》Poller的方法
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);
    // 判断Eventloop对象是否在自己的线程里面
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
    //
    void handleRead();        // wake up
    void doPendingFunctors(); // 执行回调

    using ChannelList = std::vector<Channel *>;
    std::atomic_bool looping_; // 原子操作，通过CAS实现
    std::atomic_bool quit_;    // 标识退出loop循环

    const pid_t threadId_;     // 记录当前loop所在线程的id
    Timestamp pollReturnTime_; // poller返回发生事件的channels的时间点
    std::unique_ptr<Poller> poller_;

    int wakeupFd_; // 当mainloop获取到一个新用户的channel时，通过轮巡算法选择一个subloop，通知该成员唤醒subloop处理channel
    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;
    Channel *currentActiveChannel_;

    std::atomic_bool callingPendingFunctors_; // 标识当前loop是否有需要执行的回调操作
    std::vector<Functor> pendingFunctors_;    // 存储loop需要执行的所有回调操纵
    std::mutex mutex_;                        // 互斥锁，用来保护上面的vector容器的线程安全操作
};
# MyMuduo
muduo网络库是一款功能强大、易于使用、代码设计优秀的开源网络库

通过对其深入剖析，从C++开源的muduo网络库中，学习优秀的代码设计

本项目将对其核心代码模块进行输出，实现手写muduo网络库。

可深入理解TCP协议和UDP协议、IO复用接口编程、LInux多线程编程、理解UNIX/Linux上的五种IO模型、select、poll、epoll优缺点、epoll原理和优势、Reactor模型等。

重写muduo网络库，使其不依赖于boost库。全部依赖于C++11标准语言实现，分离网络层与业务层代码

掌握基于事件驱动和事件回调的epoll+线程池面向对象编程

mainLoop与subLoop中用系统调用，wakeupfd来做线程之间的通知，没有使用消息队列，代替了生产者消费者模型，使得效率大大提高

每一个Lopp都对应着一个poller，在Reactor模型中loop相当于reactor，poller相当于Demultiplex事件分发器

流程：

TcpServer对象构造 主要构造了acceptor（创建listenfd并把其打包成acceptChannel，其accepChanel只关心读事件。)通过enableReading函数将其在poller上进行注册，之后poller来监听acceptChannel上发生的事件了

如果有事件发生调用handleRead，accept返回一个connfd执行回调newConnectionCallback，其实就是TcpServer的newConnection根据轮询算法选择一个subLoop

基于one loop one thread架构，所以通过wakeupfd唤醒subLoop，把connfd封装成channel分发给subLoop

运行在主线程当中的mainloop调用newConnection，选择一个ioLoop，创建TcpConnection设置ConnectionCallback（有新连接回调）、MessageCallback（有新数据回调）注册到subloop里、threadPool......

server.start函数调用      启动底层的线程池，创建loop子线程并开启loop.loop() 再把acceptChannel注册在baseloop的poller上 

最终开启baseLoop的loop()

如果相应的Channel有可读事件发生，subLoop就会回调Channel的readCallback，TcpConnection给相应的Channel绑定的就是TcpConnection的handleRead，通过缓冲区读取相应socket上的数据，回调用户设置的messageCallback

连接关闭响应Poller中Channel的closeCallback，回调TcpConnection的handleClose，把Channel里感兴趣的事件从Poller上进行删除，执行用户关闭连接的回调和TcpServer的removeConnection，然后回到TcpConnection中的connectionDestoryed销毁连接把Channel从Poller中删除。


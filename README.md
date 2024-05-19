# MyMuduo
muduo网络库是一款功能强大、易于使用、代码设计优秀的开源网络库

通过对其深入剖析，从C++开源的muduo网络库中，学习优秀的代码设计

本项目将对其核心代码模块进行输出，实现手写muduo网络库。

可深入理解TCP协议和UDP协议、IO复用接口编程、LInux多线程编程、理解UNIX/Linux上的五种IO模型、select、poll、epoll优缺点、epoll原理和优势、Reactor模型等。

重写muduo网络库，使其不依赖于boost库。全部依赖于C++11标准语言实现

掌握基于事件驱动和事件回调的epoll+线程池面向对象编程

用系统调用wakeupfd来做线程之间的通知，代替生产者消费者模型


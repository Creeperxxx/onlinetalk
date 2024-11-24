# 框架思路
    server初始化后调用eventloop()开启循环。每次处理两种事

## 处理新连接。
    先调用网络io模块的deal_with_newconn()，处理新连接。然后调用自己的add_unlogin_user()，将新连接存储在unlogin_map中

## 处理客户端的读事件。
    首先，以生成任务为方向考虑，可以分为三部分：读任务 处理任务 写任务。

### 读任务
    主要内容为：从产生就绪事件的套接字的队列中，取出套接字，从中读取数据，然后封装成任务，交给线程池，然后将返回结果也就是future放入任务包装模块的队列中
    具体为：event_loop中生成一个队列，将这个队列传入到任务包装模块

### 处理任务
    主要内容为：从线程池取出任务，执行任务。

### 写任务
    主要内容为：从队列中取出future，等待结果，然后将结果写入到套接字中。
    具体实现：在任务包装模块中设计一个future队列，这个队列里存放处理任务的结果，然后写任务就是从这个队列中取出结果，然后写入到套接字里。

# 各个模块的具体功能
## 网络io模块
    - 发送接收数据。传入user，通过user的socket，发送接收数据。
    - 处理新连接。

## 消息分析模块
    - 解析消息。传入消息，解析消息，返回解析好的消息放入任务包装所需的基础信息结构体中。
    
## 任务包装模块
    - 生成任务。传入基础信息结构体，根据基础信息，将其与对应的处理函数打包，生成任务
    - 取出任务。从任务队列中取出任务，返回任务。

## 线程池模块
    - 初始化线程池。初始化线程池，创建线程，放入线程池。
    - 执行任务。传入任务，执行任务。
    - 销毁线程池。销毁线程池，销毁线程。

## 数据库模块
    - 初始化数据库。初始化数据库，创建数据库，创建表。

# 问题
1. 在taskpakcage中的login_handle函数中，登录成功后需要将username和socket放入到server的login_map中，但是login_map是server的成员变量. 怎么解决？
    解决方法：使用静态函数，或者使用bind绑定一个函数传递给taskpackage，在login_handle中调用这个函数。或者传入成员函数函数指针。
2. 在login_handle中，即使username和password都正确，还要验证是否以登录，即在server的login_map中是否存在username。
    解决办法：server提供一个函数，用于验证是否以登录。使用bind包装一个函数传给taskpackage。

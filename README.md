==============================================================
 * Description:
 * Date:
 * Autor:
 * Modify:

==============================================================

#Utils: 
> - 所有三方代码，随拿随用，用例见example
> - Notify: 本工程所有代码均基于C++11标准实现

###0. cmake 编译
> - mkdir ./build
> - cd build
> - cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../platform/PC.cmake

如果你准备使用交叉编译工具链，请按照platform的sample.cmake形式，将工具链地址写入到该cmake中

###1. 系统类
#### 1) Timer
#### 2) thread
> - 线程：所有线程均继承它，可以很好的实现：同时开启，同时关闭（信号触发的形式）
> - 依赖：signals & share 目录的文件

#### 3) thread_pool
> - 线程池：当需要频繁创建线程时，为避免大量开销，采用事先开辟几个线程
> - 改造：支持单例模式，支持停止运行操作
> - github地址：https://github.com/progschj/ThreadPool
#### 4) memory_pool

###2. 数据格式类
#### 1) json
#### 2) protobuf

###3. 文本处理类
#### 1) spdlog
> - log打印：详细介绍见 spdlog/README
> - github地址：https://github.com/gabime/spdlog
#### 2) Databuf
#### 3) enum
#### 4) yaml

###4. 通信类：，，
#### 1) http
#### 2) mqtt
#### 3) asio
#### 4) socket

###5. 数据库类
#### 1) sqlite

###6. 压缩算法类
#### 1) gzip


###7. IO类：
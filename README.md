==============================================================
 * Description:
 * Date:
 * Autor:
 * Modify:
==============================================================

#Utils: 
> - 所有三方代码，随拿随用，用例见example
> - Notify: 本工程所有代码均基于C++11标准实现

### 0. cmake 编译
> - mkdir ./build
> - cd build
> - cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../platform/PC.cmake

如果你准备使用交叉编译工具链，请按照platform的sample.cmake形式，将工具链地址写入到该cmake中

### 1. 系统类
#### 1) timer
> - 包括SimpleTimer 和 Timer
> - SimpleTimer: 简单的计算时间间隔，sleep一段时间
> - Timer：监控线程，到达指定的时间点/段后，触发定义的动作
> - DateTimer: 获取日历时间等
> - **依赖：**spdlog, thread_pool, exception

#### 2) thread
> - 线程：所有线程均继承它，可以很好的实现：同时开启，同时关闭（信号触发的形式）
> - 依赖：signals & share 目录的文件

#### 3) thread_pool
> - 线程池：当需要频繁创建线程时，为避免大量开销，采用事先开辟几个线程
> - 改造：支持单例模式，支持停止运行操作
> - github地址：https://github.com/progschj/ThreadPool

#### 4) memory_pool
> - 内存池：避免频繁的new/delete导致性能降低和内存碎片化
> - 具体说明: memory_pool/ReadMe

#### 6) System类
> - endian: 大小端相关：判断，转换等
> - system：获取CPU运行时间，系统休眠等
> - signal，信号槽，用于多个线程间的信号传递


### 2. 数据格式类
#### 1) json
> - 迄今为止，我用到的最好用的json for C++，没有中之一
> - github: https://github.com/nlohmann/json

#### 2) protobuf

###3. 文本处理类
#### 1) spdlog
> - log打印：详细介绍见 spdlog/README
> - github地址：https://github.com/gabime/spdlog

#### 2) directory
> - path: 用于设置/获取/建立一个路径（可包含文件）
> - directory: 单纯用于操作目录

#### 3) text
> - file/path/string等常用类的辅助操作：打开/关闭/读取/定位等等一些列操作
> - 是STL的一个扩展，应有尽有

#### 4) optional
> - C++17已经载入到了标准库中，无需使用该文件
> - 作用：用于返回/传递/使用某种类型的对象，类似于指针的形式
> - 增加了一个 bool has_value(),用于判断是否有值，使用 value获取值
> - github: https://github.com/martinmoene/optional-lite

#### 5) enum
> - 简单易用的枚举，可以自由转化为string，int，enum等，具体可见github
> - github: https://github.com/aantron/better-enums

#### 6) yaml

### 4. 事件类
#### 1) pubsub
> - 发布/订阅机制：
> - 不足：发布&订阅为同步，即订阅时将函数注册到一个类中，发布时调用所有注册的函数；
>       这样导致，如果注册的函数中有一个是阻塞的，会影响整体的流程，导致卡死
> - 解决：发布时调用函数，可将该函数压入线程池

#### 2) event
> - 事件处理，可做事件关联，参数可变
> - 当事件到达时，执行对应的操作（事前已经定义完成）

#### 3) brain_tree
> - 行为树，是一个包含逻辑节点和行为节点的树结构；
> - 优点1：行为树由于引入了逻辑节点，它的转换条件更加少（线更少，更清晰），让拓展AI变得更加容易；
> - 优点2：行为的重用（reuse)。
> - github: https://github.com/arvidsson/BrainTree

### 5. IO类：
#### 1) iodata
> - databuffer: 可存放任意类型的数据，尤其是可存放二进制流; 可以根据类型去获取数据（如存储的 0x01 0x02 0x03 0x04 四个char，用int获取出来为0x01020304(大小端有关)）
> - iodevice: 处理各种io操作，如读/写/修改等，适配各种类型的数据

### 6. 通信类：，，
#### 1) http
#### 2) mqtt
#### 3) asio
#### 4) socket

### 7. 数据库类
#### 1) sqlite

### 8. 压缩算法类
#### 1) gzip
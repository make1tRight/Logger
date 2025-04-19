# Logger
## 异步写入日志
1. `formatMessage`通过参数包的形式组成消息
2. 将消息插入到消息队列中
3. 启用后台线程取出队列中的消息写入文件 -> 日志

## 原样转发
1. 通过模板万能引用`T&&`和`std::forward<T>(arg)`实现

## 实现日志分级
1. `[INFO]`
2. `[DEBUG]`
3. `[ERROR]`

## 日志带时间戳
1. `getCurrentTime`实现

## 多目标输出
1. 通过抽象基类与派生类实现, 派生类继承基类, 并重写write方法
    1. 实现console输出
    2. 实现file输出
## 日志轮转
1. 按文件大小轮转
    - (`5*1024*1024`达到最大值写入到新文件中)
2. 按日期轮转

```bash
g++ -std=c++11 LogTest.cpp Logger.h ./fmt/src/format.cc -I ./fmt/include -o logger
```
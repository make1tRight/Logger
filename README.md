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
# log

1.实现一个同步的日志器，异步日志器无法和异常处理配合使用，因为日志还没输出，异常就中断程序了。

2.完善LogEvent，添加 和 clear函数 和 析构函数。（同时改LogEventManager::clearLogEventNodeData函数）















# 模板的学习

1.session::setParamter    // 改成万能引用？

2.session::getParamter    // 改成万能引用？

3.threadpool::addTask







# 拓展

## 用模板实现无锁队列

1.学习LogEventManager::addLogEvent 怎么用原子操作添加节点






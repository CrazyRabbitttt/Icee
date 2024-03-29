### 基本介绍

本项目是一个轻量级的基于C++17的网络库，整体的实现主要是以 `回调` 的机制去进行实现，例如连接
建立之后会触发`listener` 的回调函数，有网络数据包到达之后会触发 `connecter` 的回调函数

整体将套接字的时候抽象成为不同的类来实现，支持快速的建立TCP服务，并且将项目打包📦成为动态库供
应用服务单独链接使用

此外，单独提供了HTTP解析与生成模块，组成了能够支持GET/HEAD请求的HTTP服务器


### 整体的实现概述
- 使用非阻塞套接字 & ET 事件处理模式处理高并发
- `one loop per thread` 设计思路配合线程池管理
- 单独使用的可拓展的库设计
- 支持HTTP的GET请求
- 支持能够调参数的缓存层加快响应
- 使用 `DocTest` 测试框架进行了单元测试的覆盖


### 项目构建
```shell

mkdir build 
cd build 
camke ..
make 

```


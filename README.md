# DouYuTv
该程序采用 Qt Creator 4.8.1 IDE开发 版本为 5.12.1

编译环境 QT 5.12.1 MinGW-64 编译
采用 QtXlsx 来写.xlsx文件 需自己编译,如果不需要支持QtXlsx，可自己在源代码里注释掉QtXlsx 相关代码，即可编译，下为QtXlsx连接地址,可自行编译

link ：https://github.com/dbzhang800/QtXlsxWriter

采用 windeployqt打包运行环境

程序参考斗鱼官方提供的HTTP请求API文档,以及弹幕服务器第三方接入协议，开发完成，省去抓包分析协议，具体也可以去百度找相关文档.

程序 涉及到 HTTP请求,线程，Socket、Json文档解析、xlsx文档读写操作

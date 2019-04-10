#include "threadscocket.h"

ThreadScocket::ThreadScocket(QObject *parent) : QObject(parent)
{
    thread = new QThread();

    thread->start();

    TcpSocket *socket = new TcpSocket;

    connect(this,&ThreadScocket::conectRoomId,socket,&TcpSocket::conRomID);

    //对应槽函数执行完毕后在往下执行  //同步调用
    connect(socket,&TcpSocket::sendBuffData,this,&ThreadScocket::getMessage,Qt::BlockingQueuedConnection);

    connect(socket,&TcpSocket::sendBuffData,this,&ThreadScocket::saveMessage,Qt::BlockingQueuedConnection);

    connect(socket,&TcpSocket::sendEror,this,&ThreadScocket::getErrorMsg);

    connect(this,&ThreadScocket::setTime,socket,&TcpSocket::timeHeartbeat);

    connect(this,&ThreadScocket::closeconnect,socket,&TcpSocket::closeSocket);

    socket->moveToThread(thread);

}


ThreadScocket:: ~ThreadScocket()
{
    thread->quit();
    thread->wait();
}

#include "tcpsocket.h"
#include <windows.h>
#include <QTextCodec>
#include <iostream>
#include <fstream>

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
   connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));

   connect(this,&TcpSocket::readyRead,this,&TcpSocket::readBuff);
}

void TcpSocket::readBuff()
{
    int packet_lenght = 0;

    int recv_bytes=this->read(reinterpret_cast<char *>(&packet_lenght), 4);

    if(recv_bytes == -1)
    {
        return;
    }

    if (packet_lenght <= 8)
    {
        return;
    }

    char buf[40960]={0};

    recv_bytes = this->read(buf,packet_lenght);

    if(recv_bytes == -1)
    {
        return ;
    }

    std::string str(buf,recv_bytes);

    std:: string dec_str(str.data() + 8, str.size() - 8);

    QString data;

    data = QString::fromStdString(dec_str);

    if (data.startsWith("type@=chatmsg"))
    {
        QStringList strListMsg;
        int uidpos = data.indexOf("uid");
        int cidpos = data.indexOf("cid");
        data = data.remove(cidpos,str.length() - cidpos);
        data.remove(0,uidpos);
        QStringList strList=data.split("/");
        QString uid = strList.at(0).mid(5);
        QString name = strList.at(1).mid(5);
        QString text = strList.at(2).mid(5);

        strListMsg.push_back(uid);
        strListMsg.push_back(name);
        strListMsg.push_back(text);

         //对应槽函数执行完毕后在往下执行  //同步调用
        emit this->sendBuffData(strListMsg);

        qDebug()<<"1";
        strListMsg.clear();
        strList.clear();
        data.clear();
     }
}

void TcpSocket::conRomID(int id)
{
    QHostInfo info = QHostInfo::fromName("openbarrage.douyutv.com");

   // QHostAddress host;

   // host = info.addresses().first().toString();

    this->connectToHost(info.addresses().first().toString(),8601);

    if(!this->waitForConnected(20000))
    {
        emit sendEror(this->errorString());
        return;
    }
    sendRoomID(id);
}

void TcpSocket::sendRoomID(int id)
{
    char c = '\0';

    std::string arr_strlog = "type@=loginreq/";

    arr_strlog.append((const char*)&c, 1);

    std::string pack_str = headData(arr_strlog);

    ioWriteData(pack_str.data(),pack_str.size());

    std::string roomID=std::to_string(id);

    std::string arr_strjoin = "type@=joingroup/rid@=";

    arr_strjoin.append(roomID);

    arr_strjoin.append("/gid@=-9999/");

    arr_strjoin.append((const char*)&c, 1);

    std::string pack_join = headData(arr_strjoin);

    ioWriteData(pack_join.data(),pack_join.size());

    timeHeartbeat();
}

void TcpSocket::timeHeartbeat()
{
    if (this->state() == QAbstractSocket::ConnectedState)
    {
        char c = '\0';

        QDateTime time = QDateTime::currentDateTime();

        LONG64 timeT = time.toTime_t();

        std::string str_time=std::to_string(timeT);

        std::string str_timeHear = "type@=keeplive/tick@=" + str_time + "/";

        str_timeHear.append((const char *)&c, 1);

        std::string str_packHear = headData(str_timeHear);

        ioWriteData(str_packHear.data(),str_packHear.size());

        tm.start();
    }
}

std::string TcpSocket::headData(const std::string array)
{
    std::string byteArrData;

    int data_len = array.length() + 8;

    short msg_type = 689;

    char encrypt = 0;

    char reserve =0;

    byteArrData.append((const char *)&data_len, sizeof(data_len));

    byteArrData.append((const char *)&data_len, sizeof(data_len));

    byteArrData.append((const char *)&msg_type, sizeof(msg_type));

    byteArrData.append((const char *)&encrypt, sizeof(encrypt));

    byteArrData.append((const char *)&reserve, sizeof(reserve));

    byteArrData.append(array.data(), array.size());

    return byteArrData;
}

void TcpSocket::ioWriteData(const char* str,const int len)
{
    if(this->write(str,len) == -1)
    {
        emit sendEror(this->errorString());
    }
}

void TcpSocket::ReadError(QAbstractSocket::SocketError)
{
    emit sendEror(this->errorString());
}

void TcpSocket::closeSocket()
{
    char c = '\0';

    std::string arr_strlog = "type@=logout/";

    arr_strlog.append((const char*)&c, 1);

    std::string pack_str = headData(arr_strlog);

    ioWriteData(pack_str.data(),pack_str.size());

    this->close();
}

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTime>
#include <QHostInfo>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);
    std::string headData(const std::string array);
    void ioWriteData(const char* str,const int len);
signals:
    void sendBuffData(const QStringList &);
    void sendEror(QString);

public slots:
    void conRomID(int id);
    void ReadError(QAbstractSocket::SocketError);
    void timeHeartbeat();
    void closeSocket();
    void readBuff();
private:
    void sendRoomID(int id);
protected:
    QTime tm;
  //  QStringList strListMsg;
};

#endif // TCPSOCKET_H

#ifndef THREADSCOCKET_H
#define THREADSCOCKET_H

#include <QObject>
#include <QThread>
#include <QVector>
#include "tcpsocket.h"

class ThreadScocket : public QObject
{
    Q_OBJECT
public:
    explicit ThreadScocket(QObject *parent = nullptr);
    ~ThreadScocket();
signals:
    void conectRoomId(int);
    void getMessage(const QStringList &);
    void saveMessage(const QStringList &);
    void getErrorMsg(QString);
    void setTime();
    void closeconnect();
public slots:

private:
    QVector <QString>content;
    QThread * thread;

};
#endif // THREADSCOCKET_H

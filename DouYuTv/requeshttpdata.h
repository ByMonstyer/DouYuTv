#ifndef REQUESHTTPDATA_H
#define REQUESHTTPDATA_H

#include <QWidget>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QVector>
#include <QDir>
#include <QMessageBox>
#include <QEventLoop>

class RequesHttpData : public QWidget
{
    Q_OBJECT

public:
    RequesHttpData();
    void createJson(const QByteArray data);
    void startRequest(const QUrl &requestedUrl);
    void createDouYuImg(const QByteArray data);
    bool fileBlag;
public:
     //直播
     QVector<QString>roomName;  //主播房间名称
     QVector<QString>nickName;  //主播名
     QVector<QString>roomSrc;   //图片
     QVector<int>roomID;  //ID获取房间
     //游戏分类
    // QVector<int>cateId; //根据 ID获取房间列表
    // QVector<QString>gameName;//游戏名称
private:
      QNetworkAccessManager * manager;
      QNetworkReply *m_networkReply;
private slots:
};

#endif // REQUESHTTPDATA_H

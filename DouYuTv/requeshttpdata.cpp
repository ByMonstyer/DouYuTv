#include "requeshttpdata.h"
#include "mainwindow.h"

RequesHttpData::RequesHttpData()
    :fileBlag(false)
{
    manager = new QNetworkAccessManager;
}

void RequesHttpData::startRequest(const QUrl &requestedUrl)
{

    QByteArray byteArray;

    m_networkReply = manager->get(QNetworkRequest(requestedUrl));

    QEventLoop eventLoop;

    connect(m_networkReply, SIGNAL(finished()), &eventLoop, SLOT(quit())); //同步请求

    eventLoop.exec();

    while (true)
    {
        byteArray = m_networkReply->readAll();

        if (m_networkReply->isFinished())
        {
            break;
        }
    }
    if(fileBlag)
    {
        createDouYuImg(byteArray);
    }
    else
    {
        createJson(byteArray);
    }
}

void RequesHttpData::createDouYuImg(const QByteArray data)
{
    QString downloadDirectory =  QDir::tempPath();

    downloadDirectory+="/DouYuTvImg";

    QString Path;

    QDir dir;

    static int unm=0;

    if (dir.exists(downloadDirectory))
    {
        unm++;

        Path = QString("%1//%2.jpg").arg(downloadDirectory).arg(unm);

        QFile file(Path);

        file.open(QIODevice::WriteOnly);

        file.write(data);

        file.close();
    }
    else
    {
        dir.mkpath(downloadDirectory);

        unm++;

        Path= QString("%1//%2.jpg").arg(downloadDirectory).arg(unm);

        QFile file(Path);

        file.open(QIODevice::WriteOnly);

        file.write(data);

        file.close();
    }
}

void RequesHttpData::createJson(const QByteArray data)
{
    QJsonParseError json_error;

    QJsonDocument parse_doucment = QJsonDocument::fromJson(data, &json_error);

    if(!(json_error.error == QJsonParseError::NoError))
    {
        qDebug()<<"解析json文件错误";
        return;
    }

    QJsonObject jsonObject = parse_doucment.object();

    if(jsonObject.contains(QStringLiteral("data")))
    {
        QJsonValue arrayValue = jsonObject.value(QStringLiteral("data"));

        if(arrayValue.isArray())
        {
            QJsonArray array = arrayValue.toArray();

            for(int i=0;i<array.size();i++)
            {
                QJsonValue value = array.at(i);

                QJsonObject josn = value.toObject();

                roomName.push_back(josn["room_name"].toString());

                nickName.push_back(josn["nickname"].toString());

                roomSrc.push_back(josn["room_src"].toString());

                roomID.push_back(josn["room_id"].toInt());
            }
        }
    }
}

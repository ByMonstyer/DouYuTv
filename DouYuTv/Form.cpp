#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"

Form::Form(int room,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose,1);

    setWindowFlags(Qt::FramelessWindowHint);

    setWindowOpacity(0.75);

    roomID = room;

    btnGroup =new QButtonGroup(this);

    btnGroup->addButton(ui->radioButton_2,1);

    moveForm();

    ui->textBrowser->document()->setMaximumBlockCount(75);

    threadsocket= new ThreadScocket;

    connect(this,&Form::conectRoomId,threadsocket,&ThreadScocket::conectRoomId);

    connect(this,&Form::closecon,threadsocket,&ThreadScocket::closeconnect);

    emit conectRoomId(this->roomID);

    connect(threadsocket,&ThreadScocket::getMessage,
            [&](const QStringList &str)
    {
        static int unm = 0;
        unm++; //清空消息
        if(unm >= 75)
        {
            ui->textBrowser->clear();
            unm = 0;
        }
        ui->textBrowser->append(str.at(1)+":"+str.at(2));
    });

    connect(threadsocket,&ThreadScocket::saveMessage,this,&Form::saveData);

    connect(threadsocket,&ThreadScocket::getErrorMsg,[&](const QString overMsg)
    {
        ui->textBrowser->append(overMsg);
    });

    connect(&time,&QTimer::timeout,threadsocket,&ThreadScocket::setTime);

    time.start(45000);
}

void Form::saveData(const QStringList &str)
{
    QString path =  QDir::currentPath()+"/DoYuTv.xlsx"; // qApp->applicationDirPath()+"DoYuTv.xlsx";

    QXlsx::Document xlsx(path);

    QFile file(path);

    if(!file.exists())  //判断文件存在，不存在创建，在写入
    {
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    QDateTime time=QDateTime::currentDateTime();

    QString strtime=time.toString("yyyy-MM-dd hh::mm::ss");

    static int row = 2; //列

    if(row == 2) //第一行只保存一次
    {
        xlsx.write(1,1,"UID");
        xlsx.write(1,2,"Name");
        xlsx.write(1,3,"内容");
        xlsx.write(1,4,"时间");
    }
    xlsx.write(row,1,str.at(0));
    xlsx.write(row,2,str.at(1));
    xlsx.write(row,3,str.at(2));
    xlsx.write(row,4,strtime);
    row++;
    xlsx.save();
}


void Form::on_pushButton_clicked()
{
    emit closeMainWin();

    emit closecon();

    time.stop();

    this->close();
}

void Form::on_pushButtonOpen_clicked()
{
    switch(btnGroup->checkedId())
    {
    case 1:
        QProcess *pro = new QProcess;

        QString path =  QDir::currentPath()+"/DoYuTv.xlsx";

        pro->start("cmd",QStringList()<<"/c"<<path);

        break;
    }
}

void Form::moveForm()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();

    QRect screenRect = desktopWidget->screenGeometry();

    move(screenRect.width()-341,0);

    RECT rect;

    ::GetWindowRect(::FindWindow(TEXT("Shell_TrayWNd"), NULL), &rect);

    setFixedSize(341,rect.top);
}

Form::~Form()
{
    delete ui;
    delete threadsocket;
}

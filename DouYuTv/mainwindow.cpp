#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mIsMax=false;

    mDrag=false;

    setWindowIcon(QIcon(":/image/title.png"));

    //设置无边框
    setWindowFlags(Qt::FramelessWindowHint);

    //界面透明
    setWindowOpacity(0.9);

    QIntValidator *validator = new QIntValidator(INT_MIN, INT_MAX, this);

    ui->lineEdit->setValidator(validator);

    moveWindow();

    httpData = new RequesHttpData;

    httpData->startRequest(QUrl("http://open.douyucdn.cn/api/RoomApi/live?limit=30"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==ui->widgetTitle || obj==ui->pushButtonMax)
    {
        if(e->type()==QEvent::MouseButtonDblClick)
        {
            on_pushButtonMax_clicked();
            return true;
        }
    }
    return QObject::eventFilter(obj,e);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mDrag = true;

        mDragPos = e->globalPos() - pos();

        e->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (mDrag && (e->buttons() && Qt::LeftButton))
    {
        move(e->globalPos() - mDragPos);

        e->accept();
    }

    if(e->globalX() >= screenRect.width()-341)
    {
        mIsMax=false;

        on_pushButtonMax_clicked();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    mDrag = false;
}

void MainWindow::on_pushButtonMin_clicked()
{
    this->showMinimized();
}

void MainWindow::on_pushButtonMax_clicked()
{
    if(mIsMax)
    {
        moveWindow();

        ui->pushButtonMax->setIcon(QIcon(":/image/max1.png"));

        ui->pushButtonMax->setToolTip(QStringLiteral("最右侧"));

        mIsMax=false;
    }
    else
    {
        moveWindow();

        ui->pushButtonMax->setIcon(QIcon(":/image/max2.png"));

        ui->pushButtonMax->setToolTip(QStringLiteral("还原"));

        mIsMax=true;
    }
}

void MainWindow::on_pushButtonClose_clicked()
{
    QString downloadDirectory =  QDir::tempPath();

    downloadDirectory+="/DouYuTvImg";

    deleteDirectory(downloadDirectory);

    qApp->exit();
}

void MainWindow::moveWindow()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();

    screenRect = desktopWidget->screenGeometry();

    if(mIsMax)
    {
        setFixedSize(341,537);

        move((screenRect.width()-341)/2,(screenRect.height()-537)/2);

        mIsMax=false;
    }
    else
    {
        move(screenRect.width()-341,0);
        RECT rect;

        ::GetWindowRect(::FindWindow(TEXT("Shell_TrayWNd"), NULL), &rect);

        setFixedSize(341,rect.top);

        mIsMax=true;
    }
}

void MainWindow::on_pushButtonGetRoom_clicked()
{
    httpData->fileBlag = true;

    for (int i=0;i<httpData->roomSrc.size();i++)
    {
       httpData->startRequest(QUrl(httpData->roomSrc.at(i)));
    }

    ui->pushButtonGetRoom->hide();

    createToolBtn();

    ui->lineEdit->setText(QString("%1").arg(httpData->roomID.at(0)));
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty())
    {
        return;
    }

    this->hide();

    Form *form = new Form(ui->lineEdit->text().toInt());

    connect(form,&Form::closeMainWin,this,[=](){this->show();});

    form->show();

    form->moveForm();
}

void MainWindow::createToolBtn()
{
    QString downloadDirectory =  QDir::tempPath();  //临时用户目录文件

    downloadDirectory+="/DouYuTvImg";

    for (int i=0; i<httpData->nickName.size(); i++)
    {
        toolBtn=new QToolButton;

        toolBtn->setFixedSize(300,50);

        toolBtn->setText(httpData->roomName.at(i) + "\n\n" + httpData->nickName.at(i));
        QString imgPath;

        imgPath = (downloadDirectory+"/%1.jpg").arg(i+1);

        QPixmap pix;

        bool flag = pix.load(imgPath);

        //加载不成功 则修改后缀名 在设置
        if(flag)
        {
            toolBtn->setIcon(QIcon(pix));
        }
        else
        {
            QString path=downloadDirectory+QString("/%1.png").arg(i+1);
            QFile::rename(imgPath,path);
            toolBtn->setIcon(QIcon(QPixmap(path)));
        }

        toolBtn->setIconSize(QSize(320,180));

        toolBtn->setAutoRaise(true);

        toolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        ui->verticalLayout_2->addWidget(toolBtn);

        connect(toolBtn,&QToolButton::clicked,[=]()
        {
            this->hide();

            Form *form = new Form(httpData->roomID.at(i));

            connect(form,&Form::closeMainWin,this,[=](){this->show();});

            form->show();

            form->moveForm();
        });
    }
}

bool MainWindow::deleteDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists())
        return true;

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    QFileInfoList fileList = dir.entryInfoList();

    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
            deleteDirectory(fi.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}


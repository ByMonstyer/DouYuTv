#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QDesktopWidget>
#include <windows.h>
#include <QTimer>
#include <QFile>
#include <QtXlsx>
#include <QDateTime>
#include <QButtonGroup>
#include "threadscocket.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(int,QWidget *parent = nullptr);
    void moveForm();
    ~Form();

signals:
  void closeMainWin();
  void conectRoomId(int);
  void closecon();
private slots:
   void on_pushButton_clicked();
   void on_pushButtonOpen_clicked();
   void saveData(const QStringList &);
private:
    Ui::Form *ui;
    int roomID;
    ThreadScocket *threadsocket;
    QTimer time;
    QButtonGroup *btnGroup;
};

#endif // FORM_H

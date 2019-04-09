#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QToolButton>
#include <QDesktopWidget>
#include <windows.h>
#include <QIntValidator>
#include "requeshttpdata.h"
#include "form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject *, QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool deleteDirectory(const QString &path);
    void moveWindow();
    ~MainWindow();

private:
    void createToolBtn();

signals:

private slots:
    void on_pushButtonMin_clicked();
    void on_pushButtonMax_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonGetRoom_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    bool mDrag;  //拖动
    QPoint mDragPos;
    bool mIsMax;
    QRect screenRect;
    RequesHttpData *httpData;
    QToolButton *toolBtn;

};

#endif // MAINWINDOW_H

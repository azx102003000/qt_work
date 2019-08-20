#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <iostream>
#include <QComboBox>
#include <QPushButton>
#include <conio.h>
#include <windows.h>
#include <QMessageBox>
#include "qxtglobalshortcut.h"
#include <QContextMenuEvent>
#include <QStringListModel>
#include <QTableWidgetItem>

#include <QTime>
#include <QTimer>

#define MAX_KEYS 15 //支持最大15个按键
#define MAX_COOR 8
#define u32 unsigned int

using namespace std;

namespace Ui {
class MainWindow;
}
//线程
class Mythread :public QThread
{
    Q_OBJECT
public:
    Mythread();
    ~Mythread();
    void run();
signals:
    void send_mouse(QString msg);
};

/*************************************************************/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool set_keys(int num);
private slots:
    void contextMenuEvent(QContextMenuEvent *event);

    void click_addrow();
    void click_addcol();
//    void actionClick(void);
//    void click_remove();
    void click_thread();
    void click_close_key();
    void click_start_key();
    void receive_thread(QString msg);
    void thread_finished();
    void item_change(QTableWidgetItem* item);

    void time_out();//获取时间
    void time_out_addnum();//计数
    void time_back();//倒计时
    void around_timebox(bool flag);
    void click_time();
    void click_timeadd();
    void click_timenumbtn();
    void click_time_start_back();//倒计时启动
    void click_suspend();//暂停倒计时
    void click_contrast_time(bool flag);

    void keys_shortcut1();
    void keys_shortcut2();
    void keys_shortcut3();
    void keys_shortcut4();
    void keys_shortcut5();
    void keys_shortcut6();
    void keys_shortcut7();
    void keys_shortcut8();
    void keys_shortcut9();
    void keys_shortcut10();
    void keys_shortcut11();
    void keys_shortcut12();
    void keys_shortcut13();
    void keys_shortcut14();
    void keys_shortcut15();


private:
    Ui::MainWindow *ui;
    Mythread *thread;
    QComboBox *box;
    QxtGlobalShortcut *shortcut;
    QTimer *timer;
    QTimer *timer_addnum;
    QTimer *timer_back;
    QStringListModel *model;
    u32 hh,ss,mm,zzz;
    u32 hh_back,ss_back,mm_back,zzz_back;
    bool time_flag;
    bool time_back_flag;
    bool time_sup;
};

#endif // MAINWINDOW_H

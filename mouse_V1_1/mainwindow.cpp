/**********************************
 *
 *          键盘代替鼠标点击器
 *      运用了第三方的api接口对热键的设置
 *
 *
 **********************************/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLineEdit>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hh = 0, ss = 0, mm = 0, zzz = 0;
    hh_back = 0, ss_back = 0, mm_back = 0, zzz_back = 0;
    time_flag = true;
    time_sup = true;
    time_back_flag = true;
    thread = new Mythread();
    timer = new QTimer(this);
    timer_addnum = new QTimer(this);
    timer_back = new QTimer(this);

    setFixedSize(750, 410);
    /************************************ Time ******************************************************/

    connect(timer, SIGNAL(timeout()), this, SLOT(time_out()));
    connect(timer_addnum, SIGNAL(timeout()), this, SLOT(time_out_addnum()));
    connect(timer_back, SIGNAL(timeout()), this, SLOT(time_back()));        //定时器
    connect(ui->around_time, SIGNAL(clicked(bool)), this, SLOT(around_timebox(bool)));
    connect(ui->time_addbtn, SIGNAL(clicked(bool)), this, SLOT(click_timeadd()));
    connect(ui->time_numbtn, SIGNAL(clicked(bool)), this, SLOT(click_timenumbtn()));
    connect(ui->time_backstart_btn, SIGNAL(clicked(bool)), this, SLOT(click_time_start_back()));
    connect(ui->suspend_btn, SIGNAL(clicked(bool)), this, SLOT(click_suspend()));

    /********************************** Mouse ********************************************************/
    connect(thread, SIGNAL(send_mouse(QString)), this, SLOT(receive_thread(QString)));
    connect(thread, SIGNAL(finished()), this, SLOT(thread_finished()));

    connect(ui->add_row_btn, SIGNAL(clicked(bool)), this, SLOT(click_addrow()));
    connect(ui->add_col_btn, SIGNAL(clicked(bool)), this, SLOT(click_addcol()));
    connect(ui->time_btn, SIGNAL(clicked(bool)), this, SLOT(click_time()));
    connect(ui->close_mouse_btn, SIGNAL(clicked(bool)), this, SLOT(click_thread()));
    connect(ui->close_key_btn, SIGNAL(clicked(bool)), this, SLOT(click_close_key()));
    connect(ui->start_key_btn, SIGNAL(clicked(bool)), this, SLOT(click_start_key()));
    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)) ,this, SLOT(item_change(QTableWidgetItem*)));
    connect(ui->contrast_time, SIGNAL(clicked(bool)), this, SLOT(click_contrast_time(bool)));

    thread->isRunning() ? thread->terminate() : thread->start();//启动线程
//    connect(ui->tableWidget, SIGNAL)
    //ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    box = new QComboBox[MAX_KEYS];
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        box[i].addItem("左键单击");
        box[i].addItem("左键双击");
        box[i].addItem("右键单击");
        box[i].addItem("右键双击");
        ui->tableWidget->setCellWidget(0 + i, 2, &box[i]);
    }
    shortcut = new QxtGlobalShortcut[MAX_KEYS];
    /*********************************** 热键的信号与槽 **************************************************************/
    connect(&shortcut[0], SIGNAL(activated()), this, SLOT(keys_shortcut1()));
    connect(&shortcut[1], SIGNAL(activated()), this, SLOT(keys_shortcut2()));
    connect(&shortcut[2], SIGNAL(activated()), this, SLOT(keys_shortcut3()));
    connect(&shortcut[3], SIGNAL(activated()), this, SLOT(keys_shortcut4()));
    connect(&shortcut[4], SIGNAL(activated()), this, SLOT(keys_shortcut5()));
    connect(&shortcut[5], SIGNAL(activated()), this, SLOT(keys_shortcut6()));
    connect(&shortcut[6], SIGNAL(activated()), this, SLOT(keys_shortcut7()));
    connect(&shortcut[7], SIGNAL(activated()), this, SLOT(keys_shortcut8()));
    connect(&shortcut[8], SIGNAL(activated()), this, SLOT(keys_shortcut9()));
    connect(&shortcut[9], SIGNAL(activated()), this, SLOT(keys_shortcut10()));
    connect(&shortcut[10], SIGNAL(activated()), this, SLOT(keys_shortcut11()));
    connect(&shortcut[11], SIGNAL(activated()), this, SLOT(keys_shortcut12()));
    connect(&shortcut[12], SIGNAL(activated()), this, SLOT(keys_shortcut13()));
    connect(&shortcut[13], SIGNAL(activated()), this, SLOT(keys_shortcut14()));
    connect(&shortcut[14], SIGNAL(activated()), this, SLOT(keys_shortcut15()));

}

MainWindow::~MainWindow()
{

    if(model != NULL)
        delete model;
    if(timer->isActive())
        timer->stop();
    if(timer_addnum->isActive())
        timer_addnum->stop();
    if(timer_back->isActive())
        timer_back->stop();
    if(thread->isRunning())
        thread->terminate();

    delete[] shortcut;
    delete[] box;
    delete thread;
    delete timer_back;
    delete timer_addnum;
    delete timer;
    delete ui;
}


void MainWindow::item_change(QTableWidgetItem *item)//qcombobox的处理
{
     qDebug()<<item->text()<<ui->tableWidget->rowCount()<<ui->comboBox->count()<<"currentRow"<<ui->tableWidget->currentRow();
     for(int i = ui->comboBox->count(); i > 0 ; i--)
     {
         ui->comboBox->removeItem(i);
         ui->comboBox_2->removeItem(i);
     }

     if(item->text().isEmpty() || item == NULL)
         return;

     for(int i = 0; i < ui->tableWidget->rowCount(); i++)
     {
        if(ui->tableWidget->item(i, 0) == NULL || ui->tableWidget->item(i, 0)->text().isEmpty())
        {
            return;
        }
        //list.append(ui->tableWidget->item(i, 0)->text());
//        ui->comboBox->addItems(list);
        ui->comboBox->insertItem(i + 1, ui->tableWidget->item(i, 0)->text());
        ui->comboBox_2->insertItem(i + 1, ui->tableWidget->item(i, 0)->text());
     }
}

void MainWindow::click_suspend()//倒计时暂停按钮函数
{

    if(ui->suspend_btn->isEnabled() == true && time_sup == true)//可以点击
    {
        ui->suspend_btn->setText("继续");
        if(timer_back->isActive())
            timer_back->stop();
        time_sup = false;
    }
    else
    {
        if(!timer_back->isActive())
            timer_back->start(1);
        //
        ui->suspend_btn->setText("暂停");
        time_sup = true;
    }
}
void MainWindow::click_time_start_back()//倒计时启动按钮函数
{
    //if(ui->timeEdit->geometry())
    //qDebug()<<ui->timeEdit->time().toString("hh:mm:ss:zzz");

    if(ui->comboBox->currentIndex() != 0)
    {
        if(ui->tableWidget->item(ui->comboBox->currentIndex(), 3) == NULL
                || ui->tableWidget->item(ui->comboBox->currentIndex(), 3)->text().isNull())
        {
            QMessageBox::warning(this, "提示", "未输入坐标请重试");
            return;
        }
    }

    if(time_back_flag == true)//定时启动
    {
        ui->time_backstart_btn->setText("停止");
        ui->suspend_btn->setEnabled(true);
        time_back_flag = false;
        hh_back = ui->timeEdit->time().toString("hh").toInt();
        ss_back = ui->timeEdit->time().toString("ss").toInt();
        mm_back = ui->timeEdit->time().toString("mm").toInt();
        zzz_back = ui->timeEdit->time().toString("zzz").toInt();
        timer_back->start(1);
    }
    else//关闭
    {
        timer_back->stop();
        hh_back = 0, ss_back = 0, mm_back = 0, zzz_back = 0;
        ui->time_backstart_btn->setText("启动");
        ui->suspend_btn->setEnabled(false);
        ui->time_bak_lab->setText("倒数");
        ui->suspend_btn->setText("暂停");
        time_back_flag = true;
        time_sup = true;
    }
}
void MainWindow::click_contrast_time(bool flag)//对比时间box
{
    if(flag == true)
    {
        if(!timer->isActive())
        {
            timer->start(1);
            qDebug()<<"timer is running";
        }
    }
    else
    {
        if(!ui->around_time->isChecked() && timer->isActive())
            timer->stop();
    }
}
void MainWindow::around_timebox(bool flag)//是否持续获取时间 box
{
//    flag == true ? timer->start() : timer->stop();
    if(flag)
    {
        if(!timer->isActive())
            timer->start(1);
    }
    else
    {
        if(!ui->contrast_time->isChecked() && timer->isActive())
            timer->stop();
    }
}

void MainWindow::time_back()//倒计时定时器槽函数    timer_back
{
    if((zzz_back--) == 0)
    {
        if((ss_back--) == 0)
        {
            if((mm_back--) == 0)
            {
                if((hh_back--) == 0)
                {
                    if(timer_back->isActive())
                    {
                        ui->time_bak_lab->setText("倒数");
                       // qDebug()<<ui->comboBox->currentIndex();
                        if(ui->comboBox->currentIndex() != 0)
                        {
                            set_keys(ui->comboBox->currentIndex() - 1);
                        }

                        timer_back->stop();
                        time_back_flag = true;
                        ui->time_backstart_btn->setText("启动");
                        ui->suspend_btn->setText("暂停");
                        ui->suspend_btn->setEnabled(false);
                        QMessageBox::information(this, "提示",
                                                 QString("计数%1完成%2动作")
                                                 .arg(ui->timeEdit->time().
                                                 toString("hh小时mm分ss秒zzz毫秒"))
                                                 .arg(ui->comboBox->currentText()));
                        return;
                    }
                }
                mm_back = 59;
            }
            ss_back = 59;
        }
        zzz_back = 999;
    }

    QString time;
    time.sprintf("%02d:%02d:%02d:%03d", hh_back, mm_back, ss_back, zzz_back);
    ui->time_bak_lab->setText(time);
}
void MainWindow::time_out_addnum()//计数定时器的超时  槽函数
{
    if((zzz++) == 999)
    {
        if((ss++) == 59)
        {
            if((mm++) == 59)
            {
                if((hh++) == 23)
                {
                    hh = 0;
                }
                mm = 0;
            }
            ss = 0;
        }
        zzz = 0;
    }
    QString time;
    time.sprintf("%02d:%02d:%02d:%03d",hh,mm,ss,zzz);
    ui->time_addlab->setText(time);


}

void MainWindow::click_time()//按下获取时间按钮
{
    ui->time_lab->setText(QTime::currentTime().toString("hh:mm:ss:zzz"));
}
void MainWindow::time_out()//超时槽函数  时间
{
//    qDebug()<<ui->timeEdit_2->time().toString("hh:mm:ss:zzz");
    if(ui->around_time->isChecked())//持续时间已按下
    {
        ui->time_lab->setText(QTime::currentTime().toString("hh:mm:ss:zzz"));
        if(ui->contrast_time->isChecked())//如果"持续时间"已按下同时对比时间也按下
        {
            if(ui->timeEdit_2->time().toString("hh:mm:ss:zzz") == QTime::currentTime().toString("hh:mm:ss:zzz"))
            {
                ui->contrast_time->setChecked(false);

                if(ui->comboBox_2->currentIndex() != 0)
                {
                    set_keys(ui->comboBox_2->currentIndex() - 1);
                    QMessageBox::information(this, "提示", QString("对比%1完成").arg(ui->comboBox_2->currentText()));
                }
                else
                    QMessageBox::information(this, "提示", "对比完成");
                if(!ui->around_time->isChecked())
                    timer->stop();
            }
        }
    }
    else //持续时间未按下但是已按下对比时间
    {
        if(ui->timeEdit_2->time().toString("hh:mm:ss:zzz") == QTime::currentTime().toString("hh:mm:ss:zzz"))
        {
            ui->contrast_time->setChecked(false);
            if(!ui->around_time->isChecked())
                timer->stop();
            if(ui->comboBox_2->currentIndex() != 0)
            {
                set_keys(ui->comboBox_2->currentIndex() - 1);
                QMessageBox::information(this, "提示", QString("对比%1完成").arg(ui->comboBox_2->currentText()));
            }
            else
                QMessageBox::information(this, "提示", "对比完成");

        }
    }
}
void MainWindow::click_timenumbtn()
{
    static QStringList list;

    if(time_flag == false)//计数
    {
//        qDebug()<<"hh"<<hh<<"ss"<<ss<<"mm"<<mm<<"zzz"<<zzz;
        QString time;
        time.sprintf("%02d:%02d:%02d:%03d", hh, mm, ss, zzz);
        list.append(time);
        model = new QStringListModel(list);
        ui->time_list->setModel(model);
    }
    else//复位
    {
        ui->time_numbtn->setEnabled(false);
        ui->time_addlab->setText("计数");
        model->removeRows(0, model->rowCount());
        list.clear();
        hh = 0, mm = 0, ss = 0, zzz = 0;
        time_flag = true;
    }
}
void MainWindow::click_timeadd()//计时
{

    if(time_flag == true)//启动
    {
        ui->time_numbtn->setEnabled(true);
        ui->time_addbtn->setText("暂停");
        ui->time_numbtn->setText("计数");
        timer_addnum->start(1);
        time_flag = false;
    }
    else//暂停
    {
        ui->time_addbtn->setText("启动");
        ui->time_numbtn->setText("复位");
        timer_addnum->stop();
        time_flag = true;
    }
//    hh = QTime::currentTime().toString("hh").toInt();
//    ss = QTime::currentTime().toString("ss").toInt();
//    mm = QTime::currentTime().toString("mm").toInt();
//    zzz = QTime::currentTime().toString("zzz").toInt();
}
void MainWindow::contextMenuEvent(QContextMenuEvent *event)//重写鼠标右键事件
{
//    qDebug()<<"contextMenuEvent";
//    QPoint point = event->pos();
//    qDebug()<< "x"<<point.x()<<"y"<<point.y();
//    QTableWidgetItem *item = ui->tableWidget->itemAt(point);
//    if(item != NULL)
//    {
//        qDebug()<<"row"<<item->row();
//        qDebug()<<"col"<<item->column();
//    }
    QModelIndex index = ui->tableWidget->selectionModel()->currentIndex();
//    qDebug()<<"row"<<index.row();
//    qDebug()<<"col"<<index.column();
    if(index.row() >= 0 || index.column() >= 0)
        ui->tableWidget->takeItem(index.row(), index.column());
}


void MainWindow::click_addrow()//添加row行
{
    const int row = ui->tableWidget->rowCount();
    if(row >= MAX_KEYS)
    {
        QMessageBox::warning(this, "提示", QString("支持最大%1个按键").arg(MAX_KEYS));
        return;
    }
    ui->tableWidget->insertRow(row);

    box[row].addItem("左键单击");
    box[row].addItem("左键双击");
    box[row].addItem("右键单击");
    box[row].addItem("右键双击");
    ui->tableWidget->setCellWidget(row, 2, &box[row]);

}

void MainWindow::click_addcol()//添加col列
{
    const int col = ui->tableWidget->columnCount();
    if(col >= MAX_COOR)
    {
        QMessageBox::warning(this, "提示", QString("支持最大%1个坐标").arg(MAX_COOR - 3));
        return;
    }
    ui->tableWidget->insertColumn(col);
}


void MainWindow::click_thread()//关闭鼠标线程
{
    if(thread->isRunning())
    {
        thread->terminate();
        ui->close_mouse_btn->setText("开启坐标");
    }
    else
    {
        thread->start();
        ui->close_mouse_btn->setText("关闭坐标");
    }
}
void MainWindow::click_start_key()//开启热键
{
    int row = ui->tableWidget->rowCount();
    //int col = ui->tableWidget->columnCount();
    int i = 0;
    QString key[row];
//    QString coor[row*(col - 3)];

    for(i = 0; i < row; i++)
    {
        if(ui->tableWidget->item(i, 0) == NULL || ui->tableWidget->item(i, 0)->text().isEmpty())
        {
             break;
        }
        else
        {
            key[i] = ui->tableWidget->item(i, 0)->text();
            if(!shortcut[i].isEnabled())
                shortcut[i].setEnabled();
            if(!shortcut[i].setShortcut(QKeySequence(key[i])))
            {

                QMessageBox::warning(this, "提示", QString("热键%1被占用").arg(key[i]));
                return;
            }

        }
    }
    if(i == 0)
    {
        QMessageBox::warning(this, "提示", "请输入热键或添加");
        return;
    }
    QMessageBox::information(this, "提示", "热键已启用");
}

void MainWindow::click_close_key()//关闭热键
{
    if(ui->tableWidget->rowCount() > 0)
    {
        for(int i = 0; i < MAX_KEYS; i++)
        {
            if(shortcut[i].isEnabled())
                shortcut[i].setDisabled();
        }
        QMessageBox::warning(this, "提示", "已关闭热键");
    }
    else
    {
        QMessageBox::warning(this, "提示", "暂未有添加热键无法关闭");
    }

   // qDebug()<<ui->tableWidget->rowCount()<<ui->tableWidget->columnCount();

}


void MainWindow::receive_thread(QString msg)//线程接受槽函数
{
    if(!msg.isEmpty())
        ui->mouse_lab->setText(msg);
}

void MainWindow::thread_finished()//线程结束信号
{
    ui->mouse_lab->clear();//清空lab的信息
}

/**********************热键槽函数*********************************************************/
bool MainWindow::set_keys(int num)
{
    if(ui->tableWidget->item(num, 3) == NULL || ui->tableWidget->item(num, 3)->text().isEmpty())
    {
//        QMessageBox::warning(this, "提示", "未使用坐标");
        return false;
    }

    for(int i = 3; i < ui->tableWidget->columnCount(); i++ )
    {
        if(ui->tableWidget->item(num, i) == NULL || ui->tableWidget->item(num, i)->text().isEmpty())
            continue;
        //设置坐标
        SetCursorPos(ui->tableWidget->item(num, i)->text().section('.', 0, 0).toInt(),
                     ui->tableWidget->item(num, i)->text().section('.', 1, 1).toInt());
        switch(box[num].currentIndex())
        {
            case 0://左键单击
                mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,0,0,0,GetMessageExtraInfo());//down and up
                break;
            case 1://左键双击
                mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,0,0,0,GetMessageExtraInfo());//down and up
                mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,0,0,0,GetMessageExtraInfo());//down and up
                break;
            case 2://右键单击
                mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP,0,0,0,GetMessageExtraInfo());//down and up
                break;
            case 3://右键双击
                mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP,0,0,0,GetMessageExtraInfo());//down and up
                mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP,0,0,0,GetMessageExtraInfo());//down and up
                break;
        }
        //间隔时间
        if(ui->tableWidget->item(num, 1) != NULL && !ui->tableWidget->item(num, 1)->text().isEmpty())
            QThread::msleep(ui->tableWidget->item(num, 1)->text().toInt());
    }
    return true;
}
void MainWindow::keys_shortcut1()
{
    if(!set_keys(0))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut2()
{
    if(!set_keys(1))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut3()
{
    if(!set_keys(2))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut4()
{
    if(!set_keys(3))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut5()
{
    if(!set_keys(4))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut6()
{
    if(!set_keys(5))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut7()
{
    if(!set_keys(6))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut8()
{
    if(!set_keys(7))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut9()
{
    if(!set_keys(8))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut10()
{
    if(!set_keys(9))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut11()
{
    if(!set_keys(10))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut12()
{
    if(!set_keys(11))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut13()
{
    if(!set_keys(12))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut14()
{
    if(!set_keys(13))
        QMessageBox::warning(this, "提示", "未使用坐标");
}
void MainWindow::keys_shortcut15()
{
    if(!set_keys(14))
        QMessageBox::warning(this, "提示", "未使用坐标");
}


/******************************** 线程 *******************************************/
Mythread::Mythread()
{}
Mythread::~Mythread()
{}
void Mythread::run()
{
    QString mouse,mouse2;
    POINT val;
    while(1)
    {
        GetCursorPos(&val);
        mouse = QString::number(val.x) + "," + QString::number(val.y);
        if(mouse != mouse2)
            emit send_mouse(mouse);
        mouse2 = mouse;
    }
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<windows.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_start_enter_clicked(); //开始按钮槽函数
    void on_exit_pressed();  //退出按钮槽函数,优先级大于clicked
    void on_Restoration_clicked();  //复位电机信号
     void on_back_clicked(); //返回按钮槽函数
     void on_verticalScrollBar_actionTriggered(int action); //滑动条槽函数

    void on_clothes_clicked();  //分类槽函数
    void on_trousers_clicked();
    void on_shoe_clicked();
    void on_handbag_clicked();

    void on_clothes1_clicked();  //衣服拍摄槽函数
    void on_clothes2_clicked();
    void on_clothes3_clicked();
    void on_clothes4_clicked();
    void on_clothes5_clicked();
    void on_clothes6_clicked();

    void on_trousers1_clicked();  //裤子拍摄槽函数
    void on_trousers2_clicked();
    void on_trousers3_clicked();
    void on_trousers4_clicked();
    void on_trousers5_clicked();
    void on_trousers6_clicked();

    void on_shoe1_clicked();  //鞋子拍摄槽函数
    void on_shoe2_clicked();
    void on_shoe3_clicked();
    void on_shoe4_clicked();
    void on_shoe5_clicked();
    void on_shoe6_clicked();
    void on_shoe7_clicked();
    void on_shoe8_clicked();

    void on_handbag1_clicked();  //包包拍摄槽函数
    void on_handbag2_clicked();
    void on_handbag3_clicked();
    void on_handbag4_clicked();
    void on_handbag5_clicked();
    void on_handbag6_clicked();
    void on_handbag7_clicked();
    void on_handbag8_clicked();
    void on_handbag9_clicked();
    void on_handbag10_clicked();

public:
    Ui::MainWindow *ui;

protected:
    void mousePressEvent(QMouseEvent *e);//鼠标操作
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    int mode = 0; //窗口模式控制
    QPoint last; //鼠标操作

    int object1_clothes = 1; //照片拍摄完毕标志位（衣服）
    int object2_clothes = 1;
    int object3_clothes = 1;
    int object4_clothes = 1;
    int object5_clothes = 1;
    int object6_clothes = 1;

    int object1_trousers = 1;//（裤子）
    int object2_trousers = 1;
    int object3_trousers = 1;
    int object4_trousers = 1;
    int object5_trousers = 1;
    int object6_trousers = 1;

    int object1_shoe = 1;//（鞋子）
    int object2_shoe = 1;
    int object3_shoe = 1;
    int object4_shoe = 1;
    int object5_shoe = 1;
    int object6_shoe = 1;
    int object7_shoe = 1;
    int object8_shoe = 1;

    int object1_handbag = 1;//（包包）
    int object2_handbag = 1;
    int object3_handbag = 1;
    int object4_handbag = 1;
    int object5_handbag = 1;
    int object6_handbag = 1;
    int object7_handbag = 1;
    int object8_handbag = 1;
    int object9_handbag = 1;
    int object10_handbag = 1;

    //定义窗口的有关变量
    HWND appwnd_capture,appwnd_PLPL;
    QString ClassName_capture;
    QString AppName_capture;

    QString ClassName_PLPL;
    QString AppName_PLPL;

    LPCWSTR app_capture;
    LPCWSTR appCaption_capture;

    LPCWSTR app_PLPL;
    LPCWSTR appCaption_PLPL;

    //串口变量
    QSerialPort *serial;
    QVector<QString>Port;

    //文件地址
    QString path;
    QString lineStr[33];
    QString lineStr_Arm[38];

    //选择不同模块电机发送标志位
    QString ARM_chose = "bbb"; //机械臂电机
    QString Turnplate_chose = "aaa"; //转盘电机
    int Restoration_sign = 1;
};

#endif // MAINWINDOW_H

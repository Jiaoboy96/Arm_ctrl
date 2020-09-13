#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>      //  调试输出用到
#include <QMessageBox>    //  显示消息框用到

#include <QDesktopServices>      //  打开链接用到
#include <QUrl>  //  打开网页需要

#include <QDir>  //  获取当前路径 需要
#include <QMouseEvent>

#define adress  "\\xArm-Python-SDK-master\\example\\wrapper\\xarm6"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)  //初始化
{
    ui->setupUi(this);

    path = QDir::currentPath();//获取程序当前目录
    path.replace("/", "\\");//将地址中的"/"替换为"\"，因为在Windows下使用的是"\"

    QFile f_turnplate(path + "\\turnplate.txt");
    QFile f_Arm(path + "\\Arm_Ctrl.txt");

    if (!f_turnplate.open(QIODevice::ReadOnly|QIODevice::Text))//打开指定文件
    {
        qDebug() << "f_turnplate File opening failed" ;
    }
    else
    {
        qDebug() << "f_turnplate File opening successed" ;
        //QMessageBox::about(NULL, "File", "File opening failed"); //跳出一个窗口显示内容
    }
    if (!f_Arm.open(QIODevice::ReadOnly|QIODevice::Text))//打开指定文件
    {
        qDebug() << "f_Arm File opening failed" ;
    }
    else
    {
        qDebug() << "f_Arm File opening successed" ;
        //QMessageBox::about(NULL, "File", "File opening failed"); //跳出一个窗口显示内容
    }

    QTextStream txt_turnplate(&f_turnplate);
    QTextStream txt_Arm(&f_Arm);

    for(int i = 0;i < 33; i++)
    {
        lineStr[i] = txt_turnplate.readLine();  //读取数据
        //path.replace("/", "\\");
        //qDebug() <<lineStr[i] ;
    }
    for(int i = 0;i < 38; i++)
    {
        lineStr_Arm[i] = txt_Arm.readLine();  //读取数据
        //path.replace("/", "\\");
        //qDebug() <<lineStr[i] ;
    }

    //查找可用的串口
    /*foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        //判断串口能否打开
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            QString portStr = info.portName();
            Port.append(portStr);
            serial.close();
        }
     }*/
    serial = new QSerialPort;
    //设置串口名
    //serial->setPortName(Port[0]);
    serial->setPortName("Port[0]");
    //打开串口
    serial->open(QIODevice::ReadWrite);
    //设置波特率
    serial->setBaudRate(115200);
    //设置数据位数 （8）
    serial->setDataBits(QSerialPort::Data8);
    //设置校验位（如0）
    serial->setParity(QSerialPort::NoParity);
    //设置停止位（如1）
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制(无)
    serial->setFlowControl(QSerialPort::NoFlowControl);

    //窗口变量赋值
    ClassName_capture = "WindowsForms10.Window.8.app.0.347abf0_r6_ad1";
    AppName_capture = QStringLiteral("Capture One");
    ClassName_PLPL = "Qt5QWindowIcon";
    AppName_PLPL = QStringLiteral("MainWindow");

    app_capture = reinterpret_cast<LPCWSTR>(ClassName_capture.data());
    appCaption_capture = reinterpret_cast<LPCWSTR>(AppName_capture.data());
    app_PLPL = reinterpret_cast<LPCWSTR>(ClassName_PLPL.data());
    appCaption_PLPL = reinterpret_cast<LPCWSTR>(AppName_PLPL.data());

    appwnd_capture = FindWindow(app_capture, appCaption_capture);   //获得窗口句柄
    appwnd_PLPL = FindWindow(app_PLPL, appCaption_PLPL);

    //一直显示的
    ui->top_back->setVisible(true); //顶部黑框一直显示
    ui->exit->setVisible(true); //退出键一直显示

    //设置窗口标题栏自定义以及取消边框
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    //不能拉伸主窗口的大小
    setFixedSize(this->width(),this->height());
    //将滑动条位置与槽函数连接
    connect(ui->verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(on_verticalScrollBar_actionTriggered(int)));

    //进入界面隐藏按钮
    ui->back->setVisible(false); //返回按钮
    ui->Restoration->setVisible(false); //复位按钮
    ui->scrollArea_shoe->setVisible(true); //滑动区域一直显示
     ui->scrollArea_handbag->setVisible(true);
    ui->verticalScrollBar->setVisible(false); //滑动条按钮

    ui->clothes->setVisible(false); //衣服分类按钮
    ui->trousers->setVisible(false); //裤子分类按钮
    ui->shoe->setVisible(false); //鞋分类按钮
    ui->handbag->setVisible(false); //包包分类按钮

    ui->clothes1->setVisible(false); //01衣服
    ui->clothes2->setVisible(false); //02衣服
    ui->clothes3->setVisible(false); //03衣服
    ui->clothes4->setVisible(false); //04衣服
    ui->clothes5->setVisible(false); //05衣服
    ui->clothes6->setVisible(false); //06衣服
    ui->trousers1->setVisible(false); //01裤子
    ui->trousers2->setVisible(false); //02裤子
    ui->trousers3->setVisible(false); //03裤子
    ui->trousers4->setVisible(false); //04裤子
    ui->trousers5->setVisible(false); //05裤子
    ui->trousers6->setVisible(false); //06裤子
    ui->shoe1->setVisible(false); //01鞋子
    ui->shoe2->setVisible(false); //02鞋子
    ui->shoe3->setVisible(false); //03鞋子
    ui->shoe4->setVisible(false); //04鞋子
    ui->shoe5->setVisible(false); //05鞋子
    ui->shoe6->setVisible(false); //06鞋子
    ui->shoe7->setVisible(false); //07鞋子
    ui->shoe8->setVisible(false); //08鞋子
    ui->handbag1->setVisible(false); //01包包
    ui->handbag2->setVisible(false); //02包包
    ui->handbag3->setVisible(false); //03包包
    ui->handbag4->setVisible(false); //04包包
    ui->handbag5->setVisible(false); //05包包
    ui->handbag6->setVisible(false); //06包包
    ui->handbag7->setVisible(false); //07包包
    ui->handbag8->setVisible(false); //08包包
    ui->handbag9->setVisible(false); //09包包
    ui->handbag10->setVisible(false); //10包包

    ui->one->setVisible(false); //数字1(衣服裤子)
    ui->two->setVisible(false); //数字2(衣服裤子)
    ui->three->setVisible(false); //数字3(衣服裤子)
    ui->four->setVisible(false); //数字4(衣服裤子)
    ui->five->setVisible(false); //数字5(衣服裤子)
    ui->six->setVisible(false); //数字6(衣服裤子)
    ui->one_shoe->setVisible(false); //数字1鞋子
    ui->two_shoe->setVisible(false); //数字2鞋子
    ui->three_shoe->setVisible(false); //数字3鞋子
    ui->four_shoe->setVisible(false); //数字4鞋子
    ui->five_shoe->setVisible(false); //数字5鞋子
    ui->six_shoe->setVisible(false); //数字6鞋子
    ui->seven_shoe->setVisible(false); //数字7鞋子
    ui->eight_shoe->setVisible(false); //数字8鞋子
    ui->one_handbag->setVisible(false); //数字1包包
    ui->two_handbag->setVisible(false); //数字2包包
    ui->three_handbag->setVisible(false); //数字3包包
    ui->four_handbag->setVisible(false); //数字4包包
    ui->five_handbag->setVisible(false); //数字5包包
    ui->six_handbag->setVisible(false); //数字6包包
    ui->seven_handbag->setVisible(false); //数字7包包
    ui->eight_handbag->setVisible(false); //数字8包包
    ui->nine_handbag->setVisible(false); //数字8包包
    ui->ten_handbag->setVisible(false); //数字8包包

    ui->White_Point1->setVisible(false); //白圆1(衣服裤子)
    ui->White_Point2->setVisible(false); //白圆2(衣服裤子)
    ui->White_Point3->setVisible(false); //白圆3(衣服裤子)
    ui->White_Point4->setVisible(false); //白圆4(衣服裤子)
    ui->White_Point5->setVisible(false); //白圆5(衣服裤子)
    ui->White_Point6->setVisible(false); //白圆6(衣服裤子)
    ui->White_Point_shoe1->setVisible(false); //白圆1鞋子
    ui->White_Point_shoe2->setVisible(false); //白圆2鞋子
    ui->White_Point_shoe3->setVisible(false); //白圆3鞋子
    ui->White_Point_shoe4->setVisible(false); //白圆4鞋子
    ui->White_Point_shoe5->setVisible(false); //白圆5鞋子
    ui->White_Point_shoe6->setVisible(false); //白圆6鞋子
    ui->White_Point_shoe7->setVisible(false); //白圆7鞋子
    ui->White_Point_shoe8->setVisible(false); //白圆8鞋子
    ui->White_Point_handbag1->setVisible(false); //白圆1包包
    ui->White_Point_handbag2->setVisible(false); //白圆2包包
    ui->White_Point_handbag3->setVisible(false); //白圆3包包
    ui->White_Point_handbag4->setVisible(false); //白圆4包包
    ui->White_Point_handbag5->setVisible(false); //白圆5包包
    ui->White_Point_handbag6->setVisible(false); //白圆6包包
    ui->White_Point_handbag7->setVisible(false); //白圆7包包
    ui->White_Point_handbag8->setVisible(false); //白圆8包包
    ui->White_Point_handbag9->setVisible(false); //白圆9包包
    ui->White_Point_handbag10->setVisible(false); //白圆10包包

    ui->tick1->setVisible(false); //对勾1(衣服裤子)
    ui->tick2->setVisible(false); //对勾2(衣服裤子)
    ui->tick3->setVisible(false); //对勾3(衣服裤子)
    ui->tick4->setVisible(false); //对勾4(衣服裤子)
    ui->tick5->setVisible(false); //对勾5(衣服裤子)
    ui->tick6->setVisible(false); //对勾6(衣服裤子)
    ui->tick_shoe1->setVisible(false); //对勾1鞋子
    ui->tick_shoe2->setVisible(false); //对勾2鞋子
    ui->tick_shoe3->setVisible(false); //对勾3鞋子
    ui->tick_shoe4->setVisible(false); //对勾4鞋子
    ui->tick_shoe5->setVisible(false); //对勾5鞋子
    ui->tick_shoe6->setVisible(false); //对勾6鞋子
    ui->tick_shoe7->setVisible(false); //对勾7鞋子
    ui->tick_shoe8->setVisible(false); //对勾8鞋子
    ui->tick_handbag1->setVisible(false); //对勾1包包
    ui->tick_handbag2->setVisible(false); //对勾2包包
    ui->tick_handbag3->setVisible(false); //对勾3包包
    ui->tick_handbag4->setVisible(false); //对勾4包包
    ui->tick_handbag5->setVisible(false); //对勾5包包
    ui->tick_handbag6->setVisible(false); //对勾6包包
    ui->tick_handbag7->setVisible(false); //对勾7包包
    ui->tick_handbag8->setVisible(false); //对勾8包包
    ui->tick_handbag9->setVisible(false); //对勾9包包
    ui->tick_handbag10->setVisible(false); //对勾10包包
    ui->next->setVisible(false); //拍完next按钮

    //进入界面显示按钮（只有logo按钮）
    ui->start_enter->setVisible(true); //进入logo按钮


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_enter_clicked()  //进入界面按钮（只显示分类按钮）
{
    ui->start_enter->setVisible(false);  //logo按钮隐藏

    ui->clothes->setVisible(true);  //分类按钮显示
    ui->trousers->setVisible(true);
    ui->shoe->setVisible(true);
    ui->handbag->setVisible(true);

    ui->back->setVisible(true); //返回按钮显示
    ui->Restoration->setVisible(true); //复位按钮显示

    mode = 1;  //进入第一个窗口模式
}

void MainWindow::on_clothes_clicked()  //衣服分类按钮
{
    ui->trousers->setVisible(false);  //首先关闭分类按钮
    ui->shoe->setVisible(false);
    ui->handbag->setVisible(false);
    ui->clothes->setVisible(false);

    ui->one->setVisible(true);  //衣服，白圆和数字显示
    ui->two->setVisible(true);
    ui->three->setVisible(true);
    ui->four->setVisible(true);
    ui->five->setVisible(true);
    ui->six->setVisible(true);
    ui->White_Point1->setVisible(true);
    ui->White_Point2->setVisible(true);
    ui->White_Point3->setVisible(true);
    ui->White_Point4->setVisible(true);
    ui->White_Point5->setVisible(true);
    ui->White_Point6->setVisible(true);
    ui->clothes1->setVisible(true);
    ui->clothes2->setVisible(true);
    ui->clothes3->setVisible(true);
    ui->clothes4->setVisible(true);
    ui->clothes5->setVisible(true);
    ui->clothes6->setVisible(true);

    //选择控制机械臂电机,发送bbb
     serial->write(ARM_chose.toLatin1());//以ASCII码形式将数据写入串口
     //发送机械臂移动距离
     serial->write(lineStr_Arm[0].toLatin1());//以ASCII码形式将数据写入串口

     mode = 2;  //进入第二个窗口模式
}

void MainWindow::on_trousers_clicked() //裤子分类按钮
{
    ui->trousers->setVisible(false);  //首先关闭分类按钮
    ui->shoe->setVisible(false);
    ui->handbag->setVisible(false);
    ui->clothes->setVisible(false);

    ui->one->setVisible(true);  //裤子，白圆和数字显示
    ui->two->setVisible(true);
    ui->three->setVisible(true);
    ui->four->setVisible(true);
    ui->five->setVisible(true);
    ui->six->setVisible(true);
    ui->White_Point1->setVisible(true);
    ui->White_Point2->setVisible(true);
    ui->White_Point3->setVisible(true);
    ui->White_Point4->setVisible(true);
    ui->White_Point5->setVisible(true);
    ui->White_Point6->setVisible(true);
    ui->trousers1->setVisible(true);
    ui->trousers2->setVisible(true);
    ui->trousers3->setVisible(true);
    ui->trousers4->setVisible(true);
    ui->trousers5->setVisible(true);
    ui->trousers6->setVisible(true);

    //选择控制机械臂电机,发送bbb
     serial->write(ARM_chose.toLatin1());//以ASCII码形式将数据写入串口
     //发送机械臂移动距离
     serial->write(lineStr_Arm[1].toLatin1());//以ASCII码形式将数据写入串口

    mode = 2;   //进入第二个窗口模式
}

void MainWindow::on_shoe_clicked() //鞋子分类按钮
{
    ui->trousers->setVisible(false);  //首先关闭分类按钮
    ui->shoe->setVisible(false);
    ui->handbag->setVisible(false);
    ui->clothes->setVisible(false);

    ui->scrollArea_shoe->lower();  //当选鞋子时，先将鞋子的滑动区域放到最底层
    ui->scrollArea_handbag->lower();  //然后将包包的滑动区域放到最底层，保证顶层的黑框和关闭返回按钮不被挡住

    ui->verticalScrollBar->setVisible(true); //滑动条按钮显示

    ui->shoe1->setVisible(true); //鞋子，白圆和数字显示
    ui->shoe2->setVisible(true);
    ui->shoe3->setVisible(true);
    ui->shoe4->setVisible(true);
    ui->shoe5->setVisible(true);
    ui->shoe6->setVisible(true);
    ui->shoe7->setVisible(true);
    ui->shoe8->setVisible(true);

    ui->White_Point_shoe1->setVisible(true);
    ui->White_Point_shoe2->setVisible(true);
    ui->White_Point_shoe3->setVisible(true);
    ui->White_Point_shoe4->setVisible(true);
    ui->White_Point_shoe5->setVisible(true);
    ui->White_Point_shoe6->setVisible(true);
    ui->White_Point_shoe7->setVisible(true);
    ui->White_Point_shoe8->setVisible(true);

    ui->one_shoe->setVisible(true);
    ui->two_shoe->setVisible(true);
    ui->three_shoe->setVisible(true);
    ui->four_shoe->setVisible(true);
    ui->five_shoe->setVisible(true);
    ui->six_shoe->setVisible(true);
    ui->seven_shoe->setVisible(true);
    ui->eight_shoe->setVisible(true);

    //选择控制机械臂电机,发送bbb
     serial->write(ARM_chose.toLatin1());//以ASCII码形式将数据写入串口
     //发送机械臂移动距离
     serial->write(lineStr_Arm[2].toLatin1());//以ASCII码形式将数据写入串口

    mode = 2;   //进入第二个窗口模式
}

void MainWindow::on_handbag_clicked()  //包包分类按钮
{
    ui->trousers->setVisible(false); //首先关闭分类按钮
    ui->shoe->setVisible(false);
    ui->handbag->setVisible(false);
    ui->clothes->setVisible(false);

    ui->scrollArea_handbag->lower();  //当选包包时，先将包子的滑动区域放到最底层
    ui->scrollArea_shoe->lower();  //然后将鞋子的滑动区域放到最底层，保证顶层的黑框和关闭返回按钮不被挡住

    ui->verticalScrollBar->setVisible(true); //滑动条按钮

    ui->handbag1->setVisible(true); //包包，白圆和数字显示
    ui->handbag2->setVisible(true);
    ui->handbag3->setVisible(true);
    ui->handbag4->setVisible(true);
    ui->handbag5->setVisible(true);
    ui->handbag6->setVisible(true);
    ui->handbag7->setVisible(true);
    ui->handbag8->setVisible(true);
    ui->handbag9->setVisible(true);
    ui->handbag10->setVisible(true);

    ui->White_Point_handbag1->setVisible(true);
    ui->White_Point_handbag2->setVisible(true);
    ui->White_Point_handbag3->setVisible(true);
    ui->White_Point_handbag4->setVisible(true);
    ui->White_Point_handbag5->setVisible(true);
    ui->White_Point_handbag6->setVisible(true);
    ui->White_Point_handbag7->setVisible(true);
    ui->White_Point_handbag8->setVisible(true);
    ui->White_Point_handbag9->setVisible(true);
    ui->White_Point_handbag10->setVisible(true);

    ui->one_handbag->setVisible(true);
    ui->two_handbag->setVisible(true);
    ui->three_handbag->setVisible(true);
    ui->four_handbag->setVisible(true);
    ui->five_handbag->setVisible(true);
    ui->six_handbag->setVisible(true);
    ui->seven_handbag->setVisible(true);
    ui->eight_handbag->setVisible(true);
    ui->nine_handbag->setVisible(true);
    ui->ten_handbag->setVisible(true);

    //选择控制机械臂电机,发送bbb
     serial->write(ARM_chose.toLatin1());//以ASCII码形式将数据写入串口
     //发送机械臂移动距离
     serial->write(lineStr_Arm[3].toLatin1());//以ASCII码形式将数据写入串口

    mode = 2;   //进入第二个窗口模式
}

void MainWindow::on_clothes1_clicked()  //01视角衣服拍摄按钮
{
    switch (object1_clothes)
    {
    case 1:
       {
        Sleep(1000);
             QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[5]));  // 打开指定路径
            // QDesktopServices::openUrl(QUrl("E://"));  // 打开 指定路径。注意 "/"替换为"\"

            //选择控制装盘电机,发送aaa
             serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
             //选择转盘电机位置
             QString str=lineStr[0];//发送字符串
             int tmp = str.toInt();  //将角度转换为-180~180
             if(tmp < 0)
             {
                 tmp = tmp + 360;
             str = QString::number(tmp);
             }

             serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
        break;
       }
    case 2:
       { 
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->one->setVisible(false);
        ui->tick1->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object1_clothes++;
    if(object6_clothes > 2 && object5_clothes > 2 && object4_clothes > 2 && object3_clothes > 2 && object2_clothes > 2 && object1_clothes > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_clothes2_clicked() //02视角衣服拍摄按钮
{
    switch (object2_clothes)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[6]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[1];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
        }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->two->setVisible(false);
        ui->tick2->setVisible(true); //对勾
        break;
    }
    default:
        break;
    }
    object2_clothes++;
    if(object6_clothes > 2 && object5_clothes > 2 && object4_clothes > 2 && object3_clothes > 2 && object2_clothes > 2 && object1_clothes > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_clothes3_clicked() //03视角衣服拍摄按钮
{
    switch (object3_clothes)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[7]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[2];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->three->setVisible(false);
        ui->tick3->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object3_clothes++;
    if(object6_clothes > 2 && object5_clothes > 2 && object4_clothes > 2 && object3_clothes > 2 && object2_clothes > 2 && object1_clothes > 2)
     ui->next->setVisible(true); //拍完显示next按钮

}

void MainWindow::on_clothes4_clicked() //04视角衣服拍摄按钮
{
    switch (object4_clothes)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[8]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[3];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->four->setVisible(false);
        ui->tick4->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object4_clothes++;
    if(object6_clothes > 2 && object5_clothes > 2 && object4_clothes > 2 && object3_clothes > 2 && object2_clothes > 2 && object1_clothes > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_clothes5_clicked() //05视角衣服拍摄按钮
{
    switch (object5_clothes)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[9]));  // 打开指定路径
        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[4];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->five->setVisible(false);
        ui->tick5->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object5_clothes++;
    if(object6_clothes > 2 && object5_clothes > 2 && object4_clothes > 2 && object3_clothes > 2 && object2_clothes > 2 && object1_clothes > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_clothes6_clicked()  //06视角衣服拍摄按钮
{
    switch (object6_clothes)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[10]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[5];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->six->setVisible(false);
        ui->tick6->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object6_clothes++;
    if(object6_clothes > 2 && object5_clothes > 2 && object4_clothes > 2 && object3_clothes > 2 && object2_clothes > 2 && object1_clothes > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_trousers1_clicked() //01视角裤子拍摄按钮
{
    switch (object1_trousers)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[12]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[7];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->one->setVisible(false);
        ui->tick1->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object1_trousers++;
    if(object1_trousers > 2 && object2_trousers > 2 && object3_trousers > 2 && object4_trousers > 2 && object5_trousers > 2 && object6_trousers > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_trousers2_clicked() //02视角裤子拍摄按钮
{
    switch (object2_trousers)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[13]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[8];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->two->setVisible(false);
        ui->tick2->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object2_trousers++;
    if(object1_trousers > 2 && object2_trousers > 2 && object3_trousers > 2 && object4_trousers > 2 && object5_trousers > 2 && object6_trousers > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_trousers3_clicked() //03视角裤子拍摄按钮
{
    switch (object3_trousers)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[14]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[9];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->three->setVisible(false);
        ui->tick3->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object3_trousers++;
    if(object1_trousers > 2 && object2_trousers > 2 && object3_trousers > 2 && object4_trousers > 2 && object5_trousers > 2 && object6_trousers > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_trousers4_clicked() //04视角裤子拍摄按钮
{
    switch (object4_trousers)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[15]));  // 打开指定路径
        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[10];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->four->setVisible(false);
        ui->tick4->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object4_trousers++;
    if(object1_trousers > 2 && object2_trousers > 2 && object3_trousers > 2 && object4_trousers > 2 && object5_trousers > 2 && object6_trousers > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_trousers5_clicked() //05视角裤子拍摄按钮
{
    switch (object5_trousers)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[16]));  // 打开指定路径
        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[11];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->five->setVisible(false);
        ui->tick5->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object5_trousers++;
    if(object1_trousers > 2 && object2_trousers > 2 && object3_trousers > 2 && object4_trousers > 2 && object5_trousers > 2 && object6_trousers > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_trousers6_clicked() //06视角裤子拍摄按钮
{
    switch (object6_trousers)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[17]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[12];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->six->setVisible(false);
        ui->tick6->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object6_trousers++;
    if(object1_trousers > 2 && object2_trousers > 2 && object3_trousers > 2 && object4_trousers > 2 && object5_trousers > 2 && object6_trousers > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe1_clicked() //01视角鞋子拍摄按钮
{
    switch (object1_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[19]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[14];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->one_shoe->setVisible(false);
        ui->tick_shoe1->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object1_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe2_clicked() //02视角鞋子拍摄按钮
{
    switch (object2_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[20]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[15];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->two_shoe->setVisible(false);
        ui->tick_shoe2->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object2_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe3_clicked() //03视角鞋子拍摄按钮
{
    switch (object3_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[21]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[16];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->three_shoe->setVisible(false);
        ui->tick_shoe3->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object3_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe4_clicked() //04视角鞋子拍摄按钮
{
    switch (object4_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[22]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[17];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->four_shoe->setVisible(false);
        ui->tick_shoe4->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object4_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe5_clicked() //05视角鞋子拍摄按钮
{
    switch (object5_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[23]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[18];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->five_shoe->setVisible(false);
        ui->tick_shoe5->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object5_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe6_clicked() //06视角鞋子拍摄按钮
{
    switch (object6_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[24]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[19];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->six_shoe->setVisible(false);
        ui->tick_shoe6->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object6_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe7_clicked() //07视角鞋子拍摄按钮
{
    switch (object7_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[25]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[20];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->seven_shoe->setVisible(false);
        ui->tick_shoe7->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object7_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_shoe8_clicked() //08视角鞋子拍摄按钮
{
    switch (object8_shoe)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[26]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[21];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->eight_shoe->setVisible(false);
        ui->tick_shoe8->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object8_shoe++;
    if(object1_shoe > 2 && object2_shoe > 2 && object3_shoe > 2 && object4_shoe > 2 && object5_shoe > 2
            && object6_shoe > 2&& object7_shoe > 2&& object8_shoe > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag1_clicked() //01视角包包拍摄按钮
{
    switch (object1_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[28]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[23];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->one_handbag->setVisible(false);
        ui->tick_handbag1->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object1_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag2_clicked() //02视角包包拍摄按钮
{
    switch (object2_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[29]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[24];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->two_handbag->setVisible(false);
        ui->tick_handbag2->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object2_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag3_clicked() //03视角包包拍摄按钮
{
    switch (object3_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[30]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[25];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->three_handbag->setVisible(false);
        ui->tick_handbag3->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object3_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag4_clicked() //04视角包包拍摄按钮
{
    switch (object4_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[31]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[26];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->four_handbag->setVisible(false);
        ui->tick_handbag4->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object4_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag5_clicked() //05视角包包拍摄按钮
{
    switch (object5_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[32]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[27];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->five_handbag->setVisible(false);
        ui->tick_handbag5->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object5_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag6_clicked() //06视角包包拍摄按钮
{
    switch (object6_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[33]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[28];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->six_handbag->setVisible(false);
        ui->tick_handbag6->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object6_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag7_clicked() //07视角包包拍摄按钮
{
    switch (object7_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[34]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[29];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->seven_handbag->setVisible(false);
        ui->tick_handbag7->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object7_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag8_clicked() //08视角包包拍摄按钮
{
    switch (object8_handbag)
    {
    case 1:
       {
        Sleep(1000);
       QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[35]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[30];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->eight_handbag->setVisible(false);
        ui->tick_handbag8->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object8_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag9_clicked() //09视角包包拍摄按钮
{
    switch (object9_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[36]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[31];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->nine_handbag->setVisible(false);
        ui->tick_handbag9->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object9_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_handbag10_clicked() //10视角包包拍摄按钮
{
    switch (object10_handbag)
    {
    case 1:
       {
        Sleep(1000);
        QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\" + lineStr_Arm[37]));  // 打开指定路径

        //选择控制装盘电机,发送aaa
         serial->write(Turnplate_chose.toLatin1());//以ASCII码形式将数据写入串口
         //选择转盘电机位置
         QString str=lineStr[32];//发送字符串
         int tmp = str.toInt();  //将角度转换为-180~180
         if(tmp < 0)
         {
             tmp = tmp + 360;
         str = QString::number(tmp);
         }

         serial->write(str.toLatin1());//以ASCII码形式将数据写入串口
             break;
         }
    case 2:
       {
        SetForegroundWindow(appwnd_capture);  //将相机窗口放在顶层
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('K',0,0,0);
        keybd_event('K',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
        SetForegroundWindow(appwnd_PLPL);  //将控制软件窗口放在顶层

        ui->ten_handbag->setVisible(false);
        ui->tick_handbag10->setVisible(true); //对勾
        break;
        }
    default:
        break;
    }
    object10_handbag++;
    if(object1_handbag > 2 && object2_handbag > 2 && object3_handbag > 2 && object4_handbag > 2 && object5_handbag > 2
            && object6_handbag > 2 && object7_handbag > 2 && object8_handbag > 2 && object9_handbag > 2 && object10_handbag > 2)
     ui->next->setVisible(true); //拍完显示next按钮
}

void MainWindow::on_back_clicked()  //返回按钮（按模式）
{
    switch (mode) {
    case 1:
        ui->back->setVisible(false); //返回按钮
        ui->Restoration->setVisible(false); //复位按钮
        ui->clothes->setVisible(false); //衣服分类按钮
        ui->trousers->setVisible(false); //裤子分类按钮
        ui->shoe->setVisible(false); //鞋分类按钮
        ui->handbag->setVisible(false); //包包分类按钮
        ui->start_enter->setVisible(true); //进入logo按钮
        mode = 0;
        break;
    case 2:
        ui->clothes->setVisible(true); //衣服分类按钮
        ui->trousers->setVisible(true); //裤子分类按钮
        ui->shoe->setVisible(true); //鞋分类按钮
        ui->handbag->setVisible(true); //包包分类按钮

        ui->clothes1->setVisible(false); //01衣服
        ui->clothes2->setVisible(false); //02衣服
        ui->clothes3->setVisible(false); //03衣服
        ui->clothes4->setVisible(false); //04衣服
        ui->clothes5->setVisible(false); //05衣服
        ui->clothes6->setVisible(false); //06衣服
        ui->trousers1->setVisible(false); //01裤子
        ui->trousers2->setVisible(false); //02裤子
        ui->trousers3->setVisible(false); //03裤子
        ui->trousers4->setVisible(false); //04裤子
        ui->trousers5->setVisible(false); //05裤子
        ui->trousers6->setVisible(false); //06裤子
        ui->shoe1->setVisible(false); //01鞋子
        ui->shoe2->setVisible(false); //02鞋子
        ui->shoe3->setVisible(false); //03鞋子
        ui->shoe4->setVisible(false); //04鞋子
        ui->shoe5->setVisible(false); //05鞋子
        ui->shoe6->setVisible(false); //06鞋子
        ui->shoe7->setVisible(false); //07鞋子
        ui->shoe8->setVisible(false); //08鞋子
        ui->handbag1->setVisible(false); //01包包
        ui->handbag2->setVisible(false); //02包包
        ui->handbag3->setVisible(false); //03包包
        ui->handbag4->setVisible(false); //04包包
        ui->handbag5->setVisible(false); //05包包
        ui->handbag6->setVisible(false); //06包包
        ui->handbag7->setVisible(false); //07包包
        ui->handbag8->setVisible(false); //08包包
        ui->handbag9->setVisible(false); //09包包
        ui->handbag10->setVisible(false); //10包包
        ui->one->setVisible(false); //数字1
        ui->two->setVisible(false); //数字2
        ui->three->setVisible(false); //数字3
        ui->four->setVisible(false); //数字4
        ui->five->setVisible(false); //数字5
        ui->six->setVisible(false); //数字6
        ui->one_shoe->setVisible(false); //数字1鞋子
        ui->two_shoe->setVisible(false); //数字2鞋子
        ui->three_shoe->setVisible(false); //数字3鞋子
        ui->four_shoe->setVisible(false); //数字4鞋子
        ui->five_shoe->setVisible(false); //数字5鞋子
        ui->six_shoe->setVisible(false); //数字6鞋子
        ui->seven_shoe->setVisible(false); //数字7鞋子
        ui->eight_shoe->setVisible(false); //数字8鞋子
        ui->one_handbag->setVisible(false); //数字1包包
        ui->two_handbag->setVisible(false); //数字2包包
        ui->three_handbag->setVisible(false); //数字3包包
        ui->four_handbag->setVisible(false); //数字4包包
        ui->five_handbag->setVisible(false); //数字5包包
        ui->six_handbag->setVisible(false); //数字6包包
        ui->seven_handbag->setVisible(false); //数字7包包
        ui->eight_handbag->setVisible(false); //数字8包包
        ui->nine_handbag->setVisible(false); //数字9包包
        ui->ten_handbag->setVisible(false); //数字10包包
        ui->White_Point1->setVisible(false); //白圆1
        ui->White_Point2->setVisible(false); //白圆2
        ui->White_Point3->setVisible(false); //白圆3
        ui->White_Point4->setVisible(false); //白圆4
        ui->White_Point5->setVisible(false); //白圆5
        ui->White_Point6->setVisible(false); //白圆6
        ui->White_Point_shoe1->setVisible(false); //白圆1鞋子
        ui->White_Point_shoe2->setVisible(false); //白圆2鞋子
        ui->White_Point_shoe3->setVisible(false); //白圆3鞋子
        ui->White_Point_shoe4->setVisible(false); //白圆4鞋子
        ui->White_Point_shoe5->setVisible(false); //白圆5鞋子
        ui->White_Point_shoe6->setVisible(false); //白圆6鞋子
        ui->White_Point_shoe7->setVisible(false); //白圆7鞋子
        ui->White_Point_shoe8->setVisible(false); //白圆8鞋子
        ui->White_Point_handbag1->setVisible(false); //白圆1包包
        ui->White_Point_handbag2->setVisible(false); //白圆2包包
        ui->White_Point_handbag3->setVisible(false); //白圆3包包
        ui->White_Point_handbag4->setVisible(false); //白圆4包包
        ui->White_Point_handbag5->setVisible(false); //白圆5包包
        ui->White_Point_handbag6->setVisible(false); //白圆6包包
        ui->White_Point_handbag7->setVisible(false); //白圆7包包
        ui->White_Point_handbag8->setVisible(false); //白圆8包包
        ui->White_Point_handbag9->setVisible(false); //白圆9包包
        ui->White_Point_handbag10->setVisible(false); //白圆10包包
        ui->tick1->setVisible(false); //对勾1
        ui->tick2->setVisible(false); //对勾2
        ui->tick3->setVisible(false); //对勾3
        ui->tick4->setVisible(false); //对勾4
        ui->tick5->setVisible(false); //对勾5
        ui->tick6->setVisible(false); //对勾6
        ui->tick_shoe1->setVisible(false); //对勾1
        ui->tick_shoe2->setVisible(false); //对勾2
        ui->tick_shoe3->setVisible(false); //对勾3
        ui->tick_shoe4->setVisible(false); //对勾4
        ui->tick_shoe5->setVisible(false); //对勾5
        ui->tick_shoe6->setVisible(false); //对勾6
        ui->tick_shoe7->setVisible(false); //对勾7
        ui->tick_shoe8->setVisible(false); //对勾8
        ui->tick_handbag1->setVisible(false); //对勾1
        ui->tick_handbag2->setVisible(false); //对勾2
        ui->tick_handbag3->setVisible(false); //对勾3
        ui->tick_handbag4->setVisible(false); //对勾4
        ui->tick_handbag5->setVisible(false); //对勾5
        ui->tick_handbag6->setVisible(false); //对勾6
        ui->tick_handbag7->setVisible(false); //对勾7
        ui->tick_handbag8->setVisible(false); //对勾8
        ui->tick_handbag9->setVisible(false); //对勾9
        ui->tick_handbag10->setVisible(false); //对勾10

        ui->next->setVisible(false); //拍完next按钮
        ui->verticalScrollBar->setVisible(false); //滑动条按钮
        ui->scrollArea_shoe->move(0,30);  //滑动区域置开始位置
        ui->scrollArea_handbag->move(0,30);
        ui->verticalScrollBar->setValue(0);  //将滑块的位置到开始位置

        object1_clothes = 1; //拍摄标志位置1
        object2_clothes = 1; //拍摄标志位置1
        object3_clothes = 1; //拍摄标志位置1
        object4_clothes = 1; //拍摄标志位置1
        object5_clothes = 1; //拍摄标志位置1
        object6_clothes = 1; //拍摄标志位置1

        object1_trousers = 1; //拍摄标志位置1
        object2_trousers = 1; //拍摄标志位置1
        object3_trousers = 1; //拍摄标志位置1
        object4_trousers = 1; //拍摄标志位置1
        object5_trousers = 1; //拍摄标志位置1
        object6_trousers = 1; //拍摄标志位置1

        object1_shoe = 1; //拍摄标志位置1
        object2_shoe = 1; //拍摄标志位置1
        object3_shoe = 1; //拍摄标志位置1
        object4_shoe = 1; //拍摄标志位置1
        object5_shoe = 1; //拍摄标志位置1
        object6_shoe = 1; //拍摄标志位置1
        object7_shoe = 1; //拍摄标志位置1
        object8_shoe = 1; //拍摄标志位置1

        object1_handbag = 1; //拍摄标志位置1
        object2_handbag = 1; //拍摄标志位置1
        object3_handbag = 1; //拍摄标志位置1
        object4_handbag = 1; //拍摄标志位置1
        object5_handbag = 1; //拍摄标志位置1
        object6_handbag = 1; //拍摄标志位置1
        object7_handbag = 1; //拍摄标志位置1
        object8_handbag = 1; //拍摄标志位置1
        object9_handbag = 1; //拍摄标志位置1
        object10_handbag = 1; //拍摄标志位置1

        mode = 1; //模式退一
        break;
    default:
        break;
    }
}

void MainWindow::on_Restoration_clicked()  //复位电机信号
{
    switch(Restoration_sign)
    {
        case 1:
        {
            Sleep(1000);
            QDesktopServices::openUrl(QUrl("file:///" + path + adress + "\\Res.pyw"));  // 打开指定路径
            break;
        }
        case 2:
        {
            Sleep(1000);
            //选择控制机械臂电机,发送bbb
             serial->write(ARM_chose.toLatin1());//以ASCII码形式将数据写入串口

             //发送机械臂移动距离
            QString Start_str = "000";
            serial->write(Start_str.toLatin1());//以ASCII码形式将数据写入串口
        }
        default:
            break;
    }
    Restoration_sign++;
    if(Restoration_sign == 3)
    {
        Restoration_sign = 1;
    }
}

void MainWindow::on_exit_pressed()  //退出按钮信号，等待按键释放，优先级大于clicked
{
    //QDesktopServices::openUrl(QUrl("file:///" + lineStr_Arm[37]));  // 打开当前目录的路径

    //选择控制机械臂电机,发送bbb
     //serial->write(ARM_chose.toLatin1());//以ASCII码形式将数据写入串口
     //发送机械臂移动距离

     //QString Start_str = "000";
     //serial->write(Start_str.toLatin1());//以ASCII码形式将数据写入串口
}

void MainWindow::on_verticalScrollBar_actionTriggered(int action) //滑动条槽函数
{
    double p=static_cast<double>(action)/static_cast<double>(ui->verticalScrollBar->maximum());
    ui->scrollArea_shoe->move(0,-(ui->scrollArea_shoe->height()-1080)*p);
    ui->scrollArea_handbag->move(0,-(ui->scrollArea_handbag->height()-1080)*p);
}

void MainWindow::mousePressEvent(QMouseEvent *e) //鼠标操作
{
    last=e->globalPos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

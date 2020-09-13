Arm_CTL结构图：开始界面
	           服装类型的选择界面
	           衣服角度的选择界面

项目实现的功能：
	该项目涉及了软件开发，机械臂控制以及机械的设计。
	软件部分主要实现了对相机拍摄的控制以及向机械臂发送消息裤子机械臂的转动，由于需要自动拍摄，因此拍摄物体下面安装的转盘用来控制物体的角度。
涉及的主要函数：
	窗口的初始化函数：实现了文件数据的读取，将转台和机械臂的数据读取并且存储
			实现了串口的初始化，用来向控制中心发送数据
			获取窗口句柄，用来向另一个进程发送虚拟按键消息
			初始化一些控件的属性
	空间的槽函数：	void on_start_enter_clicked(); //开始按钮槽函数
				实现切换界面的功能
    			void on_exit_pressed();  //退出按钮槽函数,优先级大于clicked
				实现程序退出功能
    			void on_Restoration_clicked();  //复位电机信号
				控制机械臂和转盘复位的数据的复位
     			void on_back_clicked(); //返回按钮槽函数
				实现返回上一步操作的功能
     			void on_verticalScrollBar_actionTriggered(int action); //滑动条槽函数
				模拟滑动条效果

    			void on_clothes_clicked();  //分类槽函数 实现衣服类别的选择
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

用到的主要技术：
	串口通信
	虚拟按键的实现
	进程间的通信（通过句柄）
			
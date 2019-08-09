# RM2019 第八组嵌入工程日志

***

 ## 文件分类
  + HARDWARE：底层外设驱动代码
  + HALLIB：ST官方HAL库代码
  + CORE：ARM Cortex-M4内核文件及启动文件
  + FreeRTOS：FreeRTOS代码（仅保留任务调度器和软定时器）
  + SYSTEM：系统配置文件（如时钟、延时函数）
  + APP：应用层函数，里程计、PID等代码
  + Top_Layer：逻辑层函数，主要实现程序逻辑
  + USER：用户函数，用户自定义进程代码

***

 ## 通信协议（GDTP，通用数据传输协议）
 ![协议](https://img.vim-cn.com/f1/4c93cc90952987293638034c1e0460afea15bd.png "通信协议")
  + 帧头、帧尾均为0xff的定长数据包
  + PID域用于包标识，自带翻转校验
  + 为了确保数据帧中出现的0xff不干扰帧头桢尾，加入调整帧
  + 数据帧中所有的0xff均替换为0x00，并在调整帧中相应位置1
  + 包校验长度若不等于12，说明数据错误

#### 上传PC结构体
```c
typedef struct
{
	unsigned char Castle_Bule[7];   //The Number Of Every Castle Score Blue Side
	unsigned char Castle_Red[7];    //The Number Of Every Castle Score Red Side
	unsigned char Block[63];   		//Every Block Status
	unsigned char Car_Blue[2]; 		//First Byte is X
	unsigned char Car_Red[2]; 		//First Byte is X
	unsigned char Least_Time;  		//How Many Time Have Left
	unsigned char Score_Blue;  	    //Blue Score Now
	unsigned char Score_Red;    	//Red Score Now
	short Motor_Speed[8];			//Eight Motor Speed (rpm)
	long Gryo_Scope_Gryo; 			//Gryo Speed from sensor now
	long Gryo_Scope_Angle;			//Angle Sumary from sensor now
	
}Receive_Data_Package;
```
#### 妙算返回结构体
```c
typedef struct
{
	float X_Possition;
	float Y_Possition;
	float Angle;
}Receive_Data_Package;
```
#### PC返回结构体
```c
typedef struct
{
	float X_Possition;
	float Y_Possition;
	
}Receive_Data_Package;
```

***

## 遗传算法
##### 遗传算法优势
使用遗传算法的优势在于，可以对于一个随机性较大的题目拟合出近似最优解，最简单的在于一方面我们在训练我们的机器人怎么去走先手的时候，也在教它怎么去走后手同样的由于遗传算法独有的不确定性和随机性，当我们的种群数量够大的时候我们可以涵盖所有的可能性，因此在我看来使用遗传算法是最佳的。遗传算法另外一个好处在于，我们不需要知道任何策略，我们可以对这个游戏一窍不通，通过计算机上千上亿次的模拟它知道怎么走风险最小，怎么走收益最大。通过评估算法进行筛选，剔除掉得分最低的种群，剩下的种群就是我们能够得高分的“套路”。
##### 种群基因库编码
```c
typedef struct
{
	float X_Possition;
	float Y_Possition;
	
}Map_Data;
```

***

框架维护人：朱彦臻、彪临轩





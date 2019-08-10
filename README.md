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
 ![协议](https://upload.cc/i1/2019/08/10/8q7ofS.jpg "通信协议")
  + 帧头、帧尾均为0xff的定长数据包
  + PID域用于包标识，自带翻转校验
  + 为了确保数据帧中出现的0xff不干扰帧头桢尾，加入调整帧
  + 数据帧中所有的0xff均替换为0x00，并在调整帧中相应位置1
  + 包校验长度若不等于12，说明数据错误

#### 上传PC结构体
```c
typedef struct
{
	public byte[] Castle_Bule;   //The Number Of Every Castle Score Blue Side
	public byte[] Castle_Red;   //The Number Of Every Castle Score Red Side
	public byte[] Block;   //Every Block Status
	public byte[] Car_Blue; //First Byte is X
	public byte[] Car_Red; //First Byte is X 
	public byte Least_Time;  //How Many Time Have Left
	public short Score_Blue;    //Blue Score Now
	public short Score_Red;    //Red Score Now
	public short[] Motor_Speed;//Eight Motor Speed (rpm)
	public float Gryo_Scope_Gryo; //Gryo Speed from sensor now
	public float Gryo_Scope_Angle;//Angle Sumary from sensor now
	public float X_Possition;      //Car_Possition X
	public float Y_Possition;      //Car_Possition Y
	
}Receive_Data_Package;
```
将地图数据上传给上位机用于显示和决策算法，将底盘数据传递给上位机显示参数便于实时监控底盘状态。
#### 妙算返回结构体
```c
typedef struct
{
	float X_Possition;
	float Y_Possition;
	float Angle;
}Receive_Data_Package;
```
妙算给下位机视觉融合定位出的底盘朝向和XY坐标，下位机把视觉定位信息与里程计累计信息进行融合，最终得出准确的底盘位置数据用于PID闭环。
#### PC返回结构体
```c
typedef struct
{
	float X_Possition;
	float Y_Possition;
	
}Transmmit_Data_Package;
```
PC上位机给定底盘位置PID的User值，由下位机完成PID闭环和定位操作，上位机执行的是参数监控和决策算法。
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
##### 遗传算法思路
种群评估：加入“自相残杀”概念，指两个个体进行模拟比赛，并根据当前回合的分数相差给予选择概率的调整，如：一号个体和二号个体比赛，相差1分，则一号个体0.7，二号个体为为0.6；三号个体和四号个体比赛，相差10分，三号个体0.9，四号个体0.2；

第一轮：随机生成若干个体，开始算法，互相打架，结束后开始繁殖，控制个体数量，选到的扔掉让儿子代替，没选到的继续，不够的话继续随机生成，同时第一轮要保存一定数量的场地数据；

若干轮：导入上一轮的场地数据，导入遗留下来个体，继续自相残杀，继续繁殖，重复上述的步骤。跑完六个回合之后，我们需要进行最后一次筛选，这里就是筛出最优解。

终止条件：近乎最优解（最终的占领区域数最大，1：超过五十代没有比目前优解更大的则视为最优解；2：自行判断，人为的加上循环次数；）

***

框架维护人：朱彦臻、彪临轩





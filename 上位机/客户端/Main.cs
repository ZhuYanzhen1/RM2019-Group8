using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NativeWifi;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Windows.Forms.DataVisualization.Charting;
using System.Drawing.Drawing2D;
namespace 客户端
{
    public partial class Main : Form
    {
        public Main()
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            InitializeComponent();
        }
        public struct Receive_Data_Package
        {
            public byte[] Castle_Bule;   //The Number Of Every Castle Score Blue Side
            public byte[] Castle_Red;   //The Number Of Every Castle Score Red Side
            public byte[] Block;   //Every Block Status
            public byte[] Car_Blue; //First Byte is X
            public byte[] Car_Red; //First Byte is X 
            public byte Least_Time;  //How Many Time Have Left
            public short Score_Blue;    //Blue Score Now
            public short Score_Red;    //Red Score Now
            public short[] Motor_Speed;//Eight Motor Speed (rpm)  from motor1~motor8
            public float Gryo_Scope_Gryo; //Gryo Speed from sensor now
            public float Gryo_Scope_Angle;//Angle Sumary from sensor now
            public float X_Possition;      //Car_Possition X
            public float Y_Possition;      //Car_Possition Y
        };
        Socket socketClient = null;
        int Rcv_Flg = 0; //若为1则已经收到过帧头
        byte[] USART3_Rcv_Buffer = new byte[15];
        int RX_Counter = 0x01;
        public Receive_Data_Package Receive_Package = new Receive_Data_Package();
        void Send_Data(byte pid, byte[] data)
        {
	        byte[] Temp_Data=new byte[12];
	        byte cnt = 0;
	        Temp_Data[0] =  0xff;
	        Temp_Data[11] = 0;
	        for(cnt = 0;cnt<8;cnt++)
	        {
		        if(data[cnt] == 0xff)
		        {
			        Temp_Data[cnt+2] = 0x00;
                    Temp_Data[10] = Convert.ToByte(Temp_Data[11] | (1 << cnt));
		        }
		        else{
			        Temp_Data[cnt+2] = data[cnt];
		        }
	        }
            Temp_Data[1] =Convert.ToByte( Convert.ToByte(pid << 4) | Convert.ToByte((~pid) & 0x0f));										//设置pid,并取反校验
	        Temp_Data[11] = 0xff;				//填充帧尾
            socketClient.Send(Temp_Data);
        }
        void Display_Map()
        {
            int Counter = 1;
            while (true)
            {
                Button name = (Button)this.Controls.Find("btn"+Counter.ToString(), true)[0];
                name.BackgroundImage = null;
                switch(Receive_Package.Block[Counter-1])
                {
                    case 0:
                        name.BackColor = Color.White;
                        break;
                    case 1:
                        name.BackColor = Color.LightBlue;
                        break;
                    case 2:
                        name.BackColor = Color.Blue;
                        break;
                    case 3:
                        name.BackColor = Color.LightPink;
                        break;
                    case 4:
                        name.BackColor = Color.Red;
                        break;
                }
                Counter++;
                if (Counter == 64)
                    break;
            }
            int Car_Blue_Location = Receive_Package.Car_Blue[0] + 1 + Receive_Package.Car_Blue[1] * 9;
            int Car_Red_Location = Receive_Package.Car_Red[0] + 1 + Receive_Package.Car_Red[1] * 9;
            Button Location = (Button)this.Controls.Find("btn" + Car_Blue_Location.ToString(), true)[0];
            Location.BackgroundImage = 客户端.Properties.Resources.蓝车;
            Location = (Button)this.Controls.Find("btn" + Car_Red_Location.ToString(), true)[0];
            Location.BackgroundImage = 客户端.Properties.Resources.红车;
            time_least.Text = "时间：" + Receive_Package.Least_Time.ToString();
            blue_score.Text = "蓝方：" + Receive_Package.Score_Blue.ToString();
            red_score.Text = "红方：" + Receive_Package.Score_Red.ToString();
        }
        void Display_Data()
        {
            while (true)
            {
                Locate_Now.Text = "Location：(" + Receive_Package.X_Possition.ToString("F3") + "," + Receive_Package.Y_Possition.ToString("F3") + ")";
                angle.Text = "Angle:" + Receive_Package.Gryo_Scope_Angle.ToString();
                gryo.Text = "Gryo:" + Receive_Package.Gryo_Scope_Gryo.ToString();
                motor1.Text = "Motor1:" + Receive_Package.Motor_Speed[0];
                motor2.Text = "Motor2:" + Receive_Package.Motor_Speed[1];
                motor3.Text = "Motor3:" + Receive_Package.Motor_Speed[2];
                motor4.Text = "Motor4:" + Receive_Package.Motor_Speed[3];
                motor5.Text = "Motor5:" + Receive_Package.Motor_Speed[4];
                motor6.Text = "Motor6:" + Receive_Package.Motor_Speed[5];
                time_count++;
                if (time_count == delta_time)
                {
                    time_count = 0;
                    Error_Package = 0;
                    Success_Package = 0;
                }
                if (Error_Package == 0)
                    err_package_rate.Text = "丢包率：0%";
                else
                    err_package_rate.Text = "丢包率：" + (((float)Error_Package / (float)(Success_Package + Error_Package)) * 100).ToString("F2") + "%";
                if(Last_Error_Package==Error_Package)
                {
                    correct_flag.Text = "数据包正确";
                    correct_flag.ForeColor = Color.Green;
                }
                else
                {
                    correct_flag.Text = "数据包错误";
                    correct_flag.ForeColor = Color.Red;

                }
                Last_Error_Package = Error_Package;
                AddData(Receive_Package.Motor_Speed[0]);
                DrawPIcture();
                Display_Map();
                Thread.Sleep(100);
            }
        }
        byte Temp_Register = 0x00;  //Which Contains the data haven't been Handle
        int Error_Package = 0, Success_Package = 0, Last_Error_Package = 0;
        byte[] Rec_Buffer1 = new byte[1];  //接收缓冲区1
        void Received_CallBack(byte[] Receive_Buffer)
        {
            byte Buffer_Data_Counter = 0;
            byte Buffer_Flag = 0;
            while (true)
            {
                if (Buffer_Flag == 0 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
                    Buffer_Flag = 1;
                else if (Buffer_Flag == 1 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
                    break;
                else if (Buffer_Data_Counter>11)
                {
                    err_msg.AppendText("丢桢尾\r\n");
                    Error_Package++;
                    break;
                }
                Buffer_Data_Counter++;
            }
            Buffer_Data_Counter++;
            if (Buffer_Data_Counter == 12)   //if the package had been broken
            {
                byte[] Data_Buffer = new byte[8];
                byte PID_Bit = Convert.ToByte(Receive_Buffer[1] >> 4);  //Get The PID Bit
                if (PID_Bit == ((~(Receive_Buffer[1] & 0x0f)) & 0x0f))  //PID Verify Success
                {
                    byte Temp_Var = 0x00;
                    while (true)     //Memory Copy
                    {
                        Data_Buffer[Temp_Var] = Receive_Buffer[2 + Temp_Var];
                        Temp_Var++;
                        if (Temp_Var == 8)
                            break;
                    }
                    if (Receive_Buffer[10] != 0x00)   //Some Byte had been replace
                    {
                        byte Temp_Filter = 0x00;
                        Temp_Var = 0;
                        while (true)
                        {
                            if (((Receive_Buffer[10] & (Temp_Filter | (0x01 << Temp_Var))) >> Temp_Var) == 1)  //This Byte Need To Adjust
                                Data_Buffer[Temp_Var] = 0xff;    //Adjust to 0xff
                            Temp_Var++;
                            if (Temp_Var == 8)
                                break;
                        }
                    }
                    Success_Package++;
                    switch (PID_Bit)
                    {
                        case 0:
                            Temp_Var = 0;
                            while (true)
                            {
                                Receive_Package.Castle_Bule[Temp_Var] = Data_Buffer[Temp_Var];
                                Temp_Var++;
                                if (Temp_Var == 7)
                                    break;
                            }
                            Receive_Package.Castle_Red[0] = Data_Buffer[7];
                            break;
                        case 1:
                            Temp_Var = 0;
                            while (true)
                            {
                                Receive_Package.Castle_Red[Temp_Var + 1] = Data_Buffer[Temp_Var];
                                Temp_Var++;
                                if (Temp_Var == 6)
                                    break;
                            }
                            Receive_Package.Block[0] = Convert.ToByte(Data_Buffer[7] >> 5);
                            Receive_Package.Block[1] = Convert.ToByte(((Data_Buffer[7] & 0x1C) >> 2));
                            Temp_Register = Convert.ToByte((Data_Buffer[7] & 0x03) << 1);
                            break;
                        case 2:
                            Receive_Package.Block[2] = Convert.ToByte((Temp_Register) | (Data_Buffer[0] >> 7));
                            Receive_Package.Block[3] = Convert.ToByte((Data_Buffer[0] & 0x70) >> 4);
                            Receive_Package.Block[4] = Convert.ToByte((Data_Buffer[0] & 0x0e) >> 1);
                            Receive_Package.Block[5] = Convert.ToByte(((Data_Buffer[0] & 0x01) << 2) | (Data_Buffer[1] >> 6));
                            Receive_Package.Block[6] = Convert.ToByte((Data_Buffer[1] & 0x38) >> 3);
                            Receive_Package.Block[7] = Convert.ToByte(Data_Buffer[1] & 0x07);   //Data_Buffer[1] All Handled
                            Receive_Package.Block[8] = Convert.ToByte(Data_Buffer[2] >> 5);
                            Receive_Package.Block[9] = Convert.ToByte((Data_Buffer[2] & 0x1C) >> 2);
                            Receive_Package.Block[10] = Convert.ToByte(((Data_Buffer[2] & 0x03) << 1) | Data_Buffer[3] >> 7);
                            Receive_Package.Block[11] = Convert.ToByte((Data_Buffer[3] & 0x70) >> 4);
                            Receive_Package.Block[12] = Convert.ToByte((Data_Buffer[3] & 0x0e) >> 1);
                            Receive_Package.Block[13] = Convert.ToByte(((Data_Buffer[3] & 0x01) << 2) | (Data_Buffer[4] >> 6));
                            Receive_Package.Block[14] = Convert.ToByte((Data_Buffer[4] & 0x38) >> 3);
                            Receive_Package.Block[15] = Convert.ToByte(Data_Buffer[4] & 0x07);   //Data_Buffer[4] All Handled
                            Receive_Package.Block[16] = Convert.ToByte(Data_Buffer[5] >> 5);
                            Receive_Package.Block[17] = Convert.ToByte((Data_Buffer[5] & 0x1C) >> 2);
                            Receive_Package.Block[18] = Convert.ToByte(((Data_Buffer[5] & 0x03) << 1) | Data_Buffer[6] >> 7);
                            Receive_Package.Block[19] = Convert.ToByte((Data_Buffer[6] & 0x70) >> 4);
                            Receive_Package.Block[20] = Convert.ToByte((Data_Buffer[6] & 0x0e) >> 1);
                            Receive_Package.Block[21] = Convert.ToByte(((Data_Buffer[6] & 0x01) << 2) | (Data_Buffer[7] >> 6));
                            Receive_Package.Block[22] = Convert.ToByte((Data_Buffer[7] & 0x38) >> 3);
                            Receive_Package.Block[23] = Convert.ToByte(Data_Buffer[7] & 0x07);   //Data_Buffer[7] All Handled
                            break;
                        case 3:
                            Receive_Package.Block[24] = Convert.ToByte(Data_Buffer[0] >> 5);
                            Receive_Package.Block[25] = Convert.ToByte((Data_Buffer[0] & 0x1C) >> 2);
                            Receive_Package.Block[26] = Convert.ToByte(((Data_Buffer[0] & 0x03) << 1) | Data_Buffer[1] >> 7);
                            Receive_Package.Block[27] = Convert.ToByte((Data_Buffer[1] & 0x70) >> 4);
                            Receive_Package.Block[28] = Convert.ToByte((Data_Buffer[1] & 0x0e) >> 1);
                            Receive_Package.Block[29] = Convert.ToByte(((Data_Buffer[1] & 0x01) << 2) | (Data_Buffer[2] >> 6));
                            Receive_Package.Block[30] = Convert.ToByte((Data_Buffer[2] & 0x38) >> 3);
                            Receive_Package.Block[31] = Convert.ToByte(Data_Buffer[2] & 0x07);   //Data_Buffer[2] All Handled
                            Receive_Package.Block[32] = Convert.ToByte(Data_Buffer[3] >> 5);
                            Receive_Package.Block[33] = Convert.ToByte((Data_Buffer[3] & 0x1C) >> 2);
                            Receive_Package.Block[34] = Convert.ToByte(((Data_Buffer[3] & 0x03) << 1) | Data_Buffer[4] >> 7);
                            Receive_Package.Block[35] = Convert.ToByte((Data_Buffer[4] & 0x70) >> 4);
                            Receive_Package.Block[36] = Convert.ToByte((Data_Buffer[4] & 0x0e) >> 1);
                            Receive_Package.Block[37] = Convert.ToByte(((Data_Buffer[4] & 0x01) << 2) | (Data_Buffer[5] >> 6));
                            Receive_Package.Block[38] = Convert.ToByte((Data_Buffer[5] & 0x38) >> 3);
                            Receive_Package.Block[39] = Convert.ToByte(Data_Buffer[5] & 0x07);   //Data_Buffer[5] All Handled
                            Receive_Package.Block[40] = Convert.ToByte(Data_Buffer[6] >> 5);
                            Receive_Package.Block[41] = Convert.ToByte((Data_Buffer[6] & 0x1C) >> 2);
                            Receive_Package.Block[42] = Convert.ToByte(((Data_Buffer[6] & 0x03) << 1) | Data_Buffer[7] >> 7);
                            Receive_Package.Block[43] = Convert.ToByte((Data_Buffer[7] & 0x70) >> 4);
                            Receive_Package.Block[44] = Convert.ToByte((Data_Buffer[7] & 0x0e) >> 1);
                            Temp_Register = Convert.ToByte((Data_Buffer[7] & 0x01) << 2);
                            break;
                        case 4:
                            Receive_Package.Block[45] = Convert.ToByte(Temp_Register | (Data_Buffer[0] >> 6));
                            Receive_Package.Block[46] = Convert.ToByte((Data_Buffer[0] & 0x38) >> 3);
                            Receive_Package.Block[47] = Convert.ToByte(Data_Buffer[0] & 0x07);   //Data_Buffer[0] All Handled
                            Receive_Package.Block[48] = Convert.ToByte(Data_Buffer[1] >> 5);
                            Receive_Package.Block[49] = Convert.ToByte((Data_Buffer[1] & 0x1C) >> 2);
                            Receive_Package.Block[50] = Convert.ToByte(((Data_Buffer[1] & 0x03) << 1) | Data_Buffer[2] >> 7);
                            Receive_Package.Block[51] = Convert.ToByte((Data_Buffer[2] & 0x70) >> 4);
                            Receive_Package.Block[52] = Convert.ToByte((Data_Buffer[2] & 0x0e) >> 1);
                            Receive_Package.Block[53] = Convert.ToByte(((Data_Buffer[2] & 0x01) << 2) | (Data_Buffer[3] >> 6));
                            Receive_Package.Block[54] = Convert.ToByte((Data_Buffer[3] & 0x38) >> 3);
                            Receive_Package.Block[55] = Convert.ToByte(Data_Buffer[3] & 0x07);   //Data_Buffer[3] All Handled
                            Receive_Package.Block[56] = Convert.ToByte(Data_Buffer[4] >> 5);
                            Receive_Package.Block[57] = Convert.ToByte((Data_Buffer[4] & 0x1C) >> 2);
                            Receive_Package.Block[58] = Convert.ToByte(((Data_Buffer[4] & 0x03) << 1) | Data_Buffer[5] >> 7);
                            Receive_Package.Block[59] = Convert.ToByte((Data_Buffer[5] & 0x70) >> 4);
                            Receive_Package.Block[60] = Convert.ToByte((Data_Buffer[5] & 0x0e) >> 1);
                            Receive_Package.Block[61] = Convert.ToByte(((Data_Buffer[5] & 0x01) << 2) | (Data_Buffer[6] >> 6));
                            Receive_Package.Block[62] = Convert.ToByte((Data_Buffer[6] & 0x38) >> 3);
                            Receive_Package.Car_Blue[0] = Convert.ToByte(Data_Buffer[7] >> 4);
                            Receive_Package.Car_Blue[1] = Convert.ToByte(Data_Buffer[7] & 0x0F);
                            break;
                        case 5:
                            Receive_Package.Car_Red[0] = Convert.ToByte(Data_Buffer[0] >> 4);
                            Receive_Package.Car_Red[1] = Convert.ToByte(Data_Buffer[0] & 0x0F);
                            Receive_Package.Least_Time = Data_Buffer[1];
                            Receive_Package.Score_Blue = Convert.ToInt16(Data_Buffer[5].ToString("X2") + Data_Buffer[4].ToString("X2"), 16);
                            Receive_Package.Score_Red = Convert.ToInt16(Data_Buffer[7].ToString("X2") + Data_Buffer[6].ToString("X2"), 16);
                            break;
                        case 6:
                            Temp_Var = 0;
                            while (true)
                            {
                                //Int16 a = Convert.ToInt16(Data_Buffer[Temp_Var + 1] << 8);
                                Receive_Package.Motor_Speed[Temp_Var / 2] = Convert.ToInt16((Data_Buffer[Temp_Var + 1].ToString("X2")
                                    + Data_Buffer[Temp_Var].ToString("X2")), 16);
                                Temp_Var += 2;
                                if (Temp_Var == 8)
                                    break;
                            }
                            break;
                        case 7:
                            Temp_Var = 0;
                            while (true)
                            {
                                Receive_Package.Motor_Speed[(Temp_Var / 2) + 4] = Convert.ToInt16((Data_Buffer[Temp_Var + 1].ToString("X2")
                                    + Data_Buffer[Temp_Var].ToString("X2")), 16);
                                Temp_Var += 2;
                                if (Temp_Var == 8)
                                    break;
                            }
                            break;
                        case 8:
                            Receive_Package.Gryo_Scope_Gryo = Convert.ToInt32(Data_Buffer[0].ToString("X2")
                                    + Data_Buffer[1].ToString("X2") + Data_Buffer[2].ToString("X2") + Data_Buffer[3].ToString("X2"), 16);
                            Receive_Package.Gryo_Scope_Angle = Convert.ToInt32(Data_Buffer[4].ToString("X2")
                                    + Data_Buffer[5].ToString("X2") + Data_Buffer[6].ToString("X2") + Data_Buffer[7].ToString("X2"), 16);
                            Receive_Package.Gryo_Scope_Gryo = Receive_Package.Gryo_Scope_Gryo / 10000.0f;
                            Receive_Package.Gryo_Scope_Angle = Receive_Package.Gryo_Scope_Angle / 10000.0f;
                            break;
                        case 9:
                            Receive_Package.X_Possition = Convert.ToInt32(Data_Buffer[0].ToString("X2")
                                    + Data_Buffer[1].ToString("X2") + Data_Buffer[2].ToString("X2") + Data_Buffer[3].ToString("X2"), 16);
                            Receive_Package.Y_Possition = Convert.ToInt32(Data_Buffer[4].ToString("X2")
                                    + Data_Buffer[5].ToString("X2") + Data_Buffer[6].ToString("X2") + Data_Buffer[7].ToString("X2"), 16);
                            Receive_Package.X_Possition = (Receive_Package.X_Possition / 10000.0f);
                            Receive_Package.Y_Possition = (Receive_Package.Y_Possition / 10000.0f);
                            //Application.DoEvents();
                            break;
                        case 10:
                            //Here To Do The ACK Response
                            break;
                    }
                }
            }
            else
            {
                err_msg.AppendText("丢数据包\r\n");
                Error_Package++;
                Thread.Sleep(100);
            }
        }
        static void Fill_Array(byte[] arr, byte fill)
        {
            for (int i = 0; i < arr.Length; i++)
            {
                arr[i] = fill;
            }
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            Receive_Package.Block = new byte[63];
            Receive_Package.Car_Blue = new byte[2];
            Receive_Package.Car_Red = new byte[2];
            Receive_Package.Castle_Bule = new byte[7];
            Receive_Package.Castle_Red = new byte[7];
            Receive_Package.Motor_Speed = new short[8];
        }
        Queue<float> data1 = new Queue<float>();
        private void AddData(int number1)
        {
            data1.Enqueue(number1);
            if (data1.Count > 100)
                data1.Dequeue();//数据在此时移动
        }
        private Bitmap DrawPIcture()
        {
            Bitmap bmp1 = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            pictureBox1.Image = bmp1;
            PointF[] points1 = new PointF[data1.Count];
            float[] dataInt1 = data1.ToArray();
            for (int i = 0; i < data1.Count; i++)
            {
                points1[i] = new PointF((bmp1.Width / 100) * i * 1.1f, ((pictureBox1.Height/2) - (dataInt1[i] / 200)));
            }
            if (data1.Count > 1)
                using (var g1 = Graphics.FromImage(bmp1))
                {
                    Pen pe = new Pen(Color.Blue,2);
                    g1.Clear(BackColor);
                    g1.SmoothingMode = SmoothingMode.HighQuality;
                    g1.DrawCurve(pe, points1);
                }
            return bmp1;
        }
        byte Ignore_Flag = 0;
        int delta_time = 50;
        int time_count = 0;
        private void RecMsg()
        {
            while (true)
            {
                byte[] Buffer = new byte[1];
                try
                {
                    socketClient.Receive(Buffer);
                    if (Buffer[0] == 0xff && Rcv_Flg == 1 && Ignore_Flag == 0)
                    {
                        Rcv_Flg = 0;//结束收发
                        RX_Counter = 1;
                        USART3_Rcv_Buffer[11] = 0xff;
                        if (USART3_Rcv_Buffer[0] == 0xff && USART3_Rcv_Buffer[1] == 0x00 && USART3_Rcv_Buffer[11] == 0xff)
                        {
                            err_msg.AppendText("包错误\r\n");
                            Ignore_Flag = 1;
                            Fill_Array(USART3_Rcv_Buffer, 0x00);
                            Rcv_Flg = 0;//结束收发
                            RX_Counter = 1;
                            Error_Package++;
                            Thread.Sleep(1000);
                        }
                        else
                            Received_CallBack(USART3_Rcv_Buffer);
                    }
                    else if (Buffer[0] == 0xff && Ignore_Flag == 1)
                    {
                        Ignore_Flag = 0;
                        Fill_Array(USART3_Rcv_Buffer, 0x00);
                        Rcv_Flg = 0;//结束收发
                        RX_Counter = 1;
                        Error_Package++;
                    }
                    else if (Buffer[0] == 0xff && Rcv_Flg == 0 && Ignore_Flag == 0 )
                    {
                        Rcv_Flg = 1;
                        RX_Counter = 1;
                        Fill_Array(USART3_Rcv_Buffer, 0x00);
                        USART3_Rcv_Buffer[0] = 0xff;
                    }
                    else if (Rcv_Flg == 1)
                    {
                        USART3_Rcv_Buffer[RX_Counter] = Buffer[0];
                        RX_Counter++;
                        if (RX_Counter > 11)
                        {
                            Rcv_Flg = 1;
                            RX_Counter = 0;
                            Fill_Array(USART3_Rcv_Buffer, 0x00);
                            err_msg.AppendText("中断丢桢尾\r\n");
                            Thread.Sleep(100);
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("系统异常消息:" + ex.Message);
                    Connect_To.Enabled = true;
                    break;
                }
            }
        }
        private void button10_Click(object sender, EventArgs e)
        {
            try
            {
                socketClient = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                IPAddress ipaddress = IPAddress.Parse("192.168.4.1");
                //IPAddress ipaddress = IPAddress.Parse("10.21.30.45");
                IPEndPoint endpoint = new IPEndPoint(ipaddress, int.Parse("8080"));
                socketClient.Connect(endpoint);
                Thread threadClient = new Thread(RecMsg);
                threadClient.IsBackground = true;
                threadClient.Start();
                MessageBox.Show("已与服务端建立连接,可以开始通信...\r\n");
                Connect_To.Enabled = false;
                Thread Display_Thread = new Thread(Display_Data);
                Display_Thread.IsBackground = true;
                Display_Thread.Start();
            }
            catch
            {
                MessageBox.Show("连接失败!");
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte Cup_Num,Ball_Num,Start_Flag;
            byte[] Transmmit_Data=new byte[8];
            Cup_Num = 1;
            Ball_Num = 2;
            Start_Flag = 0;
            short X_Pos = Convert.ToInt16((Convert.ToSingle(x_e_pos.Text.ToString()) * 1000) / 1);
            short Y_Pos = Convert.ToInt16((Convert.ToSingle(y_e_pos.Text.ToString()) * 1000) / 1);
            Transmmit_Data[0] = Ball_Num;
            Transmmit_Data[1] = Cup_Num;
            Transmmit_Data[2] = Start_Flag;

            Transmmit_Data[4] = Convert.ToByte(X_Pos >> 8);
            Transmmit_Data[5] = Convert.ToByte(X_Pos & 0x00ff);
            Transmmit_Data[6] = Convert.ToByte(Y_Pos >> 8);
            Transmmit_Data[7] = Convert.ToByte(Y_Pos & 0x00ff);
            Send_Data(0, Transmmit_Data);
        }
    }
}

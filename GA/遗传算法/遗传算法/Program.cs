using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace 遗传算法
{
    class Program
    {
        static int POP_NUM = 500;  //个体数
        static int STEP_NUM = 30;  //步数
        enum STEP
        {
            UP=0,
            DOWN=1,
            LEFT=2,
            RIGHT=3,
            PLACE_CUP=4,
            PLACE_BALL=5,
        }
        struct Population
        {
            public STEP[] Step;
        }
        //30s，1s为1个单位
        static Population[] Populate = new Population[POP_NUM];
        byte[] Map = new byte[63];
        static float[] Eva_Value = new float[POP_NUM];
        static float[] Mate_Prob = new float[POP_NUM];
        static STEP Get_Next_Step(int Counter)
        {

        }
        static void Init_Seed()
        {
            Random Rank = new Random();
            int Counter = 0, Counter2 = 0;
            while (true)
            {
                Populate[Counter].Step = new STEP[STEP_NUM];
                //Console.Write("Populate" + Counter.ToString() + ":");
                while (true)
                {
                    Populate[Counter].Step[Counter2] = Get_Next_Step(Counter);
                    //Console.Write("(" + Populate[Counter].X[Counter2].ToString() + "," + Populate[Counter].Y[Counter2] + ") ");
                    Counter2++;
                    if (Counter2 == STEP_NUM)
                        break;
                }
                //Console.Write("\r\n");
                Counter2 = 0;
                Counter++;
                if (Counter == POP_NUM)
                    break;
            }
        }
        static void Evaluate_Popular()
        {
            int Counter = 0;
            while (true)
            {
                Eva_Value[Counter] = Evaluate_Function(Populate[Counter]);
                Counter++;
                if (Counter == POP_NUM)
                    break;
            }
        }
        static float Evaluate_Function(Population pop)
        {
            return 0;
        }
        static void Fill_Array(float[] arr,float fill)
        {
            for (int i = 0; i < arr.Length; i++)
            {
                arr[i] = fill;
            } 
        }
        static void Output_Array(float[] arr)
        {
            for (int i = 0; i < arr.Length; i++)
                 Console.WriteLine("Array[{0}]={1}", i, arr[i]); 
        }
        static void Popular_Compete()
        {
            Random rank=new Random();
            int Counter = 0;
            while(true)
            {
                int a_prob=rank.Next(0,500);
                int b_prob=rank.Next(0,500);
                if (Eva_Value[a_prob] > Eva_Value[b_prob])
                {
                    Mate_Prob[a_prob] = Mate_Prob[a_prob] + ((Eva_Value[a_prob] - Eva_Value[b_prob]) / 1000.0f);
                    Mate_Prob[b_prob] = Mate_Prob[b_prob] - ((Eva_Value[a_prob] - Eva_Value[b_prob]) / 1000.0f);
                }
                if (Eva_Value[a_prob] < Eva_Value[b_prob])
                {
                    Mate_Prob[a_prob] = Mate_Prob[a_prob] - ((Eva_Value[b_prob] - Eva_Value[a_prob]) / 1000.0f);
                    Mate_Prob[b_prob] = Mate_Prob[b_prob] + ((Eva_Value[b_prob] - Eva_Value[a_prob]) / 1000.0f);
                }
                Counter++;
                if (Counter == POP_NUM)
                    break;
            }
        }
        static int Iteration_Num = 1000; //迭代1000次
        static float c = 0.2f;   //变异概率0.2
        static void Popular_Mate()
        {
            int Counter = 0;
            while(true)
            {

                Counter++;
                if (Counter == 250)
                    break;
            }
        }
        static int Select_Best()
        {
            int Counter = 1;
            float Maxinum = Eva_Value[0];
            while(true)
            {
                if (Eva_Value[Counter] > Maxinum)
                    Maxinum = Eva_Value[Counter];
                Counter++;
                if (Counter == POP_NUM)
                    break;
            }
            return Counter;
        }
        static void Popular_Variation()
        {

        }
        static void Main(string[] args)
        {
            int Iteration = 0;
            Init_Seed();        //初始化种群
            Fill_Array(Mate_Prob, 0.5f); //填充交配概率
            Console.Write("Init Success!\r\n");
            while (true)
            {
                Iteration++;
                Evaluate_Popular(); //评估个体
                Popular_Compete();  //个体竞争
                Popular_Mate();     //个体杂交
                Popular_Variation(); //个体变异
                if (Iteration == Iteration_Num)
                    break;
            }
            Console.Read();
        }
    }
}

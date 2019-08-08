using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;

namespace TspGA
{
    public partial class frmGa : Form
    {
        Bitmap image;
        Graphics formGraphics;
        int cityNum;
        static int[] x;
        static int[] y;

        static double Max_ratio;//图像放大比率

        public frmGa()
        {
            InitializeComponent();
        }

        public void readTxt(string filename)
        {
            x = new int[cityNum];
            y = new int[cityNum];
            int i = 0;
            using (StreamReader sr = new StreamReader(filename))
            {
                string line;

                while ((line = sr.ReadLine()) != null)
                {
                    //this.ListBox1.Items.Add("line ");   //读出
                    // 字符分割  
                    string[] strcol = line.Split(new char[] { ' ' });
                    x[i] = Convert.ToInt32(strcol[1]);// x坐标  
                    y[i] = Convert.ToInt32(strcol[2]);// y坐标  
                    i++;
                }
            }
        }

        public delegate void setProgressInvoke(int num);
        public void setProgressBar(int num)
        {
            if (this.InvokeRequired)
            {
                setProgressInvoke _setInvoke = new setProgressInvoke(setProgressBar);
                this.Invoke(_setInvoke, new object[] { num });
            }
            else
            {
                label3.Text = (num + 1).ToString();
                progressBar1.Value = (num % 100) + 1;
            }
        }
        public delegate void setListInvoke(string str, int flag);
        public void setList(string str, int flag)
        {
            if (this.InvokeRequired)
            {
                setListInvoke _setInvoke = new setListInvoke(setList);
                this.Invoke(_setInvoke, new object[] { str, flag });
            }
            else
            {
                switch (flag)
                {
                    case 1:
                        listBox1.Items.Add(str);
                        break;
                    case 2:
                        linitGroup.Items.Add(str);
                        break;
                    case 3:
                        llastGroup.Items.Add(str);
                        break;
                    default:
                        break;
                }
            }
        }


        private void frmGa_Load(object sender, EventArgs e)
        {
            txtScale.Text = "30";
            txtCityNum.Text = "48";
            txtMAX_GEN.Text = "2000";
            txtPc.Text = "0.8";
            txtPm.Text = "0.9";
            txtRatio.Text = "0.07";
            progressBar1.Minimum = 0;
            progressBar1.Maximum = 100;
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            cityNum = Convert.ToInt32(txtCityNum.Text);
            Max_ratio = Convert.ToSingle(txtRatio.Text);
            readTxt("data.txt");
            intiData();

            paintPoint();
            //种群规模，城市个数，最大迭代代数，交叉概率，变异概率，窗体对象
            ga gg = new ga(Convert.ToInt32(txtScale.Text), cityNum, Convert.ToInt32(txtMAX_GEN.Text), Convert.ToSingle(txtPc.Text), Convert.ToSingle(txtPm.Text), this);
            gg.init(x, y);
            Thread th = new Thread(gg.solve);
            th.Start();
            //gg.solve();

        }

        void intiData()
        {
            listBox1.Items.Clear();
            linitGroup.Items.Clear();
            llastGroup.Items.Clear();

            pictureBox1.Image = null;
            image = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            formGraphics = Graphics.FromImage(image);
            formGraphics.Clear(panel1.BackColor);

        }

        public delegate void drawLineInvoke(int[] line, int flag);
        public void paintLine(int[] line, int flag)
        {
            if (this.InvokeRequired)
            {
                drawLineInvoke _setInvoke = new drawLineInvoke(paintLine);
                this.Invoke(_setInvoke, new object[] { line, flag });
            }
            else
            {
                if (flag == 0)
                {
                    int i, xa, ya, xb, yb;
                    System.Drawing.Graphics formGraphics = pictureBox1.CreateGraphics();
                    Pen p = new Pen(Color.Orange, 1);//画笔

                    for (i = 1; i < cityNum; i++)
                    {
                        xa = Convert.ToInt32(x[line[i - 1]] * Max_ratio);
                        ya = Convert.ToInt32(y[line[i - 1]] * Max_ratio);

                        xb = Convert.ToInt32(x[line[i]] * Max_ratio);
                        yb = Convert.ToInt32(y[line[i]] * Max_ratio);
                        formGraphics.DrawLine(p, new Point(xa, ya), new Point(xb, yb));//画线
                    }

                    xa = Convert.ToInt32(x[line[0]] * Max_ratio);
                    ya = Convert.ToInt32(y[line[0]] * Max_ratio);

                    xb = Convert.ToInt32(x[line[cityNum - 1]] * Max_ratio);
                    yb = Convert.ToInt32(y[line[cityNum - 1]] * Max_ratio);
                    formGraphics.DrawLine(p, new Point(xa, ya), new Point(xb, yb));//画线

                    //paintLastPoint(line[0]);
                    pictureBox1.Image = (Image)image;
                }
                else
                {
                    int i, xa, ya, xb, yb;
                    //System.Drawing.Graphics formGraphics = pictureBox1.CreateGraphics();
                    Pen p = new Pen(Color.Orange, 1);//画笔

                    for (i = 1; i < cityNum; i++)
                    {
                        xa = Convert.ToInt32(x[line[i - 1]] * Max_ratio);
                        ya = Convert.ToInt32(y[line[i - 1]] * Max_ratio);

                        xb = Convert.ToInt32(x[line[i]] * Max_ratio);
                        yb = Convert.ToInt32(y[line[i]] * Max_ratio);
                        formGraphics.DrawLine(p, new Point(xa, ya), new Point(xb, yb));//画线
                    }

                    xa = Convert.ToInt32(x[line[0]] * Max_ratio);
                    ya = Convert.ToInt32(y[line[0]] * Max_ratio);

                    xb = Convert.ToInt32(x[line[cityNum - 1]] * Max_ratio);
                    yb = Convert.ToInt32(y[line[cityNum - 1]] * Max_ratio);
                    formGraphics.DrawLine(p, new Point(xa, ya), new Point(xb, yb));//画线

                    paintLastPoint(line[0]);
                    //pictureBox1.Image = (Image)image;
                }
            }
        }

        public delegate void paintPointInvoke();
        public void paintPoint()
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new paintPointInvoke(paintPoint));
            }
            else
            {
                //System.Drawing.Graphics formGraphics = pictureBox1.CreateGraphics();
                int i, x1, y1;
                //System.Drawing.Graphics formGraphics = pictureBox1.CreateGraphics();
                //System.Drawing.SolidBrush my0 = new System.Drawing.SolidBrush(System.Drawing.Color.Blue);//画刷
                System.Drawing.SolidBrush myBrush = new System.Drawing.SolidBrush(System.Drawing.Color.Red);//画刷
                Brush blackBrush = Brushes.Black;
                Font myFont1 = new Font("Hacttenschweiler", 7);

                for (i = 0; i < cityNum; i++)
                {
                    x1 = Convert.ToInt32(x[i] * Max_ratio);
                    y1 = Convert.ToInt32(y[i] * Max_ratio);
                    formGraphics.FillEllipse(myBrush, new Rectangle(x1, y1, 5, 5));//画实心椭圆
                    formGraphics.DrawString(i.ToString(), myFont1, blackBrush, new Point(x1, y1));//写字
                    //listBox1.Items.Add(bestTour[i].ToString() + ":" + x[bestTour[i]] + "-" + y[bestTour[i]]);
                }
                pictureBox1.Image = (Image)image;
            }
        }

        public delegate void paintLastPointInvoke(int num);
        public void paintLastPoint(int num)
        {
            if (this.InvokeRequired)
            {
                paintLastPointInvoke _setInvoke = new paintLastPointInvoke(paintLastPoint);
                this.Invoke(_setInvoke, new object[] { num });
            }
            else
            {
                System.Drawing.SolidBrush my0 = new System.Drawing.SolidBrush(System.Drawing.Color.Blue);//画刷
                Brush blackBrush = Brushes.Black;
                Font myFont1 = new Font("Hacttenschweiler", 7);

                int x1 = Convert.ToInt32(x[num] * Max_ratio);
                int y1 = Convert.ToInt32(y[num] * Max_ratio);
                formGraphics.FillEllipse(my0, new Rectangle(x1, y1, 5, 5));//画实心椭圆
                formGraphics.DrawString(num.ToString(), myFont1, blackBrush, new Point(x1, y1));//写字

                pictureBox1.Image = (Image)image;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}

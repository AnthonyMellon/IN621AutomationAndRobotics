using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Drawing;

namespace Robot_Controller
{
    public partial class Form1 : Form
    {
        String Output = "";
        Graphics graphics;

        public Form1()
        {
            InitializeComponent();
            graphics = CreateGraphics();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.Open();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            String Input;
            

            Input = serialPort1.ReadLine();
            label1.Text = Input;
            graphics.FillRectangle(new SolidBrush(Color.Red), 50, 50, 50, 50);
            graphics.FillRectangle(new SolidBrush(Color.Red), 50, 50, 100, 50);
            graphics.FillRectangle(new SolidBrush(Color.Red), 50, 50, 150, 50);
            graphics.FillRectangle(new SolidBrush(Color.Red), 50, 50, 200, 50);
            graphics.FillRectangle(new SolidBrush(Color.Red), 50, 50, 250, 50);
            graphics.FillRectangle(new SolidBrush(Color.Red), 50, 50, 300, 50);
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            Output = "";
            Output += e.KeyCode;            
            serialPort1.Write(Output);
            label2.Text = Output;
            
        }

        private int scale(int num)
        {
            int newNum = 0;

            newNum /= (2000 / 400);

            return newNum;
            
        }
    }
}

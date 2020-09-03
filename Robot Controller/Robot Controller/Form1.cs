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
using System.Text.RegularExpressions;

namespace Robot_Controller
{
    public partial class Form1 : Form
    {
        String Output = "";
        Graphics graphics;

        int[,] averageInputs = new int[8, 10];
        int currentIndex = 0;

        public Form1()
        {
            InitializeComponent();
            graphics = CreateGraphics();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.Open();
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    averageInputs[i, j] = 0;
                }
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }

        private int average (int[] inputs)
        {
            int sum = 0;
            for(int i = 0; i < inputs.Length; i++)
            {
                sum += (inputs[i]);

            }
            return sum/inputs.Length;
        }

        private int Scale(int toScale)
        {
            return toScale / (2000 / 255);
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            Output = "";
            Output += e.KeyCode;            
            serialPort1.Write(Output);
            
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            Output = "F";
            serialPort1.Write(Output);
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string Input;

            Input = serialPort1.ReadLine();
            string[] InputSplit = Input.Split(',');

            for (int i = 0; i < InputSplit.Length - 1; i++)
            {
                int value;
                try
                {

                    value = Convert.ToInt16(InputSplit[i]);
                    value = Scale(value);
                    if (value > 255)
                    {
                        value = 255;
                    }
                    averageInputs[i, currentIndex] = value;
                }
                catch (Exception ex)
                {
                }
                int[] toAverage = new int[10];
                for (int a = 0; a < 10; a++)
                {
                    toAverage[a] = averageInputs[i, a];
                }

                value = average(toAverage);
                graphics.FillRectangle(new SolidBrush(Color.FromArgb(255 - value, 255 - value, 255 - value)), ((i + 1) * 50), 50, 50, 50);
            }
            currentIndex++;
            if (currentIndex > 10)
            {
                currentIndex = 0;
            }

            for (int i = 0; i < InputSplit.Length - 1; i++)
            {
                graphics.DrawRectangle(new Pen(Color.Red), ((i + 1) * 50), 50, 50, 50);
            }

            //serialPort1.DiscardInBuffer();


        }
    }
}

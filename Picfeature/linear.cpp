#include "include1\linear.h"
#include "include1\ImageCrop.h"
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
unsigned short upCoordinate[8] = {
                               0, 0, // top (x, y)
                               0, 0, // left(x, y)
                               0, outrowsize-1, // bottom(x, y)
                               outlinesize-1, 0 // right(x, y)
                         };      //保存上管纸币图像与EE标
unsigned short downCoordinate[8] = {
                               0, 0, // top (x, y)
                               0, 0, // left(x, y)
                               0, outrowsize-1, // bottom(x, y)
                               outlinesize-1, 0 // right(x, y)
                           };  //保存下管纸币图像与背景交点坐标
/******************************************************************************/

/******************************直线拟合参数************************************/

float upLine[8] = {
                       0, 0, // k1, b1
                       0, 0, // k2, b2
                       0, 0, // k3, b3
                       0, 0  // k4, b4
                    };                 //上管图像四条边界线的斜率和截距
float downLine[8] = {
                       0, 0, // k1, b1
                       0, 0, // k2, b2
                       0, 0, // k3, b3
                       0, 0  // k4, b4
                    };	// 下管图像四条边界线的斜率和截距

/*
2018.05.24:
    重构日记：
    函数的目的就是把一个定义在pic.cpp里的变量：g_line_POINT1赋值
    这个变量存储着图像的顶点。

    注意以下变量都引用自pic.cpp 是全局变量：
    
    extern unsigned short upActualSize[2];
    extern unsigned short downActualSize[2];
    extern int k_up1;
    extern unsigned short moneyLength;
    extern unsigned short moneyHeight;
    extern volatile int g_angle;//纸币偏转角度
*/
int LinearFitting(unsigned char *__restrict image,unsigned char thresh_value, unsigned int im_row, unsigned char chan_flag)
{

    unsigned char i = 0;

    unsigned short moneyLength;
    unsigned short moneyHeight;
    unsigned char  chan;//存放由于上下管不同，保存纸币8个点的起始地址
    int angle; //上下通道的纸币旋转角度反向（简化处理）
    unsigned short coordinate[8] = {0};
    float line[8] = {0};
    unsigned short actualSize[2] = {0};
    long long Line[8] = {0};
    int g_angle;
    chan = chan_flag << 2;  //chan_flag*4
    angle = chan_flag==0?g_angle:-g_angle;
    if ((im_row<=10)||((abs(angle) <=1) && (im_row<=100))) return -1; //找点处理函数要求的条件

    Pretreatment(image, coordinate, im_row);//coordinate数组记录了纸币四个顶点的坐标。
    LenWidValue(line, actualSize, image, coordinate);//line数组记录了纸币的四条边的
    if (chan_flag == 0)
    {
        for (i = 0; i < 8; ++i)
        {
            upCoordinate[i] = coordinate[i];
            upLine[i] = line[i];
        }

        upActualSize[0] = actualSize[0];
        upActualSize[1] = ( actualSize[1] < outlinesize ) ? actualSize[1] : upCoordinate[6] - upCoordinate[2] + 1;
    }
    else
    {
        for (i = 0; i < 8; ++i)
        {
            downCoordinate[i] = coordinate[i];
            downLine[i] = line[i];
        }

        downActualSize[0] = actualSize[0];
        downActualSize[1] = ( actualSize[1] < outlinesize ) ? actualSize[1] : downCoordinate[6] - downCoordinate[2] + 1;
    }

     /******************浮点转定点**************/

    Line[0] = line[0] * Q15;
    Line[1] = line[1] * Q15;
    Line[2] = line[2] * Q15;
    Line[3] = line[3] * Q15;
    Line[4] = line[4] * Q15;
    Line[5] = line[5] * Q15;
    Line[6] = line[6] * Q15;
    Line[7] = line[7] * Q15;

     if (abs(Line[0]) < 100) //纸币没有旋转， 边线上的点为四个顶点
     {
        g_line_POINT1[chan][0].x = coordinate[2];
        g_line_POINT1[chan][0].y = 0;
        g_line_POINT1[chan][1].x = coordinate[6];
        g_line_POINT1[chan][1].y = 0;

        //下边线2个点，分别为左下顶点和右下顶点，cnt值为图像的行数
        g_line_POINT1[1+chan][0].x = coordinate[2];
        g_line_POINT1[1+chan][0].y = 330;
        g_line_POINT1[1+chan][1].x = coordinate[6];
        g_line_POINT1[1+chan][1].y = 330;

        //左边线2个点，分别为左上顶点和左下顶点
        g_line_POINT1[2+chan][0].x = coordinate[2];
        g_line_POINT1[2+chan][0].y = 0;
        g_line_POINT1[2+chan][1].x = coordinate[2];
        g_line_POINT1[2+chan][1].y = 330;

        //右边线2个点，分别为右上顶点和右下顶点
        g_line_POINT1[3+chan][0].x = coordinate[6];
        g_line_POINT1[3+chan][0].y = 0;
        g_line_POINT1[3+chan][1].x = coordinate[6];
        g_line_POINT1[3+chan][1].y = 330;
     }
     else
     {
        //上边线2个点，分别为左上顶点和右上顶点，y值为0
        // system("pause");
        g_line_POINT1[chan][0].x = 600;
        g_line_POINT1[chan][0].y = ( Line[0] * 600 + Line[1] ) >> 15;
        g_line_POINT1[chan][1].x = 800;
        g_line_POINT1[chan][1].y = ( Line[0] * 800 + Line[1] ) >> 15;

        //下边线2个点，分别为左下顶点和右下顶点，cnt值为图像的行数
        g_line_POINT1[1+chan][0].x = 600;
        g_line_POINT1[1+chan][0].y = ( Line[4] * 600 + Line[5] ) >> 15;
        g_line_POINT1[1+chan][1].x = 800;
        g_line_POINT1[1+chan][1].y = ( Line[4] * 800 + Line[5] ) >> 15;

//        //左边线2个点，分别为左上顶点和左下顶点
        g_line_POINT1[2+chan][0].x = ( (100 << 15) - Line[3] ) / Line[2];
        g_line_POINT1[2+chan][0].y = 100;
        g_line_POINT1[2+chan][1].x = ( (200 << 15) - Line[3] ) / Line[2];
        g_line_POINT1[2+chan][1].y = 200;

        //右边线2个点，分别为右上顶点和右下顶点
        g_line_POINT1[3+chan][0].x = ( (100 << 15) - Line[7] ) / Line[6];
        g_line_POINT1[3+chan][0].y = 100;
        g_line_POINT1[3+chan][1].x = ( (200 << 15) - Line[7] ) / Line[6];
        g_line_POINT1[3+chan][1].y = 200;
     }

      k_up1 = (int)(upLine[0]*256);
      moneyLength = upActualSize[1];
      moneyHeight = upActualSize[0];



    return 0;
}

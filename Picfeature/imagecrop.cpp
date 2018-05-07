#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2/opencv.hpp>
#include "include1\ImageCrop.h"
#include "include1\findfeature.h"
#include "include1\imageshear.h"
#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <iostream>
#include "include1\binary.h"

using namespace cv;
/*****************************************************************
*	函数名:	ImageCrop
*	功能描述: 截取纸币图像的某一区域图像,包含整个外接矩形框
*	形式参数: image表示纸币图像的首地址
                 parameters表示截取区域在纸币图像中的位置，单位为万分比，即将长度分成1万份所占份数
                            parameters[0]表示截取区域上边线距纸币上边线的万分比
                            parameters[1]表示截取区域下边线距纸币上边线的万分比
                            parameters[2]表示截取区域左边线距纸币左边线的万分比
                            parameters[3]表示截取区域右边线距纸币左边线的万分比
                 crop_image表示截取的图像存放的起始地址
                 chan_flag上下管标志： 0表示上管，  1表示下管
*	返回值：-1表示错误  0表示成功
* 全局变量：g_line_POINT1, 为了使速度快，采用全局变量
            crop_image_POINT1s[4]; //保存截取区域的四个顶点坐标：左上、左下、右上、右下
********************************************************************/
//POINT1 g_line_POINT1[8][2]; //保存上下管纸币上、下、左、右边界线上各找出的2个点
POINT1 crop_image_POINT1s[4];
unsigned short crp_hw[2];
unsigned short rotate_image_height=300;
unsigned short rotate_image_width=500;
int ImageCrop(unsigned char *__restrict image, const unsigned short *parameters, unsigned char *__restrict crop_image, unsigned char chan_flag, int chan)
{

//    CropAreaVertexCal(crop_image_POINT1s,parameters,0); //计算截取纸币图像区域图像的顶点坐标
    /*************************** 将截取区域包裹的矩形框区域的图像搬移到crop_image **********************************/

    unsigned short crop_image_width, crop_image_high; //截取区域包裹的矩形区域的宽度和高度
    unsigned char * t_image; //待搬运图像区域的起始地址
    unsigned short crop_image_ini_row, crop_image_ini_col;//待搬运图像区域的起始行和列
    int i,j;  //循环计数器


    //   //   namedWindow("Lu",WINDOW_NORMAL);
    //   //  cvShowImage( "Lu", src1);
//          waitKey(0);

    //POINT1 g_line_POINT1[8][2];

    CropAreaVertexCal(crop_image_POINT1s,parameters,chan_flag); //计算截取纸币图像区域图像的顶点坐标
    /*************************** 将截取区域包裹的矩形框区域的图像搬移到crop_image **********************************/
//    IplImage *src1;
//    src1 = cvLoadImage("Gray_Image.bmp");

//    cvLine(src1,cvPoint(crop_image_POINT1s[0].x,crop_image_POINT1s[0].y),cvPoint(crop_image_POINT1s[1].x,crop_image_POINT1s[1].y),CV_RGB(0,255,0),1);
//    cvLine(src1,cvPoint(crop_image_POINT1s[0].x,crop_image_POINT1s[0].y),cvPoint(crop_image_POINT1s[2].x,crop_image_POINT1s[2].y),CV_RGB(0,255,0),1);
//    cvLine(src1,cvPoint(crop_image_POINT1s[1].x,crop_image_POINT1s[1].y),cvPoint(crop_image_POINT1s[3].x,crop_image_POINT1s[3].y),CV_RGB(0,255,0),1);
//    cvLine(src1,cvPoint(crop_image_POINT1s[2].x,crop_image_POINT1s[2].y),cvPoint(crop_image_POINT1s[3].x,crop_image_POINT1s[3].y),CV_RGB(0,255,0),1);

//      cvShowImage( "原图", src1);
//      waitKey(0);
    crop_image_width = crop_image_POINT1s[2].x > crop_image_POINT1s[3].x ? crop_image_POINT1s[2].x : crop_image_POINT1s[3].x;
    crop_image_ini_col = crop_image_POINT1s[0].x < crop_image_POINT1s[1].x ? crop_image_POINT1s[0].x : crop_image_POINT1s[1].x;
    crop_image_width = crop_image_width - crop_image_ini_col;

    //计算矩形区域的高度
    crop_image_high = crop_image_POINT1s[1].y > crop_image_POINT1s[3].y ? crop_image_POINT1s[1].y : crop_image_POINT1s[3].y;
    crop_image_ini_row = 	crop_image_POINT1s[0].y < crop_image_POINT1s[2].y ? crop_image_POINT1s[0].y : crop_image_POINT1s[2].y;
    crop_image_high = crop_image_high - crop_image_ini_row;
//    if ((crop_image_width > CRO_COL) || (crop_image_high > CRO_ROW))
//    {
//        errorTag = OVER_BORDER;
//        return -1;
//    }

    //旋转的功能需要外接矩形的高度和宽度
    crp_hw[0] = crop_image_high;
    crp_hw[1] = crop_image_width;

    //将存放截取区域图像的内存填250，便于与字符前景颜色相同，采用背景色250，方便冠字号精确定位处理
    memset(crop_image, 250, CRO_COL*CRO_ROW);

    //先将timage指向要定位的图像区域
    t_image = image + crop_image_ini_row * COL + crop_image_ini_col;
    //将timage指向的高度为crop_image_high,宽度为crop_image_width的区域拷贝出来
    for (i=0; i<crop_image_high;i++)
    {
        memcpy(crop_image+i*CRO_COL, t_image, crop_image_width);
        t_image = t_image + COL; //指向下一行
    }
    /*************************  处理crop_image 四个顶点坐标 **********************************/
    crop_image_POINT1s[0].x -= crop_image_ini_col;  //左上
    crop_image_POINT1s[0].y -= crop_image_ini_row;
    crop_image_POINT1s[1].x -= crop_image_ini_col;  //左下
    crop_image_POINT1s[1].y -= crop_image_ini_row;
    crop_image_POINT1s[2].x -= crop_image_ini_col;  //右上
    crop_image_POINT1s[2].y -= crop_image_ini_row;
    crop_image_POINT1s[3].x -= crop_image_ini_col;  //右下
    crop_image_POINT1s[3].y -= crop_image_ini_row;
    unsigned char *rotate_image= new unsigned char[CRO_ROW * CRO_COL];

    int temp = ImageShear(crop_image, CRO_COL, rotate_image, &rotate_image_height, &rotate_image_width, 255,0);

    /*************************  处理crop_image 四个顶点坐标 **********************************/
//      int tem1=crop_image_POINT1s[0].x < crop_image_POINT1s[1].x ? crop_image_POINT1s[0].x : crop_image_POINT1s[1].x;
//      int tem2=crop_image_POINT1s[2].x > crop_image_POINT1s[3].x ? crop_image_POINT1s[2].x : crop_image_POINT1s[3].x;
//      int tem3=crop_image_POINT1s[0].y < crop_image_POINT1s[2].y ? crop_image_POINT1s[0].y : crop_image_POINT1s[2].y;
//      int tem4=crop_image_POINT1s[1].y > crop_image_POINT1s[3].y ? crop_image_POINT1s[1].y : crop_image_POINT1s[3].y;
//      int k=tem3*outlinesize+tem1;
//      int out=tem2-tem1;
//      int out1=tem4-tem3;

//      Mat image1(out1,out,CV_8UC1,Scalar(255));
//      for(int i=0;i<out1;i++)
//      {

//          for(int j=0;j<out;j++)
//          {

//              image1.at<uchar>(i,j)=image[k];
//              k++;
//          }
//          k-=out;
//          k+=outlinesize;
//      }

//      imwrite("test1.bmp",image1);

//      image1.release();
//    shear(image);
    if(chan==8)
    {
        int count[10];
        const char* a="people/d5.bmp";
        const char* a1="people/d10.bmp";
        const char* a2="people/d20.bmp";
        const char* a3="people/d50.bmp";
        const char* a4="people/d100.bmp";
        const char* a5="people/d6.bmp";
        const char* a6="people/d51.bmp";
        const char* a7="people/d11.bmp";
        const char* a8="people/d21.bmp";
        const char* a9="people/d101.bmp";
         count[0]=same(a);
         count[1]=same(a1);
         count[2]=same(a2);
         count[3]=same(a3);
         count[4]=same(a4);
         count[5]=same(a5);
         count[6]=same(a6);
         count[7]=same(a7);
         count[8]=same(a8);
         count[9]=same(a9);
         int min=9999;
         for(int i=0;i<10;i++)
         {
             if(count[i]<min)
             {
                 min=count[i];
             }
         }
         for(int i=0;i<10;i++)
         {
             if(min==count[i])
             {
                 return i;
             }
         }
    }
    if(chan==9)
    {
        int count[5];
        const char* a="people/x1.bmp";
        const char* a1="people/x5.bmp";
        const char* a2="people/x10.bmp";
        const char* a3="people/x20.bmp";
        const char* a4="people/x2.bmp";
         count[0]=same(a);
         count[1]=same(a1);
         count[2]=same(a2);
         count[3]=same(a3);
         count[4]=same(a4);
         int min=9999;
         for(int i=0;i<5;i++)
         {
             if(count[i]<min)
             {
                 min=count[i];
             }
         }
         for(int i=0;i<5;i++)
         {
             if(min==count[i])
             {
                 if(i==4)
                 {
                     return 0;
                 }
                 return i;
             }
         }
    }
    int count;
   count=erzhihua(chan);
    return count;
}

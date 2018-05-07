
#include "include1\ImageCrop.h"
#include "iostream"
/*****************************************************************
*	函数名:	CropAreaVertexCal
*	功能描述: 计算截取纸币图像区域图像的顶点坐标高精度版本
*	形式参数: parameters表示截取区域在纸币图像中的位置，单位为万分比，即将长度分成1万份所占份数
                      parameters[0]表示截取区域上边线距纸币上边线的万分比
                      parameters[1]表示截取区域下边线距纸币上边线的万分比
                      parameters[2]表示截取区域左边线距纸币左边线的万分比
                      parameters[3]表示截取区域右边线距纸币左边线的万分比
              crop_area_POINT1s表示计算得到的区域四个顶点的坐标：左上、左下、右上、右下
              chan_flag表示上下管
*	返回值：无
* 全局变量：g_line_POINT1, 为了使速度快，采用全局变量
            crop_image_POINT1s[4]; //保存截取区域的四个顶点坐标：左上、左下、右上、右下
            crp_hw[2]; //截取图像的高度和宽度
********************************************************************/
extern POINT1 g_line_point[8][2]; //保存上下管纸币上、下、左、右边界线上各找出的2个点
void CropAreaVertexCal(POINT1 * crop_area_POINT1s, const unsigned short * parameters, unsigned char chan_flag)
{
    long long k1_num, k1_den; //纸币图像上下边线斜率:分子、分母,分开是避免浮点处理
    long long b11; //纸币图像上边线纵截距(y轴)
    long long b12; //纸币图像下边线纵截距(y轴)
    long long k2_num, k2_den; //纸币图像左右边线斜率:分子、分母,分开是避免浮点处理
    long long b21; //纸币图像左边线横截距(x轴)
    long long b22; //纸币图像右边线横截距(x轴)
    long long k_num, k_den; //保存k1-k2的分子、分母,分开是避免浮点处理
    long long line_POINT1[16];//用于保存g_line_POINT1精度转换后的结果

    int t_chan; //临时变量
    long long t_parameters[4]; //存放参数处理的结果，避免重复计算

    int i; //计数器

    t_chan = chan_flag == 0 ? 0 : 4; //上管8个点保存在g_line_POINT1[0]至g_line_POINT1[3],下管8个点保存在g_line_POINT1[4]至g_line_POINT1[7]

    //计算上下边线的斜率

    k1_num = g_line_POINT1[t_chan][1].y-g_line_POINT1[t_chan][0].y; //斜率分子
    k1_den = g_line_POINT1[t_chan][1].x-g_line_POINT1[t_chan][0].x; //斜率分母

    //计算左右边线的斜率
    k2_num = g_line_POINT1[t_chan+2][1].y-g_line_POINT1[t_chan+2][0].y;//斜率分子
    k2_den = g_line_POINT1[t_chan+2][1].x-g_line_POINT1[t_chan+2][0].x;//斜率分母

    //为了提高计算精度，将坐标*（2^20），即精确到1/（2^20）
    #pragma MUST_ITERATE (4);
    for (i=0; i<=3; i++)
    {
       line_POINT1[i*4] = (long long) g_line_POINT1[i+t_chan][0].x << 20;
       line_POINT1[i*4+1] = (long long) g_line_POINT1[i+t_chan][0].y << 20;
       line_POINT1[i*4+2] = (long long) g_line_POINT1[i+t_chan][1].x << 20;
       line_POINT1[i*4+3] = (long long) g_line_POINT1[i+t_chan][1].y << 20;
    }


    //计算上下左右边线的截距
    b11 = line_POINT1[3]-k1_num*line_POINT1[2]/k1_den;
    b12 = line_POINT1[7]-k1_num*line_POINT1[6]/k1_den;
    if(k2_den!=0)
    {
    b21 = line_POINT1[11]-k2_num*line_POINT1[10]/k2_den;
    b22 = line_POINT1[15]-k2_num*line_POINT1[14]/k2_den;
    }
    else
    {

        b21=k2_num*line_POINT1[10];
        b22=k2_num*line_POINT1[14];
    }
//    b21=0;
//    b22=0;
    //截取区域参数计算，避免重复计算，先保存
    t_parameters[0] =  parameters[0]*(b12-b11)/10000;
    t_parameters[1] =  parameters[1]*(b12-b11)/10000;
    t_parameters[2] =  parameters[2]*(b22-b21)/10000;
    t_parameters[3] =  parameters[3]*(b22-b21)/10000;
  //  system("pause");

    if (k2_den != 0 )  //纸币有倾斜旋转,同时确保分母不为0
    {
        if((k1_num != 0))
        {
            //计算k=k1-k2
            k_num = k1_num*k2_den - k2_num * k1_den;//k1-k2的分子
            k_den = k1_den * k2_den;//k1-k2的分母

            if (k_num == 0) k_num=1; //避免除0错误

            //计算左上边线的交点
            line_POINT1[0] = (((b21-b11)+t_parameters[2]-t_parameters[0]) * k_den)/k_num;
            line_POINT1[1] = k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[0];
            crop_area_POINT1s[0].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[0].y = line_POINT1[1] >> 20;

            //计算左下边线的交点
            line_POINT1[0] = (((b21-b11)+t_parameters[2]-t_parameters[1]) * k_den)/k_num;
            line_POINT1[1] = k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[1];
            crop_area_POINT1s[1].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[1].y = line_POINT1[1] >> 20;

            //计算右上边线的交点
            line_POINT1[0] = (((b21-b11)+t_parameters[3]-t_parameters[0]) * k_den)/k_num;
            line_POINT1[1] = k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[0];
            crop_area_POINT1s[2].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[2].y = line_POINT1[1] >> 20;

            //计算右下边线的交点
            line_POINT1[0] = (((b21-b11)+t_parameters[3]-t_parameters[1]) * k_den) / k_num;
            line_POINT1[1] =  k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[1];
            crop_area_POINT1s[3].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[3].y = line_POINT1[1] >> 20;
        }
        else
        {
            //计算左上边线的交点
            line_POINT1[0] = line_POINT1[3] + parameters[0]*(line_POINT1[7] - line_POINT1[3])/10000;
            line_POINT1[1] = (line_POINT1[0] - b21 - t_parameters[2])*k2_den/k2_num;
            crop_area_POINT1s[0].y = line_POINT1[0] >> 20;
            crop_area_POINT1s[0].x = line_POINT1[1] >> 20;

            //计算左下边线的交点
            line_POINT1[0] = line_POINT1[3] + parameters[1]*(line_POINT1[7] - line_POINT1[3])/10000;
            line_POINT1[1] = (line_POINT1[0] - b21 - t_parameters[2])*k2_den/k2_num;
            crop_area_POINT1s[1].y = line_POINT1[0] >> 20;
            crop_area_POINT1s[1].x = line_POINT1[1] >> 20;

              //计算右上边线的交点
            line_POINT1[0] = line_POINT1[3] + parameters[0]*(line_POINT1[7] - line_POINT1[3])/10000;
            line_POINT1[1] = (line_POINT1[0] - b21 - t_parameters[3])*k2_den/k2_num;
            crop_area_POINT1s[2].y = line_POINT1[0] >> 20;
            crop_area_POINT1s[2].x = line_POINT1[1] >> 20;


            //计算右下边线的交点
            line_POINT1[0] = line_POINT1[3] + parameters[1]*(line_POINT1[7] - line_POINT1[3])/10000;
            line_POINT1[1] = (line_POINT1[0] - b21 - t_parameters[3])*k2_den/k2_num;
            crop_area_POINT1s[3].y = line_POINT1[0] >> 20;
            crop_area_POINT1s[3].x = line_POINT1[1] >> 20;
        }

   }
   else //if((k2_den == 0))
   {
        if(k1_num != 0)
        {
            //计算左上边线的交点
            line_POINT1[0] = line_POINT1[8] + parameters[2]*(line_POINT1[12] - line_POINT1[8])/10000;
            line_POINT1[1] = k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[0];
            crop_area_POINT1s[0].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[0].y = line_POINT1[1] >> 20;
            //计算左下边线的交点
            line_POINT1[0] = line_POINT1[8] + parameters[2]*(line_POINT1[12] - line_POINT1[8])/10000;
            line_POINT1[1] = k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[1];
            crop_area_POINT1s[1].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[1].y = line_POINT1[1] >> 20;
            //计算右上边线的交点
            line_POINT1[0] = line_POINT1[8] + parameters[3]*(line_POINT1[12] - line_POINT1[8])/10000;
            line_POINT1[1] = k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[0];
            crop_area_POINT1s[2].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[2].y = line_POINT1[1] >> 20;
            //计算右下边线的交点
            line_POINT1[0] = line_POINT1[8] + parameters[3]*(line_POINT1[12] - line_POINT1[8])/10000;
            line_POINT1[1] = k1_num * line_POINT1[0] / k1_den + b11 + t_parameters[1];
            crop_area_POINT1s[3].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[3].y = line_POINT1[1] >> 20;
        }
        else
        {
            //计算左上边线的交点
            line_POINT1[0] = line_POINT1[8] + parameters[2]*(line_POINT1[12] - line_POINT1[8])/10000;
            line_POINT1[1] = line_POINT1[3] + parameters[0]*(line_POINT1[7] - line_POINT1[3])/10000;
            crop_area_POINT1s[0].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[0].y = line_POINT1[1] >> 20;
            //计算左下边线的交点
            line_POINT1[1] = line_POINT1[3] + parameters[1]*(line_POINT1[7] - line_POINT1[3])/10000;
            crop_area_POINT1s[1].x = crop_area_POINT1s[0].x;
            crop_area_POINT1s[1].y = line_POINT1[1] >> 20;
            //计算右上边线的交点
            line_POINT1[0] = line_POINT1[8] + parameters[3]*(line_POINT1[12] - line_POINT1[8])/10000;
            crop_area_POINT1s[2].x = line_POINT1[0] >> 20;
            crop_area_POINT1s[2].y = crop_area_POINT1s[0].y;
            //计算右下边线的交点
            crop_area_POINT1s[3].x = crop_area_POINT1s[2].x;
            crop_area_POINT1s[3].y = crop_area_POINT1s[1].y;
        }
    }

    /*else  //纸币无旋转
    {
        //计算左上边线的交点
        crop_area_POINT1s[0].x = g_line_POINT1[t_chan+2][1].x + parameters[2]*(g_line_POINT1[t_chan+3][1].x - g_line_POINT1[t_chan+2][1].x)/10000;
        crop_area_POINT1s[0].y = g_line_POINT1[t_chan][1].y + parameters[0]*(g_line_POINT1[t_chan+1][1].y - g_line_POINT1[t_chan][1].y)/10000;

        //计算左下边线的交点
        crop_area_POINT1s[1].x =  crop_area_POINT1s[0].x;
        crop_area_POINT1s[1].y = g_line_POINT1[t_chan][1].y + parameters[1]*(g_line_POINT1[t_chan+1][1].y - g_line_POINT1[t_chan][1].y)/10000;

        //计算右上边线的交点
        crop_area_POINT1s[2].x = g_line_POINT1[t_chan+2][1].x + parameters[3]*(g_line_POINT1[t_chan+3][1].x - g_line_POINT1[t_chan+2][1].x)/10000;
        crop_area_POINT1s[2].y = crop_area_POINT1s[0].y;

        //计算右下边线的交点
        crop_area_POINT1s[3].x = crop_area_POINT1s[2].x;
        crop_area_POINT1s[3].y = crop_area_POINT1s[1].y;
    }
    */
}

//低精度处理，便于理解，故保留
//void CropAreaVertexCal(POINT1 * restrict crop_area_POINT1s, unsigned short * restrict parameters, unsigned char chan_flag);
//{
// int k1_num, k1_den; //纸币图像上下边线斜率:分子、分母,分开是避免浮点处理
// int b11; //纸币图像上边线纵截距(y轴)
// int b12; //纸币图像下边线纵截距(y轴)
// int k2_num, k2_den; //纸币图像左右边线斜率:分子、分母,分开是避免浮点处理
// int b21; //纸币图像左边线横截距(x轴)
// int b22; //纸币图像右边线横截距(x轴)
// int k_num, k_den; //保存k1-k2的分子、分母,分开是避免浮点处理
//
// int t_chan; //临时变量
// int t_parameters[4]; //存放参数处理的结果，避免重复计算
//
// int i; //计数器
//
// t_chan = chan_flag == 0 ? 0 : 4; //上管8个点保存在g_line_POINT1[0]至g_line_POINT1[3],下管8个点保存在g_line_POINT1[4]至g_line_POINT1[7]
//
// //计算上下左右边线的斜率
// k1_num = g_line_POINT1[t_chan][1].y-g_line_POINT1[t_chan][0].y; //斜率分子
// k1_den = g_line_POINT1[t_chan][1].x-g_line_POINT1[t_chan][0].x; //斜率分母
// k2_num = g_line_POINT1[t_chan+2][1].y-g_line_POINT1[t_chan+2][0].y;//斜率分子
// k2_den = g_line_POINT1[t_chan+2][1].x-g_line_POINT1[t_chan+2][0].x;//斜率分母
//
// if ((k1_den != 0) && (k2_den != 0))  //纸币有倾斜旋转
//  {
//  //计算上下左右边线的截距
//  b11 = g_line_POINT1[t_chan][1].y-k1_num*g_line_POINT1[t_chan][1].x/k1_den;
//  b12 = g_line_POINT1[t_chan+1][1].y-k1_num*g_line_POINT1[t_chan+1][1].x/k1_den;
//  b21 = g_line_POINT1[t_chan+2][1].y-k2_num*g_line_POINT1[t_chan+2][1].x/k2_den;
//  b22 = g_line_POINT1[t_chan+3][1].y-k2_num*g_line_POINT1[t_chan+3][1].x/k2_den;
//
//  //计算k=k1-k2
//  k_num = k1_num*k2_den - k2_num * k1_den;//k1-k2的分子
//  k_den = k1_den * k2_den;//k1-k2的分母
//
//  if (k_num == 0) k_num=1; //避免除0错误
//
//  //截取区域参数计算，避免重复计算，先保存
//  t_parameters[0] =  parameters[0]*(b12-b11)/10000;
//  t_parameters[1] =  parameters[1]*(b12-b11)/10000;
//  t_parameters[2] =  parameters[2]*(b22-b21)/10000;
//  t_parameters[3] =  parameters[3]*(b22-b21)/10000;
//
//  //计算左上边线的交点
//  crop_area_POINT1s[0].x = (((b21-b11)+t_parameters[2]-t_parameters[0]) * k_den) / k_num;
//  crop_area_POINT1s[0].y = k1_num * crop_area_POINT1s[0].x / k1_den + b11 + t_parameters[0];
//
//  //计算左下边线的交点
//  crop_area_POINT1s[1].x = (((b21-b11)+t_parameters[2]-t_parameters[1]) * k_den) / k_num;
//  crop_area_POINT1s[1].y = k1_num * crop_area_POINT1s[1].x / k1_den + b11 + t_parameters[1];
//
//   //计算右上边线的交点
//  crop_area_POINT1s[2].x = (((b21-b11)+t_parameters[3]-t_parameters[0]) * k_den) / k_num;
//  crop_area_POINT1s[2].y = k1_num * crop_area_POINT1s[2].x / k1_den + b11 + t_parameters[0];
//
//  //计算右下边线的交点
//  crop_area_POINT1s[3].x = (((b21-b11)+t_parameters[3]-t_parameters[1]) * k_den) / k_num;
//  crop_area_POINT1s[3].y = k1_num * crop_area_POINT1s[3].x / k1_den + b11 + t_parameters[1];
//  }
//  else  //纸币无旋转
//  {
//   //计算左上边线的交点
//  crop_area_POINT1s[0].x = g_line_POINT1[t_chan+2][1].x + parameters[2]*(g_line_POINT1[t_chan+3][1].x - g_line_POINT1[t_chan+2][1].x)/10000;
//  crop_area_POINT1s[0].y = g_line_POINT1[t_chan][1].y + parameters[0]*(g_line_POINT1[t_chan+1][1].y - g_line_POINT1[t_chan][1].y)/10000;
//
//  //计算左下边线的交点
//  crop_area_POINT1s[1].x =  crop_area_POINT1s[0].x;
//  crop_area_POINT1s[1].y = g_line_POINT1[t_chan][1].y + parameters[1]*(g_line_POINT1[t_chan+1][1].y - g_line_POINT1[t_chan][1].y)/10000;
//
//   //计算右上边线的交点
//  crop_area_POINT1s[2].x = g_line_POINT1[t_chan+2][1].x + parameters[3]*(g_line_POINT1[t_chan+3][1].x - g_line_POINT1[t_chan+2][1].x)/10000;
//  crop_area_POINT1s[2].y = crop_area_POINT1s[1].y;
//
//   //计算右下边线的交点
//  crop_area_POINT1s[2].x = crop_area_POINT1s[2].x;
//  crop_area_POINT1s[2].y = crop_area_POINT1s[1].y;
//  }
//}

/************************************* end of CropAreaVertexCal.c ***********************************/


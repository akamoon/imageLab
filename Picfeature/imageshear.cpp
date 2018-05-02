/*
* Copyright (c) 2014,中南大学软件学院
* All rights reserved.
*
* 文件名称：ImageShear.c
* 文件标识：无
* 摘    要：纸币图像截取区域的错切变换相关函数实现
*
* 当前版本：1.4
* 作    者：任胜兵
* 完成日期：2015年1月12日
*
*
* 引用到的库文件：tistdtypes.h
* 引用到的自定义文件：ImageShear.h
*/
#include "include1\ImageShear.h"
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
/*****************************************************************
*	函数名:	ImageShear
*	功能描述: 对截取图像区域进行错切变换，完成旋转
*	形式参数: src_image表示变换的源图像的首地址
              src_buffer_col表示源图像缓冲区图像寻址时的列数
              dst_image表示变换后目的图像的首地址
              dst_height表示变换后目的图像的高度
              dst_width表示变换后目的图像的宽度
              filled_gray_value表示变换后目的图像的填充灰度值
              up_down_flag表示图像是否进行颠倒变换,0表示不颠倒， 1表示颠倒
*	返回值：-1表示错误  0表示成功
* 全局变量：crp_hw[2]; //截取图像的高度和宽度
            crop_image_POINT1s[4]; //保存截取区域的四个顶点坐标：左上、左下、右上、右下
********************************************************************/

int ImageShear(unsigned char  * __restrict src_image, unsigned short src_buffer_col, unsigned char * __restrict dst_image, unsigned short * __restrict  dst_height, unsigned short * __restrict  dst_width, unsigned char filled_gray_value, unsigned char up_down_flag)
{

    int sin_theta;  //保存sin(theta)的值，由crop_image_POINT1s的上边线的两个坐标点计算
    int tan_half_theta;  //保存tan(theta/2)的值，由crop_image_POINT1s的上边线的两个坐标点计算
    int temp1, temp2; //临时变量
    int i,j; //计数变量，表示目标图像的第i行第j列
    int i0, j0; //表示源图像的第i0行第j0列
    unsigned short sub_pixel_row, sub_pixel_col; //表示源图像的亚像素坐标
    int src_center_x, src_center_y; //旋转前图像的中心坐标
    int dst_center_x, dst_center_y; //旋转后图像的中心坐标
    unsigned char pixel_value; //存放插值计算得到的像数值
    int x_shift,max_shift; //表示水平平移大小，旋转后由于倾斜需要在X方向水平平移
    int last_shift;

    unsigned int image_size; //变换后图像的大小，用于颠倒图像
    printf("****************************\n");
    for(int i=0;i<4;i++)
    {
        printf("(%d,%d)   ",crop_image_POINT1s[i].x,crop_image_POINT1s[i].y);
    }
//    if (crop_image_POINT1s[2].x < crop_image_POINT1s[0].x)
//    {
//        errorTag = SHEAR_NULL;
//        return -1; //确保后面操作有效
//    }
//    if ((src_image == NULL) ||(dst_image == NULL) ||(dst_height == NULL) ||(dst_width == NULL))
//    {
//        errorTag = SHEAR_OVER;
//        return -1;
//    }

    if (crop_image_POINT1s[2].y == crop_image_POINT1s[0].y)
    {	//水平时处理
        *dst_width = crp_hw[1];
        *dst_height = crp_hw[0];
        //对图像进行拷贝处理

        for (i=0; i<*dst_height; i++)
        { // 目标图像的第i行处理
            if (up_down_flag==0)
            {
                memcpy(dst_image + i*(*dst_width), src_image + i*src_buffer_col, *dst_width);
            }
            else
            {
                image_size = (*dst_height) * (*dst_width);
                for (j=0; j<*dst_width; j++)
                {
                    dst_image[image_size-i*(*dst_width)-j]= src_image[i*src_buffer_col+j];
                }
            }
        } //end of for i
    }
    else
    {   //非水平时处理
        //由crop_image_POINT1s的上边线的两个坐标点计算sin_theta、cos(theta)和tan_half_theta的值
        //为确保精度和采用整数计算，结果先左移8位
        if((crop_image_POINT1s[2].x-crop_image_POINT1s[0].x)>(crop_image_POINT1s[1].y-crop_image_POINT1s[0].y))
        {
            temp1 = crop_image_POINT1s[2].x-crop_image_POINT1s[0].x;
            temp2 = crop_image_POINT1s[2].y-crop_image_POINT1s[0].y;

            sin_theta = (int)256*temp2 /sqrt(temp1*temp1+temp2*temp2); //乘256相当于移8位
            tan_half_theta = (int)((256*(sqrt(temp1*temp1+temp2*temp2) - temp1))/ (temp2));
          //  system("pause");
        }
        else
        {

            temp1 = crop_image_POINT1s[2].x-crop_image_POINT1s[0].x;
            temp2 = crop_image_POINT1s[2].y-crop_image_POINT1s[0].y;

            sin_theta = (int)256*temp2 /sqrt(temp1*temp1+temp2*temp2); //乘256相当于移8位
            if(sin_theta>44)
            {
                temp1 = crop_image_POINT1s[2].x-crop_image_POINT1s[0].x;
                temp2 = crop_image_POINT1s[2].y-crop_image_POINT1s[0].y-(sin_theta-44)/4;
            }
            sin_theta = (int)256*temp2 /sqrt(temp1*temp1+temp2*temp2); //乘256相当于移8位
            tan_half_theta = (int)((256*(sqrt(temp1*temp1+temp2*temp2) - temp1))/ (temp2));

        }
        //由crop_image_POINT1s的宽度和高度计算旋转后目标图像的宽度和高度
        temp1 = crop_image_POINT1s[2].x-crop_image_POINT1s[0].x;
        temp2 = crop_image_POINT1s[2].y-crop_image_POINT1s[0].y;
        *dst_width = (unsigned short) (sqrt(temp1*temp1+temp2*temp2));
        temp1 = crop_image_POINT1s[1].x-crop_image_POINT1s[0].x;
        temp2 = crop_image_POINT1s[1].y-crop_image_POINT1s[0].y;
        *dst_height = (unsigned short) (sqrt(temp1*temp1+temp2*temp2));

        //计算源图像和目标图像的中心
        src_center_x = crp_hw[1] >> 1;
        src_center_y = crp_hw[0] >> 1;
        dst_center_x = ((*dst_width) >> 1);//+8; //由于图像旋转后有缩放，故需要将中心平移，8为经验值
        dst_center_y = (*dst_height) >> 1;

        //对图像进行变换
        /*   max_shift = (*dst_height)*(crop_image_POINT1s[2].y-crop_image_POINT1s[0].y+1)/(crop_image_POINT1s[2].x-crop_image_POINT1s[0].x+1);//最大偏移量 */
        /*   if (*dst_width<*dst_height)                                                                                                                */
        /*     max_shift = crop_image_POINT1s[2].y<crop_image_POINT1s[0].y ?(max_shift<<1) : max_shift;                                                   */
        //计算X的最大平移值
        max_shift = crop_image_POINT1s[3].x - crop_image_POINT1s[2].x;
        max_shift = -(*dst_height)*(max_shift*crop_image_POINT1s[2].x-crop_image_POINT1s[3].y*crop_image_POINT1s[0].y)/(crop_image_POINT1s[3].y*crop_image_POINT1s[2].x+max_shift*crop_image_POINT1s[0].y);
        max_shift = max_shift>>1;

        last_shift = 0;
        image_size = (*dst_height) * (*dst_width);

        for (i=0; i<*dst_height; i++) // 目标图像的第i行处理
        {
            //计算每一行X平移值
            x_shift = crop_image_POINT1s[3].x - crop_image_POINT1s[2].x;
            x_shift = -(i+1)*(x_shift*crop_image_POINT1s[2].x-crop_image_POINT1s[3].y*crop_image_POINT1s[0].y)/(crop_image_POINT1s[3].y*crop_image_POINT1s[2].x+x_shift*crop_image_POINT1s[0].y);

            for (j=-x_shift; j+x_shift<*dst_width; j++)//目标图像的第j列处理+abs(x_shift)
            {
                //坐标变换
                temp1 = j+max_shift - dst_center_x; //x坐标变换
                temp2 =  dst_center_y - i;//y坐标变换

                //错切变换,顺序不能改变
                temp1 = (temp1<<8) + temp2 * tan_half_theta;
                temp2 = (temp2<<8) - ((temp1 * sin_theta)>>8);
                temp1 = temp1 + ((temp2 * tan_half_theta)>>8);

                //坐标变换，计算得到源图像的坐标
                j0 = temp1 + (src_center_x<<8);
                i0 = -temp2 + (src_center_y<<8);

                //像素值处理
                sub_pixel_row = i0 & 0xff;
                sub_pixel_col = j0 & 0xff;
                i0 = i0 >> 8;
                j0 = j0 >> 8;
                if ((i0>=crp_hw[0]) || (j0>=crp_hw[1]) || (i0<0) || (j0<0))
                { //i0,j0落在外接矩形框区域之外
                    up_down_flag == 0 ? (dst_image[i*(*dst_width)+j+x_shift]=filled_gray_value) :
                                    (dst_image[image_size-i*(*dst_width)-j-x_shift]=filled_gray_value);
                }
                else //  if ((sub_pixel_row | sub_pixel_col)!=0)
                {
                    //坐标(i0，j0)存在小数，需要进行插值处理
                    //行双线性插值理，避免在循环中调用函数，故没单独出来
                    /*
                     pixel_value = ((256-sub_pixel_row)*(256-sub_pixel_col)*src_image[i0*src_buffer_col+j0]
                                   + (256-sub_pixel_row)*sub_pixel_col*src_image[i0*src_buffer_col+j0+1]
                                   + sub_pixel_row*(256-sub_pixel_col)*src_image[(i0+1)*src_buffer_col+j0]
                                   + sub_pixel_row*sub_pixel_col*src_image[(i0+1)*src_buffer_col+j0+1])>>16;
                    */
                    //三次卷积法插值

                    if ((i0>1)&&(j0>1)&&((i0+2)<crp_hw[0])&&((j0+2)<crp_hw[1]))
                    {
                        pixel_value = CubicConvolutionInterpolation(src_image, src_buffer_col, i0, j0, sub_pixel_row, sub_pixel_col);
                    }
                    else
                    {
                        pixel_value = filled_gray_value;
                    }

                    //修正插值值，防止二值化断裂
                    pixel_value =  pixel_value > src_image[i0*src_buffer_col+j0] ? src_image[i0*src_buffer_col+j0] :  pixel_value;

                    up_down_flag == 0 ? (dst_image[i*(*dst_width)+j+x_shift] = (unsigned char) (pixel_value)) :
                                      (dst_image[image_size-i*(*dst_width)-j-x_shift]=(unsigned char)(pixel_value));


                    //相邻两行移位太大进行调整
                    if ((abs(x_shift-last_shift)>=2) && (i>=2))
                    {
                        if (up_down_flag == 0)
                        {
                            if ((x_shift>0) && (j+x_shift-1<*dst_width) && (j+x_shift-1>=0)) //后移
                                dst_image[(i-1)*(*dst_width)+j+x_shift]= dst_image[(i-1)*(*dst_width)+j+x_shift-1];
                            else
                            if ((x_shift<0) && (j+x_shift+1<*dst_width) && (j+x_shift+1>=0)) //前移
                                dst_image[(i-1)*(*dst_width)+j+x_shift] = dst_image[(i-1)*(*dst_width)+j+x_shift+1];
                        }
                        else
                        {
                            if ((x_shift>0) && (j+x_shift-1<*dst_width) && (j+x_shift-1>=0)) //后移
                                dst_image[image_size-(i-1)*(*dst_width)-j-x_shift]= dst_image[image_size-(i-1)*(*dst_width)-j-x_shift-1];
                            else
                                if ((x_shift<0) && (j+x_shift+1<*dst_width) && (j+x_shift+1>=0)) //前移
                                dst_image[image_size-(i-1)*(*dst_width)-j-x_shift] = dst_image[image_size-(i-1)*(*dst_width)-j-x_shift-1];
                        }
                    }

                }   //end of else
            }//end of for j
           last_shift = x_shift;
           //报错
        }  //end of for i
    } //end of else
    //CRO_ROW*CRO_COL
  //  system("pause");
    Mat image(*dst_height,*dst_width,CV_8UC1,Scalar(255));
    int k=0;
    for(int i=0;i<*dst_height;i++)
    {
        for(int j=0;j<*dst_width;j++)
        {
            image.at<uchar>(i,j)=dst_image[k];
            k++;
        }
    }
    Size dsize = Size(*dst_width,*dst_height);
    Mat img2 = Mat(dsize, CV_8UC1);
//    resize(image, img2, dsize);
    flip(image, img2, 1);
    imwrite("test2.bmp",img2);
//    imshow("test2.bmp",img2);

////  // system("pause");

//    waitKey(0);
    return 0;
}






void shear(unsigned char *image)
{
      int tem1=crop_image_POINT1s[0].x < crop_image_POINT1s[1].x ? crop_image_POINT1s[0].x : crop_image_POINT1s[1].x;
      int tem2=crop_image_POINT1s[2].x > crop_image_POINT1s[3].x ? crop_image_POINT1s[2].x : crop_image_POINT1s[3].x;
      int tem3=crop_image_POINT1s[0].y < crop_image_POINT1s[2].y ? crop_image_POINT1s[0].y : crop_image_POINT1s[2].y;
      int tem4=crop_image_POINT1s[1].y > crop_image_POINT1s[3].y ? crop_image_POINT1s[1].y : crop_image_POINT1s[3].y;
      int k=tem3*outlinesize+tem1;
      int out=tem2-tem1;
      int out1=tem4-tem3;

      Mat image1(out1,out,CV_8UC1,Scalar(255));


      for(int i=0;i<out1;i++)
      {

          for(int j=0;j<out;j++)
          {

              image1.at<uchar>(i,j)=image[k];
              k++;
          }
          k-=out;
          k+=outlinesize;
      }

      imwrite("test1.bmp",image1);

      image1.release();
      double temp1 = (double)abs(crop_image_POINT1s[2].x-crop_image_POINT1s[0].x);
      double temp2 = (double)abs(crop_image_POINT1s[2].y-crop_image_POINT1s[0].y);
      double temp3 = atan2(temp2,temp1) * 180.0/3.1416;////3.17

      Mat src = imread("test1.bmp",0);
     // src = strengthImage_Histogram(src);
      Mat dst2;//输出图像
      int SCALE=1;
      int angle;
      if(tem3==crop_image_POINT1s[0].y)
      {
           angle = temp3;//旋转角度(正值表示逆时针旋转)
      }
      else
      {
           angle = -temp3;//旋转角度(正值表示逆时针旋转)
      }
        //为了保证无论如何旋转都能放下，输出图像的长和宽都设为输入图像对角线长度乘以SCALE
        double length = sqrt(double(src.cols*src.cols + src.rows*src.rows)) * SCALE;
        //建立临时图像，长宽都是源图像的对角线长度，将源图像复制到临时图像的中心后再变换
        Mat tempImg(length,length,src.type(),Scalar(255,255,255));//临时图像，大小和输出图像一样大
        int ROI_x = length/2 - src.cols/2;//ROI矩形左上角的x坐标
        int ROI_y = length/2 - src.rows/2;//ROI矩形左上角的y坐标
        Rect ROIRect(ROI_x,ROI_y,src.cols,src.rows);//ROI矩形
        Mat tempImgROI2(tempImg,ROIRect);//tempImg的中间部分
        src.copyTo(tempImgROI2);//将原图复制到tempImg的中心
        Point2f center(length/2,length/2);//旋转中心
        Mat M = getRotationMatrix2D(center,angle,SCALE);//计算旋转的仿射变换矩阵
        CvScalar fillval=cvScalarAll(255);
        warpAffine(tempImg,dst2,M,Size(length,length),CV_INTER_CUBIC | CV_WARP_FILL_OUTLIERS, BORDER_CONSTANT, cvScalarAll(255));//仿射变换
        Mat dst;
         dst.create( dst2.size(), dst2.type());
        Mat map_x;
        Mat map_y;
        map_x.create( dst2.size(), CV_32FC1);
        map_y.create( dst2.size(), CV_32FC1);
        for( int i = 0; i < dst2.rows; ++i)
        {
            for( int j = 0; j < dst2.cols; ++j)
            {
                map_x.at<float>(i, j) = (float) (dst2.cols - j) ;
                map_y.at<float>(i, j) = (float) i;//(src.rows - i) ;
            }
        }
        remap(dst2, dst, map_x, map_y,CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

        map_x.release();
        map_y.release();

        Mat tem(src.rows,dst.cols,CV_8UC1,Scalar(255));
        int p=ROI_y;
        for(int i=0;i<src.rows;i++)
        {
            for(int j=0;j<dst.cols;j++)
            {
                tem.at<uchar>(i,j)=dst.at<uchar>(p,j);
            }
            p++;
        }

        int w1=0,w2=0;
            for(int j=2;j<dst.cols;j++)
            {
                if(tem.at<uchar>(10,j)!=255)
                {
                    w1=j;
                    break;
                }
            }
            for(int j=dst.cols-1;j>0;j--)
            {
                if(tem.at<uchar>(10,j)!=255)
                {
                    w2=j;
                    break;
                }
            }
        Mat teme1(src.rows,w2-w1,CV_8UC1,Scalar(255));
        int p1=w1;
        for(int i=0;i<src.rows;i++)
        {
            for(int j=0;j<w2-w1;j++)
            {
                teme1.at<uchar>(i,j)=tem.at<uchar>(i,p1);
                p1++;
            }
            p1=w1;
        }

        imwrite("test2.bmp",teme1);

}


/*****************************************************************
*	                        三次卷积法插值
********************************************************************/
inline unsigned char CubicConvolutionInterpolation(unsigned char * src_image, unsigned short src_buffer_col,int i0, int j0,  unsigned short sub_pixel_row, unsigned short sub_pixel_col)
{
    int sin_x_pi_div_x_pi[513]=
    {
       805, 805, 805, 804, 804, 804, 804, 803, 803, 803, 802, 802, 801, 801, 800, 800, 799, 798,
       797, 797, 796, 795, 794, 793, 792, 791, 790, 789, 788, 787, 785, 784, 783, 782, 780, 779,
       777, 776, 774, 773, 771, 770, 768, 766, 764, 763, 761, 759, 757, 755, 753, 751, 749, 747, 745,
       743, 741, 739, 736, 734, 732, 729, 727, 725, 722, 720, 717, 715, 712, 709, 707, 704, 701, 699,
       696, 693, 690, 687, 685, 682, 679, 676, 673, 670, 667, 664, 660, 657, 654, 651, 648, 644, 641,
       638, 635, 631, 628, 624, 621, 618, 614, 611, 607, 604, 600, 596, 593, 589, 585, 582, 578, 574,
       571, 567, 563, 559, 556, 552, 548, 544, 540, 536, 532, 528, 524, 520, 516, 513, 508, 504, 500,
       496, 492, 488, 484, 480, 476, 472, 468, 464, 459, 455, 451, 447, 443, 438, 434, 430, 426, 422,
       417, 413, 409, 405, 400, 396, 392, 387, 383, 379, 375, 370, 366, 362, 357, 353, 349, 345, 340,
       336, 332, 327, 323, 319, 314, 310, 306, 302, 297, 293, 289, 284, 280, 276, 272, 267, 263, 259,
       255, 250, 246, 242, 238, 233, 229, 225, 221, 217, 213, 208, 204, 200, 196, 192, 188, 184, 180,
       176, 171, 167, 163, 159, 155, 151, 147, 144, 140, 136, 132, 128, 124, 120, 116, 112, 109, 105,
       101, 97, 94, 90, 86, 83, 79, 75, 72, 68, 64, 61, 57, 54, 50, 47, 43, 40, 36, 33, 30, 26, 23, 20, 17,
       13, 10, 7, 4, 1, -3, -6, -9, -12, -15, -18, -21, -24, -27, -30, -33, -35, -38, -41, -44, -47, -49, -52,
       -55, -57, -60, -62, -65, -67, -70, -72, -75, -77, -80, -82, -84, -87, -89, -91, -93, -95, -98, -100,
       -102, -104, -106, -108, -110, -112, -114, -116, -117, -119, -121, -123, -125, -126, -128, -130,
       -131, -133, -134, -136, -137, -139, -140, -142, -143, -144, -146, -147, -148, -149, -151, -152,
       -153, -154, -155, -156, -157, -158, -159, -160, -161, -162, -163, -163, -164, -165, -166, -166,
       -167, -168, -168, -169, -169, -170, -170, -171, -171, -172, -172, -172, -173, -173, -173, -173,
       -174, -174, -174, -174, -174, -174, -174, -174, -174, -174, -174, -174, -174, -174, -174, -173,
       -173, -173, -173, -172, -172, -172, -171, -171, -171, -170, -170, -169, -169, -168, -168, -167,
       -166, -166, -165, -165, -164, -163, -162, -162, -161, -160, -159, -159, -158, -157, -156, -155,
       -154, -153, -152, -151, -150, -149, -148, -147, -146, -145, -144, -143, -142, -141, -139, -138,
       -137, -136, -135, -133, -132, -131, -130, -128, -127, -126, -124, -123, -122, -120, -119, -117,
       -116, -115, -113, -112, -110, -109, -107, -106, -104, -103, -101, -100, -98, -97, -95, -94, -92,
       -91, -89, -88, -86, -84, -83, -81, -80, -78, -76, -75, -73, -71, -70, -68, -67, -65, -63, -62, -60,
       -58, -57, -55, -53, -52, -50, -48, -47, -45, -43, -42, -40, -38, -37, -35, -34, -32, -30, -29, -27,
       -25, -24, -22, -20, -19, -17, -15, -14, -12, -11, -9, -7, -6, -4, -3, -1, 0, 0
    };

   long A[4]; //保存矩阵A
   long C[4]; //保存矩阵B
   long temp[4];//保存中间计算结果
   int pixel_value; //保存像素值，要做饱和处理，即大于255保持为255
   A[0] = sin_x_pi_div_x_pi[sub_pixel_row+256]; //SinXPiDivXPi(sub_pixel_row+256);
   A[1] = sin_x_pi_div_x_pi[sub_pixel_row];
   A[2] = sin_x_pi_div_x_pi[256-sub_pixel_row];
   A[3] = sin_x_pi_div_x_pi[512-sub_pixel_row];

   C[0] = sin_x_pi_div_x_pi[sub_pixel_col+256];
   C[1] = sin_x_pi_div_x_pi[sub_pixel_col];
   C[2] = sin_x_pi_div_x_pi[256-sub_pixel_col];
   C[3] = sin_x_pi_div_x_pi[512-sub_pixel_col];

   temp[0] = A[0] * src_image[(i0-1)*src_buffer_col+(j0-1)]+
           A[1] * src_image[i0*src_buffer_col+(j0-1)]+
           A[2] * src_image[(i0+1)*src_buffer_col+(j0-1)]+
           A[3] * src_image[(i0+2)*src_buffer_col+(j0-1)];

   temp[1] = A[0] * src_image[(i0-1)*src_buffer_col+j0]+
           A[1] * src_image[i0*src_buffer_col+j0]+
           A[2] * src_image[(i0+1)*src_buffer_col+j0]+
           A[3] * src_image[(i0+2)*src_buffer_col+j0];

   temp[2] = A[0] * src_image[(i0-1)*src_buffer_col+j0+1]+
           A[1] * src_image[i0*src_buffer_col+j0+1]+
           A[2] * src_image[(i0+1)*src_buffer_col+j0+1]+
           A[3] * src_image[(i0+2)*src_buffer_col+j0+1];

   temp[3] = A[0] * src_image[(i0-1)*src_buffer_col+j0+2]+
           A[1] * src_image[i0*src_buffer_col+j0+2]+
           A[2] * src_image[(i0+1)*src_buffer_col+j0+2]+
           A[3] * src_image[(i0+2)*src_buffer_col+j0+2];

   pixel_value = (int) ((temp[0]*C[0]+temp[1]*C[1]+temp[2]*C[2]+temp[3]*C[3])>>18);
   if (pixel_value >255)
     return 255;
   else if(pixel_value < 0)
      return 0;
   else
      return (unsigned char)pixel_value;
}

/*********************************************************************
     *	   计算sin(x*pi)/(x*pi)的值的逼近值, 采用查找表实现    *
**********************************************************************/
/*inline int SinXPiDivXPi(int x)
{
  unsigned int abs_x;
  abs_x = x<0 ? -x : x;
  if (abs_x >= 512) return 0;
  if (abs_x >= 256) return 4*256-8*abs_x+((5*abs_x*abs_x-(abs_x*abs_x*abs_x>>8))>>8);
  return 256-((2*abs_x*abs_x-(abs_x*abs_x*abs_x>>8))>>8);
}*/

/*****************************************************************
*	函数名:	MoveImage
*	功能描述: 平移矫正裁剪的图像
*	形式参数:
*              unsigned char image //图像搬运后寻址地址
            unsigned int imageRow	 	 // 外接矩形框行数
*              unsigned int imageCol  // 外接矩形框列数
*
*	返回值： 无
*   全局变量：无
*   文件静态变量：无
*   函数静态变量：无
*------------------------------------------------------------------
*	Revision History
*	No.	      Date	    Revised by	   Item	Description
*	V1.0   2014/05/12	 HL         	 原始版本
******************************************************************/
void MoveImage(unsigned char* image,unsigned short imageRow, unsigned short imageCol)
{
    unsigned char i,j;//循环技术
    unsigned char mid_image[220*400];

    memcpy(mid_image,image,imageRow*imageCol);

    for(i=0;i<imageCol;i++)
    {
        for(j=0;j<imageRow;j++)
        {
            image[i*imageRow+j]=mid_image[(j*imageCol)+imageCol-1-i];
        }
    }
}

/*****************************  end of ImageShear.c ***************************/


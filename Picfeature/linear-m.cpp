
#include <math.h>
#include "include1\linear.h"
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
/*****************************************************************
*
*	函数名:	Min_Value
*	功能描述: 求最小值
*
*------------------------------------------------------------------
*	Revision History
*	 No.	 Date	   Revised by	   Item	Description
*	V1.3   2014/09/19	H.L         	 改进版本
******************************************************************/

unsigned char Min_Value(unsigned char leftValue, unsigned char miduleValue, unsigned char rightValue)
{
        unsigned char temp;

    if (leftValue > miduleValue)
        temp = miduleValue;
    else
        temp = leftValue;
    if (rightValue < temp)
        temp = rightValue;
    return temp;
}

/*****************************************************************
*
*	函数名:	Pretreatment
*	功能描述: 确定纸币的四个顶点
*ee
*------------------------------------------------------------------
*	Revision History
*	 No.	 Date	   Revised by	   Item	Description
*	V1.3   2014/10/23	H.L         	 改进版本
******************************************************************/

void  Pretreatment(const unsigned char image[], unsigned short coordinate[], unsigned int down_row)
{
        unsigned int i = 0, j = 0;
        unsigned char averageValue = 0;
        unsigned char flag = 0;

    /********获取图像的上顶点(采用中值滤波)********/

    for (i = 0; i < down_row; i++)
    {
        for (j = START_COLUMN; j < END_COLUMN; j ++)
        {
            if ( *(image+i*outlinesize+j) > IMAGE_VALUE )
            {//取3个数中的
                averageValue = Min_Value(  *(image+i*outlinesize+j),  *(image+i*outlinesize+j+1),*(image+i*outlinesize+j+2) );
                if ( averageValue > IMAGE_VALUE
                && ( *(image+(i+1)*outlinesize+j)) > IMAGE_VALUE
                && ( *(image+(i+2)*outlinesize+j)) > IMAGE_VALUE
                && ( *(image+(i+3)*outlinesize+j)) > IMAGE_VALUE
                  )
                {
                    coordinate[0] = j;
                    coordinate[1] = i;
                    flag = 1;
                    break;
                }
            }
        }
        if (flag)
            break;
    }

    /********获取图像下顶点(采用中值滤波)******************************/
    averageValue = 0;
    flag = 0;
    for (i = down_row; i > 0; i--)
    {
        for (j = END_COLUMN-1; j > START_COLUMN; j--)
        {
            if ( *(image+i*outlinesize+j) > IMAGE_VALUE )
            {
                averageValue = Min_Value(*(image+i*outlinesize+j), *(image+i*outlinesize+j+1), *(image+i*outlinesize+j+2) );
                if ( averageValue > IMAGE_VALUE
                && (*(image+(i-1)*outlinesize+j)) > IMAGE_VALUE
                && (*(image+(i-2)*outlinesize+j)) > IMAGE_VALUE
                && (*(image+(i-3)*outlinesize+j)) > IMAGE_VALUE
                )
                {
                    coordinate[4] = j;
                    coordinate[5] = i;
                    flag = 1;
                    break;
                }
            }
        }
        if (flag)
            break;
    }
//   cvCircle(src1,cvPoint( coordinate[4], coordinate[5]),5,CV_RGB(0,255,230),1);
    /********获取图像左顶点(采用中值滤波)********/
    averageValue = 0;
    flag = 0;
    for (j = START_COLUMN-1; j < END_COLUMN; j ++)
    {
        for (i = down_row; i>0; i -- )
        {
            if ( *(image+i*outlinesize+j) > IMAGE_VALUE )
            {
                averageValue = Min_Value( *(image+i*outlinesize+j), *(image+(i+1)*outlinesize+j), *(image+(i+2)*outlinesize+j) );
                if ( averageValue > IMAGE_VALUE
                && (*(image+i*outlinesize+j+1)) > IMAGE_VALUE
                && (*(image+i*outlinesize+j+2)) > IMAGE_VALUE
                && (*(image+i*outlinesize+j+3)) > IMAGE_VALUE
                )
                {
                    coordinate[2] = j;
                    coordinate[3] = i;
                    flag = 1;
                    break;
                }
            }
        }
        if (flag)
            break;
    }
  //  cvCircle(src1,cvPoint( coordinate[2], coordinate[3]),5,CV_RGB(0,225,4),1);
    /********获取图像右顶点(采用中值滤波)********/
    averageValue = 0;
    flag = 0;
    for (j = END_COLUMN; j > START_COLUMN; j --)
    {
        for (i = 0; i < down_row; i ++)
        {
            if ( *(image+i*outlinesize+j) > IMAGE_VALUE )
            {
                averageValue = Min_Value( *(image+i*outlinesize+j), *(image+(i+1)*outlinesize+j), *(image+(i+2)*outlinesize+j) );
                if ( averageValue > IMAGE_VALUE
                && (*(image+i*outlinesize+j-1)) > IMAGE_VALUE
                && (*(image+i*outlinesize+j-2)) > IMAGE_VALUE
                && (*(image+i*outlinesize+j-3)) > IMAGE_VALUE
                )
                {
                    coordinate[6] = j;
                    coordinate[7] = i;
                    flag = 1;
                    break;
                }
            }
        }
        if (flag)
            break;
    }

}


long long Sum_uint16(const unsigned short a[], unsigned short row, unsigned short column)
{
        unsigned int i = 0, j = 0;
    long long sum = 0;

    for (i=0; i<row; ++i)
    {
        for (j=0; j<column; ++j)
        {
            sum += *(a + i*column + j);
        }
    }

    return sum;
}

/*****************************************************************
*
*	函数名:	Sum_unsigned char
*	功能描述: 8位数组求和
*
*------------------------------------------------------------------
*	Revision History
*	 No.	 Date	   Revised by	   Item	Description
*	V1.4   2014/10/17	 H.L         	 改进版本
******************************************************************/
long long Sum_uint8(const unsigned char a[], unsigned short row, unsigned short column)
{
        unsigned int i = 0, j = 0;
    long long sum = 0;

    for (i=0; i<row; ++i)
    {
        for (j=0; j<column; ++j)
        {
            sum += *(a + i*column + j);
        }
    }

    return sum;
}

/*****************************************************************
*
*	函数名:	Ployfit
*	功能描述: 直线拟合函数
*
*------------------------------------------------------------------
*	Revision History
*	 No.	 Date	   Revised by	   Item	Description
*	V1.1   2014/09/01	 H.L         	 改进版本
******************************************************************/
void Ployfit(float *k, float *b, const unsigned short x[], const unsigned short y[], unsigned char n)
{
    long long sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
        unsigned int i = 0;
        sum_x = Sum_uint16(x, 1, n);
        sum_y = Sum_uint16(y, 1, n);
    for (i=0; i<n; ++i)
    {
        sum_xy += (*(x+i)) * (*(y+i));
        sum_xx += (*(x+i)) * (*(x+i));
    }
    *k = (sum_x * sum_y - n * sum_xy) * 1.0 / (sum_x * sum_x - n * sum_xx);
    *b = ( sum_y - sum_x*(*k) ) * 1.0 / n;
}

/*****************************************************************
*
*	函数名:	seekpos
*	功能描述: 求两个值中间的等分点
*
*------------------------------------------------------------------
*	Revision History
*	 No.	 Date	   Revised by	   Item	Description
*	V1.2   2014/09/03	 H.L         	 改进版本
******************************************************************/
unsigned char g_currencyInfo[6];
static void seekpos(unsigned short a[], unsigned short min, unsigned short max, unsigned char flag)
{
        unsigned short ratio = (max - min) >> 3;
    if (g_currencyInfo[0] == CAD && flag == 0)
    {
        a[0] = min + 100;
        a[1] = min + ((max - min) >> 1) - 100;
        a[2] = min + ((max - min) >> 1) + 100;
        a[3] = max - 100;
    }
    else
    {
        a[0] = min + ratio;
        a[1] = min + (ratio * 3);
        a[2] = min + (ratio * 5);
        a[3] = min + (ratio * 7);
    }
}

/*****************************************************************
*
*	函数名:	LenWidValue
*	功能描述: 求取纸币图像基本参数
*
*------------------------------------------------------------------
*	Revision History
*	 No.	 Date	   Revised by	   Item	Description
*	V1.5   2014/10/28	 H.L         	 改进版本
******************************************************************/
void LenWidValue(float bLine[], unsigned short ActualSize[], const unsigned char image[], const unsigned short coordinate[])
{

    /*************************所用参数的初始化****************************/
        unsigned short imgRow = outrowsize-1;                      // 图像行数
        unsigned short imgColumn =  outlinesize-1;                 // 图像列数
        unsigned short x[4] = {0}, y[4] = {0};                     // 拟合点坐标
        unsigned short i = 0, j = 0;                               // 循环变量
    float k1 = 0, k2 = 0, k3 = 0, k4 = 0;              //边界线斜率
    float b1 = 0, b2 = 0, b3 = 0, b4 = 0;              //边界线截距
//	float B1 = 0, B2 = 0;
//	unsigned short left[2] = {0, 0}, right[2] = {0, 0}, up[2] = {0, 0}, down[2] = {0, 0};
    unsigned char minPOINT1 = 0;
    /******************无论纸币是否倾斜直接求纸币图像的长和宽**************/
    imgRow = coordinate[5] - coordinate[1] + 1;
    imgColumn = coordinate[6] - coordinate[2] + 1;
    /****************下倾斜图像逆时针旋转********************/
    if (coordinate[0] > coordinate[4])
    {
        //求上边界直线方程（中间三点拟合）
        seekpos(x, coordinate[2], coordinate[0], 0);

        for (j = 0; j < 4; ++j)
        {
            minPOINT1 = 0;
            for (i = coordinate[1];i<coordinate[3] + 50; ++i)
            {
                if ( *( image+i*outlinesize+*(x+j) ) > IMAGE_VALUE )
                {
                    minPOINT1 = Min_Value(image[(i+1)*outlinesize+x[j]+1], image[(i+2)*outlinesize+x[j]+2], image[(i+3)*outlinesize+x[j]+3]);
                    if ( minPOINT1 > IMAGE_VALUE )
                    {
                        *(y+j) = i;
                        break;
                    }
                }
            }
        }
      //     system("pause");

        Ployfit(&k1, &b1, x, y, 4);

        if (fabs(k1) > 0.012)// 大于一度才做后面的拟合
        {
            //求下边界直线方程（中间三点拟合）
            seekpos(x, coordinate[4], coordinate[6], 0);
            for (j = 0; j < 4; ++j)
            {
                minPOINT1 = 0;
                for (i = coordinate[5]; i>coordinate[7] - 50; --i)
                {
                    if ( *(image+i*outlinesize+*(x+j)) > IMAGE_VALUE )
                    {
                        minPOINT1 = Min_Value(image[(i-1)*outlinesize+x[j]-1], image[(i-2)*outlinesize+x[j]-2], image[(i-3)*outlinesize+x[j]-3]);

                        if ( minPOINT1 > IMAGE_VALUE )
                        {
                            *(y+j) = i;
                            break;
                        }
                    }
                }
            }

            Ployfit(&k3, &b3, x, y, 4);

            //求左边界直线方程（中间三点拟合）
            seekpos(y, coordinate[3], coordinate[5], 1);
            for (i = 0; i < 4; ++i)
            {
                minPOINT1 = 0;
                for (j = coordinate[2]; j<coordinate[4] + 50; ++j)
                {
                    if ( *(image+(*(y+i))*outlinesize+j) > IMAGE_VALUE )
                    {
                        minPOINT1 = Min_Value(image[(y[i]-1)*outlinesize+j+1], image[(y[i]-2)*outlinesize+j+2], image[(y[i]-3)*outlinesize+j+3]);
                        if ( minPOINT1 > IMAGE_VALUE )
                        {
                            *(x+i) = j;
                            break;
                        }
                    }
                }
            }

            Ployfit(&k2, &b2, x, y, 4);

            //求右边界直线方程（中间三点拟合）
            seekpos(y, coordinate[1], coordinate[7], 1);
            for (i = 0; i < 4; ++i)
            {
                minPOINT1 = 0;
                for (j = coordinate[6]; j>coordinate[0]-50; --j)
                {
                    if ( *(image+(*(y+i))*outlinesize+j) > IMAGE_VALUE )
                    {
                        minPOINT1 = Min_Value(image[(y[i]+1)*outlinesize+j-1], image[(y[i]+2)*outlinesize+j-2], image[(y[i]+3)*outlinesize+j-3]);

                        if ( minPOINT1 > IMAGE_VALUE )
                        {
                            *(x+i) = j;
                            break;
                        }
                    }
                }
            }

            Ployfit(&k4, &b4, x, y, 4);

            if ( (fabs(k2) > 100)||(fabs(k4) > 100))
            {
                k1 = 0.0;
                k2 = 0.0;
                k3 = 0.0;
                k4 = 0.0;
                b1 = coordinate[1];
                b2 = 0;
                b3 = coordinate[5];
                b4 = 0;
                ActualSize[1] = imgColumn - 8;
                ActualSize[0] = imgRow;
            }
            else
            {

                // B1 = (b1 + b3) / 2;
       //      	B2 = (b2 + b4) / 2;

                // left[0] = (B1 - b2) / (k2 - k1);
                // left[1] = (B1 * k2 - b2 * k1) / (k2 - k1);
                // right[0] = (B1 - b4) / (k4 - k1);
                // right[1] = (B1 * k4 - b4 * k1) / (k4 - k1);
                // up[0] = (b1 - B2) / (k2 - k1);
                // up[1] = (k2 * b1 - k1 * B2) / (k2 - k1);
                // down[0] = (b3 - B2) / (k2 - k3);
                // down[1] = (k2 * b3 - k3 * B2) / (k2 - k3);

                // ActualSize[1] = sqrt( (right[0] - left[0]) * (right[0] - left[0]) + (right[1] - left[1]) * (right[1] - left[1]) );
                // ActualSize[0] = sqrt( (down[0] - up[0]) * (down[0] - up[0]) + (down[1] - up[1]) * (down[1] - up[1]) );

                // 求纸币长和宽（利用）	两对平行线的中点
                ActualSize[0] = fabs( (k3-k1)*(coordinate[2]+coordinate[6])/2 + (b3-b1) ) / sqrt(1+((k1+k3)/2)*((k1+k3)/2));
                ActualSize[1] = fabs( ( (coordinate[1]+coordinate[5])*(k4-k2)/2 +k2*b4-k4*b2 )/(k2*k4) )
                       * sqrt( ((k2+k4)/2)*((k2+k4)/2) / (1+((k2+k4)/2)*((k2+k4)/2)) );

            }

            //将求得的边界线的斜率和截距引出，方便以后使用
            bLine[0] = k1;
            bLine[1] = b1;
            bLine[2] = k2;
            bLine[3] = b2;
            bLine[4] = k3;
            bLine[5] = b3;
            bLine[6] = k4;
            bLine[7] = b4;

        }
        /*********************图像原本就是平行的或者倾角很小*************************/
        else
        {
            // 求纸币长和宽
           // system("pause");
            ActualSize[0] = imgRow;
            ActualSize[1] = imgColumn;
            bLine[0] = 0;
            bLine[1] = coordinate[1];
            bLine[2] = 0;
            bLine[3] = 0;
            bLine[4] = 0;
            bLine[5] = coordinate[5];
            bLine[6] = 0;
            bLine[7] = 0;
        }
    }
        /******************上倾斜图像顺时针旋转********************/
    else
    {
        // 求上边界直线方程（中间三点拟合）
        seekpos(x, coordinate[0], coordinate[6], 0);
        for (j = 0; j < 4; ++j)
        {
            minPOINT1 = 0;
            for (i = coordinate[1]; i < coordinate[7] + 50; ++i)
            {
                if ( *(image+i*outlinesize+*(x+j)) > IMAGE_VALUE )
                {
                    minPOINT1 = Min_Value(image[(i+1)*outlinesize+x[j]-1], image[(i+2)*outlinesize+x[j]-2], image[(i+3)*outlinesize+x[j]-3]);

                    if (minPOINT1 > IMAGE_VALUE)
                    {
                        *(y+j) = i;
                        break;
                    }
                }
            }
        }

        Ployfit(&k1, &b1, x, y, 4);

        if (fabs (k1) > 0.012)
        {
            // 求下边界直线方程（中间三点拟合）
            seekpos(x, coordinate[2], coordinate[4], 0);
            for (j = 0; j < 4; ++j)
            {
                minPOINT1 = 0;
                for (i = coordinate[5]; i > coordinate[3] - 50; --i)
                {
                    if ( *(image+i*outlinesize+*(x+j)) > IMAGE_VALUE )
                    {
                        minPOINT1 = Min_Value(image[(i-1)*outlinesize+x[j]+1], image[(i-2)*outlinesize+x[j]+2], image[(i-3)*outlinesize+x[j]+3]);

                        if (minPOINT1 > IMAGE_VALUE)
                        {
                            *(y+j) = i;
                            break;
                        }
                    }
                }
            }

            Ployfit(&k3, &b3, x, y, 4);

            //求左边界直线方程（中间三点拟合）
            seekpos(y, coordinate[1], coordinate[3], 1);
            for (i = 0; i < 4; ++i)
            {
                minPOINT1 = 0;
                for (j = coordinate[2]; j < coordinate[0] + 50; ++j)
                {
                    if ( *(image+(*(y+i))*outlinesize+j) > IMAGE_VALUE )
                    {
                        minPOINT1 = Min_Value(image[(y[i]+1)*outlinesize+j+1], image[(y[i]+2)*outlinesize+j+2], image[(y[i]+3)*outlinesize+j+3]);
                        if (minPOINT1 > IMAGE_VALUE)
                        {
                            *(x+i) = j;
                            break;
                        }
                    }
                }
            }

            Ployfit(&k2, &b2, x, y, 4);

            //求右边界直线方程（中间三点拟合）
            seekpos(y, coordinate[7], coordinate[5], 1);
            for (i = 0; i < 4; ++i)
            {
                minPOINT1 = 0;
                for (j = coordinate[6]; j > coordinate[4] - 50; --j)
                {
                    if ( *(image+(*(y+i))*outlinesize+j) > IMAGE_VALUE )
                    {
                        minPOINT1 = Min_Value(image[(y[i]-1)*outlinesize+j-1], image[(y[i]-2)*outlinesize+j-2], image[(y[i]-3)*outlinesize+j-3]);
                        if (minPOINT1 > IMAGE_VALUE)
                        {
                            *(x+i) = j;
                            break;
                        }
                    }
                }
            }

            Ployfit(&k4, &b4, x, y, 4);

            if ( (fabs(k2) > 100) || (fabs(k4) > 100) )
            {
                k1 = 0.0;
                k2 = 0.0;
                k3 = 0.0;
                k4 = 0.0;
                b1 = coordinate[1];
                b2 = 0;
                b3 = coordinate[5];
                b4 = 0;
                ActualSize[1] = imgColumn - 8;
                ActualSize[0] = imgRow;
            }
            else
            {
                // B1 = (b1 + b3) / 2;
       //      	B2 = (b2 + b4) / 2;

                // left[0] = (B1 - b2) / (k2 - k1);
                // left[1] = (B1 * k2 - b2 * k1) / (k2 - k1);
                // right[0] = (B1 - b4) / (k4 - k1);
                // right[1] = (B1 * k4 - b4 * k1) / (k4 - k1);
                // up[0] = (b1 - B2) / (k2 - k1);
                // up[1] = (k2 * b1 - k1 * B2) / (k2 - k1);
                // down[0] = (b3 - B2) / (k2 - k3);
                // down[1] = (k2 * b3 - k3 * B2) / (k2 - k3);

                // ActualSize[1] = sqrt( (right[0] - left[0]) * (right[0] - left[0]) + (right[1] - left[1]) * (right[1] - left[1]) );
                // ActualSize[0] = sqrt( (down[0] - up[0]) * (down[0] - up[0]) + (down[1] - up[1]) * (down[1] - up[1]) );

                // 求纸币长和宽（利用）	两对平行线的中点
                ActualSize[0] = fabs( (k3-k1)*(coordinate[2]+coordinate[6])/2 + (b3-b1) ) / sqrt(1+((k1+k3)/2)*((k1+k3)/2));
                ActualSize[1] = fabs( ( (coordinate[1]+coordinate[5])*(k4-k2)/2 +k2*b4-k4*b2 )/(k2*k4) )
                       * sqrt( ((k2+k4)/2)*((k2+k4)/2) / (1+((k2+k4)/2)*((k2+k4)/2)) );
            }

            //将边界线的斜率和截距引出方便后续程序使用
            bLine[0] = k1;
            bLine[1] = b1;
            bLine[2] = k2;
            bLine[3] = b2;
            bLine[4] = k3;
            bLine[5] = b3;
            bLine[6] = k4;
            bLine[7] = b4;
        }
        /*********************图像原本就是平行的或者倾角很小*************************/
        else
        {
            // 求纸币长和宽
            ActualSize[0] = imgRow;
            ActualSize[1] = imgColumn;
            bLine[0] = 0;
            bLine[1] = coordinate[1];
            bLine[2] = 0;
            bLine[3] = 0;
            bLine[4] = 0;
            bLine[5] = coordinate[5];
            bLine[6] = 0;
            bLine[7] = 0;
        }
    }
}

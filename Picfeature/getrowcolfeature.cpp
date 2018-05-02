
#include "include1\FindFeature.h"

double BlackLineHeight(unsigned char * __restrict dst_image, unsigned short h0, unsigned short w0, unsigned short h1, unsigned short w1, unsigned short im_width)
{
    int blacksum,i,j;
    int h_line = 0;
    int max_blacksum=0;
    for(i = w0;i < w1;i++)
    {
        blacksum = 0;
        for(j = h0;j < h1;j++)
        {
            if(dst_image[(j) * im_width + i] == 0)
            {
                blacksum += 1;
            }
        }
        if(blacksum >= (h1 - h0 - 4))//如果有一条竖直线的前景点是大于高度减6的
        {
            h_line += 1;
        }
        if(blacksum>max_blacksum)
        {
            max_blacksum=blacksum;
        }
    }

     return (double)max_blacksum/(h1-h0);
}


/*****************************************************************
*	函数名:BlackLineWidth
*	功能描述:计算在宽上是否有前景直线
*	形式参数: unsigned char * restrict  字符图像
*	          unsigned short h0,w0,h1,w1 字符的上左下右坐标
*             unsigned short im_width 字符图片的宽度
*
*   局部变量：
*	返回值：宽度上前景直线的数量
*   全局变量：
*------------------------------------------------------------------
*	Revision History
*	No.	    Date	     Revised by	  Item Description
*   V1.0	2016/7/21     XRL
********************************************************************/
double BlackLineWidth(unsigned char * __restrict dst_image, unsigned short h0, unsigned short w0, unsigned short h1, unsigned short w1, unsigned short im_width)
{
    int blacksum,i,j;
    int h_line = 0;
        int max_blacksum=0;
    for(i = h0;i < h1;i++)
    {
        blacksum = 0;
        for(j = w0;j < w1;j++)
        {
            if(dst_image[(i) * im_width + j] == 0)
            {
                blacksum += 1;
            }
        }
        if(blacksum >= (w1 - w0 - 4))//如果有一条竖直线前景点是大于高度-6的
        {
            h_line += 1;
        }
        if(blacksum>max_blacksum)
        {
            max_blacksum=blacksum;
        }
    }
    return (double)max_blacksum/(w1-w0);
}

/*****************************************************************
*	函数名:crosspoint
*	功能描述:计算字符的横竖线交点
*	形式参数: dst_image  字符图像
*                   unsigned short h0,w0,h1,w1 字符的上左下右坐标
*             unsigned short im_width 字符图片的宽度
*
*   局部变量：无
*	返回值：横竖线交点的个数
*   全局变量：无
*------------------------------------------------------------------
*	Revision History
*	No.	    Date	     Revised by	  Item Description
*   V1.0	2017/8/1     SWB
********************************************************************/
int *crosspoint(unsigned char * __restrict dst_image, unsigned short h0, unsigned short w0, unsigned short h1, unsigned short w1, unsigned short im_width)
{
    int blacksum;//连续黑点个数
    int point=0;//连续黑点段数
    int point_max[2];//最大黑点段数
    point_max[0]=0;
    point_max[1]=0;
    int point_m=99;
    int ff=0;//连续ff行同样的黑点段数
    for(int i = w0;i < w1;i++)
    {

        point=0;
        for(int j = h0;j < h1;j++)
        {
            blacksum = 0;
            if(dst_image[(j) * im_width + i] == 0)
            {
               blacksum += 1;
                for(int k=j+1;k<h1;k++)
                {
                    if(dst_image[(k) * im_width + i] == 0)
                    {
                         blacksum += 1;
                    }
                    if(dst_image[(k ) * im_width + i] != 0&&blacksum<2)
                    {
                        j=k;
                        if(j>=h1)
                        {
                            j=h1-1;
                        }
                        blacksum=0;
                        break;
                    }
                    if(dst_image[(k ) * im_width + i] != 0&&blacksum>=2)
                    {
                        j=k;
                        if(j>=h1)
                        {
                            j=h1-1;
                        }
                        break;
                    }
                    if(k==h1-1)
                    {
                        j=k;
                    }
                }

                if(blacksum!=0)
                {
                    point++;
                }
            }
        }
        if(point>1)
        {
            ff++;
            if(point<point_m)
            {
                point_m=point;
            }
        }
        else
        {
            ff=0;
            point_m=99;
        }
   if(ff==2&&point_m>point_max[0])
   {
       point_max[0]=point_m;

       point_m=99;
       ff=0;
   }

   else if(ff==2&&point_m<=point_max[0])
   {
       ff=0;
       point_m=99;
   }
    }
        if(point_max[0]==0)
        {
            point_max[0]=1;
        }
         point_m=99;
         ff=0;
    for(int i = h0;i < h1;i++)
    {
        point=0;
        for(int j = w0;j < w1;j++)
        {
            blacksum = 0;
            if(dst_image[(i ) * im_width + j] == 0)
            {
               blacksum += 1;
                for(int k=j+1;k<w1;k++)
                {
                    if(dst_image[(i ) * im_width + k] == 0)
                    {
                         blacksum += 1;
                    }
                    if(dst_image[(i ) * im_width + k] != 0&&blacksum<4)
                    {
                        j=k+1;
                        if(j>=w1)
                        {
                            j=w1-1;
                        }
                        blacksum=0;
                        break;
                    }
                    if(dst_image[(i ) * im_width + k] != 0&&blacksum>=4)
                    {
                        j=k+1;
                        if(j>=w1)
                        {
                            j=w1-1;
                        }
                        break;
                    }
                    if(k==w1-1)
                    {
                        j=k;
                    }
                }
                if(blacksum!=0)
                {
                    point++;
                }
            }

        }
        if(point>1)
        {
            ff++;
            if(point<point_m)
            {
                printf("+++++++++++++%d\n",point);
                point_m=point;
            }
        }
        else
        {
            ff=0;
            point_m=99;
        }
        if(ff==2&&point_m>point_max[1])
        {
            point_max[1]=point_m;
            point_m=99;
            ff=0;
        }
        else if(ff==2&&point_m<=point_max[1])
        {
            ff=0;
            point_m=99;
        }
    }
    if(point_max[1]==0)
    {
        point_max[1]=1;
    }
        return point_max;
}

/*****************************************************************
*	函数名:	GetRowFeature
*	功能描述: 竖线特征值
*	形式参数: unsigned char *fifo  字符图像
*                 unsigned char charCol  列数
*                 unsigned char charRow  行数
*	返回值：无
*       全局变量：无
*   文件静态变量：无
*   函数静态变量：无
*------------------------------------------------------------------
*	Revision History
*	No.	    Date	     Revised by	  Item Description
*   V1.1	2014/6/4     LY
******************************************************************/
unsigned char GetRowFeature(unsigned char *fifo,unsigned short charRow,unsigned short charCol)
{
    printf("********charow=%d charcol=%d*****\n",charRow,charCol);
    unsigned short  i, j;
    unsigned short continueZeroNum = 0; //记录连续0像素点的个数
    unsigned short maxContinueZeroNum = 0; //记录最大的连续0像素点的个数
    float g_perRowLineCharacter; //横线特征值
    unsigned char tag1,tag2; //横线特征存在的标志位
    tag1=0;
    tag2=0;
    for(i=1;i<=3;++i)
    {
        continueZeroNum = 0;
        maxContinueZeroNum = 0 ;
        for(j=0;j<charCol-1;++j)
        {
            if(fifo[i*charCol+j]==0)
            {
                if(fifo[i*charCol+j+1]==0)
                {
                    continueZeroNum++;
                    if(continueZeroNum >maxContinueZeroNum )
                    {
                        maxContinueZeroNum = continueZeroNum;
                    }
                }
                else
                {
                    continueZeroNum=0;
                }
            }
        }
        //记录横线特征值
        g_perRowLineCharacter = (float)maxContinueZeroNum/ (float)(charCol-2);
        if (g_perRowLineCharacter >= ROW_RATIO)
        {
            tag1++;
        }
    }

    for(i=charRow-4;i<charRow-1;++i)
    {
        continueZeroNum = 0;
        maxContinueZeroNum = 0 ;
        for(j=0;j<charCol-1;++j)
        {
            if(fifo[i*charCol+j]==0)
            {
                if(fifo[i*charCol+j+1]==0)
                {
                     continueZeroNum++;
                     if(continueZeroNum >maxContinueZeroNum )
                     {
                        maxContinueZeroNum = continueZeroNum;
                     }
                }
                else
                {
                    continueZeroNum=0;
                }
            }
        }
        //记录横线特征值
        g_perRowLineCharacter = (float)maxContinueZeroNum / (float)(charCol-2);

        if (g_perRowLineCharacter >= ROW_RATIO)
        {
            tag2++;
        }
    }
    if((tag1>0)&&(tag2==0))
    {
        return 1;
    }
    if((tag1==0)&&(tag2>0))
    {
        return 2;
    }
    if((tag1>0)&&(tag2>0))
    {
        return 3;
    }
    else
    {
    return 0;
    }
}

/*****************************************************************
*	函数名:	GetColFeature
*	功能描述: 竖线特征值
*	形式参数: 形式参数: unsigned char *fifo  字符图像
*                 unsigned short charCol  列数
*                 unsigned short charRow  行数
*	返回值：无
*   全局变量：
*   文件静态变量：无
*   函数静态变量：无
*------------------------------------------------------------------
*	Revision History
*	No.	    Date	     Revised by	  Item Description
*   V1.1	2014/6/4     LY
******************************************************************/
unsigned char GetColFeature(unsigned char *fifo,unsigned short charRow,unsigned short charCol)
{
     unsigned short i, j;
     //unsigned char m = 0; //竖线特征数组下标
     unsigned short continueZeroNum = 0; //记录连续0像素点的个数
     unsigned short maxContinueZeroNum = 0; //记录最大的连续0像素点的个数
     float g_perColLineCharacter; //竖线特征值
     unsigned char tag1,tag2; //竖线特征存在的标志位
     tag1=0;
     tag2=0;
     for(j=1;j<=charCol-1;++j)
     {
         continueZeroNum = 0;
         maxContinueZeroNum = 0 ;
         for(i=0;i<charRow-1;++i)
         {
            if(fifo[i*charCol+j]==0)
            {
                if(fifo[(i+1)*charCol+j]==0)
                {
                    continueZeroNum++;
                    if(continueZeroNum >maxContinueZeroNum )
                    {
                        maxContinueZeroNum = continueZeroNum;
                    }
                }
                else
                {
                    continueZeroNum=0;
                }
            }
         }
         //记录竖线特征值
         g_perColLineCharacter = (float)maxContinueZeroNum / (float)(charRow-2);
         if (g_perColLineCharacter >= COL_RATIO)
         {
              tag1++;
         }
     }
     for(j=charCol-4;j<charCol-1;++j)
     {
         continueZeroNum = 0;
         maxContinueZeroNum = 0 ;
         for(i=1;i<charRow-1;++i)
         {
            if(fifo[i*charCol+j]==0)
            {
                if(fifo[(i+1)*charCol+j]==0)
                {
                    continueZeroNum++;
                    if(continueZeroNum >maxContinueZeroNum )
                    {
                        maxContinueZeroNum = continueZeroNum;
                    }
                }
                else
                {
                    continueZeroNum=0;
                }
            }
        }
        //记录竖线特征值
        g_perColLineCharacter = (float)maxContinueZeroNum / (float)(charRow-2);
        if (g_perColLineCharacter >= COL_RATIO)
        {
             tag2++;
        }
    }
    if(tag1>0&&tag2==0)
    {
        return 1;
    }
    if(tag1==0&&tag2>0)
    {
        return 2;
    }
    if(tag1>0&&tag2>0)
    {
        return 3;
    }
    else
    {
        return 0;
    }
}


void FillImage(unsigned short* __restrict numFillPoint,  unsigned char* __restrict image, unsigned short imageRow, unsigned short imageColumn, unsigned char fillDirection, unsigned char fillValue)
{
    int i = 0, j = 0; // 循环变量初始化

    *numFillPoint = 0;   // 填充点个数

    switch (fillDirection)
    {

        case 0: // 从左往右填充
        {
            /*************逐行扫描将遇到字符之前的背景填充为fillValue*********/
            for (i = 1; i < (imageRow - 1); ++i)
            {

                for (j = 1; j < (imageColumn - 1); ++j)
                {
                    if (image[i * imageColumn + j])// 遇到背景填充为相应的值，填充个数递增
                    {
                        image[i * imageColumn + j] = fillValue;
                        ++(*numFillPoint);
                    }

                    else
                    {
                        break;// 遇到字符则跳出该行扫描
                    }
                }

            }

            break;
        }
        case 1: // 从右往左填充
        {
            /*************逐列扫描将遇到字符之前的背景填充为fillValue*********/
            for (i = 1; i < (imageRow - 1); ++i)
            {

                for (j = imageColumn - 2; j > 0; --j)
                {
                    if (image[i * imageColumn + j])// 遇到背景填充为相应的值，填充个数递增
                    {
                        image[i * imageColumn + j] = fillValue;
                        ++(*numFillPoint);
                    }

                    else
                    {
                        break;// 遇到字符则跳出该行扫描
                    }
                }

            }

            break;
        }
        case 2: // 从上往下填充
        {
            /*************逐列扫描将遇到字符之前的背景填充为fillValue*********/
            for (j = 1; j < (imageColumn - 1); ++j)
            {

                for (i = 1; i < (imageRow - 1); ++i)
                {
                    if (image[i * imageColumn + j])// 遇到背景填充为相应的值，填充个数递增
                    {
                        image[i * imageColumn + j] = fillValue;
                        ++(*numFillPoint);
                    }

                    else
                    {
                        break;// 遇到字符则跳出该列扫描
                    }
                }

            }
            break;
        }
        case 3: // 从下往上填充
        {
            /*************逐列扫描将遇到字符之前的背景填充为fillValue*********/
            for (j = 1; j < (imageColumn - 1); ++j)
            {

                for (i = (imageRow - 2); i > 0; --i)
                {
                    if (image[i * imageColumn + j]) // 遇到背景填充为相应的值，填充个数递增
                    {
                        image[i * imageColumn + j] = fillValue;
                        ++(*numFillPoint);
                    }

                    else
                    {
                        break; // 遇到字符则跳出该列扫描
                    }
                }

            }

            break;
        }
        default :
            break;

    }

}

unsigned char GetConcavoConvex(unsigned short* __restrict numConcavoConvexPoint, unsigned short* __restrict concavoConvexPosition, const unsigned char* __restrict image, unsigned short imageRow, unsigned short imageColumn, unsigned char position, unsigned char filledValue)
{
    int i = 0, j = 0; // 循环变量初始

    unsigned short up = 0, down = 0/*, left = 0, right = 0*/;
    unsigned char flag = 0;
    unsigned char character_flag = 0;
    unsigned short temp = 0;

    *numConcavoConvexPoint = 0; // 背景值为1的个数
    *concavoConvexPosition = 0; // 初始化凹凸区域相对位置

    switch (position)
    {

        case 0: // 获取左凹特性
        {
            /*************逐行扫描将遇到字符之前的背景为filledValue的个数统计出来*********/
            for (i = 1; i < (imageRow - 1); ++i)
            {
                temp = 0;
                character_flag = 0;
                for (j = 1; j < (imageColumn - 1); ++j)
                {
                    if (image[i * imageColumn + j] == filledValue)// 遇到背景为filledValue的统计值个数递增
                    {
                        ++temp;
                    }
                    else if (image[i * imageColumn + j] == 0)
                    {
                        character_flag = 1;
                        break;// 遇到字符则跳出该行扫描
                    }
                }

                if (temp > 1 && character_flag)
                {
                    (*numConcavoConvexPoint) += temp;
                    if (flag == 0)
                    {
                        flag = 1;
                        up = i;
                    }
                    down = i;
                }

            }

            *concavoConvexPosition = (up + down - 1) >> 1;

            if ( (*numConcavoConvexPoint) >=  (imageRow - 2) ) // 点数大于行数的0.75倍
                return 1;
            else
                return 0;

        }
        case 1: // 获取右凹特性
        {
            /*************逐行扫描将遇到字符之前的背景为filledValue的个数统计出来*********/
            for (i = 1; i < (imageRow - 1); ++i)
            {
                temp = 0;
                character_flag = 0;
                for (j = imageColumn - 2; j > 0; --j)
                {
                    if (image[i * imageColumn + j] == filledValue)// 遇到背景为filledValue的统计值个数递增
                    {
                        ++temp;
                    }
                    else if (image[i * imageColumn + j] == 0)
                    {
                        character_flag = 1;
                        break;// 遇到字符则跳出该行扫描
                    }
                }

                if (temp > 1 && character_flag)
                {
                    (*numConcavoConvexPoint) += temp;
                    if (flag == 0)
                    {
                        flag = 1;

                        up = i;
                    }
                    down = i;
                }

            }

            *concavoConvexPosition = (up + down - 1) >> 1;

            if ( (*numConcavoConvexPoint) >=  (imageRow - 2) ) // 点数大于行数的0.75倍
                return 1;
            else
                return 0;

        }
        case 2: // 获取上凹特性
        {
            /*************逐列扫描将遇到字符之前的背景为filledValue的个数统计出来*********/
            for (j = 1; j < (imageColumn - 1); ++j)
            {
                temp = 0;
                character_flag = 0;
                for (i = 1; i < (imageRow - 1); ++i)
                {
                    if (image[i * imageColumn + j] == filledValue)// 遇到背景为filledValue的统计值个数递增
                    {
                        ++temp;
                        if (flag == 0)
                        {
                            flag = 1;
                            up = i;
                            down = i;
                        }
                        else
                        {
                            up = (i < up) ? i : up;
                            down = (i > down) ? i : down;
                        }

                    }
                    else if (image[i * imageColumn + j] == 0)
                    {
                        character_flag = 1;
                        break;// 遇到字符则跳出该列扫描
                    }
                }

                if (temp > 1 && character_flag)
                {
                    (*numConcavoConvexPoint) += temp;
                }

            }

            *concavoConvexPosition = (up + down - 1) >> 1;

            if ( (*numConcavoConvexPoint) >=  (imageColumn - 2) ) // 点数大于列数的0.75倍
                return 1;
            else
                return 0;

        }
        case 3: // 获取下凹特性
        {
            /*************逐列扫描将遇到字符之前的背景为filledValue的个数统计出来*********/
            for (j = 1; j < (imageColumn - 1); ++j)
            {
                temp = 0;
                character_flag = 0;
                for (i = imageRow - 2; i > 0; --i)
                {
                    if (image[i * imageColumn + j] == filledValue)// 遇到背景为filledValue的统计值个数递增
                    {
                        ++temp;
                        if (flag == 0)
                        {
                            flag = 1;
                            up = i;
                            down = i;
                        }
                        else
                        {
                            up = (i < up) ? i : up;
                            down = (i > down) ? i : down;
                        }
                    }
                    else if (image[i * imageColumn + j] == 0)
                    {
                        character_flag = 1;
                        break;// 遇到字符则跳出该列扫描
                    }
                }

                if (temp > 1 && character_flag)
                {
                    (*numConcavoConvexPoint) += temp;
                }

            }

            *concavoConvexPosition = (up + down - 1) >> 1;

            if ( (*numConcavoConvexPoint) >=  (imageColumn - 2) ) // 点数大于列数的0.75倍
                return 1;
            else
                return 0;

        }
        default :
        {
            return 0;

        }
    }

}
/*****************************************************************
*	函数名:	GetColFeature
*	功能描述: 图片上下左右的黑点比例
*	形式参数: 形式参数: image 字符图像
*	返回值：无
*   全局变量：
*   文件静态变量：无
*   函数静态变量：无
*------------------------------------------------------------------
*	Revision History
*	No.	    Date	     Revised by	  Item Description
*   V1.0	2017/8/1     SWB
******************************************************************/
double *area( Mat img)
{
    Mat image;
    img.copyTo(image);
    for(int i=0;i<image.rows;i++)
    {
        for(int j=0;j<image.cols;j++)
        {
            if(image.at<Vec3b>(i,j)[0]!=255)
            {
                image.at<Vec3b>(i,j)[0]=0;
            }
            else
            {
                image.at<Vec3b>(i,j)[0]=1;
            }
        }
    }
//    int up=0;
//    int down=0;
//    int left=0;
//    int right=0;
//    for(int i=0;i<image.rows;i++)
//    {
//        int flag=0;
//        for(int j=0;j<image.cols;j++)
//        {
//            if(image.at<Vec3b>(i,j)[0]==0)
//            {
//                up=i;
//                flag++;
//                break;
//            }
//        }
//        if(flag!=0)
//            break;
//    }
//    for(int i=image.rows-1;i>0;i--)
//    {
//        int flag=0;
//        for(int j=0;j<image.cols;j++)
//        {
//            if(image.at<Vec3b>(i,j)[0]==0)
//            {
//                down=i;
//                flag++;
//                break;
//            }
//        }
//        if(flag!=0)
//            break;
//    }

//    for(int i=image.cols-1;i>0;i--)
//    {
//        int flag=0;
//        for(int j=0;j<image.rows;j++)
//        {
//            if(image.at<Vec3b>(j,i)[0]==0)
//            {
//                right=i;
//                flag++;
//                break;
//            }
//        }
//        if(flag!=0)
//            break;
//    }

//    for(int i=0;i<image.cols;i++)
//    {
//        int flag=0;
//        for(int j=0;j<image.rows;j++)
//        {
//            if(image.at<Vec3b>(j,i)[0]==0)
//            {
//                left=i;
//                flag++;
//                break;
//            }
//        }
//        if(flag!=0)
//            break;
//    }
    double*count= new double[2];
    count[0]=0;
    count[1]=0;
for(int i=1;i<image.rows/2-1;i++)
{
    int flag=0;
    for(int j=1;j<image.cols/2-1;j++)
    {
        if(image.at<Vec3b>(i,j)[0]==0)
        {
            if(
         image.at<Vec3b>(i,j)[0]==0&&
         image.at<Vec3b>(i-1,j+1)[0]==1&&
         image.at<Vec3b>(i,j+1)[0]==0&&
         image.at<Vec3b>(i,j-1)[0]==1&&
         image.at<Vec3b>(i+1,j)[0]==0&&
         image.at<Vec3b>(i-1,j)[0]==1&&
         image.at<Vec3b>(i+1,j+1)[0]==0&&
         image.at<Vec3b>(i+1,j-1)[0]==0&&
         image.at<Vec3b>(i-1,j-1)[0]==1
        ){
            count[0]++;
        }
            else if(
                        image.at<Vec3b>(i,j)[0]==0&&
                        image.at<Vec3b>(i-1,j+1)[0]==0&&
                        image.at<Vec3b>(i,j+1)[0]==0&&
                        image.at<Vec3b>(i,j-1)[0]==1&&
                        image.at<Vec3b>(i+1,j)[0]==0&&
                        image.at<Vec3b>(i-1,j)[0]==1&&
                        image.at<Vec3b>(i+1,j+1)[0]==0&&
                        image.at<Vec3b>(i+1,j-1)[0]==0&&
                        image.at<Vec3b>(i-1,j-1)[0]==1
                       ){
                           count[0]++;
                       }
        }

    }
}
    for(int i=image.rows/2;i<image.rows-1;i++)
    {
        int flag=0;
        for(int j=1;j<image.cols/2-1;j++)
        {
            if(image.at<Vec3b>(i,j)[0]==0)
            {
                if(
             image.at<Vec3b>(i,j)[0]==0&&
             image.at<Vec3b>(i-1,j+1)[0]==0&&
             image.at<Vec3b>(i,j+1)[0]==0&&
             image.at<Vec3b>(i,j-1)[0]==1&&
             image.at<Vec3b>(i+1,j)[0]==1&&
             image.at<Vec3b>(i-1,j)[0]==0&&
             image.at<Vec3b>(i+1,j+1)[0]==1&&
             image.at<Vec3b>(i+1,j-1)[0]==1&&
             image.at<Vec3b>(i-1,j-1)[0]==1
            ){
                count[1]++;
            }

            }

        }
    }





printf(">>>>>>>>>count=%.1f\n",count[0]);
    return count;
}

int hog(Mat picture)
{

     Mat img;
    picture.convertTo(img, CV_32F); //转换成浮点
    sqrt(img, img);                 //gamma校正
    normalize(img, img, 0, 255, NORM_MINMAX, CV_32F);//归一化[0,255]浮点数
//    int ****hoggi= new int ***[image.rows];
//    for(int i=0;i<image.rows;i++)
//    {
//        hoggi[i]= new int **[image.cols];
//        for(int j=0;j<image.cols;j++)
//        {
//            hoggi[i][j]= new int *[image.rows];
//            for(int k=0;k<image.rows;k++)
//            {
//                 hoggi[i][j][k]= new int [image.cols];
//            }
//        }
//    }
//    for(int i=0;i<image.rows;i++)
//    {
//        for(int j=0;j<image.cols;j++)
//        {
//            for(int k=0;k<image.rows;k++)
//            {
//                for(int l=0;l<image.cols;l++)
//                {
//                    hoggi[i][j][k][l]=image[k][l]-image[k][l+2]
//                }
//            }
//        }
//    }
    Mat gradient = Mat::zeros(img.rows, img.cols, CV_32F);//梯度
    Mat theta = Mat::zeros(img.rows, img.cols, CV_32F);//角度
    for (int i = 1; i < img.rows - 1; i++)
    {
        for (int j = 1; j < img.cols - 1; j++)
        {
            float Gx, Gy;
            Gx = img.at<float>(i, j + 1) - img.at<float>(i, j - 1);
            Gy = img.at<float>(i + 1, j) - img.at<float>(i - 1, j);
            gradient.at<float>(i, j) = sqrt(Gx * Gx + Gy * Gy);//梯度模值
            theta.at<float>(i, j) = float(atan2(Gy, Gx) * 180 / CV_PI);//梯度方向[-180°，180°]
        }
    }

    normalize(gradient, gradient, 0, 255, NORM_MINMAX, CV_8UC1);//归一化[0,255] 无符号整型
    normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);

    imshow("原图", picture);
    imshow("Gamma校正", img);
    imshow("梯度图", gradient);
    waitKey();

    return 0;
//    Mat image1;
//    img.copyTo(image1);
//    for(int i=0;i<img.rows;i++)
//        for(int j=0;j<img.cols;j++)
//        {
//             image1.at<uchar>(i,j)= img.at<uchar>(i,j);
//        }

}

int *Cut(Mat picture)
{
     Mat mst1(picture.rows,picture.cols/2,CV_8UC1);
     Mat mst2(picture.rows,picture.cols/2,CV_8UC1);
     Mat mst3(picture.rows/2,picture.cols,CV_8UC1);
     Mat mst4(picture.rows/2,picture.cols,CV_8UC1);
     for(int i=0;i<picture.rows;i++)
         for(int j=0;j<picture.cols/2;j++)
         {
         //   mst1.at<Vec3b>(i,j)= (20,30,40);
            mst1.at<uchar>(i,j)=picture.at<Vec3b>(i,j)[0];
         }

     int c=0;
     for(int i=0;i<picture.rows;i++)
     {
         c=0;
         for(int j=picture.cols/2;j<picture.cols;j++)
         {
            mst2.at<uchar>(i,c)=picture.at<Vec3b>(i,j)[0];
            c++;
         }
     }

     for(int i=0;i<picture.cols;i++)
         for(int j=0;j<picture.rows/2;j++)
         {
         //   mst1.at<Vec3b>(i,j)= (20,30,40);
            mst3.at<uchar>(j,i)=picture.at<Vec3b>(j,i)[0];
         }

      c=0;
     for(int i=0;i<picture.cols;i++)
     {
         c=0;
         for(int j=picture.rows/2;j<picture.rows;j++)
         {
            mst4.at<uchar>(c,i)=picture.at<Vec3b>(j,i)[0];
            c++;
         }
     }
     Mat mst5;
     Mat mst6;
     flip(mst2, mst5, 1);
     flip(mst4, mst6, 0);
     imwrite("mst1.bmp",mst1);
     imwrite("mst2.bmp",mst5);
     imwrite("mst3.bmp",mst3);
     imwrite("mst4.bmp",mst6);
     Mat matDst1(8,8,CV_8UC1);
     Mat matDst2(8,8,CV_8UC1);
     Mat matDst3(8,8,CV_8UC1);
     Mat matDst4(8,8,CV_8UC1);
     //,matDst2,matDst3,matDst4;
     resize( mst1, matDst1,Size(8, 8));
     resize( mst5, matDst2,Size(8, 8));
     resize( mst3, matDst3,Size(8, 8));
     resize( mst6, matDst4, Size(8, 8));
//     cvtColor(matDst1, matDst1, CV_BGR2GRAY);
//     cvtColor(matDst2, matDst2, CV_BGR2GRAY);
//     cvtColor(matDst3, matDst3, CV_BGR2GRAY);
//     cvtColor(matDst4, matDst4, CV_BGR2GRAY);

     int iAvg1 = 0, iAvg2 = 0,iAvg3 = 0, iAvg4 = 0;
     int arr1[64], arr2[64],arr3[64], arr4[64];

     for (int i = 0; i < 8; i++)
     {
         uchar* data1 = matDst1.ptr<uchar>(i);
         uchar* data2 = matDst2.ptr<uchar>(i);
         uchar* data3= matDst3.ptr<uchar>(i);
         uchar* data4 = matDst4.ptr<uchar>(i);
         int tmp = i * 8;
         int tmp2 = i * 8;
         for (int j = 0; j < 8; j++)
         {
             int tmp1 = tmp + j;
            int tmp3= tmp2+j;
             arr1[tmp1] = data1[j] / 4 * 4;
             arr2[tmp1] = data2[j] / 4 * 4;
             arr3[tmp3] = data3[j] / 4 * 4;
             arr4[tmp3] = data4[j] / 4 * 4;
             iAvg1 += arr1[tmp1];
             iAvg2 += arr2[tmp1];
             iAvg3 += arr3[tmp3];
             iAvg4 += arr4[tmp3];
         }
     }

     iAvg1 /= 64;
     iAvg2 /= 64;
     iAvg3 /= 64;
     iAvg4/= 64;
     for (int i = 0; i < 64; i++)
     {
         arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
         arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
         arr3[i] = (arr3[i] >= iAvg3) ? 1 : 0;
         arr4[i] = (arr4[i] >= iAvg4) ? 1 : 0;
     }

     int iDiffNum = 0;
     int iDiffNum1 = 0;
     for (int i = 0; i < 64; i++)
     {
         if (arr1[i] != arr2[i])
             ++iDiffNum;
         if (arr3[i] != arr4[i])
             ++iDiffNum1;
     }
//        cout<<"iDiffNum = "<<iDiffNum<<endl;
//        cout<<"iDiffNum1 = "<<iDiffNum1<<endl;
        int a[2];
        a[0]= iDiffNum;
        a[1]= iDiffNum1;
        return a;
}

int same1( Mat mst2,char* cmp)
{
//    Mat mst2=imread(tmp,CV_8UC1);
    Mat mst4=imread(cmp,CV_8UC1);

    Mat matDst1(8,8,CV_8UC1);
    Mat matDst2(8,8,CV_8UC1);
    resize( mst2, matDst1,Size(8, 8));
    resize( mst4, matDst2,Size(8, 8));
    int iAvg1 = 0, iAvg2 = 0;
    int arr1[64], arr2[64];

    for (int i = 0; i < 8; i++)
    {
        uchar* data1 = matDst1.ptr<uchar>(i);
        uchar* data2 = matDst2.ptr<uchar>(i);
        int tmp = i * 8;
        for (int j = 0; j < 8; j++)
        {
            int tmp1 = tmp + j;
            arr1[tmp1] = data1[j] / 4 * 4;
            arr2[tmp1] = data2[j] / 4 * 4;
            iAvg1 += arr1[tmp1];
            iAvg2 += arr2[tmp1];
        }
    }
    iAvg1 /= 64;
    iAvg2 /= 64;
    for (int i = 0; i < 64; i++)
    {
        arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
        arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
    }
    int iDiffNum = 0;
    for (int i = 0; i < 64; i++)
    {
        if (arr1[i] != arr2[i])
            ++iDiffNum;
    }
     //   cout<<"iDiffNum = "<<iDiffNum<<endl;

       int a;
       a= iDiffNum;
       return a;
}

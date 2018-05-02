#include "include1\charactercut.h"

int flagNum1[2];
int m_lib1;
//unsigned char g_connectBuf[300][500];
unsigned int Max_height;
unsigned int Min_height;
unsigned int Max_width;
unsigned int Min_width;
unsigned int SUM;
/*****************************************************************
*	函数名:Charactercut
*	功能描述: 对图像字母进行切分
*	形式参数:Mat image//图像
*                   X//图像读取的开始横坐标
*                   Y//图像读取的开始纵坐标
*                   count	//切分的图片标号
*	返回值：  f// 当前切分的图片标号
*   全局变量： *flagNum // 连通域的个数
*                       m_lib1//堆栈的存储数
*   文件静态变量：无
*   函数静态变量：无
*------------------------------------------------------------------
*	Revision History
*	No.	    Date	   Revised by	  Item Description
*   V1.0	2017/8/1   SWB            原始版本
******************************************************************/
int *Charactercut(Mat image,int X,int Y,int count)
{
    int *f= new int [2];
    f[0]=0;
    f[1]=0;
   Data Dp[600];
   int xi,xj,n=0;
   unsigned char flag5 = 2;
   SUM=0;
   flagNum1[0] = 1;   //连通域为2的圈内白点数，或者连通域为3的第一个圈内白点个数
   flagNum1[1] = 1;   //连通域为3的第二个圈内白点个数
    Max_height=X;//更新当前字符在此图片中所在的位置的最高处
    Min_height=X;//更新当前字符在此图片中所在的位置的最低处
    Max_width=Y;//更新当前字符在此图片中所在的位置的最左处
    Min_width=Y;//更新当前字符在此图片中所在的位置的最右处
    int temp1=image.rows;
    int temp2=image.cols;
                m_lib1 = 0;
                image.at<uchar>(X,Y)=flag5;
                n++;
                Dp[m_lib1].row = X;
                Dp[m_lib1].col = Y;
                m_lib1++;
                SUM++;
//                int A=0;
//利用m_lib1非空将四邻域相连的白点数都找到
                while(m_lib1 != 0) //判定栈是否为空
                {//扫描当前黑点//上，下，左，右，斜前斜后，斜左斜右。
                    xi = Dp[m_lib1-1].row;
                    xj = Dp[m_lib1-1].col;
                    m_lib1--;
                    if(xi<temp1-1)
                    Check_lib(xi+1,xj,flag5,Dp,image);
                    if(xi>0)
                    Check_lib(xi-1,xj,flag5,Dp,image);
                    if(xj>0)
                    Check_lib(xi,xj-1,flag5,Dp,image);
                    if(xj<temp2-1)
                    Check_lib(xi,xj+1,flag5,Dp,image);
                    if(xj<temp2-1&&xi<temp1-1)
                    Check_lib(xi+1,xj+1,flag5,Dp,image);
                    if(xj>0&&xi<temp1-1)
                    Check_lib(xi+1,xj-1,flag5,Dp,image);
                    if(xj<temp2-1&&xi>0)
                    Check_lib(xi-1,xj+1,flag5,Dp,image);
                    if(xj>0&&xi>0)
                    Check_lib(xi-1,xj-1,flag5,Dp,image);
                   if(SUM>350)
                   {
                       return f;
                       break;
                   }
                }
        printf("picture width: %d,picture height:%d SUM:%d    Maxborder: %d \n",Max_width-Min_width,Max_height-Min_height,SUM,Max_width);
       //判断时候符合一个字符的标准，排除噪点的可能性
        if(Max_height-Min_height>=8&&Max_height-Min_height<=35&&Max_width-Min_width>7&&Max_width-Min_width<=35&&SUM<320&&SUM>=30)
        {
            if(Max_width-Min_width>7&&Max_width-Min_width<11)
            {
                if(Max_height-Min_height>=8&&Max_height-Min_height<=10)
                {
                    //切分图片然后保存至Cut
                    Mat Cut((Max_height-Min_height)*2+2,(Max_width-Min_width)*2,CV_8UC1,Scalar(255));
                    int p1;
                  if(Min_height==0)
                  {
                      p1=Min_height;
                  }
                  else
                  p1=Min_height-2;
                    for(int i=0;i<(Max_height-Min_height)*2+2;i++)
                    {
                        int p2;
                        if(Min_width==0)
                         p2=0;
                        else
                         p2=Min_width-1;
                        for(int j=0;j<(Max_width-Min_width)*2;j++)
                        {
                           Cut.at<uchar>(i,j)=image.at<uchar>(p1,p2);
                           p2++;
                        }

                        p1++;
                        if(p1==image.rows)
                            break;
                    }


                    for(int i=0;i<Cut.rows;i++)
                    {
                        for(int j=0;j<Cut.cols;j++)
                        {

                            if(Cut.at<uchar>(i,j)==2||Cut.at<uchar>(i,j)==0)
                            {
                                Cut.at<uchar>(i,j)=0;
                            }
                            else
                            {
                                Cut.at<uchar>(i,j)=255;
                            }
                        }
                    }
                    char name[200];
                    sprintf(name,"%d.bmp" ,count);
                    Size dsize = Size(20,20);
                    Mat img2 = Mat(dsize, CV_8UC1);
                    resize(Cut, img2, dsize, 0, 0,INTER_LINEAR );
                  //  flip(Cut, img2, 1);
                    imwrite(name,img2);
                     Cut.release();
                     int *f= new int [2];
                     f[0]=Min_height;
                     f[1]=Max_width+(Max_width-Min_width)+2;
                     printf("f[0]=%d,f[1]=%d\n",f[0],f[1]);
                return f;
                }
                Mat Cut((Max_height-Min_height)+4,(Max_width-Min_width)*2,CV_8UC1,Scalar(255));
                int p1;
              if(Min_height==0)
              {
                  p1=Min_height;
              }
              else
              p1=Min_height-2;
                for(int i=0;i<Max_height-Min_height+4;i++)
                {
                    int p2;
                    if(Min_width==0)
                     p2=0;
                    else
                     p2=Min_width-1;
                    for(int j=0;j<(Max_width-Min_width)*2;j++)
                    {
                       Cut.at<uchar>(i,j)=image.at<uchar>(p1,p2);
                       p2++;
                       if(p2==image.cols)
                       {
                           break;
                       }
                    }
                    p1++;
                    if(p1==image.rows)
                        break;
                }


                for(int i=0;i<Cut.rows;i++)
                {
                    for(int j=0;j<Cut.cols;j++)
                    {

                        if(Cut.at<uchar>(i,j)==2||Cut.at<uchar>(i,j)==0)
                        {
                            Cut.at<uchar>(i,j)=0;
                        }
                        else
                        {
                            Cut.at<uchar>(i,j)=255;
                        }
                    }
                }
                char name[200];
                sprintf(name,"%d.bmp" ,count);
                imwrite(name,Cut);
                 Cut.release();
                 int *f= new int [2];
                 f[0]=Min_height;
                 f[1]=Max_width+(Max_width-Min_width)+2;
                 printf("f[0]=%d,f[1]=%d\n",f[0],f[1]);
            return f;


            }
            else
            {
                if(Max_height-Min_height>=8&&Max_height-Min_height<=10)
                {
                    Mat Cut((Max_height-Min_height)*2+2,Max_width-Min_width+4,CV_8UC1,Scalar(255));
                    int p1;
                  if(Min_height==0)
                  {
                      p1=Min_height;
                  }
                  else
                  p1=Min_height-2;
                    for(int i=0;i<(Max_height-Min_height)*2+2;i++)
                    {
                        int p2;
                        if(Min_width==0)
                         p2=0;
                        else
                         p2=Min_width-1;
                        for(int j=0;j<Max_width-Min_width+4;j++)
                        {
                           Cut.at<uchar>(i,j)=image.at<uchar>(p1,p2);
                           p2++;
                           if(p2==image.cols)
                               break;
                        }
                        p1++;
                        if(p1==image.rows)
                            break;
                    }


                    for(int i=0;i<Cut.rows;i++)
                    {
                        for(int j=0;j<Cut.cols;j++)
                        {

                            if(Cut.at<uchar>(i,j)==2||Cut.at<uchar>(i,j)==0)
                            {
                                Cut.at<uchar>(i,j)=0;
                            }
                            else
                            {
                                Cut.at<uchar>(i,j)=255;
                            }
                        }
                    }
                    char name[200];
                    sprintf(name,"%d.bmp" ,count);
                    imwrite(name,Cut);
                     Cut.release();
                     int *f= new int [2];
                     f[0]=Min_height;
                     f[1]=Max_width+2;
                     printf("f[0]=%d,f[1]=%d\n",f[0],f[1]);
                return f;
                }
            Mat Cut(Max_height-Min_height+4,Max_width-Min_width+4,CV_8UC1,Scalar(255));
              int p1;
            if(Min_height==0)
            {
                p1=Min_height;
            }
            else
            p1=Min_height-2;
            for(int i=0;i<Max_height-Min_height+4;i++)
            {
                int p2;
                if(Min_width==0)
                 p2=0;
                else
                 p2=Min_width-1;
                for(int j=0;j<Max_width-Min_width+4;j++)
                {
                   Cut.at<uchar>(i,j)=image.at<uchar>(p1,p2);

                   p2++;
                   if(p2==image.cols)
                       break;
                }
                p1++;
                if(p1==image.rows)
                    break;
            }


            for(int i=0;i<Cut.rows;i++)
            {
                for(int j=0;j<Cut.cols;j++)
                {

                    if(Cut.at<uchar>(i,j)==2||Cut.at<uchar>(i,j)==0)
                    {
                        Cut.at<uchar>(i,j)=0;
                    }
                    else
                    {
                        Cut.at<uchar>(i,j)=255;
                    }
                }
            }
            char name[200];
            sprintf(name,"%d.bmp" ,count);
            imwrite(name,Cut);
             Cut.release();
             int *f= new int [2];
             f[0]=Min_height;
             f[1]=Max_width+2;
             printf("f[0]=%d,f[1]=%d\n",f[0],f[1]);
        return f;


            }

        }
        return f;
}

void Check_lib(int i,int j,int flag, Data* Dp,Mat image)
{
   if(image.at<uchar>(i,j)==0)
   {

       if(i>Max_height)
       {
           Max_height=i;
       }
       if(i<Min_height)
       {
           Min_height=i;
       }
       if(j>Max_width)
       {
           Max_width=j;
       }
       if(j<Min_width)
       {
           Min_width=j;
       }
       image.at<uchar>(i,j)=flag;
       Dp[m_lib1].row = i;
       Dp[m_lib1].col = j;
       SUM++;
       m_lib1++;
   }

}

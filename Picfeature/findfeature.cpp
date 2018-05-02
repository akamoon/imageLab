
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2/opencv.hpp>
#include "include1\FindFeature.h"

//#include <getallfeatures.h>
using namespace cv;

unsigned int flagNum[2];
unsigned int m_lib;
unsigned char g_connectBuf[67][37];
/*****************************************************************
*	函数名:Feature
*	功能描述: 提取特征
*	形式参数:characters_feature//图像特征存储空间
*	返回值： 无
*   全局变量： *flagNum // 连通域的个数
*                       m_lib1//堆栈的存储数
*                       **g_connectBuf// 图片右下移动
*   文件静态变量：无
*   函数静态变量：无
*------------------------------------------------------------------
*	Revision History
*	No.	    Date	   Revised by	  Item Description
*   V1.0	2017/8/1   SWB            原始版本
******************************************************************/
void Feature(CHARACTER_Feature *characters_feature)
{
      char  buffer[200];
      sprintf(buffer,"%d.bmp",characters_feature->label);
      printf("%s\n",buffer);
      Mat img1=imread(buffer,CV_8UC1);
      double * same= new double[25];
      for(int i=0;i<25;i++)
      {
           char  buffer1[200];
           sprintf(buffer1,"E:\\Qt\\build-Etest-Desktop_Qt_5_6_2_MSVC2015_32bit-Debug\\pic\\charactor\\%d",i);
            _chdir(buffer1);
          {
           //  printf("%d\n",i);
              _finddata_t  imageFile;//无定义类型结构体
              long H_File;  //文件和路径
              int  bNext = 0;
              char fullPath[256];
//              printf("%d\n",_findfirst("*.bmp", &imageFile));
              if ((H_File = _findfirst("*.bmp", &imageFile)) == -1L)
              {
                  printf("error\n");
              }
                else
              {
                    int sum=0;
                    int count=0;
              while (bNext == 0)
              {
                  sprintf(fullPath,"%s",imageFile.name);
                    int a=same1(img1,fullPath);
                    sum+=a;
                    count++;
                    bNext = _findnext(H_File, &imageFile);
              }
              same[i]=(double)sum/count;
              }

          }
      }
    _chdir("E:\\Qt\\build-Etest-Desktop_Qt_5_6_2_MSVC2015_32bit-Debug\\pic");
    FILE *fp;
    if(characters_feature->label==0)
    {
     fp= fopen("test.txt","w");
    }
    else
    {
     fp= fopen("test.txt","a");
    }
    for(int i=0;i<25;i++)
    {
            fprintf(fp,"%lf\t",(double)same[i]/50);
    }
    fprintf(fp,"%d\n",0);
    img1.release();
    fclose(fp);
     //system("pause");

}


void  A(unsigned char *fifo ,unsigned short a,unsigned short b,unsigned short c)
{
    for(int i=0;i<a;i++)
    {
            if(fifo[i]==b)
            {
                fifo[i]=c;
            }
            if(fifo[i]!=0&&fifo[i]!=1)
            {
                fifo[i]=0;
            }

    }
    //system("pause");
}

void Check_lib(unsigned char i,unsigned char j,unsigned char flag,volatile Data* Dp)
{

    if(g_connectBuf[i][j]==1)
    {
        g_connectBuf[i][j] = flag; //将扫到的白点标记成flag，避免重复扫描
        if (flag == 3)
        {
            flagNum[0]++;
        }
        if (flag == 4)
        {
            flagNum[1]++;
        }
        Dp[m_lib].row = i;
        Dp[m_lib].col = j;
        m_lib++;
    }

}



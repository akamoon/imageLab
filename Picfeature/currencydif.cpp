#include "include1\backnotes.h"
unsigned int flagnum[2];
unsigned int m_lb;
//unsigned char g_connectBuf[300][500];
unsigned int Max_h;
unsigned int Min_h;
unsigned int Max_w;
unsigned int Min_w;
unsigned int SUM1;

int currendif()
{
    Mat image=imread("otsu1.bmp",0);
//    int  height=image.rows/8;
//    int  width=image.cols/8;
//    printf("%d   %d ",width,height);
//    double hist[8][8];
//    double midu[8][8];
//    for(int i=0;i<8;i++)
//        for(int j=0;j<8;j++)
//        {
//           hist[i][j]=0;
//        }
//    for(int i=0;i<8;i++)
//        for(int j=0;j<8;j++)
//        {
//            for(int k=0;k<height;k++)
//            {
//                for(int l=0;l<width;l++)
//                {
//                        hist[i][j]=image.at<uchar>(i*height+k,j*width+l);
//                }
//            }
//            midu[i][j]=hist[i][j]/(height*width);
//        }
//    FILE *fp=fopen("1.txt","a");
//    for(int i=0;i<8;i++)
//        for(int j=0;j<8;j++)
//        {
//                    fprintf(fp,"%.3lf ",midu[i][j]);
//        }
//    fprintf(fp,"\n");
//            fclose(fp); //告诉系统我们文件写完了数据更新，但是我们要要重新打开才能在写
//            fflush(fp);

//    Data1 Dp[600];
//    int xi,xj,n=0;
//    unsigned char flag5 = 1;
//        for(int j=10;j<image.cols;j++)
//        {
//                    SUM1=0;
//            if(image.at<uchar>(60,j)==0)
//            {
//                printf("Real  j=%d\n",j);
//     Max_h=60;
//     Min_h=60;
//     Max_w=j;
//     Min_w=j;
//                 m_lb = 0;
//                 image.at<uchar>(60,j)=flag5;
//                 n++;

//                 Dp[m_lb].row = 60;

//                 Dp[m_lb].col = j;

//                 m_lb++;
//                 SUM1++;
////                int A=0;
//                 //利用m_lb非空将四邻域相连的白点数都找到
//                 while(m_lb != 0) //判定栈是否为空
//                 {

//                     xi = Dp[m_lb-1].row;
//                     xj = Dp[m_lb-1].col;

//                     m_lb--;
//                     Check_lib1(xi+1,xj,flag5,Dp,image);

//                     Check_lib1(xi-1,xj,flag5,Dp,image);

//                     Check_lib1(xi,xj-1,flag5,Dp,image);

//                     Check_lib1(xi,xj+1,flag5,Dp,image);
//                     if(SUM1>1800)
//                     {
//                         flag=5;
//                         break;
//                     }
//                     printf("sum: %d \n",SUM1);
//                 }
//                    break;
//                 if(SUM1>200)
//                 {
//                     printf("SUM:  %d m_lib:%d\n",SUM1,m_lb);
//                     break;
//                 }
//        }
//        }

// return 5;

        int start=0;
        for(int j=0;j<image.cols/2;j++)
        {
            int count=0;
            for(int i=image.rows-1;i>0;i--)
            {
                if(image.at<uchar>(i,j)==0)
                {
                    count++;
                }
            }
//            printf("count %d\n",count);
            if(count>20)
            {
                start=j;
                break;
            }
        }

        int end=image.cols-1;
    for(int j=start;j<image.cols;j++)
    {
        int count=0;
        for(int i=image.rows-1;i>0;i--)
        {
            if(image.at<uchar>(i,j)==0)
            {
                count++;
            }
        }

        if(count<10)
        {
          end=j;
          break;
         }
    }

    int start1=0;
    for(int j=0;j<image.rows;j++)
    {
        int count=0;
        for(int i=0;i<image.cols;i++)
        {
            if(image.at<uchar>(j,i)==0)
            {
                count++;
            }
        }
//          printf("count11111 %d\n",count);
        if(count>80)
        {
            start1=j;
            break;
        }
    }

    int end1=image.rows-1;
for(int j=start1;j<image.rows;j++)
{
    int count=0;
    for(int i=0;i<image.cols;i++)
    {
        if(image.at<uchar>(j,i)==0)
        {
            count++;
        }
    }

    if(count<10)
    {
      end1=j;
      break;
     }
}

        Mat Cut(end1-start1,end-start+5,CV_8UC1,Scalar(255));
        int  p2=start1;
        for(int i=0;i<end1-start1;i++)
        {
            int  p1=start;
            for(int j=0;j<end-start+5;j++)
            {
               Cut.at<uchar>(i,j)=image.at<uchar>(p2,p1);
               p1++;
            }
            p2++;
        }

        int flag2=0;
        int count1=0;
//        for(int i=0;i<Cut.rows;i++)
//        {

//            for(int j=0;j<Cut.cols;j++)
//            {

//                if(Cut.at<uchar>(i,j)==0)
//                {
//                    count1++;
//                }
//            }
//        }
//        printf("count1 :%d\n",count1);
//        for(int i=0;i<Cut.rows;i++)
//        {
//            int count=0;
//            for(int j=0;j<Cut.cols;j++)
//            {
//                if(Cut.at<uchar>(i,j)==0)
//                {
//                    count++;
//                }
//            }
//       //     printf("count:%d,Cut:%d count/Cut :%f\n",count,Cut.cols,(double)count/Cut.cols);
//            if((double)count/Cut.cols>0.85)
//            {
//                flag2=i;
//                break;
//            }
//        }
                int end2=Cut.cols-1;

                    for(int j=Cut.cols-1;j>0;j--)
                    {
                        if(Cut.at<uchar>(2,j)==0)
                        {
                            end2=j;
                            break;
                        }

                    }
                   printf("end2:%d\n",end2);
                    int end3=Cut.cols-1;
                    for(int j=Cut.cols-1;j>0;j--)
                    {
                        if(Cut.at<uchar>(7,j)==0)
                        {
                            end3=j;
                            break;
                        }

                    }

                      printf("end3:%d\n",end3);
                     flag2=end3-end2;
               //     printf("count:%d,Cut:%d count/Cut :%f\n",count,Cut.cols,(double)count/Cut.cols);
//                    if((double)count/Cut.cols>0.85)
//                    {
//                        flag2=i;
//                        break;
//                    }
//                }


        printf("flag:%d\n",flag2);
//        Mat Cut(Max_h-Min_h,Max_w-Min_w,CV_8UC1,Scalar(255));
//        int p1=Min_h;

//        for(int i=0;i<Max_h-Min_h;i++)
//        {
//            int p2=Min_w;
//            for(int j=0;j<Max_w-Min_w;j++)
//            {
//               Cut.at<uchar>(i,j)=image.at<uchar>(p1,p2);
//               p2++;
//            }
//            p1++;
//        }

            imwrite("Cut1.bmp",Cut);
    if(flag2<3)
    {
    return 5;
    }
    else
    {
        return 2;
    }

}



int  currendiif()
{
            Mat image=imread("otsu1.bmp",0);
            int count=0;
            for(int i=0;i<image.rows;i++)
            {
                for(int j=image.cols/2;j<image.cols;j++)
                {
                     if(image.at<uchar>(i,j)==0)
                     {
                         count++;
                     }
                }
            }
           // printf("count123123123 :%d\n",count);
            return count;
}
//void Check_lib1(int i,int j,int flag,volatile Data1* Dp,Mat image)
//{

//    if(image.at<uchar>(i,j)==0)
//    {

//        if(i>Max_h)
//        {
//            Max_h=i;
//        }
//        if(i<Min_h)
//        {
//            Min_h=i;
//        }
//        if(j>Max_w)
//        {
//            Max_w=j;
//        }
//        if(j<Min_w)
//        {
//            Min_w=j;
//        }
//        image.at<uchar>(i,j)=flag;
//        Dp[m_lb].row = i;
//        Dp[m_lb].col = j;
//        SUM1++;
//        m_lb++;
//    }

//}
int  currendiif123()
{
    Mat image=imread("test1.bmp",0);
    int  height=image.rows/9;
    int  width=image.cols/9;
    printf("%d   %d ",width,height);
    double hist[9][9];
    double midu[9][9];
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
        {
           hist[i][j]=0;
        }
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
        {
            for(int k=0;k<height;k++)
            {
                for(int l=0;l<width;l++)
                {
                        hist[i][j]+=image.at<uchar>(i*height+k,j*width+l);
                }
            }
            midu[i][j]=hist[i][j]/(height*width);
        }
    FILE *fp=fopen("1.txt","a");
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
        {
                    fprintf(fp,"%.3lf ",midu[i][j]);
        }
            fprintf(fp,"\n");
            fclose(fp); //告诉系统我们文件写完了数据更新，但是我们要要重新打开才能在写
            fflush(fp);
    return 0;
}

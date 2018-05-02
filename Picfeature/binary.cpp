#include "include1\binary.h"
 int piexlSum;
 int thresh=0;
 float w1,u1,w2,u2,g,gmax;

int erzhihua(int flag)
{
    vector<int> hist(256);
    Mat img=imread("test2.bmp",0);
    Mat dst1;
//    imshow("img",img);
//    waitKey(0);
//    if(flag==10)
//    img=strengthImage_Histogram(img);

    gmax=0;
    thresh=0;
    for(auto it=img.begin<uchar>();it!=img.end<uchar>();++it)
        hist[*it]++;//每个阈值的像素点个数
    piexlSum=img.cols*img.rows;

    for(int i=0;i!=hist.size();i++)
    {
        w1=0;
        w2=0;
        u1=0;
        u2=0;
        for(int j=0;j!=hist.size();j++)
        {
            float k=(float)hist[j]/piexlSum;//直方图

            if(j<=i)
            {
                w1+=k;
                u1+=j*k;

            }
            else
            {
                w2+=k;
                u2+=j*k;
            }
        }

            g=w1*w2*(u1-u2)*(u1-u2);
            if(g>gmax)
            {
                gmax=g;
                thresh=i;
            }

    }
    printf("\nthresh=%d\n",thresh);
    if(flag==0)
    {
     threshold(img,dst1,thresh,255,0);
   //  dst1=quzao1(dst1,dst1.cols,dst1.rows);
     dst1=Dilation(0,dst1);
////    imshow("D",dst1);
    dst1=Erosion(2,dst1);
    }
    if(flag==1)
    {
     threshold(img,dst1,120,255,0);
   //  dst1=quzao1(dst1,dst1.cols,dst1.rows);
//     dst1=Dilation(0,dst1);
////    imshow("D",dst1);
 //   dst1=Erosion(2,dst1);
    }
    if(flag==2)
    {
     threshold(img,dst1,thresh,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);
    // dst1=Dilation(0,dst1);
//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
     dst1=Dilation(0,dst1);

    }
    if(flag==3)
    {
     threshold(img,dst1,thresh,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);
     dst1=Dilation(0,dst1);
//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
    }
    if(flag==5)
    {
     threshold(img,dst1,thresh+2,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);
     dst1=Dilation(0,dst1);
//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
    }
    if(flag==6)
    {
     threshold(img,dst1,thresh,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);
     dst1=Dilation(0,dst1);
//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
    }
    if(flag==10)
    {
        threshold(img,dst1,thresh-4,255,0);
        dst1=quzao1(dst1,dst1.cols,dst1.rows);

   //    imshow("D",dst1);
        dst1=Erosion(2,dst1);
        dst1=Dilation(2,dst1);

    }
    if(flag==4)
    {
     threshold(img,dst1,thresh,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
      dst1=Dilation(2,dst1);
    }
    if(flag==8)
    {
     threshold(img,dst1,80,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);
     dst1=Dilation(0,dst1);
//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
    }
    if(flag==22)
    {
     threshold(img,dst1,thresh-10,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
     dst1=Dilation(2,dst1);
    }
    if(flag==20)
    {
     threshold(img,dst1,thresh-4,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
     dst1=Dilation(2,dst1);
    }
    if(flag==50)
    {
     threshold(img,dst1,thresh-3,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
     dst1=Dilation(2,dst1);
    }
    if(flag==51)
    {
     threshold(img,dst1,thresh-7,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
     dst1=Dilation(2,dst1);
    }
    if(flag==100)
    {
     threshold(img,dst1,thresh-5,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(1,dst1);
     dst1=Dilation(1,dst1);
    }

    if(flag==101)
    {
     threshold(img,dst1,thresh-3,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(1,dst1);
     dst1=Dilation(1,dst1);
    }
    if(flag==21)
    {
     threshold(img,dst1,thresh-10,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);



//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
     dst1=Dilation(2,dst1);
    }


    if(flag==11)
    {
     threshold(img,dst1,thresh-4,255,0);
     dst1=quzao1(dst1,dst1.cols,dst1.rows);

//    imshow("D",dst1);
     dst1=Erosion(2,dst1);
     dst1=Dilation(2,dst1);
    }
//    else if(flag==1)
//    {
//    threshold(img,dst1,80,255,0);
//    dst1=quzao1(dst1,dst1.cols,dst1.rows);
//    dst1=Dilation(0,dst1);

////    imshow("D",dst1);
//    dst1=Erosion(2,dst1);
//    }
//    else if(flag==2)
//    {
//    threshold(img,dst1,100,255,0);
//    }
//    else
//    {

//        switch(flag)
//        {
//            case 5:
//            threshold(img,dst1,thresh-25,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 6:
//            threshold(img,dst1,thresh-25,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 10:
//            threshold(img,dst1,thresh-10,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 11:
//            threshold(img,dst1,thresh-12,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 20:
//            threshold(img,dst1,thresh-17,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 21:
//            threshold(img,dst1,thresh-25,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 50:
//            threshold(img,dst1,thresh-20,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 51:
//            threshold(img,dst1,thresh-15,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 100:
//            threshold(img,dst1,thresh-10,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//            break;
//            case 101:
//            threshold(img,dst1,thresh-9,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            break;
//        case 201:
//        threshold(img,dst1,thresh,255,0);
//        dst1=quzao1(dst1,dst1.cols,dst1.rows);
//        dst1=Dilation(0,dst1);
//        dst1=Erosion(2,dst1);
//        break;
//        case 205:
//            threshold(img,dst1,thresh-20,255,0);
//            dst1=quzao1(dst1,dst1.cols,dst1.rows);
//            dst1=Dilation(0,dst1);
//            dst1=Erosion(2,dst1);
//        break;
//        case 210:
//        threshold(img,dst1,thresh-5,255,0);
//        dst1=quzao1(dst1,dst1.cols,dst1.rows);
//        dst1=Dilation(0,dst1);
//        dst1=Erosion(2,dst1);
//        break;
//        case 220:
//        threshold(img,dst1,thresh-27,255,0);
//        dst1=quzao1(dst1,dst1.cols,dst1.rows);
//        dst1=Dilation(0,dst1);
//        dst1=Erosion(2,dst1);
//        break;
//        }
//}
    int count=0;
    for(int i=0;i<dst1.rows;i++)
    {
        for(int j=0;j<dst1.cols;j++)
        {
            if(dst1.at<uchar>(i,j)==0)
            {
                count++;
            }
        }
    }
    printf("\noutus count:  %d\n" ,count);
//    Size dsize = Size(dst1.cols,40);
//    resize(dst1, dst1, dsize);
    imwrite("otsu1.bmp",dst1);
//    imshow("otus.bmp",dst1);
//    waitKey(0);
    return count;

}

Mat strengthImage_Histogram(Mat img)
{

    Mat dst=img.clone();
    //int step = img->widthStep;
    double p_hist[256]={0};//灰度比例
    double s_hist[256]={0};//累计灰度比例
    vector<int> hist(256);
    for(auto it=img.begin<uchar>();it!=img.end<uchar>();++it)
        hist[*it]++;//每个阈值的像素点个数
    int piexlSum=img.cols*img.rows;
    for(int i = 0; i < 256; i++)
    {
        p_hist[i] = (double)hist[i]/piexlSum;
        if(i == 0)s_hist[i] = p_hist[i];
        else s_hist[i] = s_hist[i-1]+p_hist[i];
    }

    //图像增强
    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            dst.at<uchar>(i,j) = s_hist[img.at<uchar>(i,j)]*255+0.5;
        }
    }
    imwrite("testenhanced.bmp",dst);
    return dst;

}
int same(const char* cmp)
{
    Mat mst2=imread("test2.bmp",CV_8UC1);
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
        cout<<"iDiffNum = "<<iDiffNum<<endl;

       int a;
       a= iDiffNum;
       return a;
}

//#include <QCoreApplication>
#include "include1\linear.h"
#include "include1\pic.h"
#include "direct.h"
#include "include1\charactercut.h"
#include "include1\imagecrop.h"
#include "include1\backnotes.h"

//-----------暂时不知道用处的变量定义，先保留不重构---------------
unsigned char *image2;
unsigned char *image3;
unsigned char *image4;
int *countlines();
int *m;
unsigned short upActualSize[2] = {0}, downActualSize[2] = {0};
#pragma DATA_ALIGN (g_line_POINT1,4) //四字节边界对齐，便于提高处理效率
POINT1 g_line_POINT1[8][2]={0}; //保存上下管纸币上、下、左、右边界线上各找出的2个点

int k_up1;
volatile unsigned int errorTag;

unsigned short moneyLength;
unsigned short moneyHeight;
volatile int g_angle;//纸币偏转角度
//-----------暂时不知道用处的变量定义，先保留不重构---------------



 /*
 已重构：
  1.image2,image3 变量名重新定义为image_input，imageOutput
  2.image123 变量名重新定义为：imageOriginal
  3.gray_image1 变量名重新定义为：image_gray
  4.函数返回值使用 funReuslt 统一存储
  5.灰阶图转换函数
  6.switch部分的重构
  7.将写在程序段内的常量提取到程序段开头
 */


//用于识别纸币是大头或者小头时的参数
static const unsigned short PARAMETER_BIG_SMALL[4][4]=
{
    {1200,7500,4500,6800} ,//T1
    {1700, 2800, 7000, 9600},
    {3200,6000,8000,8700},
    {4000,6000,3000,8700},
};

//用于识别大头纸币面值的参数
static  const unsigned short PARAMETER_BIG_DENOMI[12][4]=
{//6900, 9100, 1100,1800
    {3500, 5700, 800, 1400}, //T1判断新旧
    {1690, 2850, 7100, 9600}, //T2
    {1700, 2860, 7100, 9600}, //旧版10，20
    {1900, 3100, 7050, 9550},//旧版 100
    {2000, 3250, 7200, 9600},//新版 100
    {6000, 7200, 1100, 3700},//新版20
    {1100,8800,4400,7200},//头像
    {1900, 2900, 6900, 9400}, //新版50
    {1800, 2860, 7000, 9500}, //旧版50
    {1700, 2850, 7000, 9500}, //新版5
    {1700, 2800, 7000, 9500}, //旧版5
    {6000, 7200, 1500, 3900}, //新版10
};

//用于识别小头纸币面值的参数
static const unsigned short PARAMETER_SMALL_DENOMI[5][4]=
{
    {6000, 7100, 6300, 8600}, //小头1美元
    {2550, 3750, 1610, 3900}, //小头5美元
    {6100, 7300, 6300, 8600}, //小头10美元
    {5800, 7000, 6100, 8400}, //小头20美元
    {2400,8000,4000,6000}
};


 /*****************************************************************
* 函数名:  ToGray
* 功能描述: 
  将原图中的图像转为灰阶，并将像素点数据输出到指定数组中
  根据需求对灰阶图像进行保存
* 形式参数: 
  Mat image_orignial： 使用OpenCV容易读取的原图数据 
  unsigned char* imagePixelStore： 存储的图像灰阶像素的数组 
  int isSave: 是否保存图像
  char* fileName: 保存图像的文件名
* 返回值： 无
*------------------------------------------------------------------
* V1.0  2018/05/07  FX
******************************************************************/
 void ToGray(Mat imageOriginal, unsigned char* imagePixelStore, int isSave, char* fileName)
 {
    Mat imageGray;//openCV图像容器，用来存储转为灰阶的图像
    cvtColor( imageOriginal, imageGray, CV_BGR2GRAY);
    
    if(isSave && fileName != NULL) imwrite(fileName, imageGray);

    int k=0;
    for(int i=0;i<400;i++)
    {
        for(int j=0;j<1440;j++)
        {
            imagePixelStore[k]=imageGray.at<uchar>(i,j);
            k++;
        }
    }
 }


/*****************************************************************
* 函数名:  recDenomination
* 功能描述: 截取图像的币值特征并输出是多少钱
* 形式参数: 
unsigned char *imageUpside ： 图像正面像素数据
unsigned char *imageDownside ： 图像背面像素数据
unsigned char *imageOutput ： 用于存储截取图像数据的存储空间
int denomination ： 币值大小
int paraIndex4New ： 对于新版钱币应该选取的参数序号
int paraIndex4Old ： 对于老板钱币应该选取的参数序号
int chan4New ： 对于新版钱币对应币值截取图像时使用的标识
int chan4Old ： 对于老版钱币对应币值截取图像时使用的标识

* 返回值：  无
*------------------------------------------------------------------
* V1.0  2018/05/07  FX
******************************************************************/
void recDenomination(
    unsigned char *imageUpside,
    unsigned char *imageDownside,
    unsigned char *imageOutput,
    int denomination,
    int paraIndex4New,
    int paraIndex4Old,
    int chan4New,
    int chan4Old)
{
    int funReuslt = 0;
    funReuslt =LinearFitting(imageDownside,99.5,396,0);
    funReuslt=ImageCrop(imageDownside,PARAMETER_BIG_DENOMI[0],imageOutput,0,1);
    if(funReuslt>600)
  {
      funReuslt =LinearFitting(imageUpside,99.5,396,0);
      funReuslt=ImageCrop(
          imageUpside,
          PARAMETER_BIG_DENOMI[paraIndex4New],
          imageOutput,
          0,chan4New);
      printf("这是新版%d美元\n", denomination);
  }
  else
   {
       funReuslt =LinearFitting(imageUpside,99.5,396,0);
       funReuslt=ImageCrop(imageUpside,
        PARAMETER_BIG_DENOMI[paraIndex4Old],
        imageOutput,
        0,chan4Old);
       printf("这是老板%d美元\n", denomination);
  }
}


/*****************************************************************
* 函数名:  Pic
* 功能描述: 通过特征选取，截取图像的方式判断币值
* 形式参数:
  chan： 标识这个图像时纸币的正面还是反面
* 返回值： 
  int型
  -1： 识别失败
  0： 识别成功
*------------------------------------------------------------------
* V1.0  20??/??/??  ??
******************************************************************/
int Pic(int chan)
{
    _chdir(trainPath);

    //**001***定义并申请图像像素的存储空间**********
    unsigned char *imageInputUpside;//用于保存从文件里读入纸币正面的图像的像素点
    unsigned char *imageInputDonwside;//用于保存从文件里读入的纸币背面的图像的像素点
    unsigned char *imageOutput;//用于接收imagecrop函数的输出的图像的像素点
    imageInputUpside = new unsigned char[1440 * 400];
    imageInputDonwside = new unsigned char[1440 * 400];
    imageOutput = new unsigned char[1440 * 400];
    //image2= new unsigned char [1440*400];
    //image3= new unsigned char [1440*400];
    //**001**************************************
    
    //***002***定义OpenCV图像容器**********
    //Mat image123;
    Mat imageOriginal;//openCV图像容器，用来接收从文件中读入的图像
    //Mat gray_image1;
    //Mat imageGray;//openCV图像容器，用来存储转为灰阶的图像
    //***002******************************

    //Debug information
    //printf("................start\n");

    //***003***将纸币图像的正面数据并转为灰阶，存储***************
    imageOriginal = imread("currency_img.bmp",CV_LOAD_IMAGE_COLOR);
    //image123=imread("currency_img.bmp",CV_LOAD_IMAGE_COLOR);
    //unsigned char *image2;//**重复了前面的image2
    //image2= new unsigned char [1440*400];
    /*
      //----005----简化为ToGgray函数---------------------
      cvtColor( imageOriginal, imageGray, CV_BGR2GRAY);
      //cvtColor( image123, gray_image1, CV_BGR2GRAY );
      imwrite("Gray_Image.bmp", imageGray);
      //imwrite("Gray_Image.bmp", gray_image1);
      //---2--把灰阶图的像素点读入数组image_input-------------------
      int k=0;
      for(int i=0;i<400;i++)
      {
          for(int j=0;j<1440;j++)
          {
            imageInputUpside[k]=imageGray.at<uchar>(i,j);
            k++;
          }
      }
      //---2-------------------------------------------------
      //----005----简化为ToGgray函数---------------------
    */
    ToGray(imageOriginal, imageInputUpside, 0, 0);
    //***003**************************************

    

    int funReuslt = 0; //用来存储函数返回的结果

    //----------执行最开始货币大小头的特征裁剪和识别--------6----
    //int temp =LinearFitting(image_input,99.5,396,0);
    funReuslt = LinearFitting(imageInputUpside,99.5,396,0);
    /*
      unsigned short parameters_Dollar[4][4]=
      {
          {1200,7500,4500,6800} ,//T1
          {1700, 2800, 7000, 9600},
          {3200,6000,8000,8700},
          {4000,6000,3000,8700},
      };
    */
    //执行货币大小头的识别
    funReuslt = ImageCrop(imageInputUpside,PARAMETER_BIG_SMALL[2],imageOutput,0,0);
    //int temp5=ImageCrop(imageInputUpside,parameters_Dollar[2],imageOutput,0,0);
    //-----------------------------------------------------6---

    //Debug Info
    //printf("\ntemp5=%d\n",temp5);
    //  system("pause");
    //if(temp5>1000)

    //判断纸币是大头还是小头，并根据大小头选取不同的特征截取参数
    if(funReuslt > 1000)
    {
      if(chan==0)
      {
        /*
          //----005----简化为ToGgray函数---------------------        
          //---3-重新读另一张图currency_img1.bmp,然后转成灰阶------
          image123=imread("currency_img1.bmp",CV_LOAD_IMAGE_COLOR);
          unsigned char *image1;
          image1= new unsigned char [1440*400];
          cvtColor( image123, gray_image1, CV_BGR2GRAY );
          imwrite("Gray_Image1.bmp", gray_image1);
          int k=0;
          //---3--------------------------------------
  
            //---4-把灰阶图传入image1数组------------------
            for(int i=0;i<400;i++)
            {
                for(int j=0;j<1440;j++)
                {
                  image1[k]=gray_image1.at<uchar>(i,j);
                  k++;
                }
            }
            //---4--------------------------------------
            //----005----简化为ToGgray函数---------------------      
        */
          //读取纸币的背面数据，并转为灰阶
          imageOriginal = imread("currency_img1.bmp",CV_LOAD_IMAGE_COLOR);
          ToGray(imageOriginal, imageInputDonwside, 0, 0);
        //int temp =LinearFitting(image2,99.5,396,0);//重复执行

        /*
         unsigned short parameters_Dollar[12][4]=
          {//6900, 9100, 1100,1800
            {3500, 5700, 800, 1400}, //T1判断新旧
            {1690, 2850, 7100, 9600}, //T2
            {1700, 2860, 7100, 9600}, //旧版10，20
            {1900, 3100, 7050, 9550},//旧版 100
            {2000, 3250, 7200, 9600},//新版 100
            {6000, 7200, 1100, 3700},//新版20
            {1100,8800,4400,7200},//头像
            {1900, 2900, 6900, 9400}, //新版50
            {1800, 2860, 7000, 9500}, //旧版50
            {1700, 2850, 7000, 9500}, //新版5
            {1700, 2800, 7000, 9500}, //旧版5
            {6000, 7200, 1500, 3900}, //新版10
          };
        */
          //根据大头钱币的特征参数，判断该大头钱币的币值
          //int temp1=ImageCrop(image2,parameters_Dollar[6],image3,0,8);
          funReuslt = ImageCrop(
              imageInputUpside,
              PARAMETER_BIG_DENOMI[6],
              imageOutput,
              0,8);
       
          //int temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,temp10,temp11,temp12,temp13,temp14,temp15,temp16,temp17,temp18,temp19,temp20,temp21;
          //switch(temp1)

          //根据不同的结果输出不同的币值，并且截取不同的特征
          switch(funReuslt)
          {
            case 0:
            case 5:
                recDenomination(
                    imageInputUpside,
                    imageInputDonwside,
                    imageOutput,
                    5,
                    9,10,
                    5,6);
                break;
            /*
                printf("这是5美元\n");
                 temp2 =LinearFitting(image1,99.5,396,0);
                 temp3=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
                if(temp3>600)
                {
                    temp4 =LinearFitting(image2,99.5,396,0);
                    temp5=ImageCrop(image2,parameters_Dollar[9],image3,0,5);
                    printf("这是新版5美元\n");
                }
                else
                {
                    temp4 =LinearFitting(image2,99.5,396,0);
                    temp5=ImageCrop(image2,parameters_Dollar[10],image3,0,6);
                    printf("这是老板5美元\n");
                }
                break;
            */

            case 1:
            case 7:
                recDenomination(
                    imageInputUpside,
                    imageInputDonwside,
                    imageOutput,
                    10,
                    11,2,
                    10,11);
                break;
            /*
             temp4 =LinearFitting(image1,99.5,396,0);
             temp5=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
              if(temp5>600)
              {
                temp4 =LinearFitting(image2,99.5,396,0);
                temp5=ImageCrop(image2,parameters_Dollar[11],image3,0,10);
                printf("这是新版10美元\n");
              }
              else
              {
                temp4 =LinearFitting(image2,99.5,396,0);
                temp5=ImageCrop(image2,parameters_Dollar[2],image3,0,11);
                printf("这是老板10美元\n");
              }
              break;
            */
            case 2:
            case 8:
                recDenomination(
                    imageInputUpside,
                    imageInputDonwside,
                    imageOutput,
                    20,
                    5,2,
                    20,21);
                break;
            /*
              temp6 =LinearFitting(image1,99.5,396,0);
              temp7=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
              if(temp7>600)
              {
                temp4 =LinearFitting(image2,99.5,396,0);
                temp5=ImageCrop(image2,parameters_Dollar[5],image3,0,20);
                printf("这是新版20美元\n");
              }
              else
              {
                temp6 =LinearFitting(image2,99.5,396,0);
                temp7=ImageCrop(image2,parameters_Dollar[2],image3,0,21);
                printf("这是老板20美元\n");
             }
              break;
            */
            case 3:
            case 6:
                recDenomination(
                    imageInputUpside,
                    imageInputDonwside,
                    imageOutput,
                    50,
                    7,8,
                    50,51);
                break;
            /*
              temp8 =LinearFitting(image1,99.5,396,0);
              temp9=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
              if(temp9>600)
              {
                temp4 =LinearFitting(image2,99.5,396,0);
                temp5=ImageCrop(image2,parameters_Dollar[7],image3,0,50);
                printf("这是新版50美元\n");
              }
              else
              {
                temp4 =LinearFitting(image2,99.5,396,0);
                temp5=ImageCrop(image2,parameters_Dollar[8],image3,0,51);
                printf("这是老板50美元\n");
              }
              break;
            */
            case 4:
            case 9:
                recDenomination(
                    imageInputUpside,
                    imageInputDonwside,
                    imageOutput,
                    100,
                    4,3,
                    100,101);
                break;
            /*
             temp10 =LinearFitting(image1,99.5,396,0);
             temp11=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
              if(temp11>600)
              {
                  temp4 =LinearFitting(image2,99.5,396,0);
                  temp5=ImageCrop(image2,parameters_Dollar[4],image3,0,100);
                  printf("这是新版100美元\n");
              }
              else
              {
                  temp4 =LinearFitting(image2,99.5,396,0);
                  temp5=ImageCrop(image2,parameters_Dollar[3],image3,0,101);
                  printf("这是老板100美元\n");
              }
              break;
              */
  
                /*---------接下来重复了-----------------
                case 5:
                 temp12 =LinearFitting(image1,99.5,396,0);
                 temp13=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
                if(temp13>600)
                {
                    temp4 =LinearFitting(image2,99.5,396,0);
                    temp5=ImageCrop(image2,parameters_Dollar[9],image3,0,5);
                    printf("这是新版5美元\n");
                }
                else
                {
                    temp4 =LinearFitting(image2,99.5,396,0);
                    temp5=ImageCrop(image2,parameters_Dollar[10],image3,0,6);
                    printf("这是老板5美元\n");
                }
                break;
                case 6:
                temp14 =LinearFitting(image1,99.5,396,0);
                temp15=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
                if(temp15>600)
                {
                    temp4 =LinearFitting(image2,99.5,396,0);
                    temp5=ImageCrop(image2,parameters_Dollar[7],image3,0,50);
                    printf("这是新版50美元\n");
                }
                else
                {
                    temp4 =LinearFitting(image2,99.5,396,0);
                    temp5=ImageCrop(image2,parameters_Dollar[8],image3,0,51);
                    printf("这是老板50美元\n");
                }
                break;
                case 7:
                temp16 =LinearFitting(image1,99.5,396,0);
                temp17=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
                if(temp17>600)
                {
                  temp4 =LinearFitting(image2,99.5,396,0);
                  temp5=ImageCrop(image2,parameters_Dollar[11],image3,0,10);
                  printf("这是新版10美元\n");
                }
                else
                {
                  temp4 =LinearFitting(image2,99.5,396,0);
                  temp5=ImageCrop(image2,parameters_Dollar[2],image3,0,11);
                  printf("这是老板10美元\n");
                }
                break;
  
                case 8:
                temp18 =LinearFitting(image1,99.5,396,0);
                temp19=ImageCrop(image1,parameters_Dollar[0],image3,0,20);
                if(temp19>600)
                {
                  temp6 =LinearFitting(image2,99.5,396,0);
                  temp7=ImageCrop(image2,parameters_Dollar[5],image3,0,20);
                  printf("这是新版20美元\n");
                }
                else
                {
                  temp6 =LinearFitting(image2,99.5,396,0);
                  temp7=ImageCrop(image2,parameters_Dollar[2],image3,0,21);
                  printf("这是老板20美元\n");
                }
                break;
                case 9:
                temp20 =LinearFitting(image1,99.5,396,0);
                temp21=ImageCrop(image1,parameters_Dollar[0],image3,0,1);
                if(temp21>600)
                {
                  temp4 =LinearFitting(image2,99.5,396,0);
                  temp5=ImageCrop(image2,parameters_Dollar[4],image3,0,100);
                  printf("这是新版100美元\n");
                }
                else
                {
                  temp4 =LinearFitting(image2,99.5,396,0);
                  temp5=ImageCrop(image2,parameters_Dollar[3],image3,0,101);
                  printf("这是老板100美元\n");
                }
                break;
            */
          }
      }
    }
    else
    {

        funReuslt =LinearFitting(imageInputUpside,99.5,396,0);
      /*
       unsigned short parameters_Dollar[5][4]=
       {
         {6000, 7100, 6300, 8600}, //小头1美元
         {2550, 3750, 1610, 3900}, //小头5美元
         {6100, 7300, 6300, 8600}, //小头10美元
         {5800, 7000, 6100, 8400}, //小头20美元
         {2400,8000,4000,6000}
       };
      */
        funReuslt =ImageCrop(imageInputUpside,PARAMETER_SMALL_DENOMI[4],imageOutput,0,9);
        switch(funReuslt)
        {
          case 0:
              funReuslt =  LinearFitting(imageInputUpside,99.5,396,0);
              funReuslt =ImageCrop(imageInputUpside,PARAMETER_SMALL_DENOMI[2],imageOutput,0,2);
              printf("这是小头1块\n");
              break;
          case 1:
              funReuslt =  LinearFitting(imageInputUpside,99.5,396,0);
              funReuslt =ImageCrop(imageInputUpside,PARAMETER_SMALL_DENOMI[2],imageOutput,0,2);
              printf("这是小头5块\n");
              break;
          case 2:
              funReuslt =  LinearFitting(imageInputUpside,99.5,396,0);
              funReuslt =ImageCrop(imageInputUpside,PARAMETER_SMALL_DENOMI[2],imageOutput,0,2);
              printf("这是小头10块\n");
              break;
          case 3:
              funReuslt =  LinearFitting(imageInputUpside,99.5,396,0);
              funReuslt =ImageCrop(imageInputUpside,PARAMETER_SMALL_DENOMI[1],imageOutput,0,22);
              printf("这是小头20块\n");
              break;
       }
    }

    return 0;
}


//-----------之后的函数没有发现有在其他地方被调用，暂时不进行重构--------------
double *pick()
{
    Mat image=imread("otsu1.bmp",CV_LOAD_IMAGE_COLOR);
    cvtColor(image, image, CV_BGR2GRAY );
    printf("height: %d  width:  %d\n",image.rows,image.cols);
    int count=0;


        for(int i=15;i<image.cols;i++)
        {
                int *temp1= new int[2];
            for(int j=15;j<image.rows;j++)
            {
                temp1[0]=0;
                temp1[1]=0;
                    if(image.at<uchar>(j,i)==0)
                    {
                        printf("i=%d,j=%d\n",i,j);
                        temp1=Charactercut(image,j,i,count);
                      //  printf("temp[0]=%d,temp[1]=%d\n",temp1[0],temp1[1]);
                     //   system("pause");

                    }
                      if(temp1[0]!=0)
                      {

                          printf("temp[0]=%d,temp[1]=%d\n",temp1[0],temp1[1]);
                         count++;
                          i=temp1[1];
                          j=temp1[0];
                          if(j>image.rows-1)
                              break;
                       //   system("pause");
                      }
                     if( i>image.cols-1)
                         break;
            }
            delete [] temp1;
        }
   // return 0;



    CHARACTER_Feature *head,*p1,*p2,*p3;
    /*head用来标记链表，p1总是用来指向新分配的内存空间，
    p2总是指向尾结点，并通过p2来链入新分配的结点*/
    head=NULL;
    for(int i=0; i<11;i++)
    {
             p1=(CHARACTER_Feature*)malloc(sizeof(CHARACTER_Feature));
              /*动态分配内存空间，并数据转换为(struct LNode)类型*/
//             printf("请输入链表中的第%d个数：",i);
//             scanf("%d",&i);
             p1->label=i;
        if(head==NULL)/*指定链表的头指针*/
         {
               head=p1;
               p3=head;
               p2=p1;
            }
         else
          {
              p2->next=p1;
              p2=p1;
             }
        p2->next=NULL;/*尾结点的后继指针为NULL(空)*/
     }

    while(head!=NULL)
    {

        Feature(head);
        head=head->next;
    }
//    int c=scaled();
//    int b=onetoone();
    double *kill= new double[11];
   // kill=SVM();
    for(int i=0;i<11;i++)
    {
    printf("%d",(int)kill[i]);
    }
    printf("\n");
       return kill;
}

int scaled()
{

    int i=0;
    int j=0;
    int *b=new int[2];
    b= countlines();
    double **a= new double*[b[1]];
   cout<<b[0]<<" "<<b[1]<<endl;      //b[0]为列
   for(i=0;i<b[1];i++)            //b[1]为行
   {
       a[i]=new double[b[0]];
   }
   for(i=0;i<b[1];i++)
  {
       for(j=0;j<b[0];j++)
       {
           a[i][j]=0;
       }
   }
   ifstream file;
   file.open("test.txt",ios::in);
   if (file.fail())
   {
      cout<<"can't open"<<endl;
      return 0;
   }
   else
   {
       for(i=0;i<b[1];i++)
           for(j=0;j<b[0];j++)
           {
               file>>a[i][j];
           }
   }
   for(i=0;i<b[1];i++)
   {
       for(j=0;j<b[0];j++)
       {
          cout<<a[i][j]<<' ';
       }
       cout<<endl;

   }
   double **c=new double * [2];

      for(i=0;i<2;i++)
      {
           c[i]= new double [b[0]];
      }
      for(i=0;i<2;i++)
      {
      for (j=0;j<b[0];j++)
      {
          c[i][j]=0;
      }
      }


   for(j=0;j<b[0];j++)
   {
       double max=0;
       double min=999;
       for(i=0;i<b[1];i++)
       {
          if(max<a[i][j])
          {
          max=a[i][j];
          }
          if(min>a[i][j])
          {
          min=a[i][j];
          }
       }
       c[0][j]=max;
       c[1][j]=min;

       cout<<"the "<<j;
          cout<<" lie min is "<<c[1][j]<<endl;     //找出最小值
       cout<<"the "<<j;
          cout<<" lie max is "<<c[0][j]<<endl;     //找出最大值

   }
    double **y =new double *[b[1]];
    double upper=1;
    double lower=0;
     for(i=0;i<b[1];i++)
     {
         y[i]=new double [b[0]];
     }
       for (i=0;i<b[1];i++)
         {
             for(j=0;j<b[0];j++)
               {
                 y[i][j]=0;
                }
          }
//     ofstream writefile;
//     writefile.open("intput.txt",ios::trunc);
         for(j=0;j<b[0]-2;j++)
        {

           for(i=0;i<b[1];i++)
            {

          y[i][j]=lower+(upper-lower)*(a[i][j]-c[1][j])/(c[0][j]-c[1][j]);     //归一化函数

          a[i][j]=y[i][j];
             }

     }

       FILE *fp;
       fp=fopen("input1.txt","a");
       for(i=0;i<b[1];i++)
         {
           for(j=0;j<b[0];j++)
           //  for(j=b[0]-1;j>=0;j--)
             {
               if(j==b[0]-1)
               fprintf(fp,"%lf",a[i][j]);
               else
               fprintf(fp,"%lf\t",a[i][j]);
//                 if(j==b[0]-1)
//                 fprintf(fp,"%lf ",a[i][j]);
//                 else
//                 fprintf(fp,"%d:%lf ",b[0]-j-1,a[i][j]);
             }
             fprintf(fp,"\n");
         }
       fclose(fp);
       return 0;
 }

int *countlines()
{
    int n=0;
    int m=0;
    int min=0;
    int *p =new int[2];
     char c;
      ifstream readfile;
        readfile.open("test.txt",ios::in);
        if(readfile.fail())
        {
            cout<<"can't open the file";
            p[0]=0;
            p[1]=0;
        }
        else {

            while(readfile.get(c))
               {
                if (c=='\t')      //计算数据之间的空格
                {
                    n++;

                }
                if(c=='\n')
                   {

                    m++;
                    min=n;
                    n=1;
                    }
                           //计算回车个数
            }
            p[0]=min;
            p[1]=m;
             cout<<"the m is "<<m<<" the n is "<<min<<endl;
         readfile.close();
        }
        return p;
}

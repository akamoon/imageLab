#include "include1\quzao.h"

Mat Dilation(int dilation_elem, Mat src)
{
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
  int dilation_size=1;
  Mat element = getStructuringElement( dilation_type,
                                       Size( 1*dilation_size+1 , 1*dilation_size+1 ),
                                       Point( 0, 0 ) );
  /// 膨胀操作
  Mat dilation_dst=src.clone();
  dilate( src, dilation_dst, element );
//  imshow( "Dilation Demo", dilation_dst );
  return dilation_dst;
}


Mat Erosion(int erosion_elem, Mat src )
{
  int erosion_type;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
  int erosion_size=1;
  Mat element = getStructuringElement( erosion_type,
                                       Size( 1*erosion_size + 1, 1*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
  /// 腐蚀操作
   Mat erosion_dst=src.clone();
  erode( src, erosion_dst, element);

//  imshow( "Erosion Demo", erosion_dst );
  return erosion_dst;
}

Mat quzao(Mat data, int width, int height)
{
    for(int i=1;i<height-1;i++)
    {
        for(int j=1;j<width-1;j++)
        {
            int flag=0;
            if(data.at<uchar>(i,j)==0)
            {
                if(data.at<uchar>(i-1,j)==255)
                {
                    flag++;
                }
                if(data.at<uchar>(i+1,j)==255)
                {
                    flag++;
                }
                if(data.at<uchar>(i,j-1)==255)
                {
                    flag++;
                }
                if(data.at<uchar>(i,j+1)==255)
                {
                    flag++;
                }
                if(flag>=3)
                {
                data.at<uchar>(i,j)=255;
                }
            }
        }
    }
    return data;
}

Mat quzao1(Mat data, int width, int height)
{
    for(int i=1;i<height-1;i++)
    {
        for(int j=1;j<width-1;j++)
        {
            int flag=0;
            if(data.at<uchar>(i,j)==0)
            {
                if(data.at<uchar>(i-1,j)==255)
                {
                    flag++;
                }
                if(data.at<uchar>(i+1,j)==255)
                {
                    flag++;
                }
                if(data.at<uchar>(i,j-1)==255)
                {
                    flag++;
                }
                if(data.at<uchar>(i,j+1)==255)
                {
                    flag++;
                }
                if(flag>3)
                {
                data.at<uchar>(i,j)=255;
                }
            }
        }
    }
    return data;
}

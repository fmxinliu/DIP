// MakeColorDib.cpp: implementation of the MakeColorDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSplit.h"
#include "MakeColorDib.h"
#include "MainFrm.h"
#include "math.h"

#define MIN(x, a, b)    min(min((x), (a)), min((x), (b)))
#define MAX(x, a, b)    max(max((x), (a)), max((x), (b)))
#define BOUND(x, a, b)  (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MakeColorDib::MakeColorDib()
{

}

MakeColorDib::~MakeColorDib()
{

}

/***************************************************************/
/*函数名称：MakeGray()                                        */
/*函数类型：void                                               */
/*功能：真彩色转化成256色灰度图像。                            */
/***************************************************************/
void MakeColorDib::MakeGray()
{
    ASSERT(this->m_pBitmapInfoHeader->biBitCount == 24);

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x += 3)
        {
            BYTE *pbyBlue = p_data++;   //得到蓝色值
            BYTE *pbyGreen = p_data++;  //得到绿色值
            BYTE *pbyRed = p_data++;    //得到红色值

            //取到原r,g,b中的最大值作为像素三分量值的新值
            int gray = MAX(*pbyRed, *pbyGreen, *pbyBlue);

            *pbyBlue = gray;     //将取到的最大值赋给像素的蓝分量
            *pbyGreen = gray;    //将取到的最大值赋给像素的绿分量
            *pbyRed = gray;      //将取到的最大值赋给像素的红分量
        }
    }
}

/***************************************************************/
/*函数名称：LightAlter(int light)                              */
/*函数类型：void                                               */
/*参数：int light，用户给定的阈值                              */
/*功能：对图像使用阈值法进行亮度调整。                         */
/***************************************************************/
void MakeColorDib::LightAlter(int light)
{
    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x++)
        {
            //调整当前点的亮度
            int temp = int(*p_data * light / 100); 
            
            //判断范围，取得合理的值
            if (temp < 0) 
                *p_data = 0;
            else if (temp > 255)
                *p_data = 255;
            else
                *p_data = temp;
            p_data++;
        }
    }
}

/***************************************************************/
/*函数名称：LightReverse()                                     */
/*函数类型：void                                               */
/*功能：图像的亮度取反。                                       */
/***************************************************************/
void MakeColorDib::LightReverse()
{
    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x++)
        {
           *p_data++ = 255 - *p_data;
        }
    }
}

/***************************************************************/
/*函数名称：ContrastAlter(int increment)                       */
/*函数类型：void                                               */
/*参数：int increment，用户给定的阈值                          */
/*功能：对图像使用阈值法调整对比度处理。                       */
/***************************************************************/
void MakeColorDib::ContrastAlter(int increment)   ///对比度处理
{
    if (increment == 0) // 保持对比度不变
        return;
   
    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    if (increment < 0) // 减小对比度
    {
        int nStretch = 255 + 2 * increment;
        for (int y = 0; y < height; y++) 
        {
            for (int x = 0; x < dibWidth; x++)
            {
                *p_data++ = (BYTE)(((*p_data) * nStretch) / 255 - increment); // g = f * ((255 + 2 * n) / 255) - n (n < 0)
            }
        }
    }
    else // 增加对比度
    {
        int nStretch = 255 - 2 * increment;
        for (int y = 0; y < height; y++) 
        {
            for (int x = 0; x < dibWidth; x++)
            {
                if (*p_data < increment)
                    *p_data = 0;
                else if (*p_data > 255 - increment)
                    *p_data = 255;
                else
                    *p_data = (BYTE)((((*p_data) - increment) * 255) / nStretch); // g = (f - n) * 255 / (255 - 2 * n)
                p_data++;
            }
        }
    }
}

/***************************************************************/
/*函数名称：Exposal()                                          */
/*函数类型：void                                               */
/*功能：图像曝光处理。                                         */
/***************************************************************/
void MakeColorDib::Exposal()
{
    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x++)
        {
            if (*p_data < 128)
                *p_data = 255 - (*p_data);
            *p_data++;
        }
    }
}

/***************************************************************/
/*函数名称：PaintColor(int red, int green, int blue)           */
/*函数类型：void                                               */
/*参数：int red、green、blue，用户给定的红绿蓝值               */
/*功能：对图像使用阈值法进行着色处理。                         */
/***************************************************************/
void MakeColorDib::PaintColor(int red, int green, int blue)
{
    ASSERT(this->m_pBitmapInfoHeader->biBitCount == 24);

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x += 3)
        {
            BYTE *pbyBlue = p_data++;   //得到蓝色值
            BYTE *pbyGreen = p_data++;  //得到绿色值
            BYTE *pbyRed = p_data++;    //得到红色值
            BYTE r = *pbyRed;
            BYTE g = *pbyGreen;
            BYTE b = *pbyBlue;

            //计算灰度值 gray = 0.3r + 0.59g + 0.11b
            BYTE gray = (BYTE)(((WORD)r * 59 + (WORD)g * 30 + (WORD)b * 11) / 100);

            //根据输入值，计算rgb分量比重
            *pbyBlue = (BYTE)((blue * gray) / 255); 
            *pbyGreen = (BYTE)((green * gray) / 255);
            *pbyRed = (BYTE)((red * gray) / 255);
        }
    }
}

/***************************************************************/
/*函数名称：Embossment()                                       */
/*函数类型：void                                               */
/*功能：产生图像浮雕处理效果。                                 */
/***************************************************************/
void MakeColorDib::Embossment()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth - rgbChannel; x++)
        {
            //突出变化部分，淡化相同部分
            int currIndex = dibWidth * y + x; // 当前像素
            int rightIndex = dibWidth * y + x + rgbChannel; // 后一像素
            int temp = p_data[currIndex] - p_data[rightIndex] + 128;

            //判断范围，取得合理的值
            if (temp < 0) 
                p_temp[currIndex] = 0;
            else if (temp > 255)
                p_temp[currIndex] = 255;
            else
                p_temp[currIndex] = temp;
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：NeonLight()                                        */
/*函数类型：void                                               */
/*功能：使图像产生霓虹处理效果。                               */
/***************************************************************/
void MakeColorDib::NeonLight()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    for (int y = 0; y < height - 1; y++) 
    {
        for (int x = 0; x < dibWidth - rgbChannel; x++)
        {
            //突出变化部分，淡化相同部分
            int currIndex = dibWidth * y + x; // 当前像素
            int rightIndex = dibWidth * y + x + rgbChannel; // 后一像素
            int nextIndex = dibWidth * (y + 1) + x; // 下一像素
            int temp = 0;
            temp = (p_data[currIndex] - p_data[rightIndex])
                 * (p_data[currIndex] - p_data[rightIndex])
                 + (p_data[currIndex] - p_data[nextIndex])
                 * (p_data[currIndex] - p_data[nextIndex]);
            temp = 2 * int(sqrt((float)temp));
   
            //判断范围，取得合理的值
            if (temp > 255)
                p_temp[currIndex] = 255;
            else
                p_temp[currIndex] = temp;
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：Smoothness()                                       */
/*函数类型：void                                               */
/*功能：使图像平滑处理。                                       */
/***************************************************************/
void MakeColorDib::Smoothness()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数
   
    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][3]; //定义(3x3)矩阵
    h[0][0] = 1;  h[0][1] = 1; h[0][2] = 1;
    h[1][0] = 1;  h[1][1] = 1; h[1][2] = 1;
    h[2][0] = 1;  h[2][1] = 1; h[2][2] = 1;

    for (int y = 0; y < height - 2; y++) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            double temp =
                //对应的第0行的值乘以矩阵对应值，再相加
                  h[0][0] * p_data[dibWidth * y + x] 
                + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
                + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //对应的第1行的值乘以矩阵对应值，再相加
                + h[1][0] * p_data[dibWidth * (y + 1) + x]
                + h[1][1] * p_data[dibWidth * (y + 1) + x + 1 * rgbChannel]
                + h[1][2] * p_data[dibWidth * (y + 1) + x + 2 * rgbChannel]
                //对应的第2行的值乘以矩阵对应值，再相加
                + h[2][0] * p_data[dibWidth * (y + 2) + x]
                + h[2][1] * p_data[dibWidth * (y + 2) + x + 1 * rgbChannel]
                + h[2][2] * p_data[dibWidth * (y + 2) + x + 2 * rgbChannel];

            //取总和的的平均值
            p_temp[dibWidth * y + x] = (BYTE)abs(temp / 9);
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：Spread()                                           */
/*函数类型：void                                               */
/*功能：图像扩散处理。                                         */
/***************************************************************/
void MakeColorDib::Spread() // 引入一些随机性，使图像如油画一般
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数
    
    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    // 从5*5邻域随机获取像素填充到当前位置
    for (int y = 2; y < height - 2; y++) 
    {
        for (int x = 2 * rgbChannel; x < dibWidth - 2 * rgbChannel; x++)
        {
            int m = rand() % 5 - 2; //取得行随机数
            int n = rand() % 5 - 2; //取得列随机数

            p_temp[dibWidth * y + x] = p_data[dibWidth * (y + n) + (x + m * rgbChannel)];
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：Sharp()                                            */
/*函数类型：void                                               */
/*功能：图像锐化处理。                                         */
/***************************************************************/
void MakeColorDib::Sharp()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    for (int y = 0; y < height - 1; y++) 
    {
        for (int x = 0; x < dibWidth - rgbChannel; x++)
        {
            int temp = 0;
            temp = p_data[dibWidth * (height - y - 2) + (x + rgbChannel)] - p_data[dibWidth * (height - y - 1) + x]; // 左对角线像素差
            temp = p_data[dibWidth * (height - y - 2) + (x + rgbChannel)] + abs(temp) / 4; // 锐化系数 1/4
            
            // 直接赋值，不同于像素超限置 255 
            p_temp[dibWidth * (height - y - 2) + (x + rgbChannel)] = temp; // #1
            //p_temp[dibWidth * (height - y - 2) + (x + rgbChannel)] = (temp > 255) ? 255 : temp; // #2
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：HighLVBO(int gaoTong)                              */
/*函数类型：void                                               */
/*参数：int gaoTong，用户给定的阈值来选择矩阵                  */
/*功能：对图像使用阈值法进行高通滤波。                         */
/***************************************************************/
void MakeColorDib::HighLVBO(int gaoTong)   //高通滤波
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][3];  //定义(3x3)矩阵
    
    if (gaoTong == 1)
    {   //矩阵1（基本高通）
        h[0][0] =  1;   h[0][1] = -2;  h[0][2] =  1;
        h[1][0] = -2;  h[1][1] =   5;  h[1][2] = -2;
        h[2][0] =  1;   h[2][1] = -2;  h[2][2] =  1;
    }
    else if (gaoTong == 2)
    {   //矩阵2（中等高通）
        h[0][0] =  0;  h[0][1] = -1; h[0][2] =  0;
        h[1][0] = -1;  h[1][1] =  5; h[1][2] = -1;
        h[2][0] =  0;  h[2][1] = -1; h[2][2] =  0;
    }
    else
    {   //矩阵3（过量高通）
        h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
        h[1][0] = -1;  h[1][1] =  9; h[1][2] = -1;
        h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1;
    }

    for (int y = height - 1; y >= 2; y--) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            int temp =
                //对应的第0行的值乘以矩阵对应值，再相加
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //对应的第1行的值乘以矩阵对应值，再相加
              + h[1][0] * p_data[dibWidth * (y - 1) + x]
              + h[1][1] * p_data[dibWidth * (y - 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y - 1) + x + 2 * rgbChannel]
                //对应的第2行的值乘以矩阵对应值，再相加
              + h[2][0] * p_data[dibWidth * (y - 2) + x]
              + h[2][1] * p_data[dibWidth * (y - 2) + x + 1 * rgbChannel]
              + h[2][2] * p_data[dibWidth * (y - 2) + x + 2 * rgbChannel];

            p_temp[dibWidth * (y - 1) + x + 1 * rgbChannel] = (temp > 255) ? 255 : abs(temp);
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：LowLVBO()                                          */
/*函数类型：void                                               */
/*功能：实现图像低通滤波(3x3)。                                */
/***************************************************************/
void MakeColorDib::LowLVBO()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    double h[3][3]; //定义(3x3)矩阵
    h[0][0] = 0.1;  h[0][1] = 0.1; h[0][2] = 0.1;
    h[1][0] = 0.1;  h[1][1] = 0.2; h[1][2] = 0.1;
    h[2][0] = 0.1;  h[2][1] = 0.1; h[2][2] = 0.1;

    for (int y = height - 1; y >= 2; y--) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            double temp =
                //对应的第0行的值乘以矩阵对应值，再相加
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //对应的第1行的值乘以矩阵对应值，再相加
              + h[1][0] * p_data[dibWidth * (y - 1) + x]
              + h[1][1] * p_data[dibWidth * (y - 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y - 1) + x + 2 * rgbChannel]
               //对应的第2行的值乘以矩阵对应值，再相加
              + h[2][0] * p_data[dibWidth * (y - 2) + x]
              + h[2][1] * p_data[dibWidth * (y - 2) + x + 1 * rgbChannel]
              + h[2][2] * p_data[dibWidth * (y - 2) + x + 2 * rgbChannel];

            p_temp[dibWidth * (y - 1) + x + 1 * rgbChannel] = (BYTE)abs(temp); // 模板系数 = 1
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：LowLVBObig()                                       */
/*函数类型：void                                               */
/*功能：实现图像低通滤波(5x5)。                                */
/***************************************************************/
void MakeColorDib::LowLVBObig()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[5][5]; //定义(5x5)矩阵
    h[0][0] = 1;  h[0][1] = 1; h[0][2] = 1; h[0][3] = 1; h[0][4] = 1;
    h[1][0] = 1;  h[1][1] = 2; h[1][2] = 2; h[1][3] = 2; h[1][4] = 1;
    h[2][0] = 1;  h[2][1] = 2; h[2][2] = 3; h[2][3] = 2; h[2][4] = 1;
    h[3][0] = 1;  h[3][1] = 2; h[3][2] = 2; h[3][3] = 2; h[3][4] = 1;
    h[4][0] = 1;  h[4][1] = 1; h[4][2] = 1; h[4][3] = 1; h[4][4] = 1;

    for (int y = height - 3; y >= 4; y--)
    {
        for (int x = 0; x < dibWidth - 4 * rgbChannel; x++)
        {     
            int temp =
                //对应的第0行的值乘以矩阵对应值，再相加
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
              + h[0][3] * p_data[dibWidth * y + x + 3 * rgbChannel]
              + h[0][4] * p_data[dibWidth * y + x + 4 * rgbChannel]
               //对应的第1行的值乘以矩阵对应值，再相加
              + h[1][0] * p_data[dibWidth * (y - 1) + x]
              + h[1][1] * p_data[dibWidth * (y - 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y - 1) + x + 2 * rgbChannel]
              + h[1][3] * p_data[dibWidth * (y - 1) + x + 3 * rgbChannel]
              + h[1][4] * p_data[dibWidth * (y - 1) + x + 4 * rgbChannel]
               //对应的第2行的值乘以矩阵对应值，再相加
              + h[2][0] * p_data[dibWidth * (y - 2) + x]
              + h[2][1] * p_data[dibWidth * (y - 2) + x + 1 * rgbChannel]
              + h[2][2] * p_data[dibWidth * (y - 2) + x + 2 * rgbChannel]
              + h[2][3] * p_data[dibWidth * (y - 2) + x + 3 * rgbChannel]
              + h[2][4] * p_data[dibWidth * (y - 2) + x + 4 * rgbChannel]
              //对应的第3行的值乘以矩阵对应值，再相加
              + h[3][0] * p_data[dibWidth * (y - 3) + x]
              + h[3][1] * p_data[dibWidth * (y - 3) + x + 1 * rgbChannel]
              + h[3][2] * p_data[dibWidth * (y - 3) + x + 2 * rgbChannel]
              + h[3][3] * p_data[dibWidth * (y - 3) + x + 3 * rgbChannel]
              + h[3][4] * p_data[dibWidth * (y - 3) + x + 4 * rgbChannel]
              //对应的第4行的值乘以矩阵对应值，再相加
              + h[4][0] * p_data[dibWidth * (y - 4) + x]
              + h[4][1] * p_data[dibWidth * (y - 4) + x + 1 * rgbChannel]
              + h[4][2] * p_data[dibWidth * (y - 4) + x + 2 * rgbChannel]
              + h[4][3] * p_data[dibWidth * (y - 4) + x + 3 * rgbChannel]
              + h[4][4] * p_data[dibWidth * (y - 4) + x + 4 * rgbChannel];
              
            p_temp[dibWidth * (y - 2) + x + 2 * rgbChannel]= (BYTE)abs(temp / 35); // 模板系数 = 35
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：ShuiPingGROW()                                     */
/*函数类型：void                                               */
/*功能：使图像水平增强。                                       */
/***************************************************************/
void MakeColorDib::ShuiPingGROW()
{
    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][1]; //定义(3x1)矩阵
    h[0][0] = -1;  
    h[1][0] = 2; 
    h[2][0] = -1;

    // 上下行像素对应相减
    for (int y = 0; y < height - 2; y++) 
    {
        for (int x = 0; x < dibWidth; x++)
        {
            int temp = h[0][0] * p_data[dibWidth * y + x]
                     + h[1][0] * p_data[dibWidth * (y + 1) + x]
                     + h[2][0] * p_data[dibWidth * (y + 2) + x];

            if (temp > 20)
                temp = abs(temp) + 100;
            else
                temp = abs(temp);

            p_temp[dibWidth * y + x] = (temp > 255) ? 255 : temp;
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：ChuiZhiGROW()                                      */
/*函数类型：void                                               */
/*功能：使图像垂直增强。                                       */
/***************************************************************/
void MakeColorDib::ChuiZhiGROW()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[1][3]; //定义(1x3)矩阵
    h[0][0] = -1; 
    h[0][1] = 2;
    h[0][2] = -1;

    // 左右列像素对应相减
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {
            int temp = h[0][0] * p_data[dibWidth * y + x]
                     + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
                     + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel];

            if (temp > 20)
                temp = abs(temp) + 100;
            else
                temp = abs(temp);

            p_temp[dibWidth * y + x] = (temp > 255) ? 255 : temp;
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：ShuangXiangGROW()                                  */
/*函数类型：void                                               */
/*功能：使图像双向增强。                                       */
/***************************************************************/
void MakeColorDib::ShuangXiangGROW()    //双向增强
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][3]; //定义(3x3)矩阵
    h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
    h[1][0] = -1;  h[1][1] =  8; h[1][2] = -1;
    h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1; 

    for (int y = 0; y < height - 2; y++) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            int temp =
                //对应的第0行的值乘以矩阵对应值，再相加
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //对应的第1行的值乘以矩阵对应值，再相加
              + h[1][0] * p_data[dibWidth * (y + 1) + x]
              + h[1][1] * p_data[dibWidth * (y + 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y + 1) + x + 2 * rgbChannel]
                //对应的第2行的值乘以矩阵对应值，再相加
              + h[2][0] * p_data[dibWidth * (y + 2) + x]
              + h[2][1] * p_data[dibWidth * (y + 2) + x + 1 * rgbChannel]
              + h[2][2] * p_data[dibWidth * (y + 2) + x + 2 * rgbChannel];

              if (temp > 20)
                  temp = abs(temp) + 100;
              else
                  temp = abs(temp);

              p_temp[dibWidth * y + x] = (temp > 255) ? 255 : temp;
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*函数名称：Mosaic()                                           */
/*函数类型：void                                               */
/*功能：使图像产生马赛克效果。                                 */
/***************************************************************/
void MakeColorDib::Mosaic()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //取得原图的数据区指针
    int width = this->GetWidth();    //取得原图的数据区宽度
    int height = this->GetHeight();  //取得原图的数据区高度
    int dibWidth = this->GetDibWidthBytes(); //取得原图的每行字节数

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    // 分块，块内填充5*5邻域平均值
    for (int y = 2; y < height - 2; y += 5) 
    {
        for (int x = 2; x < width - 2; x += 5)
        {
            for (int k = 0; k < rgbChannel; k++)
            {
                int sum = 0;       
                for (int n = -2; n <= 2; n++)
                    for (int m = -2; m <= 2; m++)
                        sum += p_data[dibWidth * (y + n) + ((x + m) * rgbChannel + k)];

                int avg = sum / 25;
                for (int n = -2; n <= 2; n++)
                    for (int m = -2; m <= 2; m++)
                        p_temp[dibWidth * (y + n) + ((x + m) * rgbChannel + k)] = (BYTE)avg;
            }   
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

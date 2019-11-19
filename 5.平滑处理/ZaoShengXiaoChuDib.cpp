#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "ZaoShengXiaoChuDib.h"
#include "MainFrm.h"
#include "DSplitView.h"
 
ZaoShengXiaoChuDib::ZaoShengXiaoChuDib()
{
}

ZaoShengXiaoChuDib::~ZaoShengXiaoChuDib()
{
}

/***************************************************************/           
/*函数名称：GuDing(int YuZhi)                                  */      
/*函数类型：void                                               */    
/*参数说明：YuZhi 给定阈值                                     */       
/*功能：对图像进行固定阀值运算。                               */
/***************************************************************/
void ZaoShengXiaoChuDib::GuDing(int YuZhi)//对图像进行固定阀值运算
{
    LPBYTE p_data=this->GetData(); //原图数据区指针
    int width=this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int height=this->GetHeight(); // 原图高度

    for(int i=0;i<height*width;i++)
    {
        if(*p_data>YuZhi)          //灰度值大于给定阈值，置为255
            *p_data=255;
        else
            *p_data=0;             //不大于置为0
        p_data++;
    }
}

/***************************************************************/ 
/*函数名称：HeiBaiDianZaoSheng()                               */
/*函数类型：void                                               */
/*功能：对二值图像的黑白点噪声消除。                             */
/***************************************************************/
void ZaoShengXiaoChuDib::HeiBaiDianZaoSheng()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    // 1.二值化
    GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用3*3的掩模(滤波器、核、窗口)滤波
    //  [1, 1, 1
    //   1, 0, 1
    //   1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {
                int avg = (p_data[lineBytes * (y - 1) + (x - 1)]
                    + p_data[lineBytes * (y - 1) + (x)]
                    + p_data[lineBytes * (y - 1) + (x + 1)]
                    + p_data[lineBytes * (y) + (x - 1)]
                    + p_data[lineBytes * (y) + (x + 1)]
                    + p_data[lineBytes * (y + 1) + (x - 1)]
                    + p_data[lineBytes * (y + 1) + (x)]
                    + p_data[lineBytes * (y + 1) + (x + 1)]) / 8; // 8邻域均值

                if (abs(avg - p_data[lineBytes * y + x]) > 127.5) // 阈值
                    p_temp[lineBytes * y + x] = avg;
            }
        }

    } else {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {
                int avg_b = (p_data[lineBytes * (y - 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y - 1) + (x) * 3]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3]
                    + p_data[lineBytes * (y) + (x - 1) * 3]
                    + p_data[lineBytes * (y) + (x + 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x) * 3]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3]) / 8; // 8邻域均值

                int avg_g = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 1]) / 8; // 8邻域均值

                int avg_r = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 2]) / 8; // 8邻域均值

                if (abs(avg_b - p_data[lineBytes * y + x * 3]) > 127.5) // 阈值
                    p_temp[lineBytes * y + x * 3] = avg_b;
                if (abs(avg_g - p_data[lineBytes * y + x * 3 + 1]) > 127.5) // 阈值
                    p_temp[lineBytes * y + x * 3 + 1] = avg_g;
                if (abs(avg_r - p_data[lineBytes * y + x * 3 + 2]) > 127.5) // 阈值
                    p_temp[lineBytes * y + x * 3 + 2] = avg_r;
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}

/***************************************************************/ 
/*函数名称：GuliHaiDian(int connec)                                  */
/*函数类型：void                                               */
/*参数：int connec，设定的连通选择                             */
/*功能：对二值图像进行消除孤立黑像素点。                       */
/***************************************************************/
void ZaoShengXiaoChuDib::GuliHaiDian(int connec)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    // 1.二值化
    GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用3*3的核滤波
    //  4邻域:  [0, 1, 0     8邻域:[1, 1, 1
    //           1, 0, 1           1, 0, 1
    //           0, 1, 0]          1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {
                if (p_data[lineBytes * y + x]) 
                    continue; // 只处理黑点

                int sum = -1;
                if (connec == 4) { // 4 邻域
                    sum = p_data[lineBytes * (y - 1) + (x)]
                        + p_data[lineBytes * (y) + (x - 1)]
                        + p_data[lineBytes * (y) + (x + 1)]
                        + p_data[lineBytes * (y + 1) + (x)];
                } else if (connec == 8) { // 8 邻域
                    sum = p_data[lineBytes * (y - 1) + (x - 1)]
                        + p_data[lineBytes * (y - 1) + (x)]
                        + p_data[lineBytes * (y - 1) + (x + 1)]
                        + p_data[lineBytes * (y) + (x - 1)]
                        + p_data[lineBytes * (y) + (x + 1)]
                        + p_data[lineBytes * (y + 1) + (x - 1)]
                        + p_data[lineBytes * (y + 1) + (x)]
                        + p_data[lineBytes * (y + 1) + (x + 1)];
                }

                // xx连接的情况下，消去周围都为255(白点)的孤立黑点
                if (sum == 255 * connec)
                    p_temp[lineBytes * y + x] = 255;
            }
        }
 
    } else {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {

                int avg_r = -1, avg_g = -1, avg_b = -1;
                
                if (p_data[lineBytes * y + x * 3] != 255) {
                    if (connec == 4) {
                        avg_b = p_data[lineBytes * (y - 1) + (x) * 3]
                            + p_data[lineBytes * (y) + (x - 1) * 3]
                            + p_data[lineBytes * (y) + (x + 1) * 3]
                            + p_data[lineBytes * (y + 1) + (x) * 3];
                    } else if (connec == 8) {
                        avg_b = p_data[lineBytes * (y - 1) + (x - 1) * 3]
                            + p_data[lineBytes * (y - 1) + (x) * 3]
                            + p_data[lineBytes * (y - 1) + (x + 1) * 3]
                            + p_data[lineBytes * (y) + (x - 1) * 3]
                            + p_data[lineBytes * (y) + (x + 1) * 3]
                            + p_data[lineBytes * (y + 1) + (x - 1) * 3]
                            + p_data[lineBytes * (y + 1) + (x) * 3]
                            + p_data[lineBytes * (y + 1) + (x + 1) * 3];
                    }
                }

                if (p_data[lineBytes * y + x * 3 + 1] != 255) {
                    if (connec == 4) {
                        avg_g = p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                            + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                            + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                            + p_data[lineBytes * (y + 1) + (x) * 3 + 1];
                    } else if (connec == 8) {
                        avg_g = p_data[lineBytes * (y - 1) + (x - 1) * 3 + 1]
                            + p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                            + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 1]
                            + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                            + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                            + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 1]
                            + p_data[lineBytes * (y + 1) + (x) * 3 + 1]
                            + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 1];
                    }
                }

                if (p_data[lineBytes * y + x * 3 + 2] != 255) {
                    if (connec == 4) {
                        avg_r = p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                            + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                            + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                            + p_data[lineBytes * (y + 1) + (x) * 3 + 2];
                    } else if (connec == 8) {
                        avg_r = p_data[lineBytes * (y - 1) + (x - 1) * 3 + 2]
                            + p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                            + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 2]
                            + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                            + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                            + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 2]
                            + p_data[lineBytes * (y + 1) + (x) * 3 + 2]
                            + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 2];
                    }
                }

                // xx连接的情况下，消去周围都为255(白点)的孤立黑点
                if (avg_b == 255 * connec)
                    p_temp[lineBytes * y + x] = 255;
                if (avg_g == 255 * connec)
                    p_temp[lineBytes * y + x] = 255;
                if (avg_r == 255 * connec)
                    p_temp[lineBytes * y + x] = 255;
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}

/***************************************************************/ 
/*函数名称：threethree()                                       */
/*函数类型：void                                               */
/*功能：对图像进行3*3均值滤波处理（滤除噪声的同时产生边缘模糊）。   */
/***************************************************************/
void ZaoShengXiaoChuDib::threethree() 
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.二值化
    //GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用3*3的掩模(滤波器、核、窗口)滤波
    //  [1, 1, 1
    //   1, 0, 1
    //   1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {
                int avg = (p_data[lineBytes * (y - 1) + (x - 1)]
                    + p_data[lineBytes * (y - 1) + (x)]
                    + p_data[lineBytes * (y - 1) + (x + 1)]
                    + p_data[lineBytes * (y) + (x - 1)]
                    + p_data[lineBytes * (y) + (x + 1)]
                    + p_data[lineBytes * (y + 1) + (x - 1)]
                    + p_data[lineBytes * (y + 1) + (x)]
                    + p_data[lineBytes * (y + 1) + (x + 1)]) / 8; // 8邻域均值

                p_temp[lineBytes * y + x] = avg;                   
            }
        }

    } else {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {
                int avg_b = (p_data[lineBytes * (y - 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y - 1) + (x) * 3]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3]
                    + p_data[lineBytes * (y) + (x - 1) * 3]
                    + p_data[lineBytes * (y) + (x + 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x) * 3]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3]) / 8; // 8邻域均值

                int avg_g = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 1]) / 8; // 8邻域均值

                int avg_r = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 2]) / 8; // 8邻域均值

                    p_temp[lineBytes * y + x * 3] = avg_b;
                    p_temp[lineBytes * y + x * 3 + 1] = avg_g;
                    p_temp[lineBytes * y + x * 3 + 2] = avg_r; 
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}

/***************************************************************/
/*函数名称：Chaoxian(int T)                                    */
/*函数类型：void                                               */
/*参数：int T，设定的阈值                                      */
/*功能：超限邻域平均法。                                       */
/***************************************************************/
void ZaoShengXiaoChuDib::Chaoxian(int T)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.二值化
    //GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用3*3的掩模(滤波器、核、窗口)滤波
    //  [1, 1, 1
    //   1, 0, 1
    //   1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {
                int avg = (p_data[lineBytes * (y - 1) + (x - 1)]
                    + p_data[lineBytes * (y - 1) + (x)]
                    + p_data[lineBytes * (y - 1) + (x + 1)]
                    + p_data[lineBytes * (y) + (x - 1)]
                    + p_data[lineBytes * (y) + (x + 1)]
                    + p_data[lineBytes * (y + 1) + (x - 1)]
                    + p_data[lineBytes * (y + 1) + (x)]
                    + p_data[lineBytes * (y + 1) + (x + 1)]) / 8; // 8邻域均值

                if (abs(avg - p_data[lineBytes * y + x]) > T) // 阈值
                    p_temp[lineBytes * y + x] = avg;
            }
        }

    } else {
        for (int y = 1; y < height - 1; y++) { // 忽略边界像素
            for (int x = 1; x < width - 1; x++) {
                int avg_b = (p_data[lineBytes * (y - 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y - 1) + (x) * 3]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3]
                    + p_data[lineBytes * (y) + (x - 1) * 3]
                    + p_data[lineBytes * (y) + (x + 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x) * 3]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3]) / 8; // 8邻域均值

                int avg_g = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 1]) / 8; // 8邻域均值

                int avg_r = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 2]) / 8; // 8邻域均值

                if (abs(avg_b - p_data[lineBytes * y + x * 3]) > T) // 阈值
                    p_temp[lineBytes * y + x * 3] = avg_b;
                if (abs(avg_g - p_data[lineBytes * y + x * 3 + 1]) > T) // 阈值
                    p_temp[lineBytes * y + x * 3 + 1] = avg_g;
                if (abs(avg_r - p_data[lineBytes * y + x * 3 + 2]) > T) // 阈值
                    p_temp[lineBytes * y + x * 3 + 2] = avg_r;
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}

/***************************************************************/ 
/*函数名称：nn(int n)                                          */
/*函数类型：void                                               */
/*参数：int n，设定的屏蔽窗口参数（奇数）                        */
/*功能：对图像进行n*n均值滤波处理（会产生细节模糊）。             */
/***************************************************************/
void ZaoShengXiaoChuDib::nn(int n)
{
    if(n < 3 || n % 2 != 1) {// 确认n为奇数
        AfxMessageBox("请输入一个大于等于3的奇数");
        return;
    }

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.二值化
    //GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用n*n的掩模(滤波器、核、窗口)滤波
    int border = (n - 1) / 2;
    if (GetRGB()) {
        for (int y = border; y < height - border; y++) { // 忽略边界像素
            for (int x = border; x < width - border; x++) {
                int sum = 0; //求N*N屏蔽窗口内的总和sum
                for (int t = -border; t <= border; t++) {
                    for (int s = -border; s <= border; s++) {
                        sum += p_data[lineBytes * (y + t) + (x + s)];
                    }
                }

                // 把n*n屏蔽窗口内的平均值四舍五入后作为显示图像像素值  
                p_temp[lineBytes * y + x] = (int)((float)sum / (n * n) + 0.5);               
            }
        }

    } else {
        for (int y = border; y < height - border; y++) { // 忽略边界像素
            for (int x = border; x < width - border; x++) {
                int sum_r = 0, sum_g = 0, sum_b = 0;
                for (int t = -border; t <= border; t++) {
                    for (int s = -border; s <= border; s++) {
                        sum_b += p_data[lineBytes * (y + t) + (x + s) * 3];
                        sum_g += p_data[lineBytes * (y + t) + (x + s) * 3 + 1];
                        sum_r += p_data[lineBytes * (y + t) + (x + s) * 3 + 2];
                    }
                }

                // 把n*n屏蔽窗口内的平均值四舍五入后作为显示图像像素值  
                p_temp[lineBytes * y + x * 3] = (int)((float)sum_b / (n * n) + 0.5);
                p_temp[lineBytes * y + x * 3 + 1] = (int)((float)sum_g / (n * n) + 0.5);
                p_temp[lineBytes * y + x * 3 + 2] = (int)((float)sum_r / (n * n) + 0.5);
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
} 

// 从小到大排序返回中值
int MidPixel(const LPBYTE pdata, int num)
{
    int minIndex;
    for (int i = 0; i < num - 1; i++) {
        minIndex = i;
        for (int j = i + 1; j < num; j++) {
            if (pdata[j] < pdata[minIndex]) {
                minIndex = j;
            }
        }

        if (i != minIndex) {
            int temp = pdata[i];
            pdata[i] = pdata[minIndex];
            pdata[minIndex] = temp;
        }
    }

    return pdata[(num - 1) / 2];
}

/***************************************************************/ 
/*函数名称：nnzhong(int n)                                      */
/*函数类型：void                                                */
/*参数：int n，设定的屏蔽窗口参数（奇数）                         */
/*功能：对图像进行n*n中值滤波处理（对孤立噪声消除比较好）。         */
/***************************************************************/
void ZaoShengXiaoChuDib::nnzhong(int n)
{
    if(n < 3 || n % 2 != 1) {// 确认n为奇数
        AfxMessageBox("请输入一个大于等于3的奇数");
        return;
    }

    int n2 = n * n;
    int n3 = (n - 1) / 2;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();
    LPBYTE p_area = new BYTE[n2];

    //// 1.二值化
    //GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用n*n的掩模(滤波器、核、窗口)滤波
    
    if (GetRGB()) {
        for (int y = n3; y < height - n3; y++) { // 忽略边界像素
            for (int x = n3; x < width - n3; x++) {
                // 抽取模板覆盖的n*n个像素
                int index = 0;
                for (int t = -n3; t <= n3; t++) {
                    for (int s = -n3; s <= n3; s++) {
                        p_area[index++] = p_data[lineBytes * (y + t) + (x + s)];
                    }
                }

                // 取n*n个像素中值
                p_temp[lineBytes * y + x] = MidPixel(p_area, n2);               
            }
        }

    } else {
        for (int y = n3; y < height - n3; y++) { // 忽略边界像素
            for (int x = n3; x < width - n3; x++) {
                // 对RGB三通道分别处理
                for (int k = 0; k < 3; k++) {
                    int index = 0;
                    for (int t = -n3; t <= n3; t++) {
                        for (int s = -n3; s <= n3; s++) {
                            p_area[index++] = p_data[lineBytes * (y + t) + (x + s) * 3 + k];
                        }
                    }

                    // 取n*n个像素中值
                    p_temp[lineBytes * y + x * 3 + k] = MidPixel(p_area, n2);
                }
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    //delete p_temp, p_area; // 会有内存泄漏
    delete p_temp;
    delete p_area;
}

/***************************************************************/
/*函数名称：shizi(int n)                                        */
/*函数类型：void                                                */
/*参数：int n，设定的屏蔽窗口参数（奇数）                         */
/*功能：对图像进行十字型中值滤波。                                */
/***************************************************************/
void ZaoShengXiaoChuDib::shizi(int n)
{
    if(n < 3 || n % 2 != 1) {// 确认n为奇数
        AfxMessageBox("请输入一个大于等于3的奇数");
        return;
    }

    int n2 = n + n - 1;
    int n3 = (n - 1) / 2;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();
    LPBYTE p_area = new BYTE[n2];

    //// 1.二值化
    //GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用n*n的掩模(滤波器、核、窗口)滤波

    if (GetRGB()) {
        for (int y = n3; y < height - n3; y++) { // 忽略边界像素
            for (int x = n3; x < width - n3; x++) {
                // 抽取模板覆盖的十字区像素
                int index = 0;
                
                // 含中心的水平部分
                for (int s = -n3; s <= n3; s++) {
                    p_area[index++] = p_data[lineBytes * y + (x + s)];
                }

                // 不含中心的垂直部分
                for (int t = -n3; t <= n3; t++) {
                    if (t == 0)
                        continue;
                    p_area[index++] = p_data[lineBytes * (y + t) + x];
                }

                // 取n*n个像素中值
                p_temp[lineBytes * y + x] = MidPixel(p_area, n2);               
            }
        }

    } else {
        for (int y = n3; y < height - n3; y++) { // 忽略边界像素
            for (int x = n3; x < width - n3; x++) {
                // 对RGB三通道分别处理
                for (int k = 0; k < 3; k++) {

                    // 抽取模板覆盖的十字区像素
                    int index = 0;

                    // 含中心的水平部分
                    for (int s = -n3; s <= n3; s++) {
                        p_area[index++] = p_data[lineBytes * y + (x + s) * 3 + k];
                    }

                    // 不含中心的垂直部分
                    for (int t = -n3; t <= n3; t++) {
                        if (t == 0)
                            continue;
                        p_area[index++] = p_data[lineBytes * (y + t) + x * 3 + k];
                    }

                    // 取n*n个像素中值
                    p_temp[lineBytes * y + x * 3 + k] = MidPixel(p_area, n2);  
                }
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
    delete p_area;
}

// 求最大像素值
int MaxPixel(const LPBYTE pdata, int num)
{
    int maxValue = pdata[0];
    for (int i = 1; i < num; i++) {
        maxValue = max(maxValue, pdata[i]);
    }

    return maxValue;
}

/***************************************************************/ 
/*函数名称：nnzuida(int n)                                      */
/*函数类型：void                                                */
/*参数：int n，设定的屏蔽窗口参数（奇数）                         */
/*功能：对图像进行N*N最大值滤波。                                */
/**************************************************************/
void ZaoShengXiaoChuDib::nnzuida(int n)
{
    if(n < 3 || n % 2 != 1) {// 确认n为奇数
        AfxMessageBox("请输入一个大于等于3的奇数");
        return;
    }

    int n2 = n * n;
    int n3 = (n - 1) / 2;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();
    LPBYTE p_area = new BYTE[n2];

    //// 1.二值化
    //GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.用n*n的掩模(滤波器、核、窗口)滤波

    if (GetRGB()) {
        for (int y = n3; y < height - n3; y++) { // 忽略边界像素
            for (int x = n3; x < width - n3; x++) {
                // 抽取模板覆盖的n*n个像素
                int index = 0;
                for (int t = -n3; t <= n3; t++) {
                    for (int s = -n3; s <= n3; s++) {
                        p_area[index++] = p_data[lineBytes * (y + t) + (x + s)];
                    }
                }

                // 取n*n个像素中值
                p_temp[lineBytes * y + x] = MaxPixel(p_area, n2);               
            }
        }

    } else {
        for (int y = n3; y < height - n3; y++) { // 忽略边界像素
            for (int x = n3; x < width - n3; x++) {
                // 对RGB三通道分别处理
                for (int k = 0; k < 3; k++) {
                    int index = 0;
                    for (int t = -n3; t <= n3; t++) {
                        for (int s = -n3; s <= n3; s++) {
                            p_area[index++] = p_data[lineBytes * (y + t) + (x + s) * 3 + k];
                        }
                    }

                    // 取n*n个像素中值
                    p_temp[lineBytes * y + x * 3 + k] = MaxPixel(p_area, n2);
                }
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
    delete p_area;
}

/***************************************************************/  
/*函数名称：suijizaosheng()                                     */
/*函数类型：void                                                */
/*功能：对图像进行随机噪声处理。                                  */
/***************************************************************/
void ZaoShengXiaoChuDib::suijizaosheng()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            // 随机噪声
            int noisepoint = rand() / 1024; // [0, RAND_MAX / 1024 = 31]
            p_data[lineBytes * y + x] = p_data[lineBytes * y + x] * 224 / 255 + noisepoint;               
        }
    }
}

/***************************************************************/
/*函数名称：jiaoyanzaosheng()                                   */
/*函数类型：void                                                */
/*功能：对图像进行椒盐（脉冲）噪声处理（黑白点）。                  */
/***************************************************************/
void ZaoShengXiaoChuDib::jiaoyanzaosheng()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            if(rand() > 31500)
                p_data[lineBytes * y + x] = 0;               
        }
    }
}

// 计算均值、方差
void Calc(const int (&kernel)[9], float &mean, float &var, int num)
{
    var = 0.0f;
    mean = 0.0f;

    if (num == 0)
        return;

    int sum = 0;
    for (int i = 0; i < 9; i++) {
        if (kernel[i] == 0)
            continue;
        sum += kernel[i];
    }

    mean = (float)sum / num; // 均值

    int num2 = 9 - num;
    for (int i = 0; i < 9; i++) {
        if (kernel[i] == 0 && num2-- > 0)
            continue;
        //var += (kernel[i] - mean) * (kernel[i] - mean);
        var += kernel[i] * kernel[i] - mean * mean;
    }

    //var /= num * num; // 方差
}

/***************************************************************/ 
/*函数名称：jubupingjun()                                      */
/*函数类型：void                                               */
/*功能：对图像进行局部平均化处理（选择式掩模平滑、自适应平滑）。    */
/**************************************************************/
void ZaoShengXiaoChuDib::jubupingjun()
{
    // 1.垂直边界方向的方差最大
    // 2.尖锐边缘比平缓区域方差大
    // 3.完成滤波的同时，不破坏区域边界的细节
    //
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.二值化
    //GuDing(100);

    // 2.像素缓存 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.自适应滤波
    int kernel[9]; // 分量
    float mean[9] = {}; // 均值
    float var[9] = {}; // 方差

    int board = 2; // 5*5的掩模
    if (GetRGB()) {
        for (int y = board; y < height - board; y++) {
            for (int x = board; x < width - board; x++) {
                // 求9种近邻区域的均值及其方差

                // a.周围8邻近
                kernel[0] = p_data[lineBytes * (y - 1) + (x - 1)];
                kernel[1] = p_data[lineBytes * (y - 1) + (x)];
                kernel[2] = p_data[lineBytes * (y - 1) + (x + 1)];
                kernel[3] = p_data[lineBytes * (y) + (x - 1)];
                kernel[4] = p_data[lineBytes * (y) + (x)];
                kernel[5] = p_data[lineBytes * (y) + (x + 1)];
                kernel[6] = p_data[lineBytes * (y + 1) + (x - 1)];
                kernel[7] = p_data[lineBytes * (y + 1) + (x)];
                kernel[8] = p_data[lineBytes * (y + 1) + (x + 1)];
                Calc(kernel, mean[0], var[0], 9);

                // b.左7邻近
                kernel[0] = p_data[lineBytes * (y - 1) + (x - 2)];
                kernel[1] = p_data[lineBytes * (y - 1) + (x - 1)];
                kernel[2] = 0;
                kernel[3] = p_data[lineBytes * (y) + (x - 2)];
                kernel[4] = p_data[lineBytes * (y) + (x - 1)];
                kernel[5] = p_data[lineBytes * (y) + (x)];
                kernel[6] = p_data[lineBytes * (y + 1) + (x - 2)];
                kernel[7] = p_data[lineBytes * (y + 1) + (x - 1)];
                kernel[8] = 0;
                Calc(kernel, mean[1], var[1], 7);


                // c.上7邻近
                kernel[0] = p_data[lineBytes * (y - 2) + (x - 1)];
                kernel[1] = p_data[lineBytes * (y - 2) + (x)];
                kernel[2] = p_data[lineBytes * (y - 2) + (x + 1)];
                kernel[3] = p_data[lineBytes * (y - 1) + (x - 1)];
                kernel[4] = p_data[lineBytes * (y - 1) + (x)];
                kernel[5] = p_data[lineBytes * (y - 1) + (x + 1)];
                kernel[6] = 0;
                kernel[7] = p_data[lineBytes * (y) + (x)];
                kernel[8] = 0;
                Calc(kernel, mean[2], var[2], 7);

                // d.右7邻近
                kernel[0] = 0;
                kernel[1] = p_data[lineBytes * (y - 1) + (x + 1)];
                kernel[2] = p_data[lineBytes * (y - 1) + (x + 2)];
                kernel[3] = p_data[lineBytes * (y) + (x)];
                kernel[4] = p_data[lineBytes * (y) + (x + 1)];
                kernel[5] = p_data[lineBytes * (y) + (x + 2)];
                kernel[6] = 0;
                kernel[7] = p_data[lineBytes * (y + 1) + (x + 1)];
                kernel[8] = p_data[lineBytes * (y + 1) + (x + 2)];
                Calc(kernel, mean[3], var[3], 7);

                // e.下7邻近
                kernel[0] = 0;
                kernel[1] = p_data[lineBytes * (y) + (x)];
                kernel[2] = 0;
                kernel[3] = p_data[lineBytes * (y + 1) + (x - 1)];
                kernel[4] = p_data[lineBytes * (y + 1) + (x)];
                kernel[5] = p_data[lineBytes * (y + 1) + (x + 1)];
                kernel[6] = p_data[lineBytes * (y + 2) + (x - 1)];
                kernel[7] = p_data[lineBytes * (y + 2) + (x)];
                kernel[8] = p_data[lineBytes * (y + 2) + (x + 1)];
                Calc(kernel, mean[4], var[4], 7);

                // f.左上7邻近
                kernel[0] = p_data[lineBytes * (y - 2) + (x - 2)];
                kernel[1] = p_data[lineBytes * (y - 2) + (x - 1)];
                kernel[2] = 0;
                kernel[3] = p_data[lineBytes * (y - 1) + (x - 2)];
                kernel[4] = p_data[lineBytes * (y - 1) + (x - 1)];
                kernel[5] = p_data[lineBytes * (y - 1) + (x)];
                kernel[6] = 0;
                kernel[7] = p_data[lineBytes * (y) + (x - 1)];
                kernel[8] = p_data[lineBytes * (y) + (x)];
                Calc(kernel, mean[5], var[5], 7);

                // g.右上7邻近
                kernel[0] = 0;
                kernel[1] = p_data[lineBytes * (y - 2) + (x + 1)];
                kernel[2] = p_data[lineBytes * (y - 2) + (x + 2)];
                kernel[3] = p_data[lineBytes * (y - 1) + (x)];
                kernel[4] = p_data[lineBytes * (y - 1) + (x + 1)];
                kernel[5] = p_data[lineBytes * (y - 1) + (x + 2)];
                kernel[6] = p_data[lineBytes * (y) + (x)];
                kernel[7] = p_data[lineBytes * (y) + (x + 1)];
                kernel[8] = 0;
                Calc(kernel, mean[6], var[6], 7);

                // h.右下7邻近
                kernel[0] = p_data[lineBytes * (y) + (x)];
                kernel[1] = p_data[lineBytes * (y) + (x + 1)];
                kernel[2] = 0;
                kernel[3] = p_data[lineBytes * (y + 1) + (x)];
                kernel[4] = p_data[lineBytes * (y + 1) + (x + 1)];
                kernel[5] = p_data[lineBytes * (y + 1) + (x + 2)];
                kernel[6] = 0;
                kernel[7] = p_data[lineBytes * (y + 2) + (x + 1)];
                kernel[8] = p_data[lineBytes * (y + 2) + (x + 2)];
                Calc(kernel, mean[7], var[7], 7);

                // i.左下7邻近
                kernel[0] = 0;
                kernel[1] = p_data[lineBytes * (y) + (x - 1)];
                kernel[2] = p_data[lineBytes * (y) + (x)];
                kernel[3] = p_data[lineBytes * (y + 1) + (x - 2)];
                kernel[4] = p_data[lineBytes * (y + 1) + (x - 1)];
                kernel[5] = p_data[lineBytes * (y + 1) + (x)];
                kernel[6] = p_data[lineBytes * (y + 2) + (x - 2)];
                kernel[7] = p_data[lineBytes * (y + 2) + (x - 1)];
                kernel[8] = 0;
                Calc(kernel, mean[8], var[8], 7);

                // 求方差最小的近邻区域索引
                int minVar = var[0];
                int minIndex = 0;
                for (int i = 1; i < 9; i++) {
                    if (minVar > var[i]) {
                        minVar = var[i];
                        minIndex = i;
                    }
                }

                // 将方差最小的邻域均值，四舍五入后作为处理后图像的像素值
                p_temp[lineBytes * y + x] = (int)(mean[minIndex] + 0.5);         
            }
        }

    } else {
        for (int y = board; y < height - board; y++) {
            for (int x = board; x < width - board; x++) {
                // 求9种近邻区域的均值及其方差
                for(int k = 0; k < 3; k++) {
                    // a.周围8邻近
                    kernel[0] = p_data[lineBytes * (y - 1) + (x - 1) * 3 + k];
                    kernel[1] = p_data[lineBytes * (y - 1) + (x) * 3 + k];
                    kernel[2] = p_data[lineBytes * (y - 1) + (x + 1) * 3 + k];
                    kernel[3] = p_data[lineBytes * (y) + (x - 1)  * 3 + k];
                    kernel[4] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y) + (x + 1) * 3 + k];
                    kernel[6] = p_data[lineBytes * (y + 1) + (x - 1) * 3 + k];
                    kernel[7] = p_data[lineBytes * (y + 1) + (x) * 3 + k];
                    kernel[8] = p_data[lineBytes * (y + 1) + (x + 1) * 3 + k];
                    Calc(kernel, mean[0], var[0], 9);

                    // b.左7邻近
                    kernel[0] = p_data[lineBytes * (y - 1) + (x - 2) * 3 + k];
                    kernel[1] = p_data[lineBytes * (y - 1) + (x - 1) * 3 + k];
                    kernel[2] = 0;
                    kernel[3] = p_data[lineBytes * (y) + (x - 2) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y) + (x - 1) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[6] = p_data[lineBytes * (y + 1) + (x - 2) * 3 + k];
                    kernel[7] = p_data[lineBytes * (y + 1) + (x - 1) * 3 + k];
                    kernel[8] = 0;
                    Calc(kernel, mean[1], var[1], 7);


                    // c.上7邻近
                    kernel[0] = p_data[lineBytes * (y - 2) + (x - 1) * 3 + k];
                    kernel[1] = p_data[lineBytes * (y - 2) + (x) * 3 + k];
                    kernel[2] = p_data[lineBytes * (y - 2) + (x + 1) * 3 + k];
                    kernel[3] = p_data[lineBytes * (y - 1) + (x - 1) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y - 1) + (x) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y - 1) + (x + 1) * 3 + k];
                    kernel[6] = 0;
                    kernel[7] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[8] = 0;
                    Calc(kernel, mean[2], var[2], 7);

                    // d.右7邻近
                    kernel[0] = 0;
                    kernel[1] = p_data[lineBytes * (y - 1) + (x + 1) * 3 + k];
                    kernel[2] = p_data[lineBytes * (y - 1) + (x + 2) * 3 + k];
                    kernel[3] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y) + (x + 1) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y) + (x + 2) * 3 + k];
                    kernel[6] = 0;
                    kernel[7] = p_data[lineBytes * (y + 1) + (x + 1) * 3 + k];
                    kernel[8] = p_data[lineBytes * (y + 1) + (x + 2) * 3 + k];
                    Calc(kernel, mean[3], var[3], 7);

                    // e.下7邻近
                    kernel[0] = 0;
                    kernel[1] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[2] = 0;
                    kernel[3] = p_data[lineBytes * (y + 1) + (x - 1) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y + 1) + (x) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y + 1) + (x + 1) * 3 + k];
                    kernel[6] = p_data[lineBytes * (y + 2) + (x - 1) * 3 + k];
                    kernel[7] = p_data[lineBytes * (y + 2) + (x) * 3 + k];
                    kernel[8] = p_data[lineBytes * (y + 2) + (x + 1) * 3 + k];
                    Calc(kernel, mean[4], var[4], 7);

                    // f.左上7邻近
                    kernel[0] = p_data[lineBytes * (y - 2) + (x - 2) * 3 + k];
                    kernel[1] = p_data[lineBytes * (y - 2) + (x - 1) * 3 + k];
                    kernel[2] = 0;
                    kernel[3] = p_data[lineBytes * (y - 1) + (x - 2) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y - 1) + (x - 1) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y - 1) + (x) * 3 + k];
                    kernel[6] = 0;
                    kernel[7] = p_data[lineBytes * (y) + (x - 1) * 3 + k];
                    kernel[8] = p_data[lineBytes * (y) + (x) * 3 + k];
                    Calc(kernel, mean[5], var[5], 7);

                    // g.右上7邻近
                    kernel[0] = 0;
                    kernel[1] = p_data[lineBytes * (y - 2) + (x + 1) * 3 + k];
                    kernel[2] = p_data[lineBytes * (y - 2) + (x + 2) * 3 + k];
                    kernel[3] = p_data[lineBytes * (y - 1) + (x) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y - 1) + (x + 1) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y - 1) + (x + 2) * 3 + k];
                    kernel[6] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[7] = p_data[lineBytes * (y) + (x + 1) * 3 + k];
                    kernel[8] = 0;
                    Calc(kernel, mean[6], var[6], 7);

                    // h.右下7邻近
                    kernel[0] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[1] = p_data[lineBytes * (y) + (x + 1) * 3 + k];
                    kernel[2] = 0;
                    kernel[3] = p_data[lineBytes * (y + 1) + (x) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y + 1) + (x + 1) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y + 1) + (x + 2) * 3 + k];
                    kernel[6] = 0;
                    kernel[7] = p_data[lineBytes * (y + 2) + (x + 1) * 3 + k];
                    kernel[8] = p_data[lineBytes * (y + 2) + (x + 2) * 3 + k];
                    Calc(kernel, mean[7], var[7], 7);

                    // i.左下7邻近
                    kernel[0] = 0;
                    kernel[1] = p_data[lineBytes * (y) + (x - 1) * 3 + k];
                    kernel[2] = p_data[lineBytes * (y) + (x) * 3 + k];
                    kernel[3] = p_data[lineBytes * (y + 1) + (x - 2) * 3 + k];
                    kernel[4] = p_data[lineBytes * (y + 1) + (x - 1) * 3 + k];
                    kernel[5] = p_data[lineBytes * (y + 1) + (x) * 3 + k];
                    kernel[6] = p_data[lineBytes * (y + 2) + (x - 2) * 3 + k];
                    kernel[7] = p_data[lineBytes * (y + 2) + (x - 1) * 3 + k];
                    kernel[8] = 0;
                    Calc(kernel, mean[8], var[8], 7);

                    // 求方差最小的近邻区域索引
                    int minVar = var[0];
                    int minIndex = 0;
                    for (int i = 1; i < 9; i++) {
                        if (minVar > var[i]) {
                            minVar = var[i];
                            minIndex = i;
                        }
                    }

                    // 将方差最小的邻域均值，四舍五入后作为处理后图像的像素值
                    p_temp[lineBytes * y + x * 3 + k] = (int)(mean[minIndex] + 0.5);  
                }
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}
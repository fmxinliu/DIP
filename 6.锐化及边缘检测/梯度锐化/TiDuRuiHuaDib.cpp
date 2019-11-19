#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "TiDuRuiHuaDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
#include "CDIB.h"
 
TiDuRuiHuaDib::TiDuRuiHuaDib()
{

}
TiDuRuiHuaDib::~TiDuRuiHuaDib()
{

}

///***************************************************************/           
/*函数名称：Erzhirihua()                                      
/*函数类型：void                                   
/*功能： 根据梯度二值化图像。            
/***************************************************************/ 
void TiDuRuiHuaDib::Erzhirihua(int T)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = 1; y < height; y++) {
        for (int x = border; x < lineBytes; x++) {
            BYTE deltaY = abs(p_data[lineBytes * y + x] - p_data[lineBytes * (y - 1) + x]);
            BYTE deltaX = abs(p_data[lineBytes * y + x] - p_data[lineBytes * y + (x - border)]);
            BYTE gxy = (deltaX + deltaY > 255) ? 255 : deltaX + deltaY;
            if (gxy >= T) {
                p_temp[lineBytes * y + x] = 255;
            } else {
                p_temp[lineBytes * y + x] = 0;
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*函数名称：GuDingRuiHua(int T, bool erase)                                       
/*函数类型：void   
/*函数参数：int T 门限值, bool erase 去掉细节
/*功能：给边缘规定一个特定的灰度级。            
/***************************************************************/ 
void TiDuRuiHuaDib::GuDingRuiHua(int T, bool erase)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = 1; y < height; y++) {
        for (int x = border; x < lineBytes; x++) {
            BYTE deltaY = abs(p_data[lineBytes * y + x] - p_data[lineBytes * (y - 1) + x]);
            BYTE deltaX = abs(p_data[lineBytes * y + x] - p_data[lineBytes * y + (x - border)]);
            BYTE gxy = (deltaX + deltaY > 255) ? 255 : deltaX + deltaY;
            
            if (erase) {
                if (gxy >= T)
                    p_temp[lineBytes * y + x] = p_data[lineBytes * y + x]; // 保留边界
                else
                    p_temp[lineBytes * y + x] = 0; // 去掉细节
            } else {
                if (gxy >= T)
                    p_temp[lineBytes * y + x] = 255; // 增强边界
                else
                    p_temp[lineBytes * y + x] = p_data[lineBytes * y + x]; // 保留细节
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Menxianruihua(int T)                                        
/*函数类型：void
/*函数参数：int T 门限值
/*功能：对图像进行门限梯度锐化。            
/*****************************************************************/ 
void TiDuRuiHuaDib::Menxianruihua(int T)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = 1; y < height; y++) {
        for (int x = border; x < lineBytes; x++) {
            BYTE deltaY = abs(p_data[lineBytes * y + x] - p_data[lineBytes * (y - 1) + x]);
            BYTE deltaX = abs(p_data[lineBytes * y + x] - p_data[lineBytes * y + (x - border)]);
            BYTE gxy = (deltaX + deltaY > 255) ? 255 : deltaX + deltaY;
            if (gxy >= T) {
                p_temp[lineBytes * y + x] = (gxy + 100 > 255) ? 255 : gxy + 100; // 增强边界
            } else {
                p_temp[lineBytes * y + x] = p_data[lineBytes * y + x]; // 保留细节
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*函数名称：TiduzhiDaiti()                                       
/*函数类型：void                                   
/*功能：以梯度值代替灰度（双向一次微分）。            
/***************************************************************/ 
void TiDuRuiHuaDib::TiduzhiDaiti()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = 1; y < height; y++) {
        for (int x = border; x < lineBytes; x++) {
            BYTE deltaY = abs(p_data[lineBytes * y + x] - p_data[lineBytes * (y - 1) + x]);
            BYTE deltaX = abs(p_data[lineBytes * y + x] - p_data[lineBytes * y + (x - border)]);
            //p_temp[lineBytes * y + x] = (int)sqrt((float)(deltaX * deltaX + deltaY * deltaY));
            p_temp[lineBytes * y + x] = (deltaX + deltaY > 255) ? 255 : deltaX + deltaY;
            //p_temp[lineBytes * y + x] = max(deltaX, deltaY);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}


// 彩色图像灰度化
void TiDuRuiHuaDib::RgbToGray()
{
    if (GetRGB())
        return;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE b = p_data[lineBytes * y + x * 3];
            BYTE g = p_data[lineBytes * y + x * 3 + 1];
            BYTE r = p_data[lineBytes * y + x * 3 + 2];
           
            BYTE gray = (BYTE)(0.11 * r + 0.59 * g + 0.3 * b); // RGB -> gray
           
            p_data[lineBytes * y + x * 3] = gray;
            p_data[lineBytes * y + x * 3 + 1] = gray;
            p_data[lineBytes * y + x * 3 + 2] = gray;
        }
    }
}

// 快速彩色图像灰度化
void TiDuRuiHuaDib::RgbToGrayFast()
{
    if (GetRGB())
        return;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE b = p_data[lineBytes * y + x * 3];
            BYTE g = p_data[lineBytes * y + x * 3 + 1];
            BYTE r = p_data[lineBytes * y + x * 3 + 2];

            BYTE gray = max(r, g, b); // RGB -> gray

            p_data[lineBytes * y + x * 3] = gray;
            p_data[lineBytes * y + x * 3 + 1] = gray;
            p_data[lineBytes * y + x * 3 + 2] = gray;
        }
    }
}

// 计算平均灰度
BYTE TiDuRuiHuaDib::AvgGray()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    
    LPBYTE p_data = GetData();

    long sum = 0;
    long num = 0;

    if (GetRGB()) {
        // 统计灰度图像的灰度
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {   
                sum += p_data[lineBytes * y + x];
                num++;
            }
        }
    }
    else
    {
        // 统计彩色图像的灰度
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE b = p_data[lineBytes * y + x * 3];
                BYTE g = p_data[lineBytes * y + x * 3 + 1];
                BYTE r = p_data[lineBytes * y + x * 3 + 2];

                BYTE gray = (BYTE)(0.11 * r + 0.59 * g + 0.3 * b);
                sum += gray;
                num++;
            }
        }
    }

    return (BYTE)(sum / num); // 平均灰度
}


// 二值化
void TiDuRuiHuaDib::Threshold()
{
    // 求平均灰度
    BYTE averageGray = AvgGray();

    if (!GetRGB()) {
        RgbToGray(); // 彩色转灰度
        ASSERT(AvgGray() == averageGray); // 彩色图像计算平均灰度 == 先灰度化后计算
    }
    
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_data = GetData();
    
    // 用平均灰度做阈值进行二值化
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) { 
            if (p_data[lineBytes * y + x] > averageGray)
                p_data[lineBytes * y + x] = 255;
            else
                p_data[lineBytes * y + x] = 0;
        }
    }
}
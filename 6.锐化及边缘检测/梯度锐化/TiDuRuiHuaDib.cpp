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
/*�������ƣ�Erzhirihua()                                      
/*�������ͣ�void                                   
/*���ܣ� �����ݶȶ�ֵ��ͼ��            
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
/*�������ƣ�GuDingRuiHua(int T, bool erase)                                       
/*�������ͣ�void   
/*����������int T ����ֵ, bool erase ȥ��ϸ��
/*���ܣ�����Ե�涨һ���ض��ĻҶȼ���            
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
                    p_temp[lineBytes * y + x] = p_data[lineBytes * y + x]; // �����߽�
                else
                    p_temp[lineBytes * y + x] = 0; // ȥ��ϸ��
            } else {
                if (gxy >= T)
                    p_temp[lineBytes * y + x] = 255; // ��ǿ�߽�
                else
                    p_temp[lineBytes * y + x] = p_data[lineBytes * y + x]; // ����ϸ��
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*�������ƣ�Menxianruihua(int T)                                        
/*�������ͣ�void
/*����������int T ����ֵ
/*���ܣ���ͼ����������ݶ��񻯡�            
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
                p_temp[lineBytes * y + x] = (gxy + 100 > 255) ? 255 : gxy + 100; // ��ǿ�߽�
            } else {
                p_temp[lineBytes * y + x] = p_data[lineBytes * y + x]; // ����ϸ��
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*�������ƣ�TiduzhiDaiti()                                       
/*�������ͣ�void                                   
/*���ܣ����ݶ�ֵ����Ҷȣ�˫��һ��΢�֣���            
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


// ��ɫͼ��ҶȻ�
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

// ���ٲ�ɫͼ��ҶȻ�
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

// ����ƽ���Ҷ�
BYTE TiDuRuiHuaDib::AvgGray()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    
    LPBYTE p_data = GetData();

    long sum = 0;
    long num = 0;

    if (GetRGB()) {
        // ͳ�ƻҶ�ͼ��ĻҶ�
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {   
                sum += p_data[lineBytes * y + x];
                num++;
            }
        }
    }
    else
    {
        // ͳ�Ʋ�ɫͼ��ĻҶ�
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

    return (BYTE)(sum / num); // ƽ���Ҷ�
}


// ��ֵ��
void TiDuRuiHuaDib::Threshold()
{
    // ��ƽ���Ҷ�
    BYTE averageGray = AvgGray();

    if (!GetRGB()) {
        RgbToGray(); // ��ɫת�Ҷ�
        ASSERT(AvgGray() == averageGray); // ��ɫͼ�����ƽ���Ҷ� == �ȻҶȻ������
    }
    
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_data = GetData();
    
    // ��ƽ���Ҷ�����ֵ���ж�ֵ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) { 
            if (p_data[lineBytes * y + x] > averageGray)
                p_data[lineBytes * y + x] = 255;
            else
                p_data[lineBytes * y + x] = 0;
        }
    }
}
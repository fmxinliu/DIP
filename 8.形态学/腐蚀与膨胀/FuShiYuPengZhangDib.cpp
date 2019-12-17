#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "FuShiYuPengZhangDib.h"
#include "MainFrm.h"


 
 
FuShiYuPengZhangDib::FuShiYuPengZhangDib()
{
}
FuShiYuPengZhangDib::~FuShiYuPengZhangDib()
{
}


 

///***************************************************************/           
/*函数名称：ChuiZhiFuShi()                                        
/*函数类型：void                                      
/*功能：对图像进行垂直腐蚀。            
/***************************************************************/ 
void FuShiYuPengZhangDib::ChuiZhiFuShi()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = GetDibWidthBytes();   // 取得原图的每行字节数(4字节对齐)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // 指向DIB像素指针
    LPBYTE p_temp = new BYTE[size]; // 指向缓存DIB图像的指针
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // 使用垂直方向的结构元素进行腐蚀
        for (int y = 1; y < height - 1; y++) { // 由于使用3×1的结构元素，为防止越界，所以不处理最上边和最下边的两列像素
            for (int x = 0; x < width; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 0;
                for (int j = -1; j <= 1; j++) {  // 如果源图像中(0,-1)、(0,0)、(0,1)三个点之一有白点，则将目标图像中的(0,0)点赋成白色
                    if (p_data[dibWidth * (y - j) + x] > 128) {
                        p_temp[offset] = 255;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 1; y < height - 1; y++) {
            //// 方法1:按行像素处理
            //for (int x = 0; x < dibWidth; x++) {
            //    LONG offset = dibWidth * y + x;
            //    p_temp[offset] = p_data[offset];
            //    for (int j = -1; j <= 1; j++) {
            //        if (p_data[dibWidth * (y - j) + x] > 128) {
            //            p_temp[offset] = 255;
            //            break;
            //        }
            //    }
            //}

            // 方法2:按DIB像素宽度处理
            for (int x = 0; x < width; x++) {
                LONG offset = dibWidth * y + x * 3;
                p_temp[offset] = p_data[offset];
                p_temp[offset + 1] = p_data[offset + 1];
                p_temp[offset + 2] = p_data[offset + 2];
                for (int k = 0; k < 3; k++) {
                    for (int j = -1; j <= 1; j++) {
                        if (p_data[dibWidth * (y - j) + x * 3 + k] > 128) {
                            p_temp[offset + k] = 255;
                            break;
                        }
                    }
                }
            }

            //// 方法3:按bgr处理
            //for (int x = 3; x < width * 3 - 3; x += 3) {
            //    LONG offset = dibWidth * y + x;
            //    p_temp[offset] = p_data[offset];
            //    p_temp[offset + 1] = p_data[offset + 1];
            //    p_temp[offset + 2] = p_data[offset + 2];
            //    for (int k = 0; k < 3; k++) {
            //        for (int j = -1; j <= 1; j++) {
            //            if (p_data[dibWidth * (y - j) + x + k] > 128) {
            //                p_temp[offset + k] = 255;
            //                break;
            //            }
            //        }
            //    }
            //}
        }
    }

    memcpy(p_data, p_temp, size); 
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Shuipingfushi()                                        
/*函数类型：void                                      
/*功能：对图像进行水平腐蚀。            
/***************************************************************/ 
void FuShiYuPengZhangDib::Shuipingfushi()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = GetDibWidthBytes();   // 取得原图的每行字节数(4字节对齐)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // 指向DIB像素指针
    LPBYTE p_temp = new BYTE[size]; // 指向缓存DIB图像的指针
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // 使用水平方向的结构元素进行腐蚀
        for (int y = 0; y < height; y++) {
            for (int x = 1; x < width - 1; x++) { // 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 0;
                for (int i = -1; i <= 1; i++) {  // 如果源图像中(-1,0)、(0,0)、(1,0)三个点之一有白点，则将目标图像中的(0,0)点赋成白色
                    if (p_data[offset + i] > 128) {
                        p_temp[offset] = 255;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            //// 方法1:按行像素处理
            //for (int x = 3; x < dibWidth - 3; x++) {
            //    LONG offset = dibWidth * y + x;
            //    p_temp[offset] = p_data[offset];
            //    for (int i = -1; i <= 1; i++) {
            //        if (p_data[offset + i * 3] > 128) {
            //            p_temp[offset] = 255;
            //            break;
            //        }
            //    }
            //}

            //// 方法2:按DIB像素宽度处理
            //for (int x = 1; x < width - 1; x++) {
            //    LONG offset = dibWidth * y + x * 3;
            //    p_temp[offset] = p_data[offset];
            //    p_temp[offset + 1] = p_data[offset + 1];
            //    p_temp[offset + 2] = p_data[offset + 2];
            //    for (int j = 0; j < 3; j++) {
            //        for (int i = -1; i <= 1; i++) {
            //            if (p_data[offset + j + i * 3] > 128) {
            //                p_temp[offset + j] = 255;
            //                break;
            //            }
            //        }
            //    }
            //}

            // 方法3:按bgr处理
            for (int x = 3; x < width * 3 - 3; x += 3) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = p_data[offset];
                p_temp[offset + 1] = p_data[offset + 1];
                p_temp[offset + 2] = p_data[offset + 2];
                for (int j = 0; j < 3; j++) {
                    for (int i = -1; i <= 1; i++) {
                        if (p_data[offset + j + i * 3] > 128) {
                            p_temp[offset + j] = 255;
                            break;
                        }
                    }
                }
            }
        }
    }

    memcpy(p_data, p_temp, size); 
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Quanfangxiangfushi()                                        
/*函数类型：void                                      
/*功能：对图像进行全方向腐蚀。            
/***************************************************************/
void FuShiYuPengZhangDib::Quanfangxiangfushi()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = GetDibWidthBytes();   // 取得原图的每行字节数(4字节对齐)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // 指向DIB像素指针
    LPBYTE p_temp = new BYTE[size]; // 指向缓存DIB图像的指针
    memset(p_temp, 255, size);

    // 3×3的结构元素
    int B[9] = {1, 0, 1,
                0, 0, 0,
                1, 0, 1};

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // 使用全方向的结构元素进行腐蚀
        for (int y = 1; y < height - 1; y++) { // 由于使用3×3的结构元素，为防止越界，所以不处理最左、右、上、下四边的像素
            for (int x = 1; x < width - 1; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 0; // 当前点先赋黑
                
                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // 如果源图像中3×3结构元素对应位置有白点，则将目标图像中的(0,0)点赋成白色
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // 忽略白点(背景)
                            continue;

                        if (p_data[dibWidth * (y - j) + (x + i)] > 128) {
                            p_temp[offset] = 255;
                            stop = true;
                            break;
                        }
                    }
                }
            }
        }
    } else {
        for (int y = 1; y < height - 1; y++) {
            // 方法1:按行像素处理
            for (int x = 3; x < dibWidth - 3; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = p_data[offset];
                
                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // 如果源图像中3×3结构元素对应位置有白点，则将目标图像中的(0,0)点赋成白色
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // 忽略白点(背景)
                            continue;
                        
                        if (p_data[dibWidth * (y - j) + (x + i * 3)] > 128) {
                            p_temp[offset] = 255;
                            stop = true;
                            break;
                        }
                    }
                }
            }

            //// 方法2:按DIB像素宽度处理
            //for (int x = 1; x < width - 1; x++) {
            //    LONG offset = dibWidth * y + x * 3;
            //    p_temp[offset] = p_data[offset];
            //    p_temp[offset + 1] = p_data[offset + 1];
            //    p_temp[offset + 2] = p_data[offset + 2];

            //    for (int k = 0; k < 3; k++) {
            //        bool stop = false;
            //        for (int j = -1; j <= 1 && !stop; j++) {
            //            for (int i = -1; i <= 1; i++) {
            //                if (B[i + j] == 1)
            //                    continue;

            //                if (p_data[dibWidth * (y - j) + (x + i) * 3 + k] > 128) {
            //                    p_temp[offset + k] = 255;
            //                    stop = true;
            //                    break;
            //                }
            //            }
            //        }
            //    }
            //}

            //// 方法3:按bgr处理
            //for (int x = 3; x < width * 3 - 3; x += 3) {
            //    LONG offset = dibWidth * y + x;
            //    p_temp[offset] = p_data[offset];
            //    p_temp[offset + 1] = p_data[offset + 1];
            //    p_temp[offset + 2] = p_data[offset + 2];
            //
            //    for (int k = 0; k < 3; k++) {
            //        bool stop = false;
            //        for (int j = -1; j <= 1 && !stop; j++) {
            //            for (int i = -1; i <= 1; i++) {
            //                if (B[i + j] == 1)
            //                    continue;

            //                if (p_data[dibWidth * (y - j) + (x + i * 3) + k] > 128) {
            //                    p_temp[offset + k] = 255;
            //                    stop = true;
            //                    break;
            //                }
            //            }
            //        }
            //    }
            //}
        }
    }

    memcpy(p_data, p_temp, size); 
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Shuipingpengzhang()                                        
/*函数类型：void                                      
/*功能：对图像进行水平膨胀。            
/***************************************************************/
void FuShiYuPengZhangDib::Shuipingpengzhang()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = GetDibWidthBytes();   // 取得原图的每行字节数(4字节对齐)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // 指向DIB像素指针
    LPBYTE p_temp = new BYTE[size]; // 指向缓存DIB图像的指针
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // 使用水平方向的结构元素进行膨胀
        for (int y = 0; y < height; y++) {
            for (int x = 1; x < width - 1; x++) { // 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 255;
                for (int i = -1; i <= 1; i++) {  // 如果源图像中(-1,0)、(0,0)、(1,0)三个点之一有黑点，则将目标图像中的(0,0)点赋成黑色
                    if (p_data[offset + i] < 128) {
                        p_temp[offset] = 0;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            // 方法1:按行像素处理
            for (int x = 3; x < dibWidth - 3; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = p_data[offset];
                for (int i = -1; i <= 1; i++) {
                    if (p_data[offset + i * 3] < 128) {
                        p_temp[offset] = 0;
                        break;
                    }
                }
            }
        }
    }

    memcpy(p_data, p_temp, size); 
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Chuizhipengzhang()                                        
/*函数类型：void                                      
/*功能：对图像进行垂直膨胀。            
/***************************************************************/
void FuShiYuPengZhangDib::Chuizhipengzhang()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = GetDibWidthBytes();   // 取得原图的每行字节数(4字节对齐)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // 指向DIB像素指针
    LPBYTE p_temp = new BYTE[size]; // 指向缓存DIB图像的指针
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // 使用垂直方向的结构元素进行膨胀
        for (int y = 1; y < height - 1; y++) { // 由于使用3×1的结构元素，为防止越界，所以不处理最上边和最下边的两列像素
            for (int x = 0; x < width; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 255;
                for (int j = -1; j <= 1; j++) {  // 如果源图像中(0,-1)、(0,0)、(0,1)三个点之一有黑点，则将目标图像中的(0,0)点赋成黑色
                    if (p_data[dibWidth * (y - j) + x] < 128) {
                        p_temp[offset] = 0;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 1; y < height - 1; y++) {
            // 方法1:按行像素处理
            for (int x = 0; x < dibWidth; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = p_data[offset];
                for (int j = -1; j <= 1; j++) {
                    if (p_data[dibWidth * (y - j) + x] < 128) {
                        p_temp[offset] = 0;
                        break;
                    }
                }
            }
        }
    }

    memcpy(p_data, p_temp, size); 
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Quanfangxiangpengzhang()                                        
/*函数类型：void                                      
/*功能：对图像进行全方向膨胀。            
/***************************************************************/
void FuShiYuPengZhangDib::Quanfangxiangpengzhang()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = GetDibWidthBytes();   // 取得原图的每行字节数(4字节对齐)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // 指向DIB像素指针
    LPBYTE p_temp = new BYTE[size]; // 指向缓存DIB图像的指针
    memset(p_temp, 255, size);

    // 3×3的结构元素
    int B[9] = {1, 0, 1,
        0, 0, 0,
        1, 0, 1};

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // 使用全方向的结构元素进行膨胀
        for (int y = 1; y < height - 1; y++) { // 由于使用3×3的结构元素，为防止越界，所以不处理最左、右、上、下四边的像素
            for (int x = 1; x < width - 1; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 255; // 当前点先赋白
                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // 如果源图像中3×3结构元素对应位置有黑点，则将目标图像中的(0,0)点赋成黑色
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // 忽略白点(背景)
                            continue;

                        if (p_data[dibWidth * (y - j) + (x + i)] < 128) {
                            p_temp[offset] = 0;
                            stop = true;
                            break;
                        }
                    }
                }
            }
        }
    } else {
        for (int y = 1; y < height - 1; y++) {
            // 方法1:按行像素处理
            for (int x = 3; x < dibWidth - 3; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = p_data[offset];

                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // 如果源图像中3×3结构元素对应位置有黑点，则将目标图像中的(0,0)点赋成黑色
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // 忽略白点(背景)
                            continue;

                        if (p_data[dibWidth * (y - j) + (x + i * 3)] < 128) {
                            p_temp[offset] = 0;
                            stop = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    memcpy(p_data, p_temp, size); 
    delete p_temp;
}

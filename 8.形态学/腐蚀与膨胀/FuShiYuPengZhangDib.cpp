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
/*�������ƣ�ChuiZhiFuShi()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ����д�ֱ��ʴ��            
/***************************************************************/ 
void FuShiYuPengZhangDib::ChuiZhiFuShi()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    LPBYTE p_temp = new BYTE[size]; // ָ�򻺴�DIBͼ���ָ��
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
        for (int y = 1; y < height - 1; y++) { // ����ʹ��3��1�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������
            for (int x = 0; x < width; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 0;
                for (int j = -1; j <= 1; j++) {  // ���Դͼ����(0,-1)��(0,0)��(0,1)������֮һ�а׵㣬��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ
                    if (p_data[dibWidth * (y - j) + x] > 128) {
                        p_temp[offset] = 255;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 1; y < height - 1; y++) {
            //// ����1:�������ش���
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

            // ����2:��DIB���ؿ�ȴ���
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

            //// ����3:��bgr����
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
/*�������ƣ�Shuipingfushi()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ�����ˮƽ��ʴ��            
/***************************************************************/ 
void FuShiYuPengZhangDib::Shuipingfushi()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    LPBYTE p_temp = new BYTE[size]; // ָ�򻺴�DIBͼ���ָ��
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
        for (int y = 0; y < height; y++) {
            for (int x = 1; x < width - 1; x++) { // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 0;
                for (int i = -1; i <= 1; i++) {  // ���Դͼ����(-1,0)��(0,0)��(1,0)������֮һ�а׵㣬��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ
                    if (p_data[offset + i] > 128) {
                        p_temp[offset] = 255;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            //// ����1:�������ش���
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

            //// ����2:��DIB���ؿ�ȴ���
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

            // ����3:��bgr����
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
/*�������ƣ�Quanfangxiangfushi()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ�����ȫ����ʴ��            
/***************************************************************/
void FuShiYuPengZhangDib::Quanfangxiangfushi()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    LPBYTE p_temp = new BYTE[size]; // ָ�򻺴�DIBͼ���ָ��
    memset(p_temp, 255, size);

    // 3��3�ĽṹԪ��
    int B[9] = {1, 0, 1,
                0, 0, 0,
                1, 0, 1};

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // ʹ��ȫ����ĽṹԪ�ؽ��и�ʴ
        for (int y = 1; y < height - 1; y++) { // ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ����������ҡ��ϡ����ıߵ�����
            for (int x = 1; x < width - 1; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 0; // ��ǰ���ȸ���
                
                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // ���Դͼ����3��3�ṹԪ�ض�Ӧλ���а׵㣬��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // ���԰׵�(����)
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
            // ����1:�������ش���
            for (int x = 3; x < dibWidth - 3; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = p_data[offset];
                
                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // ���Դͼ����3��3�ṹԪ�ض�Ӧλ���а׵㣬��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // ���԰׵�(����)
                            continue;
                        
                        if (p_data[dibWidth * (y - j) + (x + i * 3)] > 128) {
                            p_temp[offset] = 255;
                            stop = true;
                            break;
                        }
                    }
                }
            }

            //// ����2:��DIB���ؿ�ȴ���
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

            //// ����3:��bgr����
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
/*�������ƣ�Shuipingpengzhang()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ�����ˮƽ���͡�            
/***************************************************************/
void FuShiYuPengZhangDib::Shuipingpengzhang()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    LPBYTE p_temp = new BYTE[size]; // ָ�򻺴�DIBͼ���ָ��
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // ʹ��ˮƽ����ĽṹԪ�ؽ�������
        for (int y = 0; y < height; y++) {
            for (int x = 1; x < width - 1; x++) { // ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 255;
                for (int i = -1; i <= 1; i++) {  // ���Դͼ����(-1,0)��(0,0)��(1,0)������֮һ�кڵ㣬��Ŀ��ͼ���е�(0,0)�㸳�ɺ�ɫ
                    if (p_data[offset + i] < 128) {
                        p_temp[offset] = 0;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            // ����1:�������ش���
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
/*�������ƣ�Chuizhipengzhang()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ����д�ֱ���͡�            
/***************************************************************/
void FuShiYuPengZhangDib::Chuizhipengzhang()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    LPBYTE p_temp = new BYTE[size]; // ָ�򻺴�DIBͼ���ָ��
    memset(p_temp, 255, size);

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // ʹ�ô�ֱ����ĽṹԪ�ؽ�������
        for (int y = 1; y < height - 1; y++) { // ����ʹ��3��1�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������
            for (int x = 0; x < width; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 255;
                for (int j = -1; j <= 1; j++) {  // ���Դͼ����(0,-1)��(0,0)��(0,1)������֮һ�кڵ㣬��Ŀ��ͼ���е�(0,0)�㸳�ɺ�ɫ
                    if (p_data[dibWidth * (y - j) + x] < 128) {
                        p_temp[offset] = 0;
                        break;
                    }
                }
            }
        }
    } else {
        for (int y = 1; y < height - 1; y++) {
            // ����1:�������ش���
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
/*�������ƣ�Quanfangxiangpengzhang()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ�����ȫ�������͡�            
/***************************************************************/
void FuShiYuPengZhangDib::Quanfangxiangpengzhang()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    LPBYTE p_temp = new BYTE[size]; // ָ�򻺴�DIBͼ���ָ��
    memset(p_temp, 255, size);

    // 3��3�ĽṹԪ��
    int B[9] = {1, 0, 1,
        0, 0, 0,
        1, 0, 1};

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        // ʹ��ȫ����ĽṹԪ�ؽ�������
        for (int y = 1; y < height - 1; y++) { // ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ����������ҡ��ϡ����ıߵ�����
            for (int x = 1; x < width - 1; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = 255; // ��ǰ���ȸ���
                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // ���Դͼ����3��3�ṹԪ�ض�Ӧλ���кڵ㣬��Ŀ��ͼ���е�(0,0)�㸳�ɺ�ɫ
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // ���԰׵�(����)
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
            // ����1:�������ش���
            for (int x = 3; x < dibWidth - 3; x++) {
                LONG offset = dibWidth * y + x;
                p_temp[offset] = p_data[offset];

                bool stop = false;
                for (int j = -1; j <= 1 && !stop; j++) { // ���Դͼ����3��3�ṹԪ�ض�Ӧλ���кڵ㣬��Ŀ��ͼ���е�(0,0)�㸳�ɺ�ɫ
                    for (int i = -1; i <= 1; i++) {
                        if (B[i + j] == 1) // ���԰׵�(����)
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

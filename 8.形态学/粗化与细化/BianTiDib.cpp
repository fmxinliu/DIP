#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BianTiDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"



BianTiDib::BianTiDib()
{

}
BianTiDib::~BianTiDib()
{
}



///***************************************************************/           
/*�������ƣ�Xihua()                                       
/*�������ͣ�void                                      
/*���ܣ���ͼ�����ϸ������            
/***************************************************************/
void BianTiDib::Xihua()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    LPBYTE p_temp = new BYTE[size]; // ָ�򻺴�DIBͼ���ָ��

    BYTE S[5][5]; // 5��5������������ֵ
    BOOL fp = TRUE; // ѭ��������־
    int Num = 0;
    int T = 0; // ��ֵ��ֵ

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        T = 127; // ������ֵ��Ϊ�Ǳ���
        while (fp) {
            fp = FALSE;
            memset(p_temp, 255, size);

            // ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
            for (int y = 2; y < height - 2; y++) {
                for (int x = 2; x < width - 2; x++) {
                    LONG offset = dibWidth * y + x;

                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (p_data[offset] > T)
                        continue;

                    // ��á���ǰ�㡿���ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (int j = 0; j < 5; j++)  {
                        for (int i = 0; i < 5; i++) {
                            //int t = dibWidth * (2 - j) + (i - 2); // 5*5����
                            int t = dibWidth * (2 - j) + (i - 2) * 2;
                            //int xx = dibWidth * (y - (j - 2)) + x + (i - 2) * 2; // ͼ���ã�ģ��Ҳ����
                            //int yy = offset + t;
                            //if (p_data[xx] > 127)
                            if (p_data[offset + t] > T)
                                S[j][i] = 0;
                            else
                                S[j][i] = 1;                             
                        }
                    }

                    // �ж�����1�Ƿ������
                    Num = S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                        + S[2][3] + S[3][1] + S[3][2] + S[3][3];
                    if (Num < 2 || Num > 6) {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����1

                    // �ж�����2�Ƿ������
                    Num = 0;
                    if (S[1][2] == 0 && S[1][1] == 1) 
                        Num++;
                    if (S[1][1] == 0 && S[2][1] == 1)
                        Num++;
                    if (S[2][1] == 0 && S[3][1] == 1)
                        Num++;
                    if (S[3][1] == 0 && S[3][2] == 1)
                        Num++;
                    if (S[3][2] == 0 && S[3][3] == 1)
                        Num++;
                    if (S[3][3] == 0 && S[2][3] == 1)
                        Num++;
                    if (S[2][3] == 0 && S[1][3] == 1)
                        Num++;
                    if (S[1][3] == 0 && S[1][2] == 1)
                        Num++;
                    if (Num != 1) {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����2

                    // �ж�����3�Ƿ������
                    if (S[1][2] * S[2][1] * S[2][3] != 0)
                    {
                        Num = 0;
                        if (S[0][2] == 0 && S[0][1] == 1)
                            Num++;
                        if (S[0][1] == 0 && S[1][1] == 1)
                            Num++;
                        if (S[1][1] == 0 && S[2][1] == 1)
                            Num++;
                        if (S[2][1] == 0 && S[2][2] == 1)
                            Num++;
                        if (S[2][2] == 0 && S[2][3] == 1)
                            Num++;
                        if (S[2][3] == 0 && S[1][3] == 1)
                            Num++;
                        if (S[1][3] == 0 && S[0][3] == 1)
                            Num++;
                        if (S[0][3] == 0 && S[0][2] == 1)
                            Num++;
                        if (Num == 1) {
                            p_temp[offset] = 0;
                            continue;
                        }
                    } // end ����3

                    // �ж�����4�Ƿ������
                    if (S[1][2] * S[2][1] * S[3][2] != 0)
                    {
                        Num = 0;
                        if (S[1][1] == 0 && S[1][0] == 1)
                            Num++;
                        if (S[1][0] == 0 && S[2][0] == 1)
                            Num++;
                        if (S[2][0] == 0 && S[3][0] == 1)
                            Num++;
                        if (S[3][0] == 0 && S[3][1] == 1)
                            Num++;
                        if (S[3][1] == 0 && S[3][2] == 1)
                            Num++;
                        if (S[3][2] == 0 && S[2][2] == 1)
                            Num++;
                        if (S[2][2] == 0 && S[1][2] == 1)
                            Num++;
                        if (S[1][2] == 0 && S[1][1] == 1)
                            Num++;
                        if (Num == 1) {
                            p_temp[offset] = 0;
                            continue;
                        }
                    } // end ����4

                    // ���������������ɾ���õ�
                    p_temp[offset] = 255;
                    fp = TRUE;
                } // end for (int x = 2 ; x < width - 2; x++)
            } // end for (int y = 2; y < height - 2; y++)

            // ����ϸ�����ͼ��
            memcpy(p_data, p_temp, size);
        } // end while (fp)
    }
    else {
        T = 245; // ������ֵ��Ϊ�Ǳ���
        while (fp) {
            fp = FALSE;
            memset(p_temp, 255, size);

            // ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
            for (int y = 2; y < height - 2; y++) {
                for (int x = 2; x < width - 2; x++) {
                    LONG offset = dibWidth * y + x * 3;

                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (p_data[offset] > T)
                        continue;

                    // ��á���ǰ�㡿���ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (int j = 0; j < 5; j++)  {
                        for (int i = 0; i < 5; i++) {
                            int t = dibWidth * (2 - j) + (i - 2) * 3;
                            if (p_data[offset + t] > T)
                                S[j][i] = 0;
                            else
                                S[j][i] = 1;                             
                        }
                    }

                    // �ж�����1�Ƿ������
                    Num = S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                        + S[2][3] + S[3][1] + S[3][2] + S[3][3];
                    if (Num < 2 || Num > 6) {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����1

                    // �ж�����2�Ƿ������
                    Num = 0;
                    if (S[1][2] == 0 && S[1][1] == 1) 
                        Num++;
                    if (S[1][1] == 0 && S[2][1] == 1)
                        Num++;
                    if (S[2][1] == 0 && S[3][1] == 1)
                        Num++;
                    if (S[3][1] == 0 && S[3][2] == 1)
                        Num++;
                    if (S[3][2] == 0 && S[3][3] == 1)
                        Num++;
                    if (S[3][3] == 0 && S[2][3] == 1)
                        Num++;
                    if (S[2][3] == 0 && S[1][3] == 1)
                        Num++;
                    if (S[1][3] == 0 && S[1][2] == 1)
                        Num++;
                    if (Num != 1) {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����2

                    // �ж�����3�Ƿ������
                    if (S[1][2] * S[2][1] * S[2][3] != 0)
                    {
                        Num = 0;
                        if (S[0][2] == 0 && S[0][1] == 1)
                            Num++;
                        if (S[0][1] == 0 && S[1][1] == 1)
                            Num++;
                        if (S[1][1] == 0 && S[2][1] == 1)
                            Num++;
                        if (S[2][1] == 0 && S[2][2] == 1)
                            Num++;
                        if (S[2][2] == 0 && S[2][3] == 1)
                            Num++;
                        if (S[2][3] == 0 && S[1][3] == 1)
                            Num++;
                        if (S[1][3] == 0 && S[0][3] == 1)
                            Num++;
                        if (S[0][3] == 0 && S[0][2] == 1)
                            Num++;
                        if (Num == 1) {
                            p_temp[offset] = 0;
                            p_temp[offset + 1] = 0;
                            p_temp[offset + 2] = 0;
                            continue;
                        }
                    } // end ����3

                    // �ж�����4�Ƿ������
                    if (S[1][2] * S[2][1] * S[3][2] != 0)
                    {
                        Num = 0;
                        if (S[1][1] == 0 && S[1][0] == 1)
                            Num++;
                        if (S[1][0] == 0 && S[2][0] == 1)
                            Num++;
                        if (S[2][0] == 0 && S[3][0] == 1)
                            Num++;
                        if (S[3][0] == 0 && S[3][1] == 1)
                            Num++;
                        if (S[3][1] == 0 && S[3][2] == 1)
                            Num++;
                        if (S[3][2] == 0 && S[2][2] == 1)
                            Num++;
                        if (S[2][2] == 0 && S[1][2] == 1)
                            Num++;
                        if (S[1][2] == 0 && S[1][1] == 1)
                            Num++;
                        if (Num == 1) {
                            p_temp[offset] = 0;
                            p_temp[offset + 1] = 0;
                            p_temp[offset + 2] = 0;
                            continue;
                        }
                    } // end ����4

                    // ���������������ɾ���õ�
                    p_temp[offset] = 255;
                    p_temp[offset + 1] = 255;
                    p_temp[offset + 2] = 255;
                    fp = TRUE;
                } // end for (int x = 2; x < width - 2; x++)
            } // end for (int y = 2; y < height - 2; y++)

            // ����ϸ�����ͼ��
            memcpy(p_data, p_temp, size);
        } // end while (fp)
    }

    // ��ֵϸ���������յ�ͼ��
    memcpy(p_data, p_temp, size);
    delete p_temp;
}


///***************************************************************/           
/*�������ƣ�Cuhua()                                       
/*�������ͣ�void                                      
/*���ܣ���ͼ����дֻ�����            
/***************************************************************/
void BianTiDib::Cuhua()
{
    // ѭ������
    LONG i;
    LONG j;
    // ָ��DIB����ָ��
    LPBYTE p_data;
    // �ҵ�DIBͼ��������ʼλ��
    p_data = this->GetData();
    if(m_pBitmapInfoHeader->biBitCount<9)//�Ҷ�ͼ��
    {
        // DIB�Ŀ��
        LONG wide = GetWidth();
        // DIB�ĸ߶�
        LONG height = GetHeight();
        // �Ը����ؽ��лҶ�ת��
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                // �����ظ���ɫ�������ж�ֵ���󲹴���
                unsigned char temp = *((unsigned char *)p_data + wide * j +i);
                if (temp > 127)
                    *((unsigned char *)p_data + wide * j + i) = 0;
                else
                    *((unsigned char *)p_data + wide * j + i) = 255;
            }
        }
        // ���󲹺��ٶ�ͼ�����ϸ��
        Xihua(); 
    }
    else//24λ���ɫ
    {
        // DIB�Ŀ��
        LONG wide = GetDibWidthBytes();
        // DIB�ĸ߶�
        LONG height = GetHeight();
        // �Ը����ؽ��лҶ�ת��
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                // �����ظ���ɫ�������ж�ֵ���󲹴���
                unsigned char temp = *((unsigned char *)p_data + wide * j +i);
                if (temp > 127)
                    *((unsigned char *)p_data + wide * j + i) = 0;
                else
                    *((unsigned char *)p_data + wide * j + i) = 255;                      
            }
        }
        // ���󲹺��ٶ�ͼ�����ϸ��
        Xihua(); 
    }
}



///***************************************************************/           
/*�������ƣ�Zhongzhoubianhuan()                                       
/*�������ͣ�void                                      
/*���ܣ���ͼ���������任��            
/***************************************************************/
void BianTiDib::Zhongzhoubianhuan()
{
    // ѭ������ 
    LONG i;
    LONG j;
    LONG m;
    LONG n;
    // 5��5������������ֵ
    unsigned char S[5][5];
    // ������
    unsigned char Num;
    // ѭ��������־ 
    BOOL fp=TRUE;
    // ָ��DIB����ָ��
    LPBYTE p_data;
    // ָ��Դͼ���ָ��
    LPBYTE    lpSrc;
    // ָ�򻺴�ͼ���ָ��
    LPBYTE    lpDst;
    // ָ�򻺴�DIBͼ���ָ��
    LPBYTE    temp;
    // �ҵ�DIBͼ��������ʼλ��
    p_data = GetData();
    if(m_pBitmapInfoHeader->biBitCount<9)//�Ҷ�ͼ��
    {    
        // DIB�Ŀ��
        LONG wide = GetWidth();
        // DIB�ĸ߶� 
        LONG height = GetHeight();
        // ��ʱ�����ڴ棬�Ա�����ͼ��
        temp =  new BYTE [wide*height];
        // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
        lpDst = (LPBYTE)temp;
        memset(lpDst, (BYTE)255, wide * height);
        while (fp)
        {
            fp = FALSE;
            // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
            lpDst = (LPBYTE)temp;
            memset(lpDst, (BYTE)255, wide * height);
            // ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
            for (j= 2; j < height - 2; j++)
            {
                for (i = 2  ; i < wide - 2  ; i ++)
                {
                    // ָ��Դͼ������j�У���i�����ص�ָ��            
                    lpSrc = (LPBYTE)(p_data + wide * j + i);
                    // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
                    lpDst = (LPBYTE)(temp + wide * j + i);
                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (*lpSrc > 127)
                        continue;
                    // ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (m = 0; m < 5; m++)
                    {
                        for (n = 0; n < 5; n++)
                        {
                            if (*(lpSrc + (2 - m) * wide + (n - 2)  ) > 127)
                                S[m][n] = 0;
                            else
                                S[m][n] = 1;
                        }
                    }
                    // �ж�����1-1�Ƿ������
                    Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                    + S[2][3] + S[3][1]    + S[3][2] + S[3][3];
                    if (Num < 2 || Num >6)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // �ж�����1-2�Ƿ������
                    Num = 0;
                    if (S[1][2] == 0 && S[1][1] == 1) 
                        Num++;
                    if (S[1][1] == 0 && S[2][1] == 1)
                        Num++;
                    if (S[2][1] == 0 && S[3][1] == 1)
                        Num++;
                    if (S[3][1] == 0 && S[3][2] == 1)
                        Num++;
                    if (S[3][2] == 0 && S[3][3] == 1)
                        Num++;
                    if (S[3][3] == 0 && S[2][3] == 1)
                        Num++;
                    if (S[2][3] == 0 && S[1][3] == 1)
                        Num++;
                    if (S[1][3] == 0 && S[1][2] == 1)
                        Num++;
                    if (Num != 1)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // �ж�����1-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[3][2] != 0)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // �ж�����1-4�Ƿ������
                    if (S[2][1] * S[3][2] * S[2][3] != 0)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // ���������������ɾ���õ�
                    *lpDst = 255;             
                }
            }
            // ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
            for (j = 2; j < height - 2; j++)
            {
                for (i = 2  ; i < wide - 2  ; i ++)
                {
                    // ָ��Դͼ������j�У���i�����ص�ָ��            
                    lpSrc = (LPBYTE)(p_data + wide * j + i);
                    // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
                    lpDst = (LPBYTE)(temp + wide *j + i);
                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (*lpSrc > 127)
                        continue;
                    // ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (m = 0; m < 5; m++)
                    {
                        for (n = 0; n < 5; n++)
                        {
                            if (*(lpSrc + (2 - m) * wide + (n - 2)  ) > 200)
                                S[m][n] = 0;
                            else
                                S[m][n] = 1;
                        }
                    }
                    // �ж�����2-1�Ƿ������
                    Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                    + S[2][3] + S[3][1]    + S[3][2] + S[3][3];
                    if (Num < 2 || Num >6)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // �ж�����2-2�Ƿ������
                    Num = 0;
                    if (S[1][2] == 0 && S[1][1] == 1) 
                        Num++;
                    if (S[1][1] == 0 && S[2][1] == 1)
                        Num++;
                    if (S[2][1] == 0 && S[3][1] == 1)
                        Num++;
                    if (S[3][1] == 0 && S[3][2] == 1)
                        Num++;
                    if (S[3][2] == 0 && S[3][3] == 1)
                        Num++;
                    if (S[3][3] == 0 && S[2][3] == 1)
                        Num++;
                    if (S[2][3] == 0 && S[1][3] == 1)
                        Num++;
                    if (S[1][3] == 0 && S[1][2] == 1)
                        Num++;
                    if (Num != 1)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // �ж�����2-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[2][3] != 0)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // �ж�����2-4�Ƿ������
                    if (S[1][2] * S[3][2] * S[2][3] != 0)
                    {
                        *lpDst = 0;             
                        continue;
                    }
                    // ���������������ɾ���õ�
                    *lpDst = 255;             
                    fp = TRUE;
                }
            }
            // ���Ʊ任���ͼ��
            memcpy(p_data, temp, wide * height);
        }
        // ���Ʊ任���ͼ��
        memcpy(p_data, temp, wide * height);
        //  �ͷ��ڴ�
        delete temp; 
    }
    else//24λ���ɫ
    {
        // DIB�Ŀ��
        LONG wide = GetDibWidthBytes();
        // DIB�ĸ߶� 
        LONG height = GetHeight();
        // ��ʱ�����ڴ棬�Ա�����ͼ��
        temp =  new BYTE [wide*height];
        // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
        lpDst = (LPBYTE)temp;
        memset(lpDst, (BYTE)255, wide * height);
        while (fp)
        {
            fp = FALSE;
            // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
            lpDst = (LPBYTE)temp;
            memset(lpDst, (BYTE)255, wide * height);
            // ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
            for (j= 2; j < height - 2; j++)
            {
                for (i = 2*3  ; i < wide - 2 *3 ; i =i+3)
                {
                    // ָ��Դͼ������j�У���i�����ص�ָ��            
                    lpSrc = (LPBYTE)(p_data + wide * j + i);
                    // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
                    lpDst = (LPBYTE)(temp + wide * j + i);
                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (*lpSrc > 127)
                        continue;
                    // ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (m = 0; m < 5; m++)
                    {
                        for (n = 0; n < 5; n++)
                        {
                            if (*(lpSrc + (2 - m) * wide + (n - 2) *3 ) > 200)
                                S[m][n] = 0;
                            else
                                S[m][n] = 1;
                        }
                    }
                    // �ж�����1-1�Ƿ������
                    Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                    + S[2][3] + S[3][1]    + S[3][2] + S[3][3];
                    if (Num < 2 || Num >6)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // �ж�����1-2�Ƿ������
                    Num = 0;
                    if (S[1][2] == 0 && S[1][1] == 1) 
                        Num++;
                    if (S[1][1] == 0 && S[2][1] == 1)
                        Num++;
                    if (S[2][1] == 0 && S[3][1] == 1)
                        Num++;
                    if (S[3][1] == 0 && S[3][2] == 1)
                        Num++;
                    if (S[3][2] == 0 && S[3][3] == 1)
                        Num++;
                    if (S[3][3] == 0 && S[2][3] == 1)
                        Num++;
                    if (S[2][3] == 0 && S[1][3] == 1)
                        Num++;
                    if (S[1][3] == 0 && S[1][2] == 1)
                        Num++;
                    if (Num != 1)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // �ж�����1-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[3][2] != 0)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // �ж�����1-4�Ƿ������
                    if (S[2][1] * S[3][2] * S[2][3] != 0)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // ���������������ɾ���õ�
                    *lpDst = 255;
                    *(lpDst+1) = 255;
                    *(lpDst+2) = 255;
                }
            }
            // ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
            for (j = 2; j < height - 2; j++)
            {
                for (i = 2*3  ; i < wide - 2*3  ; i=i +3)
                {
                    // ָ��Դͼ������j�У���i�����ص�ָ��            
                    lpSrc = (LPBYTE)(p_data + wide * j + i);
                    // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
                    lpDst = (LPBYTE)(temp + wide *j + i);
                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (*lpSrc > 127)
                        continue;
                    // ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (m = 0; m < 5; m++)
                    {
                        for (n = 0; n < 5; n++)
                        {
                            if (*(lpSrc + (2 - m) * wide + (n - 2)*3  ) > 200)
                                S[m][n] = 0;
                            else
                                S[m][n] = 1;
                        }
                    }
                    // �ж�����2-1�Ƿ������
                    Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                    + S[2][3] + S[3][1]    + S[3][2] + S[3][3];
                    if (Num < 2 || Num >6)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // �ж�����2-2�Ƿ������
                    Num = 0;
                    if (S[1][2] == 0 && S[1][1] == 1) 
                        Num++;
                    if (S[1][1] == 0 && S[2][1] == 1)
                        Num++;
                    if (S[2][1] == 0 && S[3][1] == 1)
                        Num++;
                    if (S[3][1] == 0 && S[3][2] == 1)
                        Num++;
                    if (S[3][2] == 0 && S[3][3] == 1)
                        Num++;
                    if (S[3][3] == 0 && S[2][3] == 1)
                        Num++;
                    if (S[2][3] == 0 && S[1][3] == 1)
                        Num++;
                    if (S[1][3] == 0 && S[1][2] == 1)
                        Num++;
                    if (Num != 1)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // �ж�����2-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[2][3] != 0)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // �ж�����2-4�Ƿ������
                    if (S[1][2] * S[3][2] * S[2][3] != 0)
                    {
                        *lpDst = 0;
                        *(lpDst+1) = 0;
                        *(lpDst+2) = 0;
                        continue;
                    }
                    // ���������������ɾ���õ�
                    *lpDst = 255;
                    *(lpDst+1) = 255;
                    *(lpDst+2) = 255;
                    fp = TRUE;
                }
            }
            // ���Ʊ任���ͼ��
            memcpy(p_data, temp, wide * height);
        }
        // ���Ʊ任���ͼ��
        memcpy(p_data, temp, wide * height);    
        //  �ͷ��ڴ�
        delete temp; 
    }
}

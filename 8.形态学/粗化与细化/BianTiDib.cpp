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
    // 1.����ͼ�񲹼�
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = GetDibWidthBytes();   // ȡ��ԭͼ��ÿ���ֽ���(4�ֽڶ���)
    LONG size = dibWidth * height;

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LONG offset = dibWidth * y + x;
            if (p_data[offset] > 127)
                p_data[offset] = 0;
            else
                p_data[offset] = 255;
        }
    }

    // 2.���󲹺�ͼ�����ٽ���ϸ��
    Xihua();
}


///***************************************************************/           
/*�������ƣ�Zhongzhoubianhuan()                                       
/*�������ͣ�void                                      
/*���ܣ���ͼ���������任��            
/***************************************************************/
void BianTiDib::Zhongzhoubianhuan()
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
    int T2 = 0; // ��ֵ��ֵ2

    if (m_pBitmapInfoHeader->biBitCount < 9) {
        T = 127; // ������ֵ��Ϊ�Ǳ���
        T2 = 200;
        while (fp) {
            fp = FALSE;
            memset(p_temp, 255, size);

            // 1.��һ��ɾ��
            for (int y = 2; y < height - 2; y++) {
                for (int x = 2; x < width - 2; x++) {
                    LONG offset = dibWidth * y + x;

                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (p_data[offset] > T)
                        continue;

                    // ��á���ǰ�㡿���ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (int j = 0; j < 5; j++)  {
                        for (int i = 0; i < 5; i++) {
                            int t = dibWidth * (2 - j) + (i - 2);
                            if (p_data[offset + t] > T)
                                S[j][i] = 0;
                            else
                                S[j][i] = 1;                             
                        }
                    }

                    // �ж�����1-1�Ƿ������
                    Num = S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                        + S[2][3] + S[3][1] + S[3][2] + S[3][3];
                    if (Num < 2 || Num > 6) {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����1-1

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
                    if (Num != 1) {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����1-2

                    // �ж�����1-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[3][2] != 0)
                    {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����1-3

                    // �ж�����1-4�Ƿ������
                    if (S[2][1] * S[3][2] * S[2][3] != 0)
                    {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����1-4

                    // ���������������ɾ���õ�
                    p_temp[offset] = 255;
                } // end for (int x = 2 ; x < width - 2; x++)
            } // end for (int y = 2; y < height - 2; y++)


            // 2.�ڶ���ɾ��
            for (int y = 2; y < height - 2; y++) {
                for (int x = 2; x < width - 2; x++) {
                    LONG offset = dibWidth * y + x;

                    // ���Դͼ���е�ǰ��Ϊ��ɫ��������
                    if (p_data[offset] > T)
                        continue;

                    // ��á���ǰ�㡿���ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                    for (int j = 0; j < 5; j++)  {
                        for (int i = 0; i < 5; i++) {
                            int t = dibWidth * (2 - j) + (i - 2);
                            if (p_data[offset + t] > T2)
                                S[j][i] = 0;
                            else
                                S[j][i] = 1;                             
                        }
                    }

                    // �ж�����2-1�Ƿ������
                    Num = S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                        + S[2][3] + S[3][1] + S[3][2] + S[3][3];
                    if (Num < 2 || Num > 6) {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����2-1

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
                    if (Num != 1) {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����2-2

                    // �ж�����2-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[2][3] != 0)
                    {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����2-3

                    // �ж�����2-4�Ƿ������
                    if (S[1][2] * S[3][2] * S[2][3] != 0)
                    {
                        p_temp[offset] = 0;
                        continue;
                    } // end ����2-4

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
        T = 127; // ������ֵ��Ϊ�Ǳ���
        T2 = 200;
        while (fp) {
            fp = FALSE;
            memset(p_temp, 255, size);

            // 1.��һ��ɾ��
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
                            if (p_data[offset + t] > T2)
                                S[j][i] = 0;
                            else
                                S[j][i] = 1;                             
                        }
                    }

                    // �ж�����1-1�Ƿ������
                    Num = S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                        + S[2][3] + S[3][1] + S[3][2] + S[3][3];
                    if (Num < 2 || Num > 6) {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����1-1

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
                    if (Num != 1) {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����1-2

                    // �ж�����1-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[3][2] != 0)
                    {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����1-3

                    // �ж�����1-4�Ƿ������
                    if (S[2][1] * S[3][2] * S[2][3] != 0)
                    {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����1-4

                    // ���������������ɾ���õ�
                    p_temp[offset] = 255;
                    p_temp[offset + 1] = 255;
                    p_temp[offset + 2] = 255;
                } // end for (int x = 2 ; x < width - 2; x++)
            } // end for (int y = 2; y < height - 2; y++)


            // 2.�ڶ���ɾ��
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
                            if (p_data[offset + t] > T2)
                                S[j][i] = 0;
                            else
                                S[j][i] = 1;                             
                        }
                    }

                    // �ж�����2-1�Ƿ������
                    Num = S[1][1] + S[1][2] + S[1][3] + S[2][1] 
                        + S[2][3] + S[3][1] + S[3][2] + S[3][3];
                    if (Num < 2 || Num > 6) {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����2-1

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
                    if (Num != 1) {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����2-2

                    // �ж�����2-3�Ƿ������
                    if (S[1][2] * S[2][1] * S[2][3] != 0)
                    {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����2-3

                    // �ж�����2-4�Ƿ������
                    if (S[1][2] * S[3][2] * S[2][3] != 0)
                    {
                        p_temp[offset] = 0;
                        p_temp[offset + 1] = 0;
                        p_temp[offset + 2] = 0;
                        continue;
                    } // end ����2-4

                    // ���������������ɾ���õ�
                    p_temp[offset] = 255;
                    p_temp[offset + 1] = 255;
                    p_temp[offset + 2] = 255;
                    fp = TRUE;
                } // end for (int x = 2 ; x < width - 2; x++)
            } // end for (int y = 2; y < height - 2; y++)

            // ����ϸ�����ͼ��
            memcpy(p_data, p_temp, size);
        } // end while (fp)
    }

    // ��ֵϸ���������յ�ͼ��
    memcpy(p_data, p_temp, size);
    delete p_temp;
}

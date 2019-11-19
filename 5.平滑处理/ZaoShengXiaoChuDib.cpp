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
/*�������ƣ�GuDing(int YuZhi)                                  */      
/*�������ͣ�void                                               */    
/*����˵����YuZhi ������ֵ                                     */       
/*���ܣ���ͼ����й̶���ֵ���㡣                               */
/***************************************************************/
void ZaoShengXiaoChuDib::GuDing(int YuZhi)//��ͼ����й̶���ֵ����
{
    LPBYTE p_data=this->GetData(); //ԭͼ������ָ��
    int width=this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int height=this->GetHeight(); // ԭͼ�߶�

    for(int i=0;i<height*width;i++)
    {
        if(*p_data>YuZhi)          //�Ҷ�ֵ���ڸ�����ֵ����Ϊ255
            *p_data=255;
        else
            *p_data=0;             //��������Ϊ0
        p_data++;
    }
}

/***************************************************************/ 
/*�������ƣ�HeiBaiDianZaoSheng()                               */
/*�������ͣ�void                                               */
/*���ܣ��Զ�ֵͼ��ĺڰ׵�����������                             */
/***************************************************************/
void ZaoShengXiaoChuDib::HeiBaiDianZaoSheng()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    // 1.��ֵ��
    GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��3*3����ģ(�˲������ˡ�����)�˲�
    //  [1, 1, 1
    //   1, 0, 1
    //   1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
            for (int x = 1; x < width - 1; x++) {
                int avg = (p_data[lineBytes * (y - 1) + (x - 1)]
                    + p_data[lineBytes * (y - 1) + (x)]
                    + p_data[lineBytes * (y - 1) + (x + 1)]
                    + p_data[lineBytes * (y) + (x - 1)]
                    + p_data[lineBytes * (y) + (x + 1)]
                    + p_data[lineBytes * (y + 1) + (x - 1)]
                    + p_data[lineBytes * (y + 1) + (x)]
                    + p_data[lineBytes * (y + 1) + (x + 1)]) / 8; // 8�����ֵ

                if (abs(avg - p_data[lineBytes * y + x]) > 127.5) // ��ֵ
                    p_temp[lineBytes * y + x] = avg;
            }
        }

    } else {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
            for (int x = 1; x < width - 1; x++) {
                int avg_b = (p_data[lineBytes * (y - 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y - 1) + (x) * 3]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3]
                    + p_data[lineBytes * (y) + (x - 1) * 3]
                    + p_data[lineBytes * (y) + (x + 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x) * 3]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3]) / 8; // 8�����ֵ

                int avg_g = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 1]) / 8; // 8�����ֵ

                int avg_r = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 2]) / 8; // 8�����ֵ

                if (abs(avg_b - p_data[lineBytes * y + x * 3]) > 127.5) // ��ֵ
                    p_temp[lineBytes * y + x * 3] = avg_b;
                if (abs(avg_g - p_data[lineBytes * y + x * 3 + 1]) > 127.5) // ��ֵ
                    p_temp[lineBytes * y + x * 3 + 1] = avg_g;
                if (abs(avg_r - p_data[lineBytes * y + x * 3 + 2]) > 127.5) // ��ֵ
                    p_temp[lineBytes * y + x * 3 + 2] = avg_r;
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}

/***************************************************************/ 
/*�������ƣ�GuliHaiDian(int connec)                                  */
/*�������ͣ�void                                               */
/*������int connec���趨����ͨѡ��                             */
/*���ܣ��Զ�ֵͼ������������������ص㡣                       */
/***************************************************************/
void ZaoShengXiaoChuDib::GuliHaiDian(int connec)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    // 1.��ֵ��
    GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��3*3�ĺ��˲�
    //  4����:  [0, 1, 0     8����:[1, 1, 1
    //           1, 0, 1           1, 0, 1
    //           0, 1, 0]          1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
            for (int x = 1; x < width - 1; x++) {
                if (p_data[lineBytes * y + x]) 
                    continue; // ֻ����ڵ�

                int sum = -1;
                if (connec == 4) { // 4 ����
                    sum = p_data[lineBytes * (y - 1) + (x)]
                        + p_data[lineBytes * (y) + (x - 1)]
                        + p_data[lineBytes * (y) + (x + 1)]
                        + p_data[lineBytes * (y + 1) + (x)];
                } else if (connec == 8) { // 8 ����
                    sum = p_data[lineBytes * (y - 1) + (x - 1)]
                        + p_data[lineBytes * (y - 1) + (x)]
                        + p_data[lineBytes * (y - 1) + (x + 1)]
                        + p_data[lineBytes * (y) + (x - 1)]
                        + p_data[lineBytes * (y) + (x + 1)]
                        + p_data[lineBytes * (y + 1) + (x - 1)]
                        + p_data[lineBytes * (y + 1) + (x)]
                        + p_data[lineBytes * (y + 1) + (x + 1)];
                }

                // xx���ӵ�����£���ȥ��Χ��Ϊ255(�׵�)�Ĺ����ڵ�
                if (sum == 255 * connec)
                    p_temp[lineBytes * y + x] = 255;
            }
        }
 
    } else {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
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

                // xx���ӵ�����£���ȥ��Χ��Ϊ255(�׵�)�Ĺ����ڵ�
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
/*�������ƣ�threethree()                                       */
/*�������ͣ�void                                               */
/*���ܣ���ͼ�����3*3��ֵ�˲������˳�������ͬʱ������Եģ������   */
/***************************************************************/
void ZaoShengXiaoChuDib::threethree() 
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.��ֵ��
    //GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��3*3����ģ(�˲������ˡ�����)�˲�
    //  [1, 1, 1
    //   1, 0, 1
    //   1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
            for (int x = 1; x < width - 1; x++) {
                int avg = (p_data[lineBytes * (y - 1) + (x - 1)]
                    + p_data[lineBytes * (y - 1) + (x)]
                    + p_data[lineBytes * (y - 1) + (x + 1)]
                    + p_data[lineBytes * (y) + (x - 1)]
                    + p_data[lineBytes * (y) + (x + 1)]
                    + p_data[lineBytes * (y + 1) + (x - 1)]
                    + p_data[lineBytes * (y + 1) + (x)]
                    + p_data[lineBytes * (y + 1) + (x + 1)]) / 8; // 8�����ֵ

                p_temp[lineBytes * y + x] = avg;                   
            }
        }

    } else {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
            for (int x = 1; x < width - 1; x++) {
                int avg_b = (p_data[lineBytes * (y - 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y - 1) + (x) * 3]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3]
                    + p_data[lineBytes * (y) + (x - 1) * 3]
                    + p_data[lineBytes * (y) + (x + 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x) * 3]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3]) / 8; // 8�����ֵ

                int avg_g = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 1]) / 8; // 8�����ֵ

                int avg_r = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 2]) / 8; // 8�����ֵ

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
/*�������ƣ�Chaoxian(int T)                                    */
/*�������ͣ�void                                               */
/*������int T���趨����ֵ                                      */
/*���ܣ���������ƽ������                                       */
/***************************************************************/
void ZaoShengXiaoChuDib::Chaoxian(int T)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.��ֵ��
    //GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��3*3����ģ(�˲������ˡ�����)�˲�
    //  [1, 1, 1
    //   1, 0, 1
    //   1, 1, 1]
    if (GetRGB()) {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
            for (int x = 1; x < width - 1; x++) {
                int avg = (p_data[lineBytes * (y - 1) + (x - 1)]
                    + p_data[lineBytes * (y - 1) + (x)]
                    + p_data[lineBytes * (y - 1) + (x + 1)]
                    + p_data[lineBytes * (y) + (x - 1)]
                    + p_data[lineBytes * (y) + (x + 1)]
                    + p_data[lineBytes * (y + 1) + (x - 1)]
                    + p_data[lineBytes * (y + 1) + (x)]
                    + p_data[lineBytes * (y + 1) + (x + 1)]) / 8; // 8�����ֵ

                if (abs(avg - p_data[lineBytes * y + x]) > T) // ��ֵ
                    p_temp[lineBytes * y + x] = avg;
            }
        }

    } else {
        for (int y = 1; y < height - 1; y++) { // ���Ա߽�����
            for (int x = 1; x < width - 1; x++) {
                int avg_b = (p_data[lineBytes * (y - 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y - 1) + (x) * 3]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3]
                    + p_data[lineBytes * (y) + (x - 1) * 3]
                    + p_data[lineBytes * (y) + (x + 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3]
                    + p_data[lineBytes * (y + 1) + (x) * 3]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3]) / 8; // 8�����ֵ

                int avg_g = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 1]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 1]) / 8; // 8�����ֵ

                int avg_r = (p_data[lineBytes * (y - 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y - 1) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y) + (x + 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x - 1) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x) * 3 + 2]
                    + p_data[lineBytes * (y + 1) + (x + 1) * 3 + 2]) / 8; // 8�����ֵ

                if (abs(avg_b - p_data[lineBytes * y + x * 3]) > T) // ��ֵ
                    p_temp[lineBytes * y + x * 3] = avg_b;
                if (abs(avg_g - p_data[lineBytes * y + x * 3 + 1]) > T) // ��ֵ
                    p_temp[lineBytes * y + x * 3 + 1] = avg_g;
                if (abs(avg_r - p_data[lineBytes * y + x * 3 + 2]) > T) // ��ֵ
                    p_temp[lineBytes * y + x * 3 + 2] = avg_r;
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}

/***************************************************************/ 
/*�������ƣ�nn(int n)                                          */
/*�������ͣ�void                                               */
/*������int n���趨�����δ��ڲ�����������                        */
/*���ܣ���ͼ�����n*n��ֵ�˲����������ϸ��ģ������             */
/***************************************************************/
void ZaoShengXiaoChuDib::nn(int n)
{
    if(n < 3 || n % 2 != 1) {// ȷ��nΪ����
        AfxMessageBox("������һ�����ڵ���3������");
        return;
    }

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.��ֵ��
    //GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��n*n����ģ(�˲������ˡ�����)�˲�
    int border = (n - 1) / 2;
    if (GetRGB()) {
        for (int y = border; y < height - border; y++) { // ���Ա߽�����
            for (int x = border; x < width - border; x++) {
                int sum = 0; //��N*N���δ����ڵ��ܺ�sum
                for (int t = -border; t <= border; t++) {
                    for (int s = -border; s <= border; s++) {
                        sum += p_data[lineBytes * (y + t) + (x + s)];
                    }
                }

                // ��n*n���δ����ڵ�ƽ��ֵ�����������Ϊ��ʾͼ������ֵ  
                p_temp[lineBytes * y + x] = (int)((float)sum / (n * n) + 0.5);               
            }
        }

    } else {
        for (int y = border; y < height - border; y++) { // ���Ա߽�����
            for (int x = border; x < width - border; x++) {
                int sum_r = 0, sum_g = 0, sum_b = 0;
                for (int t = -border; t <= border; t++) {
                    for (int s = -border; s <= border; s++) {
                        sum_b += p_data[lineBytes * (y + t) + (x + s) * 3];
                        sum_g += p_data[lineBytes * (y + t) + (x + s) * 3 + 1];
                        sum_r += p_data[lineBytes * (y + t) + (x + s) * 3 + 2];
                    }
                }

                // ��n*n���δ����ڵ�ƽ��ֵ�����������Ϊ��ʾͼ������ֵ  
                p_temp[lineBytes * y + x * 3] = (int)((float)sum_b / (n * n) + 0.5);
                p_temp[lineBytes * y + x * 3 + 1] = (int)((float)sum_g / (n * n) + 0.5);
                p_temp[lineBytes * y + x * 3 + 2] = (int)((float)sum_r / (n * n) + 0.5);
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
} 

// ��С�������򷵻���ֵ
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
/*�������ƣ�nnzhong(int n)                                      */
/*�������ͣ�void                                                */
/*������int n���趨�����δ��ڲ�����������                         */
/*���ܣ���ͼ�����n*n��ֵ�˲������Թ������������ȽϺã���         */
/***************************************************************/
void ZaoShengXiaoChuDib::nnzhong(int n)
{
    if(n < 3 || n % 2 != 1) {// ȷ��nΪ����
        AfxMessageBox("������һ�����ڵ���3������");
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

    //// 1.��ֵ��
    //GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��n*n����ģ(�˲������ˡ�����)�˲�
    
    if (GetRGB()) {
        for (int y = n3; y < height - n3; y++) { // ���Ա߽�����
            for (int x = n3; x < width - n3; x++) {
                // ��ȡģ�帲�ǵ�n*n������
                int index = 0;
                for (int t = -n3; t <= n3; t++) {
                    for (int s = -n3; s <= n3; s++) {
                        p_area[index++] = p_data[lineBytes * (y + t) + (x + s)];
                    }
                }

                // ȡn*n��������ֵ
                p_temp[lineBytes * y + x] = MidPixel(p_area, n2);               
            }
        }

    } else {
        for (int y = n3; y < height - n3; y++) { // ���Ա߽�����
            for (int x = n3; x < width - n3; x++) {
                // ��RGB��ͨ���ֱ���
                for (int k = 0; k < 3; k++) {
                    int index = 0;
                    for (int t = -n3; t <= n3; t++) {
                        for (int s = -n3; s <= n3; s++) {
                            p_area[index++] = p_data[lineBytes * (y + t) + (x + s) * 3 + k];
                        }
                    }

                    // ȡn*n��������ֵ
                    p_temp[lineBytes * y + x * 3 + k] = MidPixel(p_area, n2);
                }
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    //delete p_temp, p_area; // �����ڴ�й©
    delete p_temp;
    delete p_area;
}

/***************************************************************/
/*�������ƣ�shizi(int n)                                        */
/*�������ͣ�void                                                */
/*������int n���趨�����δ��ڲ�����������                         */
/*���ܣ���ͼ�����ʮ������ֵ�˲���                                */
/***************************************************************/
void ZaoShengXiaoChuDib::shizi(int n)
{
    if(n < 3 || n % 2 != 1) {// ȷ��nΪ����
        AfxMessageBox("������һ�����ڵ���3������");
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

    //// 1.��ֵ��
    //GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��n*n����ģ(�˲������ˡ�����)�˲�

    if (GetRGB()) {
        for (int y = n3; y < height - n3; y++) { // ���Ա߽�����
            for (int x = n3; x < width - n3; x++) {
                // ��ȡģ�帲�ǵ�ʮ��������
                int index = 0;
                
                // �����ĵ�ˮƽ����
                for (int s = -n3; s <= n3; s++) {
                    p_area[index++] = p_data[lineBytes * y + (x + s)];
                }

                // �������ĵĴ�ֱ����
                for (int t = -n3; t <= n3; t++) {
                    if (t == 0)
                        continue;
                    p_area[index++] = p_data[lineBytes * (y + t) + x];
                }

                // ȡn*n��������ֵ
                p_temp[lineBytes * y + x] = MidPixel(p_area, n2);               
            }
        }

    } else {
        for (int y = n3; y < height - n3; y++) { // ���Ա߽�����
            for (int x = n3; x < width - n3; x++) {
                // ��RGB��ͨ���ֱ���
                for (int k = 0; k < 3; k++) {

                    // ��ȡģ�帲�ǵ�ʮ��������
                    int index = 0;

                    // �����ĵ�ˮƽ����
                    for (int s = -n3; s <= n3; s++) {
                        p_area[index++] = p_data[lineBytes * y + (x + s) * 3 + k];
                    }

                    // �������ĵĴ�ֱ����
                    for (int t = -n3; t <= n3; t++) {
                        if (t == 0)
                            continue;
                        p_area[index++] = p_data[lineBytes * (y + t) + x * 3 + k];
                    }

                    // ȡn*n��������ֵ
                    p_temp[lineBytes * y + x * 3 + k] = MidPixel(p_area, n2);  
                }
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
    delete p_area;
}

// ���������ֵ
int MaxPixel(const LPBYTE pdata, int num)
{
    int maxValue = pdata[0];
    for (int i = 1; i < num; i++) {
        maxValue = max(maxValue, pdata[i]);
    }

    return maxValue;
}

/***************************************************************/ 
/*�������ƣ�nnzuida(int n)                                      */
/*�������ͣ�void                                                */
/*������int n���趨�����δ��ڲ�����������                         */
/*���ܣ���ͼ�����N*N���ֵ�˲���                                */
/**************************************************************/
void ZaoShengXiaoChuDib::nnzuida(int n)
{
    if(n < 3 || n % 2 != 1) {// ȷ��nΪ����
        AfxMessageBox("������һ�����ڵ���3������");
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

    //// 1.��ֵ��
    //GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.��n*n����ģ(�˲������ˡ�����)�˲�

    if (GetRGB()) {
        for (int y = n3; y < height - n3; y++) { // ���Ա߽�����
            for (int x = n3; x < width - n3; x++) {
                // ��ȡģ�帲�ǵ�n*n������
                int index = 0;
                for (int t = -n3; t <= n3; t++) {
                    for (int s = -n3; s <= n3; s++) {
                        p_area[index++] = p_data[lineBytes * (y + t) + (x + s)];
                    }
                }

                // ȡn*n��������ֵ
                p_temp[lineBytes * y + x] = MaxPixel(p_area, n2);               
            }
        }

    } else {
        for (int y = n3; y < height - n3; y++) { // ���Ա߽�����
            for (int x = n3; x < width - n3; x++) {
                // ��RGB��ͨ���ֱ���
                for (int k = 0; k < 3; k++) {
                    int index = 0;
                    for (int t = -n3; t <= n3; t++) {
                        for (int s = -n3; s <= n3; s++) {
                            p_area[index++] = p_data[lineBytes * (y + t) + (x + s) * 3 + k];
                        }
                    }

                    // ȡn*n��������ֵ
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
/*�������ƣ�suijizaosheng()                                     */
/*�������ͣ�void                                                */
/*���ܣ���ͼ����������������                                  */
/***************************************************************/
void ZaoShengXiaoChuDib::suijizaosheng()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            // �������
            int noisepoint = rand() / 1024; // [0, RAND_MAX / 1024 = 31]
            p_data[lineBytes * y + x] = p_data[lineBytes * y + x] * 224 / 255 + noisepoint;               
        }
    }
}

/***************************************************************/
/*�������ƣ�jiaoyanzaosheng()                                   */
/*�������ͣ�void                                                */
/*���ܣ���ͼ����н��Σ����壩���������ڰ׵㣩��                  */
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

// �����ֵ������
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

    mean = (float)sum / num; // ��ֵ

    int num2 = 9 - num;
    for (int i = 0; i < 9; i++) {
        if (kernel[i] == 0 && num2-- > 0)
            continue;
        //var += (kernel[i] - mean) * (kernel[i] - mean);
        var += kernel[i] * kernel[i] - mean * mean;
    }

    //var /= num * num; // ����
}

/***************************************************************/ 
/*�������ƣ�jubupingjun()                                      */
/*�������ͣ�void                                               */
/*���ܣ���ͼ����оֲ�ƽ��������ѡ��ʽ��ģƽ��������Ӧƽ������    */
/**************************************************************/
void ZaoShengXiaoChuDib::jubupingjun()
{
    // 1.��ֱ�߽緽��ķ������
    // 2.�����Ե��ƽ�����򷽲��
    // 3.����˲���ͬʱ�����ƻ�����߽��ϸ��
    //
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_temp;
    LPBYTE p_data = GetData();

    //// 1.��ֵ��
    //GuDing(100);

    // 2.���ػ��� 
    p_temp = new BYTE[lineBytes * height];
    memcpy(p_temp, p_data, lineBytes * height);

    // 3.����Ӧ�˲�
    int kernel[9]; // ����
    float mean[9] = {}; // ��ֵ
    float var[9] = {}; // ����

    int board = 2; // 5*5����ģ
    if (GetRGB()) {
        for (int y = board; y < height - board; y++) {
            for (int x = board; x < width - board; x++) {
                // ��9�ֽ�������ľ�ֵ���䷽��

                // a.��Χ8�ڽ�
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

                // b.��7�ڽ�
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


                // c.��7�ڽ�
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

                // d.��7�ڽ�
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

                // e.��7�ڽ�
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

                // f.����7�ڽ�
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

                // g.����7�ڽ�
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

                // h.����7�ڽ�
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

                // i.����7�ڽ�
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

                // �󷽲���С�Ľ�����������
                int minVar = var[0];
                int minIndex = 0;
                for (int i = 1; i < 9; i++) {
                    if (minVar > var[i]) {
                        minVar = var[i];
                        minIndex = i;
                    }
                }

                // ��������С�������ֵ�������������Ϊ�����ͼ�������ֵ
                p_temp[lineBytes * y + x] = (int)(mean[minIndex] + 0.5);         
            }
        }

    } else {
        for (int y = board; y < height - board; y++) {
            for (int x = board; x < width - board; x++) {
                // ��9�ֽ�������ľ�ֵ���䷽��
                for(int k = 0; k < 3; k++) {
                    // a.��Χ8�ڽ�
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

                    // b.��7�ڽ�
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


                    // c.��7�ڽ�
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

                    // d.��7�ڽ�
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

                    // e.��7�ڽ�
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

                    // f.����7�ڽ�
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

                    // g.����7�ڽ�
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

                    // h.����7�ڽ�
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

                    // i.����7�ڽ�
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

                    // �󷽲���С�Ľ�����������
                    int minVar = var[0];
                    int minIndex = 0;
                    for (int i = 1; i < 9; i++) {
                        if (minVar > var[i]) {
                            minVar = var[i];
                            minIndex = i;
                        }
                    }

                    // ��������С�������ֵ�������������Ϊ�����ͼ�������ֵ
                    p_temp[lineBytes * y + x * 3 + k] = (int)(mean[minIndex] + 0.5);  
                }
            }
        }
    }

    memcpy(p_data, p_temp, lineBytes * height);
    delete p_temp;
}
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
/*�������ƣ�MakeGray()                                        */
/*�������ͣ�void                                               */
/*���ܣ����ɫת����256ɫ�Ҷ�ͼ��                            */
/***************************************************************/
void MakeColorDib::MakeGray()
{
    ASSERT(this->m_pBitmapInfoHeader->biBitCount == 24);

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x += 3)
        {
            BYTE *pbyBlue = p_data++;   //�õ���ɫֵ
            BYTE *pbyGreen = p_data++;  //�õ���ɫֵ
            BYTE *pbyRed = p_data++;    //�õ���ɫֵ

            //ȡ��ԭr,g,b�е����ֵ��Ϊ����������ֵ����ֵ
            int gray = MAX(*pbyRed, *pbyGreen, *pbyBlue);

            *pbyBlue = gray;     //��ȡ�������ֵ�������ص�������
            *pbyGreen = gray;    //��ȡ�������ֵ�������ص��̷���
            *pbyRed = gray;      //��ȡ�������ֵ�������صĺ����
        }
    }
}

/***************************************************************/
/*�������ƣ�LightAlter(int light)                              */
/*�������ͣ�void                                               */
/*������int light���û���������ֵ                              */
/*���ܣ���ͼ��ʹ����ֵ���������ȵ�����                         */
/***************************************************************/
void MakeColorDib::LightAlter(int light)
{
    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x++)
        {
            //������ǰ�������
            int temp = int(*p_data * light / 100); 
            
            //�жϷ�Χ��ȡ�ú����ֵ
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
/*�������ƣ�LightReverse()                                     */
/*�������ͣ�void                                               */
/*���ܣ�ͼ�������ȡ����                                       */
/***************************************************************/
void MakeColorDib::LightReverse()
{
    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x++)
        {
           *p_data++ = 255 - *p_data;
        }
    }
}

/***************************************************************/
/*�������ƣ�ContrastAlter(int increment)                       */
/*�������ͣ�void                                               */
/*������int increment���û���������ֵ                          */
/*���ܣ���ͼ��ʹ����ֵ�������Աȶȴ���                       */
/***************************************************************/
void MakeColorDib::ContrastAlter(int increment)   ///�Աȶȴ���
{
    if (increment == 0) // ���ֶԱȶȲ���
        return;
   
    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    if (increment < 0) // ��С�Աȶ�
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
    else // ���ӶԱȶ�
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
/*�������ƣ�Exposal()                                          */
/*�������ͣ�void                                               */
/*���ܣ�ͼ���ع⴦��                                         */
/***************************************************************/
void MakeColorDib::Exposal()
{
    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

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
/*�������ƣ�PaintColor(int red, int green, int blue)           */
/*�������ͣ�void                                               */
/*������int red��green��blue���û������ĺ�����ֵ               */
/*���ܣ���ͼ��ʹ����ֵ��������ɫ����                         */
/***************************************************************/
void MakeColorDib::PaintColor(int red, int green, int blue)
{
    ASSERT(this->m_pBitmapInfoHeader->biBitCount == 24);

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth; x += 3)
        {
            BYTE *pbyBlue = p_data++;   //�õ���ɫֵ
            BYTE *pbyGreen = p_data++;  //�õ���ɫֵ
            BYTE *pbyRed = p_data++;    //�õ���ɫֵ
            BYTE r = *pbyRed;
            BYTE g = *pbyGreen;
            BYTE b = *pbyBlue;

            //����Ҷ�ֵ gray = 0.3r + 0.59g + 0.11b
            BYTE gray = (BYTE)(((WORD)r * 59 + (WORD)g * 30 + (WORD)b * 11) / 100);

            //��������ֵ������rgb��������
            *pbyBlue = (BYTE)((blue * gray) / 255); 
            *pbyGreen = (BYTE)((green * gray) / 255);
            *pbyRed = (BYTE)((red * gray) / 255);
        }
    }
}

/***************************************************************/
/*�������ƣ�Embossment()                                       */
/*�������ͣ�void                                               */
/*���ܣ�����ͼ�񸡵���Ч����                                 */
/***************************************************************/
void MakeColorDib::Embossment()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth - rgbChannel; x++)
        {
            //ͻ���仯���֣�������ͬ����
            int currIndex = dibWidth * y + x; // ��ǰ����
            int rightIndex = dibWidth * y + x + rgbChannel; // ��һ����
            int temp = p_data[currIndex] - p_data[rightIndex] + 128;

            //�жϷ�Χ��ȡ�ú����ֵ
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
/*�������ƣ�NeonLight()                                        */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ������޺紦��Ч����                               */
/***************************************************************/
void MakeColorDib::NeonLight()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    for (int y = 0; y < height - 1; y++) 
    {
        for (int x = 0; x < dibWidth - rgbChannel; x++)
        {
            //ͻ���仯���֣�������ͬ����
            int currIndex = dibWidth * y + x; // ��ǰ����
            int rightIndex = dibWidth * y + x + rgbChannel; // ��һ����
            int nextIndex = dibWidth * (y + 1) + x; // ��һ����
            int temp = 0;
            temp = (p_data[currIndex] - p_data[rightIndex])
                 * (p_data[currIndex] - p_data[rightIndex])
                 + (p_data[currIndex] - p_data[nextIndex])
                 * (p_data[currIndex] - p_data[nextIndex]);
            temp = 2 * int(sqrt((float)temp));
   
            //�жϷ�Χ��ȡ�ú����ֵ
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
/*�������ƣ�Smoothness()                                       */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��ƽ������                                       */
/***************************************************************/
void MakeColorDib::Smoothness()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���
   
    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][3]; //����(3x3)����
    h[0][0] = 1;  h[0][1] = 1; h[0][2] = 1;
    h[1][0] = 1;  h[1][1] = 1; h[1][2] = 1;
    h[2][0] = 1;  h[2][1] = 1; h[2][2] = 1;

    for (int y = 0; y < height - 2; y++) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            double temp =
                //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
                  h[0][0] * p_data[dibWidth * y + x] 
                + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
                + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
                + h[1][0] * p_data[dibWidth * (y + 1) + x]
                + h[1][1] * p_data[dibWidth * (y + 1) + x + 1 * rgbChannel]
                + h[1][2] * p_data[dibWidth * (y + 1) + x + 2 * rgbChannel]
                //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
                + h[2][0] * p_data[dibWidth * (y + 2) + x]
                + h[2][1] * p_data[dibWidth * (y + 2) + x + 1 * rgbChannel]
                + h[2][2] * p_data[dibWidth * (y + 2) + x + 2 * rgbChannel];

            //ȡ�ܺ͵ĵ�ƽ��ֵ
            p_temp[dibWidth * y + x] = (BYTE)abs(temp / 9);
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*�������ƣ�Spread()                                           */
/*�������ͣ�void                                               */
/*���ܣ�ͼ����ɢ����                                         */
/***************************************************************/
void MakeColorDib::Spread() // ����һЩ����ԣ�ʹͼ�����ͻ�һ��
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���
    
    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    // ��5*5���������ȡ������䵽��ǰλ��
    for (int y = 2; y < height - 2; y++) 
    {
        for (int x = 2 * rgbChannel; x < dibWidth - 2 * rgbChannel; x++)
        {
            int m = rand() % 5 - 2; //ȡ���������
            int n = rand() % 5 - 2; //ȡ���������

            p_temp[dibWidth * y + x] = p_data[dibWidth * (y + n) + (x + m * rgbChannel)];
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*�������ƣ�Sharp()                                            */
/*�������ͣ�void                                               */
/*���ܣ�ͼ���񻯴���                                         */
/***************************************************************/
void MakeColorDib::Sharp()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    for (int y = 0; y < height - 1; y++) 
    {
        for (int x = 0; x < dibWidth - rgbChannel; x++)
        {
            int temp = 0;
            temp = p_data[dibWidth * (height - y - 2) + (x + rgbChannel)] - p_data[dibWidth * (height - y - 1) + x]; // ��Խ������ز�
            temp = p_data[dibWidth * (height - y - 2) + (x + rgbChannel)] + abs(temp) / 4; // ��ϵ�� 1/4
            
            // ֱ�Ӹ�ֵ����ͬ�����س����� 255 
            p_temp[dibWidth * (height - y - 2) + (x + rgbChannel)] = temp; // #1
            //p_temp[dibWidth * (height - y - 2) + (x + rgbChannel)] = (temp > 255) ? 255 : temp; // #2
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*�������ƣ�HighLVBO(int gaoTong)                              */
/*�������ͣ�void                                               */
/*������int gaoTong���û���������ֵ��ѡ�����                  */
/*���ܣ���ͼ��ʹ����ֵ�����и�ͨ�˲���                         */
/***************************************************************/
void MakeColorDib::HighLVBO(int gaoTong)   //��ͨ�˲�
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][3];  //����(3x3)����
    
    if (gaoTong == 1)
    {   //����1��������ͨ��
        h[0][0] =  1;   h[0][1] = -2;  h[0][2] =  1;
        h[1][0] = -2;  h[1][1] =   5;  h[1][2] = -2;
        h[2][0] =  1;   h[2][1] = -2;  h[2][2] =  1;
    }
    else if (gaoTong == 2)
    {   //����2���еȸ�ͨ��
        h[0][0] =  0;  h[0][1] = -1; h[0][2] =  0;
        h[1][0] = -1;  h[1][1] =  5; h[1][2] = -1;
        h[2][0] =  0;  h[2][1] = -1; h[2][2] =  0;
    }
    else
    {   //����3��������ͨ��
        h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
        h[1][0] = -1;  h[1][1] =  9; h[1][2] = -1;
        h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1;
    }

    for (int y = height - 1; y >= 2; y--) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            int temp =
                //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
              + h[1][0] * p_data[dibWidth * (y - 1) + x]
              + h[1][1] * p_data[dibWidth * (y - 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y - 1) + x + 2 * rgbChannel]
                //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
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
/*�������ƣ�LowLVBO()                                          */
/*�������ͣ�void                                               */
/*���ܣ�ʵ��ͼ���ͨ�˲�(3x3)��                                */
/***************************************************************/
void MakeColorDib::LowLVBO()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    double h[3][3]; //����(3x3)����
    h[0][0] = 0.1;  h[0][1] = 0.1; h[0][2] = 0.1;
    h[1][0] = 0.1;  h[1][1] = 0.2; h[1][2] = 0.1;
    h[2][0] = 0.1;  h[2][1] = 0.1; h[2][2] = 0.1;

    for (int y = height - 1; y >= 2; y--) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            double temp =
                //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
              + h[1][0] * p_data[dibWidth * (y - 1) + x]
              + h[1][1] * p_data[dibWidth * (y - 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y - 1) + x + 2 * rgbChannel]
               //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
              + h[2][0] * p_data[dibWidth * (y - 2) + x]
              + h[2][1] * p_data[dibWidth * (y - 2) + x + 1 * rgbChannel]
              + h[2][2] * p_data[dibWidth * (y - 2) + x + 2 * rgbChannel];

            p_temp[dibWidth * (y - 1) + x + 1 * rgbChannel] = (BYTE)abs(temp); // ģ��ϵ�� = 1
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*�������ƣ�LowLVBObig()                                       */
/*�������ͣ�void                                               */
/*���ܣ�ʵ��ͼ���ͨ�˲�(5x5)��                                */
/***************************************************************/
void MakeColorDib::LowLVBObig()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[5][5]; //����(5x5)����
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
                //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
              + h[0][3] * p_data[dibWidth * y + x + 3 * rgbChannel]
              + h[0][4] * p_data[dibWidth * y + x + 4 * rgbChannel]
               //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
              + h[1][0] * p_data[dibWidth * (y - 1) + x]
              + h[1][1] * p_data[dibWidth * (y - 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y - 1) + x + 2 * rgbChannel]
              + h[1][3] * p_data[dibWidth * (y - 1) + x + 3 * rgbChannel]
              + h[1][4] * p_data[dibWidth * (y - 1) + x + 4 * rgbChannel]
               //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
              + h[2][0] * p_data[dibWidth * (y - 2) + x]
              + h[2][1] * p_data[dibWidth * (y - 2) + x + 1 * rgbChannel]
              + h[2][2] * p_data[dibWidth * (y - 2) + x + 2 * rgbChannel]
              + h[2][3] * p_data[dibWidth * (y - 2) + x + 3 * rgbChannel]
              + h[2][4] * p_data[dibWidth * (y - 2) + x + 4 * rgbChannel]
              //��Ӧ�ĵ�3�е�ֵ���Ծ����Ӧֵ�������
              + h[3][0] * p_data[dibWidth * (y - 3) + x]
              + h[3][1] * p_data[dibWidth * (y - 3) + x + 1 * rgbChannel]
              + h[3][2] * p_data[dibWidth * (y - 3) + x + 2 * rgbChannel]
              + h[3][3] * p_data[dibWidth * (y - 3) + x + 3 * rgbChannel]
              + h[3][4] * p_data[dibWidth * (y - 3) + x + 4 * rgbChannel]
              //��Ӧ�ĵ�4�е�ֵ���Ծ����Ӧֵ�������
              + h[4][0] * p_data[dibWidth * (y - 4) + x]
              + h[4][1] * p_data[dibWidth * (y - 4) + x + 1 * rgbChannel]
              + h[4][2] * p_data[dibWidth * (y - 4) + x + 2 * rgbChannel]
              + h[4][3] * p_data[dibWidth * (y - 4) + x + 3 * rgbChannel]
              + h[4][4] * p_data[dibWidth * (y - 4) + x + 4 * rgbChannel];
              
            p_temp[dibWidth * (y - 2) + x + 2 * rgbChannel]= (BYTE)abs(temp / 35); // ģ��ϵ�� = 35
        }
    }

    memcpy(p_data, p_temp, size);
    delete []p_temp;
}

/***************************************************************/
/*�������ƣ�ShuiPingGROW()                                     */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��ˮƽ��ǿ��                                       */
/***************************************************************/
void MakeColorDib::ShuiPingGROW()
{
    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][1]; //����(3x1)����
    h[0][0] = -1;  
    h[1][0] = 2; 
    h[2][0] = -1;

    // ���������ض�Ӧ���
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
/*�������ƣ�ChuiZhiGROW()                                      */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��ֱ��ǿ��                                       */
/***************************************************************/
void MakeColorDib::ChuiZhiGROW()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[1][3]; //����(1x3)����
    h[0][0] = -1; 
    h[0][1] = 2;
    h[0][2] = -1;

    // ���������ض�Ӧ���
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
/*�������ƣ�ShuangXiangGROW()                                  */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��˫����ǿ��                                       */
/***************************************************************/
void MakeColorDib::ShuangXiangGROW()    //˫����ǿ
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    int h[3][3]; //����(3x3)����
    h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
    h[1][0] = -1;  h[1][1] =  8; h[1][2] = -1;
    h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1; 

    for (int y = 0; y < height - 2; y++) 
    {
        for (int x = 0; x < dibWidth - 2 * rgbChannel; x++)
        {     
            int temp =
                //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
                h[0][0] * p_data[dibWidth * y + x] 
              + h[0][1] * p_data[dibWidth * y + x + 1 * rgbChannel]
              + h[0][2] * p_data[dibWidth * y + x + 2 * rgbChannel]
                //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
              + h[1][0] * p_data[dibWidth * (y + 1) + x]
              + h[1][1] * p_data[dibWidth * (y + 1) + x + 1 * rgbChannel]
              + h[1][2] * p_data[dibWidth * (y + 1) + x + 2 * rgbChannel]
                //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
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
/*�������ƣ�Mosaic()                                           */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ�����������Ч����                                 */
/***************************************************************/
void MakeColorDib::Mosaic()
{
    int rgbChannel = (this->m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    // �ֿ飬�������5*5����ƽ��ֵ
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

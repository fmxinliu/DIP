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
/*�������ƣ�NeonLight()                                        */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ������޺紦��Ч����                               */
/***************************************************************/
void MakeColorDib::NeonLight()   //�޺紦��
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-4;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-1;i++)    // ÿ��
        {
             int pby_pt=0;
             //������ִ���㷨
             pby_pt=(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-1)*DibWidth+i+3))
                   *(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-1)*DibWidth+i+3))
                   +(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-2)*DibWidth+i))
                   *(*(p_data+(height-j-1)*DibWidth+i)-*(p_data+(height-j-2)*DibWidth+i));
            *(p_temp+(height-j-1)*DibWidth+i)=2*int(sqrt((float)pby_pt));
            //�жϺϷ���
            if(*(p_temp+(height-j-1)*DibWidth+i)<0)
                 *(p_temp+(height-j-1)*DibWidth+i)=0;
            if(*(p_temp+(height-j-1)*DibWidth+i)>255)
                 *(p_temp+(height-j-1)*DibWidth+i)=255;
        }
    }
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;   //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�Smoothness()                                       */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��ƽ������                                       */
/***************************************************************/
void MakeColorDib::Smoothness()   //ƽ������
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    int h[3][3];////����(3x3)����
    h[0][0] = 1;  h[0][1] = 1; h[0][2] = 1;
    h[1][0] = 1;  h[1][1] = 1; h[1][2] = 1;
    h[2][0] = 1;  h[2][1] = 1; h[2][2] = 1;
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-2;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-8;i++)    // ÿ��
        {
            double pby_pt=0;
                    //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
             pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
                    +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
                    +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
                    //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
                    +h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
                    +h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
                    +h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
                    //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
                    +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
                    +h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
                    +h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
            *(p_temp+(height-j-2)*DibWidth+i+3)=abs(int(pby_pt/9));//ȡ�ܺ͵ĵ�ƽ��ֵ
        }
    }
       memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
       delete []p_temp;//ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�Embossment()                                       */
/*�������ͣ�void                                               */
/*���ܣ�����ͼ�񸡵���Ч����                                 */
/***************************************************************/
void MakeColorDib::Embossment()
{
    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���

    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < dibWidth - 3; x++)
        {
            //�����ص�ÿ������ִ���㷨
            int currIndex = dibWidth * y + x;
            int nextIndex = dibWidth * y + x + 3;
            int temp = p_data[currIndex] - p_data[nextIndex] + 128;

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
/*�������ƣ�Spread()                                           */
/*�������ͣ�void                                               */
/*���ܣ�ͼ����ɢ����                                         */
/***************************************************************/
void MakeColorDib::Spread() // ����һЩ����ԣ�ʹͼ�����ͻ�һ��
{
    ASSERT(this->m_pBitmapInfoHeader->biBitCount == 24);

    BYTE *p_data = this->GetData();  //ȡ��ԭͼ��������ָ��
    int width = this->GetWidth();    //ȡ��ԭͼ�����������
    int height = this->GetHeight();  //ȡ��ԭͼ���������߶�
    int dibWidth = this->GetDibWidthBytes(); //ȡ��ԭͼ��ÿ���ֽ���
    
    int size = dibWidth * height;
    BYTE *p_temp = new BYTE[size];

    // ��5*5���������ȡ������䵽��ǰλ��
    for (int y = 2; y < height - 2; y++) 
    {
        for (int x = 2 * 3; x < dibWidth - 2 * 3; x++)
        {
            int m = rand() % 5 - 2; //ȡ���������
            int n = rand() % 5 - 2; //ȡ���������

            p_temp[dibWidth * y + x] = p_data[dibWidth * (y + n) + (x + m * 3)];
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
void MakeColorDib::Sharp()   //ͼ����
{
     BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    BYTE *p_temp=new BYTE[height*DibWidth];
    for(int j=0;j<height-1;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-5;i++)    // ÿ��
        {
            int pby_pt=0;    
            pby_pt= *(p_data+(height-j-2)*DibWidth+i+3)
                   -*(p_data+(height-j-1)*DibWidth+i);
            *(p_temp+(height-j-2)*DibWidth+i+3)=*(p_data+(height-j-2)*DibWidth+i+3)
                                                 +abs(int(pby_pt/4));
            if(*(p_temp+(height-j-2)*DibWidth+i+3)>255)
               *(p_temp+(height-j-2)*DibWidth+i+3)=255;
        }
    }
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�HighLVBO(int m_GaoTong)                            */
/*�������ͣ�void                                               */
/*������int m_GaoTong���û���������ֵ��ѡ�����                */
/*���ܣ���ͼ��ʹ����ֵ�����и�ͨ�˲���                         */
/***************************************************************/
void MakeColorDib::HighLVBO(int m_GaoTong)   //��ͨ�˲�
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    int h[3][3];  ////����(3x3)����
    if(m_GaoTong==1)
    {   //����1��������ͨ��
        h[0][0] =1;   h[0][1] =-2;  h[0][2] =1;
        h[1][0] =-2;  h[1][1] =5;   h[1][2] =-2;
        h[2][0] =1;   h[2][1] =-2;  h[2][2] =1;
    }
    else if(m_GaoTong==2)
    {   //����2���еȸ�ͨ��
        h[0][0] = 0;   h[0][1] = -1; h[0][2] = 0;
        h[1][0] = -1;  h[1][1] =  5; h[1][2] = -1;
        h[2][0] = 0;   h[2][1] = -1; h[2][2] = 0;
    }
    else
    {   //����3��������ͨ��
        h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
        h[1][0] = -1;  h[1][1] =  9; h[1][2] = -1;
        h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1;
    }
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-2;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-8;i++)    // ÿ��
        {
            int pby_pt=0;
            //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
            pby_pt=  h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
                +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
                +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
                //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
                +h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
                +h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
                +h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
                //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
                +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
                +h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
                +h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
            *(p_temp+(height-j-2)*DibWidth+i+3)=abs(pby_pt);
            if(pby_pt>255) //�ж��Ƿ�Խ��
                *(p_temp+(height-j-2)*DibWidth+i+3)=255;
        }
    }
       memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�LowLVBO()                                          */
/*�������ͣ�void                                               */
/*���ܣ�ʵ��ͼ���ͨ�˲�(3x3)��                                */
/***************************************************************/
void MakeColorDib::LowLVBO()   //��ͨ�˲�(3x3)
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    double h[3][3];////����(3x3)����
    h[0][0] = 0.1;  h[0][1] = 0.1; h[0][2] = 0.1;
    h[1][0] = 0.1;  h[1][1] = 0.2; h[1][2] = 0.1;
    h[2][0] = 0.1;  h[2][1] = 0.1; h[2][2] = 0.1;
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-2;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-8;i++)    // ÿ��
        {
            double pby_pt=0;
            //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������    
            pby_pt=  h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
                +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
                +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
                //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
                +h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
                +h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
                +h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
                //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
                +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
                +h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
                +h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
            *(p_temp+(height-j-2)*DibWidth+i+3)=abs(int(pby_pt));
        }
    }
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�LowLVBObig()                                       */
/*�������ͣ�void                                               */
/*���ܣ�ʵ��ͼ���ͨ�˲�(5x5)��                                */
/***************************************************************/
void MakeColorDib::LowLVBObig()   //��ͨ�˲�(5x5)
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    int h[5][5];//����(5x5)����
    h[0][0] = 1;  h[0][1] = 1; h[0][2] = 1; h[0][3] = 1; h[0][4] = 1;
    h[1][0] = 1;  h[1][1] = 2; h[1][2] = 2; h[1][3] = 2; h[1][4] = 1;
    h[2][0] = 1;  h[2][1] = 2; h[2][2] = 3; h[2][3] = 2; h[2][4] = 1;
    h[3][0] = 1;  h[3][1] = 2; h[3][2] = 2; h[3][3] = 2; h[3][4] = 1;
    h[4][0] = 1;  h[4][1] = 1; h[4][2] = 1; h[4][3] = 1; h[4][4] = 1;
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-4;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-14;i++)    // ÿ��
        {
            int pby_pt=0;
            //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
            pby_pt=h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
                  +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
                  +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
                  +h[0][3]*(*(p_data+(height-j-1)*DibWidth+i+9))
                  +h[0][4]*(*(p_data+(height-j-1)*DibWidth+i+12))
                  //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
                  +h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
                  +h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
                  +h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
                  +h[1][3]*(*(p_data+(height-j-2)*DibWidth+i+9))
                  +h[1][4]*(*(p_data+(height-j-2)*DibWidth+i+12))
                  //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
                  +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
                  +h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
                  +h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6))
                  +h[2][3]*(*(p_data+(height-j-3)*DibWidth+i+9))
                  +h[2][4]*(*(p_data+(height-j-3)*DibWidth+i+12))
                  //��Ӧ�ĵ�3�е�ֵ���Ծ����Ӧֵ�������
                  +h[3][0]*(*(p_data+(height-j-4)*DibWidth+i))
                  +h[3][1]*(*(p_data+(height-j-4)*DibWidth+i+3))
                  +h[3][2]*(*(p_data+(height-j-4)*DibWidth+i+6))
                  +h[3][3]*(*(p_data+(height-j-4)*DibWidth+i+9))
                  +h[3][4]*(*(p_data+(height-j-4)*DibWidth+i+12))
                  //��Ӧ�ĵ�4�е�ֵ���Ծ����Ӧֵ�������
                  +h[4][0]*(*(p_data+(height-j-5)*DibWidth+i))
                  +h[4][1]*(*(p_data+(height-j-5)*DibWidth+i+3))
                  +h[4][2]*(*(p_data+(height-j-5)*DibWidth+i+6))
                  +h[4][3]*(*(p_data+(height-j-5)*DibWidth+i+9))
                  +h[4][4]*(*(p_data+(height-j-5)*DibWidth+i+12));
                  //Ϊ�˼��㷽�����ǰѳ���35������Ȩ�ͣ��������ܺ�֮��
            *(p_temp+(height-j-3)*DibWidth+i+6)=abs(int(pby_pt/35));
        }
    }
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�ShuiPingGROW()                                     */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��ˮƽ��ǿ��                                       */
/***************************************************************/
void MakeColorDib::ShuiPingGROW()   //ˮƽ��ǿ
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    int h[3][1];//����(3x1)����
    h[0][0] = -1;  
    h[1][0] = 2; 
    h[2][0] = -1;
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-2;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-8;i++)    // ÿ��
        {
            int pby_pt=0;
            //��Ӧ��3�е�ֵ�˷ֱ��Ծ����Ӧֵ�������
            pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
                +h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
                +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i));
            if(pby_pt>20)
                *(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt)+100;
            else
                *(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt);
        }
    }
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�ChuiZhiGROW()                                      */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��ֱ��ǿ��                                       */
/***************************************************************/
void MakeColorDib::ChuiZhiGROW()   //��ֱ��ǿ
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    int h[1][3];//����(1x3)����
    h[0][0] = -1; 
    h[0][1] = 2;
    h[0][2] = -1;
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-2;j++)    // ÿ��
    {
        for(int i=0;i<DibWidth-8;i++)    // ÿ��
        {
            int pby_pt=0;
            //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
            pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
                +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
                +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6));
            if(pby_pt>20)
                *(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt)+100;
            else
                *(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt);
        }
    }
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�ShuangXiangGROW()                                  */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ��˫����ǿ��                                       */
/***************************************************************/
void MakeColorDib::ShuangXiangGROW()    //˫����ǿ
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    int h[3][3];//����(3x3)����
    h[0][0] = -1;  h[0][1] = -1; h[0][2] = -1;
    h[1][0] = -1;  h[1][1] =  8; h[1][2] = -1;
    h[2][0] = -1;  h[2][1] = -1; h[2][2] = -1; 
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-2;j++)    // ÿ��
    {    
        for(int i=0;i<DibWidth-8;i++)    // ÿ��
        {
            int pby_pt=0;
            //��Ӧ�ĵ�0�е�ֵ���Ծ����Ӧֵ�������
            pby_pt= h[0][0]*(*(p_data+(height-j-1)*DibWidth+i))
                +h[0][1]*(*(p_data+(height-j-1)*DibWidth+i+3))
                +h[0][2]*(*(p_data+(height-j-1)*DibWidth+i+6))
                //��Ӧ�ĵ�1�е�ֵ���Ծ����Ӧֵ�������
                +h[1][0]*(*(p_data+(height-j-2)*DibWidth+i))
                +h[1][1]*(*(p_data+(height-j-2)*DibWidth+i+3))
                +h[1][2]*(*(p_data+(height-j-2)*DibWidth+i+6))
                //��Ӧ�ĵ�2�е�ֵ���Ծ����Ӧֵ�������
                +h[2][0]*(*(p_data+(height-j-3)*DibWidth+i))
                +h[2][1]*(*(p_data+(height-j-3)*DibWidth+i+3))
                +h[2][2]*(*(p_data+(height-j-3)*DibWidth+i+6));
            if(pby_pt>20)
                *(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt)+100;
            else
                *(p_temp+(height-j-2)*DibWidth+i)=abs(pby_pt);
        }
    }    
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

/***************************************************************/
/*�������ƣ�Mosaic()                                           */
/*�������ͣ�void                                               */
/*���ܣ�ʹͼ�����������Ч����                                 */
/***************************************************************/
void MakeColorDib::Mosaic()    //������
{
    int m;
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    BYTE *p_temp=new BYTE[height*DibWidth];    // ��ʱ�����ڴ棬�Ա�����ͼ��
    for(int j=0;j<height-4;j+=5)    // ÿ��
    {    
        for(int i=0;i<DibWidth-14;i+=15)    // ÿ��
        {   //��Ӧ��Χ(5x5)������ɫֵ���ƽ��
            int pby_pt=0;
            for(int m=0;m<5;m++)
                for(int n=0;n<15;n+=3)
                {   
                    pby_pt+=*(p_data+(height-j-1-m)*DibWidth+i+n);
                }
                
            for(m=0;m<5;m++)
                for(int n=0;n<14;n+=3)
                {
                    *(p_temp+(height-j-1-m)*DibWidth+i+n)=int(pby_pt/25);
                }    
            //��Ӧ��Χ(5x5)������ɫֵ���ƽ��
            pby_pt=0;
            for(m=0;m<5;m++)
                for(int n=0;n<15;n+=3)
                {
                    pby_pt+=*(p_data+(height-j-1-m)*DibWidth+i+n+1);
                }
            for(m=0;m<5;m++)
                for(int n=0;n<14;n+=3)
                {
                    *(p_temp+(height-j-1-m)*DibWidth+i+n+1)=int(pby_pt/25);
                }
            //��Ӧ��Χ(5x5)�����ɫֵ���ƽ��
            pby_pt=0;
            for(m=0;m<5;m++)
                for(int n=0;n<15;n+=3)
                {
                    pby_pt+=*(p_data+(height-j-1-m)*DibWidth+i+n+2);
                }
            for(m=0;m<5;m++)
                for(int n=0;n<14;n+=3)
                {
                    *(p_temp+(height-j-1-m)*DibWidth+i+n+2)=int(pby_pt/25);
                }
        }            
    }
    memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
    delete []p_temp;  //ɾ����ʱ�����ڴ�
}

#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "WeiFenDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
#include "CDIB.h"

WeiFenDib::WeiFenDib()
{
}

WeiFenDib::~WeiFenDib()
{
}

/***************************************************************/           
/*�������ƣ�ZongXiang()                                        */
/*�������ͣ�void                                               */
/*���ܣ���ͼ���������΢��                                     */
/***************************************************************/
void WeiFenDib::ZongXiang()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = 1; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            p_temp[lineBytes * y + x] = abs(p_data[lineBytes * y + x] - p_data[lineBytes * (y - 1) + x]);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

/***************************************************************/           
/*�������ƣ�HengXiang()                                        */
/*�������ͣ�void                                               */                         
/*���ܣ���ͼ����к���΢��                                     */
/***************************************************************/
void WeiFenDib::HengXiang()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = 0; y < height; y++) {
        for (int x = border; x < lineBytes; x++) {
            // ��24λ���ɫ��RGB��ͨ���ֱ��Ӧ΢�ּ���
            p_temp[lineBytes * y + x] = abs(p_data[lineBytes * y + x] - p_data[lineBytes * y + (x - border)]);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

/***************************************************************/        
/*�������ƣ�ShuangXiang()                                      */
/*�������ͣ�void                                               */                   
/*���ܣ���ͼ�����˫��һ��΢��                                 */
/***************************************************************/
void WeiFenDib::ShuangXiang()
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
            p_temp[lineBytes * y + x] = (int)sqrt((float)(deltaX * deltaX + deltaY * deltaY));
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}


/***************************************************************/           
/*�������ƣ�ErCi1()                                            */
/*�������ͣ�void                                               */         
/*���ܣ���ͼ����ж���΢��                                     */
/*�������£�       +1  -2  +1                                  */
/*                 -2  +4  -2                                  */
/*                 +1  -2  +1                                  */
/*                                                             */
/***************************************************************/
void WeiFenDib::ErCi1()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    // 3*3ģ�壬ȥ���߽�����
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < lineBytes - 1; x++) {
            p_temp[lineBytes * y + x] = abs(
                    (1)  * p_data[lineBytes * (y - 1) + (x - border)]
                +   (-2) * p_data[lineBytes * (y - 1) + (x)]
                +   (1)  * p_data[lineBytes * (y - 1) + (x + border)]
                +   (-2) * p_data[lineBytes *    y    + (x - border)]
                +   (4)  * p_data[lineBytes *    y    + (x)]
                +   (-2) * p_data[lineBytes *    y    + (x + border)]
                +   (1)  * p_data[lineBytes * (y + 1) + (x - border)]
                +   (-2) * p_data[lineBytes * (y + 1) + (x)]
                +   (1)  * p_data[lineBytes * (y + 1) + (x + border)]
                );
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

/***************************************************************/           
/*�������ƣ�ErCi2()                                            */
/*�������ͣ�void                                               */            
/*���ܣ���ͼ����ж���΢��                                     */
/*�������£�    +1  +1  +1                                     */
/*              +1  -8  +1                                     */
/*              +1  +1  +1                                     */
/*                                                             */
/***************************************************************/
void WeiFenDib::ErCi2()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    // 3*3ģ�壬ȥ���߽�����
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < lineBytes - 1; x++) {
            p_temp[lineBytes * y + x] = abs(
                (1)  * p_data[lineBytes * (y - 1) + (x - border)]
            +   (1)  * p_data[lineBytes * (y - 1) + (x)]
            +   (1)  * p_data[lineBytes * (y - 1) + (x + border)]
            +   (1)  * p_data[lineBytes *    y    + (x - border)]
            +   (-8) * p_data[lineBytes *    y    + (x)]
            +   (1)  * p_data[lineBytes *    y    + (x + border)]
            +   (1)  * p_data[lineBytes * (y + 1) + (x - border)]
            +   (1)  * p_data[lineBytes * (y + 1) + (x)]
            +   (1)  * p_data[lineBytes * (y + 1) + (x + border)]
            );
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

/***************************************************************/           
/*�������ƣ�ErCi3()                                            */
/*�������ͣ�void                                               */
/*����ֵ������ɹ�����TRUE;����ʧ�ܷ���FALSE��                 */        
/*���ܣ���ͼ����ж���΢��                                     */
/*�������£�    +1+1+1                                         */
/*              +1+1+1                                         */
/*              +1+1+1                                         */
/*        +1+1+1-4-4-4 +1+1+1                                  */
/*        +1+1+1-4-4-4 +1+1+1                                  */
/*        +1+1+1-4-4-4 +1+1+1                                  */
/*              +1+1+1                                         */
/*              +1+1+1                                         */
/*              +1+1+1                                         */
/*                                                             */
/***************************************************************/
void WeiFenDib::ErCi3()
{
    LPBYTE  p_data;     //ԭͼ������ָ��
    int wide,height;//ԭͼ������
    p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
    if (m_pBitmapInfoHeader->biBitCount<9)        //�Ҷ�ͼ��
    {        
        int x,y;   //ѭ������
        int cent3x3,up3x3,down3x3,right3x3,left3x3;   //���ĵ����������ĸ�������ĺ�    
        BYTE**  temp1=new BYTE*  [height+8];
        BYTE**    temp2=new BYTE*  [height];
        for (y=0;y<height;y++)
            temp2[y]=new BYTE [wide];
        for (y=0;y<height+8;y++)
             temp1[y]=new BYTE [wide+8];
        for (y=0;y<height;y++)
            for(x=0;x<wide;x++)
                temp2[y][x]=0;
           for (y=0;y<height+8;y++)
            for(x=0;x<wide+8;x++)
                temp1[y][x]=0;
        for (y=0;y<height;y++)
            for(x=0;x<wide;x++)
                temp1[y+4][x+4]=p_data[wide*y+x];
        for (y=4;y<height+4;y++)
            for(x=4;x<wide+4;x++)
            {
                cent3x3=-4* temp1[y-1][x-1]-4* temp1[y-1][x]-4* temp1[y-1][x+1]-4* temp1[y][x-1]-4* temp1[y][x]-4* temp1[y][x+1]-4* temp1[y+1][x-1]-4* temp1[y+1][x]-4* temp1[y+1][x+1];
                up3x3= temp1[y-4][x-1]+ temp1[y-4][x]+ temp1[y-4][x+1]+ temp1[y-3][x-1]+ temp1[y-3][x]+ temp1[y-3][x+1]+ temp1[y-2][x-1]+ temp1[y-2][x]+ temp1[y-2][x+1];
                down3x3= temp1[y+2][x-1]+ temp1[y+2][x]+ temp1[y+2][x+1]+ temp1[y+3][x-1]+ temp1[y+3][x]+ temp1[y+3][x+1]+ temp1[y+4][x-1]+ temp1[y+4][x]+ temp1[y+4][x+1];
                right3x3=temp1[y-1][x+2]+ temp1[y-1][x+3]+ temp1[y-1][x+4]+ temp1[y][x+2]+ temp1[y][x+3]+ temp1[y][x+4]+ temp1[y+1][x+2]+ temp1[y+1][x+3]+ temp1[y+1][x+4];
                left3x3=temp1[y-1][x-4]+ temp1[y-1][x-3]+ temp1[y-1][x-2]+ temp1[y][x-4]+ temp1[y][x-3]+ temp1[y][x-2]+ temp1[y+1][x-4]+ temp1[y+1][x-3]+ temp1[y+1][x-2];
                temp2[y-4][x-4]=abs(cent3x3+up3x3+down3x3+right3x3+left3x3);
            }
        for (y=0;y<height;y++)
             for(x=0;x<wide;x++)
                p_data[wide*y+x]=temp2[y][x];
    }
    else     //24λ��ɫ
    {    
        int DibWidth;    //ԭͼ������    
        DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
        BYTE *p_temp=new BYTE[height*DibWidth];
        for(int j=1;j<height-2;j++)    // ÿ��
        {
            for(int i=3;i<DibWidth-8;i++)    // ÿ��
            {
                int pby_pt=0;
                pby_pt=*(p_data+(height-j)*DibWidth+i-3)-2*(*(p_data+(height-j)*DibWidth+i))
                    +*(p_data+(height-j)*DibWidth+i+3)-2*(*(p_data+(height-j-1)*DibWidth+i-3))
                    +4*(*(p_data+(height-j-1)*DibWidth+i))-2*(*(p_data+(height-j-1)*DibWidth+i+3))
                    +*(p_data+(height-j-2)*DibWidth+i-3)-2*(*(p_data+(height-j-2)*DibWidth+i))
                    +*(p_data+(height-j-2)*DibWidth+i+3);
                *(p_temp+(height-j-1)*DibWidth+i)=abs(pby_pt);
            }
        }
        memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
        delete []p_temp;  //ɾ����ʱ�����ڴ�
        BYTE *p_temp1=new BYTE[height*DibWidth];
        for(int a=1;a<height-2;a++)    // ÿ��
        {
            for(int b=3;b<DibWidth-8;b++)    // ÿ��
            {
                int pby_pt=0;
                pby_pt=*(p_data+(height-a)*DibWidth+b-3)+*(p_data+(height-a)*DibWidth+b)
                    +*(p_data+(height-a)*DibWidth+b+3)+*(p_data+(height-a-1)*DibWidth+b-3)
                    -8*(*(p_data+(height-a-1)*DibWidth+b))+*(p_data+(height-a-1)*DibWidth+b+3)
                    +*(p_data+(height-a-2)*DibWidth+b-3)+*(p_data+(height-a-2)*DibWidth+b)
                    +*(p_data+(height-a-2)*DibWidth+b+3);
                *(p_temp1+(height-a-1)*DibWidth+b)=abs(pby_pt);
            }
        }
        memcpy(p_data,p_temp,height*DibWidth);  // ���ƴ�����ͼ��
        delete []p_temp1;  //ɾ����ʱ�����ڴ�
    }
}
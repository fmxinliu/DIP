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
/*函数名称：ZongXiang()                                        */
/*函数类型：void                                               */
/*功能：对图像进行纵向微分                                     */
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
/*函数名称：HengXiang()                                        */
/*函数类型：void                                               */                         
/*功能：对图像进行横向微分                                     */
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
            // 对24位真彩色，RGB三通道分别对应微分计算
            p_temp[lineBytes * y + x] = abs(p_data[lineBytes * y + x] - p_data[lineBytes * y + (x - border)]);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

/***************************************************************/        
/*函数名称：ShuangXiang()                                      */
/*函数类型：void                                               */                   
/*功能：对图像进行双向一次微分                                 */
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
/*函数名称：ErCi1()                                            */
/*函数类型：void                                               */         
/*功能：对图像进行二次微分                                     */
/*算子如下：       +1  -2  +1                                  */
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

    // 3*3模板，去除边界像素
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
/*函数名称：ErCi2()                                            */
/*函数类型：void                                               */            
/*功能：对图像进行二次微分                                     */
/*算子如下：    +1  +1  +1                                     */
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

    // 3*3模板，去除边界像素
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
/*函数名称：ErCi3()                                            */
/*函数类型：void                                               */
/*返回值：处理成功返回TRUE;处理失败返回FALSE。                 */        
/*功能：对图像进行二次微分                                     */
/*算子如下：    +1+1+1                                         */
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
    LPBYTE  p_data;     //原图数据区指针
    int wide,height;//原图长、宽
    p_data=this->GetData ();
    wide=this->GetWidth ();
    height=this->GetHeight ();
    if (m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
    {        
        int x,y;   //循环变量
        int cent3x3,up3x3,down3x3,right3x3,left3x3;   //中心的上下左右四个区域积的和    
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
    else     //24位彩色
    {    
        int DibWidth;    //原图长、宽    
        DibWidth=this->GetDibWidthBytes();   //取得原图的每行字节数
        BYTE *p_temp=new BYTE[height*DibWidth];
        for(int j=1;j<height-2;j++)    // 每行
        {
            for(int i=3;i<DibWidth-8;i++)    // 每列
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
        memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
        delete []p_temp;  //删除暂时分配内存
        BYTE *p_temp1=new BYTE[height*DibWidth];
        for(int a=1;a<height-2;a++)    // 每行
        {
            for(int b=3;b<DibWidth-8;b++)    // 每列
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
        memcpy(p_data,p_temp,height*DibWidth);  // 复制处理后的图像
        delete []p_temp1;  //删除暂时分配内存
    }
}
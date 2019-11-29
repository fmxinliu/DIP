#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BingXingBianJieDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
BingXingBianJieDib::BingXingBianJieDib()
{
}
BingXingBianJieDib::~BingXingBianJieDib()
{
}

// 彩色图像灰度化
void BingXingBianJieDib::RgbToGray()
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

void BingXingBianJieDib::Lunkuotiqu2(int T)
{
    this->RgbToGray();
    //this->Lunkuotiqu(T);

    int width = this->GetWidth(); // 原图宽度
    int height = this->GetHeight(); // 原图高度
    int lineBytes = this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //原图数据区指针
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    // 二值化
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // 判断黑点周围8领域是否全为黑，如果全黑则置白
    for (int y = 1; y < height - 1; y++) {
        for (int x = 3; x < width - 3; x++) {
            if (p_data[lineBytes * y + x * 3] == 0) {
                int n1 = p_data[lineBytes * (y - 1) + (x - 3) * 3];
                int n2 = p_data[lineBytes * (y - 1) + (x) * 3];
                int n3 = p_data[lineBytes * (y - 1) + (x + 3) * 3];
                int n4 = p_data[lineBytes * (y) + (x - 3) * 3];
                int n5 = p_data[lineBytes * (y) + (x + 3) * 3];
                int n6 = p_data[lineBytes * (y + 1) + (x - 3) * 3];
                int n7 = p_data[lineBytes * (y + 1) + (x) * 3];
                int n8 = p_data[lineBytes * (y + 1) + (x + 3) * 3];

                if (n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 == 0) {
                    p_temp[lineBytes * y + x * 3] = 255; // 掏空内部点
                    p_temp[lineBytes * y + x * 3 + 1] = 255;
                    p_temp[lineBytes * y + x * 3 + 2] = 255;
                }
                else {
                    p_temp[lineBytes * y + x * 3] = 0;
                    p_temp[lineBytes * y + x * 3 + 1] = 0;
                    p_temp[lineBytes * y + x * 3 + 2] = 0;
                }
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Lunkuotiqu(int T)                                      
/*函数类型：void  
/*函数参数：int T 二值化阈值
/*功能：对图像进行轮廓提取（对二值图，采用掏空内部点法）。            
/***************************************************************/ 
void BingXingBianJieDib::Lunkuotiqu(int T)
{
    int width = this->GetWidth(); // 原图宽度
    int height = this->GetHeight(); // 原图高度
    int lineBytes = this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //原图数据区指针
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    width = GetRGB() ? width : lineBytes;

    // 二值化
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // 判断黑点周围8领域是否全为黑，如果全黑则置白
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (p_data[lineBytes * y + x] == 0) {
                int n1 = p_data[lineBytes * (y - 1) + (x - 1)];
                int n2 = p_data[lineBytes * (y - 1) + (x)];
                int n3 = p_data[lineBytes * (y - 1) + (x + 1)];
                int n4 = p_data[lineBytes * (y) + (x - 1)];
                int n5 = p_data[lineBytes * (y) + (x + 1)];
                int n6 = p_data[lineBytes * (y + 1) + (x - 1)];
                int n7 = p_data[lineBytes * (y + 1) + (x)];
                int n8 = p_data[lineBytes * (y + 1) + (x + 1)];
                
                if (n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 == 0)
                    p_temp[lineBytes * y + x] = 255; // 掏空内部点
                else
                    p_temp[lineBytes * y + x] = 0;
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Lunkuogenzong()                                      
/*函数类型：void      
/*函数参数：int T 二值化阈值
/*功能：对图像进行边界跟踪。            
/***************************************************************/ 
void BingXingBianJieDib::Lunkuogenzong(int T)
{
    if (GetRGB() == NULL)
        return;

    int width = this->GetWidth(); // 原图宽度
    int height = this->GetHeight(); // 原图高度
    int lineBytes = this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //原图数据区指针
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    ////width = GetRGB() ? width : lineBytes;

    // 二值化
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // 八个方向和起始扫描方向
    const int direction[8][2]={
        {-1, 1},  // 左下
        {0, 1},   // 下
        {1, 1},   // 右下
        {1, 0},   // 右
        {1, -1},  // 右上
        {0, -1},  // 上
        {-1, -1}, // 左上 
        {-1, 0}   // 左
    };

    Point startPoint; // 起始边界点

    // 1.先找到最左上方的边界点
    bool bFindStartPoint = false;
    for (int y = 0; y < height && !bFindStartPoint; y++) {
        for (int x = 0; x < width && !bFindStartPoint; x++) {
            if (p_data[lineBytes * y + x] == 0) {
                p_temp[lineBytes * y + x] = 0;
                bFindStartPoint = true;
                startPoint.Width = x;
                startPoint.Height = y;
            }
        }
    }

    // 2.由于起始点是在左下方，故起始扫描沿左上方向
    int beginDirect = 0;

    // 从起始点开始扫描
    Point pt;
    Point currentPoint = startPoint;
    while (true) {
        // 沿扫描方向查看一个像素
        pt.Width = currentPoint.Width  + direction[beginDirect][0];
        pt.Height = currentPoint.Height + direction[beginDirect][1];
        if (pt.Width < 0 || pt.Width >= width || pt.Height < 0 || pt.Height >= height) {
            // 扫描方向顺时针旋转一格
            beginDirect++;
            if(beginDirect == 8)
                beginDirect = 0;
            continue;
        }
 
        int pixel = p_data[lineBytes * pt.Height + pt.Width]; 
        if (pixel == 0) {
            currentPoint = pt;
            p_temp[lineBytes * pt.Height + pt.Width] = 0; // 边界点
            if(currentPoint.Height == startPoint.Height && currentPoint.Width == startPoint.Width)
                break;

            // 扫描的方向逆时针旋转两格
            beginDirect--;
            if(beginDirect == -1)
                beginDirect = 7;
            beginDirect--;
            if(beginDirect == -1)
                beginDirect = 7;
        }
        else {
            // 扫描方向顺时针旋转一格
            beginDirect++;
            if(beginDirect == 8)
                beginDirect = 0;
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp; 
}

///***************************************************************/           
/*函数名称：Zhongzitianchong(CPoint SeedPoint)                                      
/*函数类型：void
/*参数说明：SeedPoint  ---选区种子点                                     
/*功能：对图像进行种子填充。            
/***************************************************************/ 
void BingXingBianJieDib::Zhongzitianchong(CPoint SeedPoint)
{
    // 指向源图像的指针
    LPBYTE    lpSrc;
    //图像的高和宽
    int height,wide ;
    //像素值
    long pixel;
    //种子堆栈及指针
    Seed *Seeds;
    int StackPoint;
    LPBYTE  temp;
    //当前像素位置
    int iCurrentPixelx,iCurrentPixely;
    temp =this->GetData();
    lpSrc=temp;
    if(m_pBitmapInfoHeader->biBitCount<9)    //灰度图像
    {
        height=this->GetHeight();
        wide=this->GetWidth();         
        for(int  j=0;j<height;j++)
        {
            for(int  i=0;i<wide;i++)
            {
                if(*lpSrc>110)
                    *lpSrc=255;
                else
                    *lpSrc=0; 
                lpSrc++;
            }
        }         
        //初始化种子
        Seeds = new Seed[wide*height];
        Seeds[1].Height = SeedPoint.y;
        Seeds[1].Width = SeedPoint.x;
        StackPoint = 1;
        while( StackPoint != 0)
        {
            //取出种子
            iCurrentPixelx = Seeds[StackPoint].Width;
            iCurrentPixely = Seeds[StackPoint].Height;
            StackPoint--;
            lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx;
            //取得当前指针处的像素值，注意要转换为unsigned char型
            pixel =  *lpSrc;     
            //将当前点涂黑
            *lpSrc =0;
            //判断左面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixelx > 0)
            {
                lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx - 1;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely;
                    Seeds[StackPoint].Width = iCurrentPixelx - 1;
                }
            }
            //判断上面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixely < height - 1)
            {
                lpSrc = (LPBYTE)temp + wide * (iCurrentPixely + 1) + iCurrentPixelx;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely + 1;
                    Seeds[StackPoint].Width = iCurrentPixelx;
                }
            }
            //判断右面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixelx < wide - 1)
            {
                lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx + 1;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely;
                    Seeds[StackPoint].Width = iCurrentPixelx + 1;
                }
            }
            //判断下面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixely > 0)
            {
                lpSrc = (LPBYTE)temp + wide * (iCurrentPixely - 1) + iCurrentPixelx;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely - 1;
                    Seeds[StackPoint].Width = iCurrentPixelx;
                }         
            }
        }
        //释放堆栈
        delete Seeds;
    }
    else    //24位彩色
    {
        height=this->GetHeight();
        wide=this->GetDibWidthBytes();         
        //初始化种子
        Seeds = new Seed[wide*height];
        Seeds[1].Height = SeedPoint.y;
        Seeds[1].Width = SeedPoint.x*3;
        StackPoint = 1;
        while( StackPoint != 0)
        {
            //取出种子
            iCurrentPixelx = Seeds[StackPoint].Width;
            iCurrentPixely = Seeds[StackPoint].Height;
            StackPoint--;
            lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx;
            //取得当前指针处的像素值，注意要转换为unsigned char型
            pixel =  *lpSrc;     
            //将当前点涂黑
            *lpSrc =0;
            //判断左面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixelx > 0)
            {
                lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx - 1;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely;
                    Seeds[StackPoint].Width = iCurrentPixelx - 1;
                }
            }
            //判断上面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixely < height - 1)
            {
                lpSrc = (LPBYTE)temp + wide * (iCurrentPixely + 1) + iCurrentPixelx;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely + 1;
                    Seeds[StackPoint].Width = iCurrentPixelx;
                }         
            }
            //判断右面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixelx < wide - 1)
            {
                lpSrc = (LPBYTE)temp + wide * iCurrentPixely + iCurrentPixelx + 1;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely;
                    Seeds[StackPoint].Width = iCurrentPixelx + 1;
                }
            }
            //判断下面的点，如果为白，则压入堆栈
            //注意防止越界
            if(iCurrentPixely > 0)
            {
                lpSrc = (LPBYTE)temp + wide * (iCurrentPixely - 1) + iCurrentPixelx;
                //取得当前指针处的像素值，注意要转换为unsigned char型
                pixel =  *lpSrc;
                if (pixel == 255)
                {
                    StackPoint++;
                    Seeds[StackPoint].Height = iCurrentPixely - 1;
                    Seeds[StackPoint].Width = iCurrentPixelx;
                }         
            }
        }
        //释放堆栈
        delete Seeds;
    }
}

///***************************************************************/           
/*函数名称：Qiyuzengzhang(CPoint point)                                      
/*函数类型：void
/*参数说明：point  ---获得生长点                                     
/*功能：对图像进行区域生长。            
/***************************************************************/
void BingXingBianJieDib::Qiyuzengzhang(CPoint point)
{
    // 循环变量
    int i;
    int j;
    // 指向DIB象素指针
    LPBYTE p_data;
    // 找到DIB图像象素起始位置    
    p_data=GetData();
    // DIB的宽度
    LONG wide = GetWidth();    
    // DIB的高度
    LONG height =GetHeight();
    if(m_pBitmapInfoHeader->biBitCount<9)    //灰度图像
    {
        // 计算种子点一的灰度值
        unsigned char  zhongzi=*(p_data+point.y*wide+point.x);    
        // 对各像素进行灰度转换
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                //获取各颜色分量
                unsigned char temp = *((unsigned char *)p_data + wide * j +i);
                if (abs(temp - zhongzi) < 10)    //当前点同种子一灰度值比较接近
                {                                
                    //将种子一的颜色赋给当前像素             
                    *((unsigned char *)p_data + wide * j + i ) = temp;
                }
                else 
                    *((unsigned char *)p_data + wide * j + i ) =255;
            }
        }
    }
    else    //24位彩色
    {
        // 计算种子点一的灰度值
        int  zhongzi=*(p_data+(height-point.y)*wide*3+point.x*3);
        int   zhongzi2=*(p_data+(height-point.y)*wide*3+point.x*3+1);
        int   zhongzi3=*(p_data+(height-point.y)*wide*3+point.x*3+2);      
        // 对各像素进行灰度转换
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                //获取各颜色分量
                int  temp = *((unsigned char *)p_data + 3*wide * j +i*3);
                int  temp2 = *((unsigned char *)p_data + 3*wide * j +i*3+1);
                int  temp3 = *((unsigned char *)p_data + 3*wide * j +i*3+2);
                if (abs(temp - zhongzi) < 10&&abs(temp2 - zhongzi2) < 10&&abs(temp3 - zhongzi3) < 10)    //当前点同种子一灰度值比较接近
                {                                
                    //将种子一的颜色赋给当前像素             
                    *(p_data + 3*wide * j + i*3 ) = temp;
                    *(p_data +3*wide* j + i*3+1 ) = temp2;
                    *(p_data +3*wide * j + i*3+2 ) = temp3;
                }
                else 
                {
                    *(p_data + 3*wide * j + i*3 ) =255;
                    *(p_data + 3*wide * j + i*3+1 ) =255;
                    *(p_data + 3*wide * j + i*3+2 ) = 255;
                }
            }
        }
    } 
}

///***************************************************************/           
/*函数名称：Lunkuotiqu(CPoint SeedPoint)                                      
/*函数类型：void
/*参数说明：SeedPoint  ---种子点选取                                     
/*功能：对24位彩色图像进行轮廓提取。            
/***************************************************************/
void BingXingBianJieDib::Lunkuotiqu(CPoint SeedPoint)
{
    LPBYTE  p_data ;     //原图数据区指针
    int R,G,B,R1,G1,B1;
    int wide,height;    //原图长、宽
    // 指向源图像的指针
    LPBYTE    lpSrc;    
    // 指向缓存图像的指针
    LPBYTE    lpDst;    
    // 指向缓存DIB图像的指针
    LPBYTE    temp;    
    //循环变量
    int i;
    int j;
    p_data=GetData();
    wide=GetWidth();
    height=GetHeight();
    temp = new BYTE[wide * height*3];
    // 初始化新分配的内存，设定初始值为255
    memset(temp,  255, wide * height*3);
    lpSrc=p_data+wide*3*(height-SeedPoint.y)+3*SeedPoint.x;
    R1=*lpSrc;
    lpSrc++;
    G1=*lpSrc;
    lpSrc++;
    B1=*lpSrc;
    for(j=0;j<height;j++)
    {
        for(i=0;i<wide;i++)
        {
            lpSrc = (LPBYTE)p_data + wide * j*3 + i*3;
            // 指向目标图像倒数第j行，第i个象素的指针            
            lpDst = (LPBYTE)temp + wide * j*3 + i*3;
            int I;
            I=0;
            for(int k=i-1;k<i+2;k++)
            {
                for(int l=j-1;l<j+2;l++)
                {
                    if(k>=0&&l>=0&&k<wide&&l<height)
                    {
                        R=*(p_data+l*wide*3+k*3);
                        G=*(p_data+l*wide*3+k*3+1);
                        B=*(p_data+l*wide*3+k*3+2);
                        if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
                            I++;
                    }
                }
            }
            if(I!=9)
            {    
                *lpDst=*lpSrc;
                *(lpDst+1)=*(lpSrc+1);
                *(lpDst+2)=*(lpSrc+2);
            } 
            else
            {
                *lpDst=255;
                *(lpDst+1)=255;
                *(lpDst+2)=255;
            }
        }
    }
    memcpy(p_data,temp, wide * height*3);
    delete temp;
}

/*************************************************************/
//此算法只适用于左边有边界的图像//
/*************************************************************/
void BingXingBianJieDib::Lunkuogenzong(CPoint SeedPoint)
{
    // 指向源图像的指针
    LPBYTE    lpSrc;
    LPBYTE   p_data ;
    int R1,G1,B1;
    int R,G,B;
    // 指向缓存图像的指针
    LPBYTE    lpDst;
    // 指向缓存DIB图像的指针
    LPBYTE    temp;
    int wide;
    int height; 
    //循环变量
    int i;
    int j;
    //像素值
    int pixel;
    //是否找到起始点及回到起始点
    bool bFindStartPoint;
    //是否扫描到一个边界点
    bool bFindPoint;
    //起始边界点与当前边界点
    Point StartPoint,CurrentPoint;
    //八个方向和起始扫描方向
    int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
    int BeginDirect;
    p_data=GetData();
    wide=this->GetWidth();
    height=this->GetHeight();
    // 暂时分配内存，以保存新图像
    temp = new BYTE[wide*height*3];
    // 初始化新分配的内存，设定初始值为255
    lpDst = temp;
    memset(temp, (BYTE)255, wide * height*3);
    //先找到最左方的边界点
    lpSrc=p_data+wide*3*(height-SeedPoint.y)+3*SeedPoint.x;//确定鼠标点的色度值
    R1=*lpSrc;
    lpSrc++;
    G1=*lpSrc;
    lpSrc++;
    B1=*lpSrc;
    bFindStartPoint = false;
    int s= height-SeedPoint.y;
    for(i=SeedPoint.x;i>1;i--)  //从鼠标点开始向左寻找边界起始点
    {
        lpSrc = (LPBYTE)p_data + wide * (height-SeedPoint.y)*3 + i*3;
        // 指向目标图像倒数第j行，第i个象素的指针            
        lpDst = (LPBYTE)temp + wide * (height-SeedPoint.y)*3 + i*3;
        int count;
        count=0;
        for(int k=i-1;k<i+2;k++)
        {
            for(int l=s-1;l<s+2;l++)
            {
                if(k>=0&&l>=0&&k<wide&&l<height)
                {
                    R=*(p_data+l*wide*3+k*3);
                    G=*(p_data+l*wide*3+k*3+1);
                    B=*(p_data+l*wide*3+k*3+2);
                    if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
                        count++;
                }
            }
        }
        if(count!=9)
        {
            bFindStartPoint = true;
            StartPoint.Height = s;
            StartPoint.Width = i;
            // 指向目标图像倒数第j行，第i个象素的指针            
            lpDst = (LPBYTE)(temp + wide * s*3 + i*3);    
            *lpDst = 0;
            *(lpDst+1)=0;
            *(lpDst+2)=0;
            break;
        } 
    }            
    //由于起始点是在左下方，故起始扫描沿左上方向
    BeginDirect = 0;
    //跟踪边界
    bFindStartPoint = false;
    //从初始点开始扫描
    CurrentPoint.Height = StartPoint.Height;
    CurrentPoint.Width = StartPoint.Width;
    while(!bFindStartPoint)
    {
        bFindPoint = false;
        while(!bFindPoint)
        {
            //沿扫描方向查看一个像素
            lpSrc = (LPBYTE)(p_data + 3*wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
                + 3*(CurrentPoint.Width + Direction[BeginDirect][0]));
            R =*lpSrc;
            G=*(lpSrc+1);
            B=*(lpSrc+2);
            if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
            {
                bFindPoint = true;
                CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
                CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
                if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
                {
                    bFindStartPoint = true;
                }
                lpDst =  (LPBYTE)(temp + 3*wide * CurrentPoint.Height + 3*CurrentPoint.Width);
                *lpDst = *lpSrc;
                *(lpDst+1) = *(lpSrc+1);
                *(lpDst+2) = *(lpSrc+2);
                //扫描的方向逆时针旋转两格
                BeginDirect--;
                if(BeginDirect == -1)
                    BeginDirect = 7;
                BeginDirect--;
                if(BeginDirect == -1)
                    BeginDirect = 7;
            }
            else
            {
                //扫描方向顺时针旋转一格
                BeginDirect++;
                if(BeginDirect == 8)
                    BeginDirect = 0;
            }
        }
    }
    // 复制图像
    memcpy(p_data, temp, 3*wide * height);
    // 释放内存
    delete temp; 
}

///**************************************************************/           
/*函数名称：Fenbutongji(int *huidu)                               
/*函数类型：void                                         
/*变量说明：huidu 灰度分布统计                                                        
/*功能：对图像进行灰度直方图统计。                     
/***************************************************************/
void BingXingBianJieDib::Fenbutongji(int *huidu)
{
#if _DEBUG
    long sum = 0;
    memset(huidu, 0, sizeof(int) * 256);
    for (int i = 0; i < 256; i++)
        sum += huidu[i];
    ASSERT(sum == 0);
#endif

    int width = this->GetWidth();   // 长     
    int height = this->GetHeight(); // 宽
    int lineBytes = this->GetDibWidthBytes(); // 4 字节对齐后的宽度

    LPBYTE p_data = this->GetData(); // 像素指针

    // 对各像素进行灰度统计
    if (this->GetRGB()) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE gray = p_data[lineBytes * y + x];
                huidu[gray]++; // 灰度统计计数
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE b = p_data[lineBytes * y + x * 3];
                BYTE g = p_data[lineBytes * y + x * 3 + 1];
                BYTE r = p_data[lineBytes * y + x * 3 + 2];
                BYTE gray = (BYTE)(0.11 * r + 0.59 * g + 0.3 * b); // 24位彩色图转灰度
                huidu[gray]++; // 灰度统计计数
            }
        }
    }
}

///**************************************************************/           
/*函数名称：Zhifangtu(float *tongji)                               
/*函数类型：void                                         
/*变量说明：tongji 灰度分布密度统计                                                        
/*功能：对图像进行灰度直方图统计。                     
/***************************************************************/
void BingXingBianJieDib::Zhifangtu(float *tongji)
{
    int huidu[256] = {};  // 灰度计数   
    int width = this->GetWidth();   // 长     
    int height = this->GetHeight(); // 宽
    int lineBytes = this->GetDibWidthBytes(); // 4 字节对齐后的宽度

    LPBYTE p_data = this->GetData(); // 像素指针

    // 对各像素进行灰度统计
    if (this->GetRGB()) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE gray = p_data[lineBytes * y + x];
                huidu[gray]++; // 灰度统计计数
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE b = p_data[lineBytes * y + x * 3];
                BYTE g = p_data[lineBytes * y + x * 3 + 1];
                BYTE r = p_data[lineBytes * y + x * 3 + 2];
                BYTE gray = (BYTE)(0.11 * r + 0.59 * g + 0.3 * b); // 24位彩色图转灰度
                huidu[gray]++; // 灰度统计计数
            }
        }
    }
    
    // 计算灰度分布密度
    for(int i = 0; i < 256; i++) {
        tongji[i] = huidu[i] / (height * width * 1.0f);
    }
}

///***************************************************************/           
/*函数名称：BanYuZhi(int Yuzhi)                                      
/*函数类型：void
/*参数说明：Yuzhi  ---阈值选取                                     
/*功能：对图像进行半阈值分割。            
/***************************************************************/
void BingXingBianJieDib::BanYuZhi(int Yuzhi)
{
    this->RgbToGray();

    int width = this->GetWidth(); // 原图宽度
    int height = this->GetHeight(); // 原图高度
    int lineBytes = this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    LPBYTE p_data = this->GetData (); //原图数据区指针

    for (int i = 0; i < lineBytes * height; i++) {
        if(p_data[i] - Yuzhi < 30)
            ;               // 保持不变
        else
            p_data[i] = 255;
    }
}

///***************************************************************/           
/*函数名称：Yuzhifenge(int Yuzhi)                                      
/*函数类型：void
/*参数说明：Yuzhi  ---阈值选取                                     
/*功能：对图像进行阈值分割。            
/***************************************************************/
void BingXingBianJieDib::Yuzhifenge(int Yuzhi)
{
    this->RgbToGray();

    int width = this->GetWidth(); // 原图宽度
    int height = this->GetHeight(); // 原图高度
    int lineBytes = this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    LPBYTE p_data = this->GetData (); //原图数据区指针

    for (int i = 0; i < lineBytes * height; i++) {
        if(abs(p_data[i] - Yuzhi) < 30)
            p_data[i] = Yuzhi; // 前景
        else
            p_data[i] = 255;   // 背景
    }
}

///***************************************************************/           
/*函数名称：Diedaifazhi(int *tongji)                                       
/*函数类型：void                                      
/*功能：对图像进行迭代阀值选取。            
/***************************************************************/ 
void BingXingBianJieDib::Diedaifazhi()
{
    int huidu[256];
    this->Fenbutongji(huidu);  // 直方图灰度值统计
    
    int width = this->GetWidth();   // 长     
    int height = this->GetHeight(); // 宽
    int lineBytes = this->GetDibWidthBytes(); // 4 字节对齐后的宽度
    LPBYTE p_data = this->GetData(); // 像素指针

    int T1 = 127; // 初始阈值
    int T2 = 0; // 迭代阈值

    int Temp0, Temp1, Temp2, Temp3;
    Temp0 = Temp1 = Temp2 = Temp3 = 0;

    while (true)
    {
        // 计算下一个迭代阀值
        for (int i = 0; i < T1 + 1; i++) // 区间1
        {
            Temp0 += huidu[i] * i;
            Temp1 += huidu[i];
        }
        for (int i = T1 + 1; i < 256; i++) // 区间2
        {
            Temp2 += huidu[i] * i;
            Temp3 += huidu[i];
        }
        T2 = (Temp0 / Temp1 + Temp2 / Temp3) / 2;
        // 看迭代结果是否已收敛
        if (T1 == T2)
            break;
        else
            T1 = T2;

        if (T2 < 0)
            break;

        Temp0 = Temp1 = Temp2 = Temp3 = 0;
    }

    this->RgbToGray();

    // 对各像素进行二值化
    for (int k = 0; k < lineBytes * height; k++)
    { 
        if (p_data[k] > T1)
            p_data[k] = 255;
        else
            p_data[k] = 0;
    }
}

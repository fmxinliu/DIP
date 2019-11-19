#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "JiHeBianHuanDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
#define PI 3.1415926535
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

#define RADIAN(angle) ((angle)*PI/180.0) //角度到弧度转化的宏

JiHeBianHuanDib::JiHeBianHuanDib()
{

}

JiHeBianHuanDib::~JiHeBianHuanDib()
{

}

///***************************************************************/           
/*函数名称：JingXiang(bool fangshi)                                        
/*函数类型：void
/*参数说明：fangshi --判断标志为 true 水平镜像，为 false 垂直镜像                                      
/*功能：对图像进行镜像显示。            
/***************************************************************/ 
void JiHeBianHuanDib::JingXiang(bool fangshi)
{
    // 指向源图像的指针
    LPBYTE    lpSrc;
    LPBYTE  p_data;
    // 指向要复制区域的指针
    LPBYTE    lpDst;
    // 指向复制图像的指针
    LPBYTE    temp;
    // 循环变量
    LONG    i;
    LONG    j;
    //图像的高与宽
    long height=this->GetHeight();
    long wide=this->GetWidth();
    p_data=this->GetData();
    // 暂时分配内存，以保存一行图像
    temp = new BYTE[wide*height];
    // 判断镜像方式
    if (fangshi)        // 水平镜像
    {
        if (m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
        {
            // 针对图像每行进行操作
            for(j = 0; j < height; j++)
            {
                for(i = 0; i <  wide;i++)
                {
                    lpSrc = (LPBYTE)p_data +wide *j + i;
                    lpDst = (LPBYTE)temp + wide* j+ wide - i;
                    *lpDst = *lpSrc;
                }
            }
            memcpy(p_data,temp, wide*height);
        }
        else    //24位彩色
        {
            wide=this->GetDibWidthBytes();
            temp=new BYTE[height*wide];
            // 针对图像每行进行操作
            for(j = 0; j < height; j++)
            {
                for(i = 0; i <  wide;i=i+3)
                {
                    lpSrc = (LPBYTE)(p_data +wide *j + i);
                    lpDst = (LPBYTE)temp + wide* j+ wide - i;
                    *(lpDst) = *lpSrc;
                    *(lpDst+1) = *(lpSrc+1);
                    *(lpDst+2) = *(lpSrc+2);
                }            
            }
            memcpy(p_data,temp, wide*height);
        }        
    }
    else        // 垂直镜像
    {
           if (m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
        {
            // 针对上半图像进行操作
            for(i = 0; i <  wide;i++)
            {
                for(j = 0; j < height; j++)
                {
                    lpSrc = (LPBYTE)p_data + wide * j+i;
                    lpDst = (LPBYTE)temp + wide * (height - j - 1)+i;
                    *lpDst = *lpSrc;
                }
            }
            memcpy(p_data,temp, wide*height);           
        }
        else    //24位彩色        
        {
            wide=this->GetDibWidthBytes();
            temp=new BYTE[height*wide];
            // 针对上半图像进行操作
            for(i = 0; i <  wide;i++)
            {
                for(j = 0; j < height; j++)
                {
                    lpSrc = (LPBYTE)p_data + wide * j+i;
                    lpDst = (LPBYTE)temp + wide * (height - j - 1)+i;
                    *lpDst = *lpSrc;
                }
            }
            memcpy(p_data,temp, wide*height);
            // 释放内存
            delete temp;        
        }       
    }
}

///***************************************************************/           
/*函数名称：Zhuanzhi()                                        
/*函数类型：void                                     
/*功能：对图像进行转置处理。            
/***************************************************************/
void JiHeBianHuanDib::Zhuanzhi()
{
    // 图像的宽度和高度
    LONG    wide=this->GetWidth();
    LONG    height=this->GetHeight();
    //图像每行字节数
    LONG lLineBytes=(((wide*8)+31)/32*4);
    //新图像每行字节数
    LONG lNewLineBytes=(((height*8)+31)/32*4);
    // 指向源图像的指针
    LPBYTE    p_data=this->GetData();
    // 指向源象素的指针
    LPBYTE    lpSrc;
    // 指向转置图像对应象素的指针
    LPBYTE    lpDst;
    // 指向转置图像的指针
    LPBYTE    temp;    
    // 循环变量
    LONG    i;
    LONG    j;
    if (m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
    { 
        //分配内存
        temp=new BYTE[wide*lNewLineBytes];
        
        // 针对图像每行进行操作
        for(j = 0; j < height; j++)
        {
            // 针对每行图像每列进行操作
            for(i = 0; i < wide;i++)
            {
                // 指向源DIB第j行，第i个象素的指针
                lpSrc = (LPBYTE)p_data + lLineBytes * j + i;
                // 指向转置DIB第i行，第j个象素的指针
                // 注意此处wide和height是源DIB的宽度和高度，应该互换
                lpDst = (LPBYTE)temp + lNewLineBytes* i + j;
                // 复制象素
                *lpDst = *lpSrc;
            }
        }
    }
    else    //24位彩色
    {
        lLineBytes=(((wide*3*8)+31)/32*4);
        lNewLineBytes=(((height*3*8)+31)/32*4);
        temp=new BYTE[wide*lNewLineBytes];
        // 针对图像每行进行操作
        for(j = 0; j < height; j++)
        {
            // 针对每行图像每列进行操作
            for(i = 0; i < wide;i++)
            {
                // 指向源DIB第j行，第i个象素的指针
                lpSrc = (LPBYTE)p_data + lLineBytes * j + i * 3;
                // 指向转置DIB第i行，第j个象素的指针
                // 注意此处wide和height是源DIB的宽度和高度，应该互换
                lpDst = (LPBYTE)temp + lNewLineBytes* i + j * 3;
                // 复制象素
                lpDst[0] = lpSrc[0]; // B
                lpDst[1] = lpSrc[1]; // G
                lpDst[2] = lpSrc[2]; // R
            }        
        }
    }

    SaveImage(height, wide, temp);
    delete []temp;
}

///***************************************************************/           
/*函数名称：Xuanzhuan(int RotateAngle)                                        
/*函数类型：void
/*参数说明：RotateAngle --旋转角度                                     
/*功能：对图像进行旋转处理。            
/*****************************************************************/
void JiHeBianHuanDib::Xuanzhuan(double rotateAngle)
{
    if (rotateAngle / 360 < 1e-5) {
        return;
    }

    LONG width = GetWidth();
    LONG height = GetHeight();
    
    LONG lineBytes, lineBytesNew;
    LPBYTE temp, lpSrc, lpDst;

    // 以图像中心为原点，建立数学坐标系
    double srcX1 = -0.5 * width, srcY1 =  0.5 * height; // 左上
    double srcX2 =  0.5 * width, srcY2 =  0.5 * height;
    double srcX3 =  0.5 * width, srcY3 = -0.5 * height;
    double srcX4 = -0.5 * width, srcY4 = -0.5 * height; // 左下

    // 角度到弧度的转化
    double angle = RADIAN(rotateAngle);
    double cosa = cos(angle);
    double sina = sin(angle);

    // 计算旋转后四个顶点的坐标
    double dstX1 = srcX1 * cosa - srcY1 * sina;
    double dstY1 = srcX1 * sina + srcY1 * cosa;
    double dstX2 = srcX2 * cosa - srcY2 * sina;
    double dstY2 = srcX2 * sina + srcY2 * cosa;
    double dstX3 = srcX3 * cosa - srcY3 * sina;
    double dstY3 = srcX3 * sina + srcY3 * cosa;
    double dstX4 = srcX4 * cosa - srcY4 * sina;
    double dstY4 = srcX4 * sina + srcY4 * cosa;

    // 由对角线计算旋转后的图像宽、高
    LONG widthNew = max(fabs(dstX1 - dstX3), fabs(dstX2 - dstX4)) + 0.5; // 四舍五入
    LONG heightNew = max(fabs(dstY1 - dstY3), fabs(dstY2 - dstY4)) + 0.5;

    // 通过矩阵运算，将旋转后的图像，再转换回图像坐标系
    double deltaX = -0.5 * widthNew * cosa - 0.5 * heightNew * sina + 0.5 * width; // 逆时针为正
    double deltaY =  0.5 * widthNew * sina - 0.5 * heightNew * cosa + 0.5 * height;

    if (GetRGB()) {
        lineBytes = WIDTHBYTES(width * 8);
        lineBytesNew = WIDTHBYTES(widthNew * 8);
        temp = new BYTE[lineBytesNew * heightNew];
        memset(temp, 0, lineBytesNew * heightNew);

        for (int y = 0; y < heightNew; y++) {
            for (int x = 0; x < widthNew; x++) {
                int x0 = (int)( x * cosa + y * sina + deltaX);
                int y0 = (int)(-x * sina + y * cosa + deltaY);

                // 在原图范围内
                if((x0 >= 0) && (x0 < width) && (y0 >= 0) && (y0 < height)) {
                    lpDst = temp + lineBytesNew * y + x;
                    lpSrc = m_pData + lineBytes * y0 + x0;
                    *lpDst = *lpSrc; // 最邻近插值法
                }
            }
        }

    } else {
        lineBytes = WIDTHBYTES(width * 8 * 3);
        lineBytesNew = WIDTHBYTES(widthNew * 8 * 3);
        temp = new BYTE[lineBytesNew * heightNew];
        memset(temp, 0, lineBytesNew * heightNew);

        for (int y = 0; y < heightNew; y++) {
            for (int x = 0; x < widthNew; x++) {
                int x0 = (int)( x * cosa + y * sina + deltaX);
                int y0 = (int)(-x * sina + y * cosa + deltaY);

                // 在原图范围内
                if((x0 >= 0) && (x0 < width) && (y0 >= 0) && (y0 < height)) {
                    lpDst = temp + lineBytesNew * y + x * 3;
                    lpSrc = m_pData + lineBytes * y0 + x0 * 3;
                    lpDst[0] = lpSrc[0];
                    lpDst[1] = lpSrc[1];
                    lpDst[2] = lpSrc[2];
                }
            }
        }
    }

    SaveImage(widthNew, heightNew, temp);
    delete [] temp;
}

//{
//    DWORD        OffBits,SrcBufSize,DstBufSize,DstLineBytes;
//    LPBYTE       lpTempPtr,lpPtr,lpSrc,lpTemp;
//    double       SrcX1,SrcY1,SrcX2,SrcY2;
//    double       SrcX3,SrcY3,SrcX4,SrcY4;
//    double       DstX1,DstY1,DstX2,DstY2;
//    double       DstX3,DstY3,DstX4,DstY4;
//    DWORD        x0,y0,x1,y1;
//    double       cosa,sina; 
//    double       num1,num2;
//    int          LineBytes;
//    //角度到弧度的转化
//    rotateAngle=(double)RADIAN(rotateAngle);
//    cosa=(double)cos((double)rotateAngle);
//    sina=(double)sin((double)rotateAngle);
//    //原图的宽度和高度
//    lpSrc=this->GetData();
//    int Wold=this->GetWidth();
//    int Hold=this->GetHeight();
//    //原图的四个角的坐标
//    SrcX1=(double)(-0.5*Wold);
//    SrcY1=(double)(0.5*Hold);
//    SrcX2=(double)(0.5*Wold);
//    SrcY2=(double)(0.5*Hold);
//    SrcX3=(double)(-0.5*Wold);
//    SrcY3=(double)(-0.5*Hold);
//    SrcX4=(double)(0.5*Wold);
//    SrcY4=(double)(-0.5*Hold);
//    //新图四个角的坐标
//    DstX1=cosa*SrcX1+sina*SrcY1;
//    DstY1=-sina*SrcX1+cosa*SrcY1;
//    DstX2=cosa*SrcX2+sina*SrcY2;
//    DstY2=-sina*SrcX2+cosa*SrcY2;
//    DstX3=cosa*SrcX3+sina*SrcY3;
//    DstY3=-sina*SrcX3+cosa*SrcY3;
//    DstX4=cosa*SrcX4+sina*SrcY4;
//    DstY4=-sina*SrcX4+cosa*SrcY4;
//    //计算新图的宽度，高度
//    int Wnew = (DWORD)(max(fabs(DstX4-DstX1), fabs(DstX3-DstX2))+0.5);
//    int Hnew = (DWORD)(max(fabs(DstY4-DstY1), fabs(DstY3-DstY2))+0.5);
//    //计算矩阵(2.9)中的两个常数，这样不用以后每次都计算了
//    num1=(double)( -0.5*Wnew*cosa-0.5*Hnew*sina+0.5*Wold);
//    num2=(double)(0.5*Wnew*sina-0.5*Hnew*cosa+0.5*Hold);
//    if(m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
//    {
//        //将新的缓冲区中的每个字节都填成255，这样以后未处理的象素就是白色
//        DstBufSize=Wnew*Hnew;
//        lpTempPtr=new BYTE[Wnew*Hnew];
//        memset(lpTempPtr,(BYTE)255,Wnew*Hnew);
//        lpTemp=lpTempPtr;
//        for(y1=0;y1<Hnew;y1++)
//        {
//            for(x1=0;x1<Wnew;x1++)
//            {
//                //x0,y0为对应的原图上的坐标
//                x0= (DWORD)(x1*cosa+y1*sina+num1);
//                y0= (DWORD)(-1.0f*x1*sina+y1*cosa+num2);
//                if( (x0>=0) && (x0<Wold) && (y0>=0) && (y0<Hold))   //在原图范围内
//                {
//                    lpPtr=lpSrc+y0*Wold+x0;
//                    lpTempPtr=lpTemp+y1*Wnew+x1;
//                    *lpTempPtr=*lpPtr; //进行象素的复制
//                }
//            }
//        }
//        this->SetWidth(Wnew);
//        this->SetHeight(Hnew);
//        this->m_pData=lpTemp;
//    }
//    else    //24位彩色
//    {
//        //将新的缓冲区中的每个字节都填成255，这样以后未处理的象素就是白色
//        DstBufSize=Wnew*Hnew*3;
//        lpTempPtr=new BYTE[Wnew*Hnew*3];
//        memset(lpTempPtr,(BYTE)255,Wnew*Hnew*3);
//        lpTemp=lpTempPtr;
//        for(y1=0;y1<Hnew;y1++)
//        {
//            for(x1=0;x1<Wnew;x1++)
//            {
//                //x0,y0为对应的原图上的坐标
//                x0= (DWORD)(x1*cosa+y1*sina+num1);
//                y0= (DWORD)(-1.0f*x1*sina+y1*cosa+num2);
//                if((x0>=0) && (x0<Wold) && (y0>=0) && (y0<Hold))   //在原图范围内
//                {
//                    lpPtr=lpSrc+y0*Wold*3+x0*3;
//                    lpTempPtr=lpTemp+y1*Wnew*3+x1*3;                    
//                    *lpTempPtr++=*lpPtr++; //进行象素的复制
//                    *lpTempPtr++=*lpPtr++; //进行象素的复制
//                    *lpTempPtr=*lpPtr; //进行象素的复制
//                    lpTempPtr=lpTemp;
//                }
//            }
//        }
//        this->SetWidth(Wnew);
//        this->SetHeight(Hnew);
//        this->m_pData=lpTemp;
//    }
//}

void JiHeBianHuanDib::clearmem2()
{
    LONG temp;
    temp=m_pBitmapInfoHeader->biHeight;
    m_pBitmapInfoHeader->biHeight=m_pBitmapInfoHeader->biWidth;
    m_pBitmapInfoHeader->biWidth=temp;
}

///***************************************************************/           
/*函数名称：FangDa(float xzoom, float yzoom)                                        
/*函数类型：void
/*参数说明：xzoom --水平放大的比率
/*         yzoom --垂直放大的比率                                     
/*功能：对图像进行放大处理。            
/*****************************************************************/
void JiHeBianHuanDib::FangDa(float xzoom, float yzoom)
{
    LONG width = GetWidth();
    LONG height = GetHeight();
    LONG widthNew = width * xzoom + 0.5;
    LONG heightNew = height * yzoom + 0.5;
    LONG lineBytes, lineBytesNew;
    LPBYTE temp, lpSrc, lpDst;

    if (GetRGB()) {
        lineBytes = WIDTHBYTES(width * 8);
        lineBytesNew = WIDTHBYTES(widthNew * 8);
        temp = new BYTE[lineBytesNew * heightNew];

        // 方法三：优化2（临近点插值）
        for (int y = 0; y < heightNew; y++) {
            for (int x = 0; x < widthNew; x++) {
                int x0 = int(x / xzoom + 0.5);
                int y0 = int(y / yzoom + 0.5);

                if((x0 >= 0) && (x0 < width) && (y0 >= 0) && (y0 < height)) {
                    lpDst = temp + lineBytesNew * y  + x;
                    lpSrc = m_pData + lineBytes * y0 + x0;    
                    *lpDst=*lpSrc;
                } 
            }
        }

        //// 方法一：优化1
        //int k1 = (int)(xzoom + 0.5);
        //int k2 = (int)(yzoom + 0.5);
        //if (k1 > 0 && k2 > 0) {
        //    for (int y = 0; y < heightNew; y += k2) {
        //        for (int x = 0; x < widthNew; x += k1) {
        //            int x0 = int(x / xzoom + 0.5);
        //            int y0 = int(y / yzoom + 0.5);
        //            if (x0 < width && y0 < height) {
        //                lpSrc = m_pData + lineBytes * y0 + x0;
        //                for (int j = 0; j < k2; j++) {
        //                    for (int i = 0; i < k1; i++) {
        //                        int i0 = x + i;
        //                        int j0 = y + j;
        //                        if (i0 < widthNew && j0 < heightNew) {
        //                            lpDst = temp + lineBytesNew * j0  + i0;
        //                            *lpDst = *lpSrc;
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //    }
        //}

        //// 方法一：由原图像映射新图像（存在灰边）
        //int k1 = (int)(xzoom + 0.5);
        //int k2 = (int)(yzoom + 0.5);
        //for (int y = 0; y < height ; y++) {
        //    for (int x = 0; x < width ; x++) {
        //        lpSrc = m_pData + lineBytes * y + x;       
        //        for (int j = 0; j < k2; j++) {
        //            for (int i = 0; i < k1; i++) {
        //                int i0 = k1 * x + i;
        //                int j0 = k2 * y + j;
        //                lpDst = temp + lineBytesNew * j0  + i0;
        //                *lpDst = *lpSrc;
        //            }
        //        }
        //    }
        //}

    } else {
        lineBytes = WIDTHBYTES(width * 8 * 3);
        lineBytesNew = WIDTHBYTES(widthNew * 8 * 3);
        temp = new BYTE[lineBytesNew * heightNew];

        for (int y = 0; y < heightNew; y++) {
            for (int x = 0; x < widthNew; x++) {
                int x0 = int(x / xzoom + 0.5);
                int y0 = int(y / yzoom + 0.5);

                if((x0 >= 0) && (x0 < width) && (y0 >= 0) && (y0 < height)) {
                    lpDst = temp + lineBytesNew * y  + x  * 3;
                    lpSrc = m_pData + lineBytes * y0 + x0 * 3;    
                    lpDst[0] = lpSrc[0];
                    lpDst[1] = lpSrc[1];
                    lpDst[2] = lpSrc[2];
                }
            }
        }
    }

    SaveImage(widthNew, heightNew, temp);
    delete [] temp;
}

///***************************************************************/           
/*函数名称：SuoXiao(float xzoom, float yzoom)                                        
/*函数类型：void
/*参数说明：xzoom --水平缩小的比率
/*         yzoom --垂直缩小的比率                                     
/*功能：对图像进行缩小处理。            
/*****************************************************************/
void JiHeBianHuanDib::SuoXiao(float xzoom, float yzoom)
{
    LONG width = GetWidth();
    LONG height = GetHeight();
    LONG widthNew = LONG(width * xzoom + 0.5);
    LONG heightNew = LONG(height * yzoom + 0.5);
    LONG lineBytes, lineBytesNew;
    LPBYTE temp, lpSrc, lpDst;

    if (GetRGB()) {
        lineBytes = WIDTHBYTES(width * 8);
        lineBytesNew = WIDTHBYTES(widthNew * 8);
        temp = new BYTE[lineBytesNew * heightNew];
        for (int y = 0; y < heightNew; y++) {
            for (int x = 0; x < widthNew; x++) {
                int x0 = int(x / xzoom + 0.5);
                int y0 = int(y / yzoom + 0.5);

                if (x0 < width && y0 < height) {
                    lpDst = temp + lineBytesNew * y + x;
                    lpSrc = m_pData + lineBytes * y0 + x0;
                    *lpDst = *lpSrc;
                }
            }
        }
    } else {
        lineBytes = WIDTHBYTES(width * 8 * 3);
        lineBytesNew = WIDTHBYTES(widthNew * 8 * 3);
        temp = new BYTE[lineBytesNew * heightNew];
        for (int y = 0; y < heightNew; y++) {
            for (int x = 0; x < widthNew; x++) {
                int x0 = int(x / xzoom + 0.5);
                int y0 = int(y / yzoom + 0.5);

                if (x0 < width && y0 < height) {
                    lpDst = temp + lineBytesNew * y + x * 3;
                    lpSrc = m_pData + lineBytes * y0 + x0 * 3;
                    lpDst[0] = lpSrc[0];
                    lpDst[1] = lpSrc[1];
                    lpDst[2] = lpSrc[2];
                }
            }
        }
    }

    SaveImage(widthNew, heightNew, temp);
    delete [] temp;
}
//{
//    // 源图像的宽度和高度
//    LONG    wide;
//    LONG    height;
//    LONG    LineBytes;
//    // 缩放后图像的宽度和高度
//    LONG    newwide;
//    LONG    newheight;
//    // 指向源图像的指针
//    LPBYTE    p_data ;
//    LPBYTE  temp;
//    temp=this->GetData();
//    p_data= temp;
//    // 指向源象素的指针
//    LPBYTE    lpSrc;
//    // 指向缩放图像对应象素的指针
//    LPBYTE    lpDst;
//    // 指向缩放图像的指针
//    LPBYTE    hDIB;
//    // 循环变量（象素在新DIB中的坐标）
//    LONG    i;
//    LONG    j;
//    // 象素在源DIB中的坐标
//    LONG    i0;
//    LONG    j0;
//    if(m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
//    {
//        // 获取图像的宽度
//        wide = this->GetWidth();
//        LineBytes=(wide*8+31)/32*4;
//        // 获取图像的高度
//        height = this->GetHeight();
//        // 计算缩放后的图像实际宽度和高度
//        // 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
//        newwide = (LONG) (wide * xzoom  + 0.5);
//        newheight = (LONG) (height * yzoom  + 0.5);
//        // 分配内存，以保存新DIB
//        hDIB = new BYTE[ newwide * newheight*3 ];
//        int k1 =yzoom+ 0.5;
//        int k2 =xzoom+ 0.5;
//        // 针对图像每行进行操作
//        for(j = 0; j < height-1; j=j+k1)
//        {
//            // 针对图像每列进行操作
//            for(i = 0; i < wide-1; i=i+k2)
//            {
//                // 指向新DIB第i行，第j个象素的指针
//                // 注意此处宽度和高度是新DIB的宽度和高度
//                lpDst = (LPBYTE)hDIB  + newwide * j + i;
//                // 计算该象素在源DIB中的坐标
//                j0 = (LONG) (j / yzoom  + 0.5);
//                i0 = (LONG) (i / xzoom  + 0.5);
//                // 判断是否在源图范围内
//                if( (i0 >= 0) && (i0 < wide) && (j0 >= 0) && (j0 < height))
//                {
//                    // 指向源DIB第i0行，第j0个象素的指针
//                    lpSrc = (LPBYTE)p_data + wide * j0 + i0;
//                    // 复制象素
//                    *lpDst = *lpSrc;
//                }
//                else
//                {
//                    // 对于源图中没有的象素，直接赋值为255
//                    *lpDst = 255;
//                }
//            }
//        }
//        // 复制缩小后的图像
//        for(j=0;j<height;j++)
//        {    
//            for(i=0;i<wide;i++)
//            {
//                if(j<=newheight&&i<=newwide)
//                { 
//                    lpDst = (LPBYTE)hDIB  + newwide * j + i;
//                    *p_data=*lpDst;             
//                }
//                else *p_data=255;
//                p_data++;
//            }
//        }        
//        delete hDIB; 
//    }
//    else    //24位彩色
//    {
//        // 获取图像的宽度
//        wide = this->GetDibWidthBytes();
//        LineBytes = (((wide * 8)+31)/32*4);
//        // 获取图像的高度
//        height = this->GetHeight();
//        // 计算缩放后的图像实际宽度
//        // 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
//        newwide = (LONG) (LineBytes * xzoom  + 0.5);
//        // 计算缩放后的图像高度
//        newheight = (LONG) (height * yzoom  + 0.5);
//        // 分配内存，以保存新DIB
//        hDIB = new BYTE[ newwide * newheight*3 ];
//        int k1 =(int)(yzoom+ 0.5);
//        int k2 =(int)(xzoom+ 0.5);
//        // 针对图像每行进行操作
//        for(j = 0; j < height-1; j=j+k1)
//        {
//            // 针对图像每列进行操作
//            for(i = 0; i < LineBytes-1; i=i+k2*3)
//            {
//                // 指向新DIB第i行，第j个象素的指针
//                // 注意此处宽度和高度是新DIB的宽度和高度
//                lpDst = (LPBYTE)hDIB  + newwide * j + i;            
//                // 计算该象素在源DIB中的坐标
//                j0 = (LONG) (j / yzoom  + 0.5);
//                i0 = (LONG) (i / xzoom  + 0.5);
//                // 判断是否在源图范围内
//                if( (i0 >= 0) && (i0 < LineBytes) && (j0 >= 0) && (j0 < height))
//                {                
//                    // 指向源DIB第i0行，第j0个象素的指针
//                    lpSrc = (LPBYTE)p_data + LineBytes * j0 + i0;
//                    // 复制象素
//                    *lpDst = *lpSrc;
//                    *(lpDst+1)=*(lpSrc+1);
//                    *(lpDst+2)=*(lpSrc+2);               
//                }
//                else
//                {
//                    // 对于源图中没有的象素，直接赋值为255
//                    *lpDst = 255;
//                }
//            }
//        }
//        // 复制缩小后的图像
//        for(j=0;j<height;j++)
//        {    
//            for(i=0;i<LineBytes;i++)
//            {
//                if(j<=newheight&&i<=newwide)
//                {   
//                    lpDst = (LPBYTE)hDIB  + newwide * j + i;
//                    *p_data=*lpDst;
//                
//                }
//                else *p_data=255;
//                
//                p_data++;
//            }
//        }
//    }
//}

/***************************************************************/           
/*函数名称：PingYi(int m_Xmove, int m_Ymove)                                       
/*函数类型：void
/*参数说明：m_Xmove 图像的水平平移量，m_Ymove 图像的垂直平移量                                    
/*功能：对图像进行平移显示。            
/***************************************************************/ 
void JiHeBianHuanDib::PingYi(int m_Xmove, int m_Ymove)
{
    // 指向源图像的指针
    LPBYTE    lpSrc;
    LPBYTE   p_data;
    // 指向要复制区域的指针
    LPBYTE    lpDst;
    // 指向复制图像的指针
    LPBYTE    temp;
    // 循环变量
    int    i,j;
    //图像的高和宽
    LONG wide,height;
    // 图像每行的字节数
    LONG lLineBytes;
    p_data=GetData();
    wide=GetWidth();
    height=GetHeight();
    if (m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
    {    
        // 计算图像每行的字节数
        lLineBytes = (((wide * 8)+31)/32*4);
        // 暂时分配内存，以保存新图像
        temp=new BYTE [lLineBytes * height];
        // 初始化新分配的内存，设定初始值为255
        lpDst = (LPBYTE)temp;
        memset(lpDst, (BYTE)255, lLineBytes * height);

        for( j = 0; j < height; j++) {
            for( i = 0; i < wide; i++) {
                // 计算该象素在源DIB中的坐标
                lpSrc = (LPBYTE)p_data + lLineBytes * (height - 1 - j) + i;
                int i0,j0;
                // 计算该象素在新DIB中的坐标
                i0 = i + m_Xmove;
                j0 = j + m_Ymove;
                // 判断是否在新图范围内
                if( (i0 >= 0) && (i0 < wide) && (j0 >= 0) && (j0 < height))
                {
                    lpDst = (LPBYTE)temp + lLineBytes *(height - 1 - j0) + i0;
                    // 复制象素
                    *lpDst = *lpSrc;
                }
                else
                {
                    // 对于源图中没有的象素，直接赋值为255
                    * ((unsigned char*)lpDst) = 255;
                }    
            }
        }
        // 复制平移后的图像
        memcpy(p_data, temp, lLineBytes * height);
        // 释放内存
        delete temp;
    }
    else     //24位彩色
    {     
        //int wide = this->GetDibWidthBytes() ;   //取得原图的每行字节数
        int lLineBytes = this->GetDibWidthBytes() ;   //取得原图的每行字节数
        BYTE *temp=new BYTE[height*lLineBytes];
        // 初始化新分配的内存，设定初始值为255
        lpDst = (LPBYTE)temp;
        memset(lpDst, (BYTE)255, lLineBytes * height);

        for(int j = 0; j < height; j++) {
            for( i = 0; i < lLineBytes; i++) {            
                // 计算该象素在源DIB中的坐标
                lpSrc = (LPBYTE)p_data + lLineBytes * (height - 1 - j) + i;
                int i0,j0;
                // 计算该象素在新DIB中的坐标
                i0 = i + m_Xmove*3;
                j0 = j + m_Ymove;

                // 判断是否在新图范围内
                if( (i0 >= 0) && (i0 < lLineBytes) && (j0 >= 0) && (j0 < height))
                {
                    lpDst = (LPBYTE)temp + lLineBytes * (height - 1 - j0) + i0;
                    // 复制象素
                    *lpDst = *lpSrc;
                }
                else
                {
                    // 对于源图中没有的象素，直接赋值为255
                    * ((unsigned char*)lpDst) = 255;
                }            
            }
        }
        // 复制平移后的图像
        memcpy(p_data, temp, lLineBytes * height);
    }
}
//{
//    // 指向源图像的指针
//    LPBYTE    lpSrc;
//    LPBYTE   p_data;
//    // 指向要复制区域的指针
//    LPBYTE    lpDst;
//    // 指向复制图像的指针
//    LPBYTE    temp;
//    // 循环变量
//    int    i,j;
//    //图像的高和宽
//    LONG wide,height;
//    // 图像每行的字节数
//    LONG lLineBytes;
//    p_data=GetData();
//    wide=GetWidth();
//    height=GetHeight();
//    if (m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
//    {    
//        // 计算图像每行的字节数
//        lLineBytes = (((wide * 8)+31)/32*4);    
//        // 暂时分配内存，以保存新图像
//        temp=new BYTE [lLineBytes*height];
//        // 初始化新分配的内存，设定初始值为255
//        lpDst = (LPBYTE)temp;
//        memset(lpDst, (BYTE)255, lLineBytes * height);
//        // 每行
//        for( i = 0; i < wide; i++) 
//        {
//            // 每列
//            for( j = 0; j < height; j++) 
//            {
//                // 计算该象素在源DIB中的坐标
//                lpSrc = (LPBYTE)p_data + lLineBytes * (height - 1 - j) + i;
//                int i0,j0;
//                // 计算该象素在新DIB中的坐标
//                i0 = i + m_Xmove;
//                j0 = j + m_Ymove;
//                // 判断是否在新图范围内
//                if( (i0 >= 0) && (i0 < wide) && (j0 >= 0) && (j0 < height))
//                {
//                    lpDst = (LPBYTE)temp + lLineBytes *(height - 1 - j0) + i0;
//                    // 复制象素
//                    *lpDst = *lpSrc;
//                }
//                else
//                {
//                    // 对于源图中没有的象素，直接赋值为255
//                    * ((unsigned char*)lpDst) = 255;
//                }
//            }
//        }
//        // 复制平移后的图像
//        memcpy(p_data, temp, lLineBytes * height);
//        // 释放内存
//        delete temp;
//    }
//    else     //24位彩色
//    {          
//        wide=this->GetDibWidthBytes() ;   //取得原图的每行字节数
//        BYTE *temp=new BYTE[height*wide];
//        // 初始化新分配的内存，设定初始值为255
//        lpDst = (LPBYTE)temp;
//        memset(lpDst, (BYTE)255, wide * height);
//        // 每列
//        for(int j = 0; j < height; j++)
//        {
//            // 每行
//            for( i = 0; i < wide; i++)
//            {            
//                // 计算该象素在源DIB中的坐标
//                lpSrc = (LPBYTE)p_data + wide * (height - 1 - j) + i;
//                int i0,j0;
//                // 计算该象素在新DIB中的坐标
//                i0 = i + m_Xmove*3;
//                j0 = j + m_Ymove;
//                
//                // 判断是否在新图范围内
//                if( (i0 >= 0) && (i0 < wide) && (j0 >= 0) && (j0 < height))
//                {
//                    lpDst = (LPBYTE)temp + wide*(height - 1 - j0) + i0;
//                    // 复制象素
//                    *lpDst = *lpSrc;
//                }
//                else
//                {
//                    // 对于源图中没有的象素，直接赋值为255
//                    * ((unsigned char*)lpDst) = 255;
//                }            
//            }
//        }
//        // 复制平移后的图像
//        memcpy(p_data, temp, wide * height);
//    }
//}

///*********************************************************************************/           
/*函数名称：FangDa(LPBYTE temp,float xzoom ,float yzoom,LONG wide,LONG    height,LONG    newwide,LONG newheight)
/*函数类型：LPBYTE 
/*参数说明：temp  --新图像的首地址
/*          xzoom --水平放大的比率
/*          yzoom --垂直放大的比率  
/*          wide  --原图像的宽度
/*            height--原图像的高度
/*            newwide--新图像的宽度
/*            newheight--新图像的高度
/*函数类型：LPBYTE                         
/*功能：对图像进行放大处理。            
/************************************************************************************/
LPBYTE JiHeBianHuanDib::FangDa(LPBYTE temp, float xzoom, float yzoom,LONG wide,LONG    height,LONG    newwide,LONG newheight)
{
    // 指向源图像的指针
    LPBYTE    p_data ;
    LPBYTE  temp1;
    // 指向源象素的指针
    LPBYTE    lpSrc;
    // 指向缩放图像对应象素的指针
    LPBYTE    lpDst;
    int    i;
    int    j;
    // 象素在源DIB中的坐标
    int    i0;
    int    j0;
    p_data=this->GetData();
    // 获取图像的宽度
    int    k1=(int)yzoom ;
    int    k2= (int)xzoom ; 
    if(m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
    {
        int width = wide;
        int widthNew = newwide;
        int heightNew = newheight;
        int lineBytes = WIDTHBYTES(width * 8);
        int lineBytesNew = WIDTHBYTES(widthNew * 8);
        temp1 = new BYTE[lineBytesNew * heightNew];
        memset(temp, 0, lineBytesNew * heightNew);

        for (int y = 0; y < heightNew; y++) {
            for (int x = 0; x < widthNew; x++) {
                int x0 = int(x / xzoom + 0.5);
                int y0 = int(y / yzoom + 0.5);

                if( (x0 >= 0) && (x0 < width) && (y0 >= 0) && (y0 < height)) {
                    lpDst = temp1 + lineBytesNew * y  + x;
                    lpSrc = m_pData + lineBytes * y0 + x0;    
                    *lpDst=*lpSrc;
                }
            }
        }

        temp=temp1;
        return temp; 
    }
    else    //24位彩色        
    {
        // 分配内存，以保存新DIB
        temp1 = new BYTE[newwide * newheight*4];
        memset(temp1,255,newwide * newheight*4);
        int r,g,b;
        // 针对图像每行进行操作
        for(j = 0; j < height; j++)
        {
            // 针对图像每列进行操作
            for(i = 0; i < wide; i++)
            {
                lpSrc = (LPBYTE)p_data + wide * j*3 + i*3;
                r=*lpSrc++;
                g=*lpSrc++;
                b=*lpSrc;
                // 复制象素
                for(int n=0;n<k1;n++)
                for(int m=0;m<k2;m++)
                {
                    // 指向源DIB第i0行，第j0个象素的指针    
                    lpDst = (LPBYTE)temp1  + newwide *(j*k1+n) + (i+m)*k2*3;
                    *lpDst++ = r;   
                    *lpDst++ = g;      
                    *lpDst = b;        
                }
            }
        }
        // 复制转置后的图像    
        temp=temp1;
        // 释放内存
        return temp;
    }
}

/***************************************************************/           
/*函数名称：SaveImage(LONG width, LONG height, LPBYTE buffer)                                       
/*函数类型：void
/*参数说明：width 新图像的像素宽度，height 新图像的像素高度，buffer 像素                                 
/*功能：更新图像。            
/***************************************************************/ 
void JiHeBianHuanDib::SaveImage(LONG width, LONG height, LPBYTE buffer)
{
    LONG bitmapWidth = GetWidth();
    LONG bitmapHeight = GetHeight();
    LONG lineBytes, lineBytesNew;
    
    if (GetRGB()) {
        lineBytes = WIDTHBYTES(bitmapWidth * 8);
        lineBytesNew = WIDTHBYTES(width * 8);
    } else {
        lineBytes = WIDTHBYTES(bitmapWidth * 8 * 3);
        lineBytesNew = WIDTHBYTES(width * 8 * 3);
    }
    
    LONG bufferSizeOld = lineBytes * bitmapHeight;
    LONG bufferSizeNew = lineBytesNew * height;

    LPBYTE lpDst = pDib + size - bufferSizeOld;
    ASSERT(lpDst == m_pData);

    // 重新分配内存
    m_valid = FALSE;
    if (bufferSizeOld < bufferSizeNew) {
        int sizeNew = size + bufferSizeNew - bufferSizeOld;
        LPBYTE temp = (BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, sizeNew);
        memcpy(temp, pDib, size - bufferSizeOld);
        memcpy(temp + size - bufferSizeOld, buffer, bufferSizeNew);
        GlobalFreePtr(pDib);

        pDib = temp;
        m_pBitmapInfo = (BITMAPINFO*) pDib;
        m_pBitmapInfoHeader = (BITMAPINFOHEADER*) pDib;

        m_pRGB = (RGBQUAD*)(pDib +
            m_pBitmapInfoHeader->biSize);
        int m_numberOfColors = GetNumberOfColors();
        if (m_pBitmapInfoHeader->biClrUsed == 0)
            m_pBitmapInfoHeader->biClrUsed =
            m_numberOfColors;
        DWORD colorTableSize = m_numberOfColors *
            sizeof(RGBQUAD);
        m_pData = pDib + m_pBitmapInfoHeader->biSize
            + colorTableSize;
        if (m_pRGB == (RGBQUAD*)m_pData) // No color table
            m_pRGB = NULL;

        m_pBitmapInfoHeader->biWidth = width;
        m_pBitmapInfoHeader->biHeight = height;
        m_pBitmapInfoHeader->biSizeImage = GetSize();
    } else {
        memcpy(m_pData, buffer, bufferSizeNew);
        m_pBitmapInfoHeader->biWidth = width;
        m_pBitmapInfoHeader->biHeight = height;
    }

    m_valid = TRUE;
}
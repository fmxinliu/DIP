#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BianYuanJianCeDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 int j;
BianYuanJianCeDib::BianYuanJianCeDib()
{
  
}
BianYuanJianCeDib::~BianYuanJianCeDib()
{
}

/***************************************************************/
/*函数名称：Templat(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)                                         */
/*函数类型：void                                               */
/*参数：LPBYTE p_data:指向原DIB图像指针                        */
/*      int lineBytes:原图像4字节对齐宽度                      */
/*      int height:原图像高度                                  */
/*      int tempH:模板高度                                     */
/*      int tempW:模板宽度                                     */
/*      int tempMX:模板的中心元素X坐标(<tempW-1)               */
/*      int tempMY:模板的中心元素Y坐标(<tempH-1)               */
/*      float *fpArray:指向模板数组的指针                      */
/*      float fCoef:模板系数                                   */
/*功能：用指定模板对灰度图像进行操作。                            */
/***************************************************************/
void BianYuanJianCeDib::Template(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = tempMY; y < height- (tempH - tempMY) + 1; y++) {
        for (int x = border * tempMX; x < lineBytes - border * (tempW - tempMX) + 1; x++) {
    //for (int y = tempMY; y < height - tempMY; y++) {
    //    for (int x = border * tempMX; x < lineBytes - border * tempMX; x++) {
            // 每个通道单独卷积
            float fResult = 0.0f;
            for(int t = 0; t < tempH; t++)
                for(int s = 0; s < tempW; s++)
                    fResult += p_data[lineBytes * (y - tempMY + t) + (x - border * tempMX + s * border)] * fpArray[tempW * t + s];
                    
            //for(int t = -tempMY; t <= tempMY; t++)
            //    for(int s = -tempMX; s <= tempMX; s++)
            //        fResult += p_data[lineBytes * (y + t) + (x + s * border)] * fpArray[tempW * (tempMY + t) + (tempMX + s)];
            // 乘上系数
            fResult *= fCoef;
            // 取绝对值
            fResult = (float)fabs(fResult);
            // 赋值
            if(fResult > 255)
                p_temp[lineBytes * y + x] = 255;
            else
                p_temp[lineBytes * y + x] = (int)(fResult + 0.5);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

void BianYuanJianCeDib::Templat(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{  
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = tempMY; y < height- tempH + tempMY + 1; y++) {
        for (int x = tempMX; x < lineBytes - tempW + tempMX + 1; x++) {
            // 卷积
            float fResult = 0.0f;
            for(int t = 0; t < tempH; t++)
                for(int s = 0; s < tempW; s++)
                    fResult += p_data[lineBytes * (y -tempMY + t) + (x - tempMX + s)] * fpArray[tempW * t + s];
            // 乘上系数
            fResult *= fCoef;
            // 取绝对值
            fResult = (float)fabs(fResult);
            // 赋值
            if(fResult > 255)
                p_temp[lineBytes * y + x] = 255;
            else
                p_temp[lineBytes * y + x] = (int)(fResult + 0.5);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
    return;
}

/***************************************************************/
/*函数名称：Templat24bit(BYTE *m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)                                         */
/*函数类型：void                                               */
/*参数：BYTE* m_pdata:指向原DIB图像指针                        */
/*      int DibWidth:原图像宽度                                */
/*      int height:原图像高度                                  */
/*      int tempH:模板高度                                     */
/*      int tempW:模板宽度                                     */
/*      int tempMX:模板的中心元素X坐标(<tempW-1)               */
/*      int tempMY:模板的中心元素Y坐标(<tempH-1)               */
/*      float *fpArray:指向模板数组的指针                      */
/*      float fCoef：模板系数                                  */
/*功能：用指定模板对24位彩色图像进行操作。                     */
/***************************************************************/
void BianYuanJianCeDib::Templat24bit(BYTE *m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{
    int i,j,k,l;  //循环变量
    BYTE*  p_temp=new BYTE[DibWidth*height];    //新图像缓冲区
    //初始化新图像为原始图像
    memcpy( p_temp,m_pdata,DibWidth*height);
    float fResult;    //像素值计算结果
    for(j=tempMY;j<height-tempH+tempMY+1;j++)
        for(i=3*tempMX;i<DibWidth-3*tempW+3*tempMX+1;i++)
        {
            //计算像素值
            fResult=0;
            for(k=0;k<tempH;k++)
                for(l=0;l<tempW;l++)
                    fResult=fResult+m_pdata[(j-tempMY+k)*DibWidth+(i-3*tempMX+l*3)]*fpArray[k*tempW+l];
            //乘上系数
            fResult*=fCoef;
            //取绝对值
            fResult=(float)fabs(fResult);
            //判断是否超过255
            if(fResult>255)
                //若超过255，直接赋值为255
                p_temp[j*DibWidth+i]=255;
            else
                //未超过255，赋值为计算结果
                p_temp[j*DibWidth+i]=(int)(fResult+0.5);
        }

    memcpy(m_pdata, p_temp,DibWidth*height);   //复制处理后的图像
    delete  []p_temp;
}

///***************************************************************/           
/*函数名称：Robert()                                      
/*函数类型：void                                     
/*功能：用罗伯特算子对图像进行边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Robert()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;
    int pixel[4];   //Robert算子

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    // 2*2模板
    for (int y = 0; y < height - border; y++) {
        for (int x = 0; x < lineBytes - border; x++) {
            // 生成Robert算子
            pixel[0] = p_data[lineBytes * y + x];
            pixel[1] = p_data[lineBytes * y + (x + border)];
            pixel[2] = p_data[lineBytes * (y + 1) + x];
            pixel[3] = p_data[lineBytes * (y + 1) + (x + border)];

            BYTE deltaX = abs(pixel[3] - pixel[0]);
            BYTE deltaY = abs(pixel[2] - pixel[1]);
            p_temp[lineBytes * y + x] = (deltaX + deltaY > 255) ? 255 : deltaX + deltaY;
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}
 
///***************************************************************/           
/*函数名称：PreWitt()                                      
/*函数类型：void                                     
/*功能：用普瑞维特算子对图像进行边缘检测。            
/***************************************************************/
void BianYuanJianCeDib::PreWitt()
{
    int tempH;  //模板高度
    int tempW;  //模板宽度
    int tempMY;   //模板中心元素Y坐标
    int tempMX;   //模板中心元素X坐标

    float tempC;  //模板系数
    float kernel[9];   //模板数组

    int width = GetWidth();   //原图长
    int height = GetHeight(); //原图宽
    int lineBytes = GetDibWidthBytes(); //原图4字节对齐后的宽
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = this->GetData ();   //原图数据区指针
    LPBYTE p_tempX = new BYTE[size]; //x方向卷积后的图像指针
    LPBYTE p_tempY = new BYTE[size]; //y方向卷积后的图像指针

    //拷贝原图像到缓存图像
    memcpy(p_tempX, p_data, size);
    memcpy(p_tempY, p_data, size);

    //设置Prewitt模板1参数
    tempW = 3;
    tempH = 3;
    tempC = 1.0;
    tempMY = 1;
    tempMX = 1;
    kernel[0] = -1.0f;
    kernel[1] = -1.0f;
    kernel[2] = -1.0f;
    kernel[3] =  0.0f;
    kernel[4] =  0.0f;
    kernel[5] =  0.0f;
    kernel[6] =  1.0f;
    kernel[7] =  1.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempX, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //设置Prewitt模板2参数
    kernel[0] = -1.0f;
    kernel[1] =  0.0f;
    kernel[2] =  1.0f;
    kernel[3] = -1.0f;
    kernel[4] =  0.0f;
    kernel[5] =  1.0f;
    kernel[6] = -1.0f;
    kernel[7] =  0.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempY, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_data[index] = max(p_tempX[index], p_tempY[index]); // 取梯度G = max{Gx， Gy}
        }
    }

    delete p_tempX;
    delete p_tempY;
}

///***************************************************************/           
/*函数名称：Sobel()                                      
/*函数类型：void                                     
/*功能：用索伯尔算子对图像进行边缘检测。            
/***************************************************************/
void BianYuanJianCeDib::Sobel()
{
    int tempH;  //模板高度
    int tempW;  //模板宽度
    int tempMY;   //模板中心元素Y坐标
    int tempMX;   //模板中心元素X坐标
    
    float tempC;  //模板系数
    float kernel[9];   //模板数组
    
    int width = GetWidth();   //原图长
    int height = GetHeight(); //原图宽
    int lineBytes = GetDibWidthBytes(); //原图4字节对齐后的宽
    int size = lineBytes * height;

    LPBYTE p_data = this->GetData ();   //原图数据区指针
    LPBYTE p_tempX = new BYTE[size]; //x方向卷积后的图像指针
    LPBYTE p_tempY = new BYTE[size]; //y方向卷积后的图像指针

    //拷贝原图像到缓存图像
    memcpy(p_tempX, p_data, size);
    memcpy(p_tempY, p_data, size);

    //设置Sobel模板1参数
    tempW = 3;
    tempH = 3;
    tempC = 1.0;
    tempMY = 1;
    tempMX = 1;
    kernel[0] = -1.0f;
    kernel[1] = -2.0f;
    kernel[2] = -1.0f;
    kernel[3] =  0.0f;
    kernel[4] =  0.0f;
    kernel[5] =  0.0f;
    kernel[6] =  1.0f;
    kernel[7] =  2.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempX, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //设置Sobel模板2参数
    kernel[0] = -1.0f;
    kernel[1] =  0.0f;
    kernel[2] =  1.0f;
    kernel[3] = -2.0f;
    kernel[4] =  0.0f;
    kernel[5] =  2.0f;
    kernel[6] = -1.0f;
    kernel[7] =  0.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempY, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_data[index] = max(p_tempX[index], p_tempY[index]); // 取梯度G = max{Gx， Gy}
        }
    }
    
    delete p_tempX;
    delete p_tempY;
}

///***************************************************************/           
/*函数名称：Laplacian()                                      
/*函数类型：void       
/*函数参数：bool onlyborder 只保留边界，不保留细节
/*功能：用拉普拉斯算子对图像边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Laplacian(bool onlyborder)
{
    int tempH;  //模板高度
    int tempW;  //模板宽度
    int tempMY;   //模板中心元素Y坐标
    int tempMX;   //模板中心元素X坐标

    float tempC;  //模板系数
    float kernel[9];   //模板数组

    int width = GetWidth();   //原图长
    int height = GetHeight(); //原图宽
    int lineBytes = GetDibWidthBytes(); //原图4字节对齐后的宽
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = this->GetData ();   //原图数据区指针
    LPBYTE p_temp = new BYTE[size];
    memcpy(p_temp, p_data, size);

    //设置Laplacian模板参数
    tempW = 3;
    tempH = 3;
    tempC = 1.0;
    tempMY = 1;
    tempMX = 1;
    kernel[0] = -1.0f;
    kernel[1] = -1.0f;
    kernel[2] = -1.0f;
    kernel[3] = -1.0f;
    kernel[4] =  8.0f; // 只保留边界
    kernel[5] = -1.0f;
    kernel[6] = -1.0f;
    kernel[7] = -1.0f;
    kernel[8] = -1.0f;

    if (!onlyborder)
        kernel[4] =  9.0f; // 边界 + 内部信息（叠加原图像）=================================

    this->Template(p_temp, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);
    
    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Guasslaplacian()                                      
/*函数类型：void                                    
/*功能：用高斯拉普拉斯算子对图像边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Guasslaplacian()
{
    int tempH;  //模板高度
    int tempW;  //模板宽度
    int tempMY;   //模板中心元素Y坐标
    int tempMX;   //模板中心元素X坐标

    float tempC;  //模板系数
    float kernel[25];   //模板数组

    int width = GetWidth();   //原图长
    int height = GetHeight(); //原图宽
    int lineBytes = GetDibWidthBytes(); //原图4字节对齐后的宽
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = this->GetData ();   //原图数据区指针
    LPBYTE p_temp = new BYTE[size];
    memcpy(p_temp, p_data, size);

    //设置Guasslaplacian模板参数
    tempW = 5;
    tempH = 5;
    tempC = 0.25;
    tempMY = 2;
    tempMX = 2;
    kernel[0]  = -2.0f;
    kernel[1]  = -4.0f;
    kernel[2]  = -4.0f;
    kernel[3]  = -4.0f;
    kernel[4]  = -2.0f;
    kernel[5]  = -4.0f;
    kernel[6]  =  0.0f;
    kernel[7]  =  8.0f;
    kernel[8]  =  0.0f;
    kernel[9]  = -4.0f;
    kernel[10] = -4.0f;
    kernel[11] =  8.0f;
    kernel[12] = 24.0f;
    kernel[13] =  8.0f;
    kernel[14] = -4.0f;
    kernel[15] = -4.0f;
    kernel[16] =  0.0f;
    kernel[17] =  8.0f;
    kernel[18] =  0.0f;
    kernel[19] = -4.0f;
    kernel[20] = -2.0f;
    kernel[21] = -4.0f;
    kernel[22] = -4.0f;
    kernel[23] = -4.0f;
    kernel[24] = -2.0f;
    this->Template(p_temp, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*函数名称：Krisch()                                      
/*函数类型：void                                     
/*功能：用克瑞斯算子对图像边缘检测。            
/***************************************************************/ 
void BianYuanJianCeDib::Krisch()
{
    int tempH;  //模板高度
    int tempW;  //模板宽度
    int tempMY;   //模板中心元素Y坐标
    int tempMX;   //模板中心元素X坐标

    float tempC;  //模板系数
    float kernel[9];   //模板数组

    int width = GetWidth();   //原图长
    int height = GetHeight(); //原图宽
    int lineBytes = GetDibWidthBytes(); //原图4字节对齐后的宽
    int size = lineBytes * height;

    LPBYTE p_data = this->GetData ();   //原图数据区指针
    LPBYTE p_temp1 = new BYTE[size]; //模板1卷积后的图像指针
    LPBYTE p_temp2 = new BYTE[size]; //模板2卷积后的图像指针

    //拷贝原图像到缓存图像
    memcpy(p_temp1, p_data, size);
    memcpy(p_temp2, p_data, size);

    //设置Kirsch模板1参数
    tempW = 3;
    tempH = 3;
    tempC = 0.5;
    tempMY = 1;
    tempMX = 1;
    kernel[0] =  5.0f;
    kernel[1] =  5.0f;
    kernel[2] =  5.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp1, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //设置Kirsch模板2参数
    kernel[0] = -3.0f;
    kernel[1] =  5.0f;
    kernel[2] =  5.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] =  5.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]); //取2个方向中的最大值
        }
    }

    memcpy(p_temp2, p_data, size);

    //设置Kirsch模板3参数
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] =  5.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] =  5.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] =  5.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //设置Kirsch模板4参数
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] =  5.0f;
    kernel[6] = -3.0f;
    kernel[7] =  5.0f;
    kernel[8] =  5.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //设置Kirsch模板5参数
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] =  5.0f;
    kernel[7] =  5.0f;
    kernel[8] =  5.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //设置Kirsch模板6参数
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] =  5.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] =  5.0f;
    kernel[7] =  5.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //设置Kirsch模板7参数
    kernel[0] =  5.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] =  5.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] =  5.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //设置Kirsch模板7参数
    kernel[0] =  5.0f;
    kernel[1] =  5.0f;
    kernel[2] = -3.0f;
    kernel[3] =  5.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //求两幅缓存图像的最大值
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_data, p_temp1, size);

    delete p_temp1;
    delete p_temp2;
}

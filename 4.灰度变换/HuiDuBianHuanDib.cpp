#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "HuiDuBianHuanDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
HuiDuBianHuanDib::HuiDuBianHuanDib()
{
}
HuiDuBianHuanDib::~HuiDuBianHuanDib()
{
}

///***************************************************************/           
/*函数名称：FanSeBianHuan()                                        
/*函数类型：void                                           
/*功能：对图像进行反色变换                            
/***************************************************************/
void HuiDuBianHuanDib::FanSeBianHuan()//对图像进行反色变换
{
    LPBYTE p_data=this->GetData (); //原图数据区指针
    int wide=this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int height=this->GetHeight(); // 原图高度

    for(int i=0;i<height*wide;i++)
    {
        unsigned char  temp;
        temp=*(p_data+i);
        *(p_data+i)=255- temp;
    }    
}

///***************************************************************/           
/*函数名称：Fei0()                                                */
/*函数类型：void                                                */
/*功能：对图像进行非零取一运算。                                */
/****************************************************************/
void HuiDuBianHuanDib::Fei0()//对图像进行非零取一运算
{
    LPBYTE p_data=this->GetData (); //原图数据区指针
    int wide=this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int height=this->GetHeight(); // 原图高度

    if (!GetRGB())
        RgbToGray();

    for(int i=0;i<height*wide;i++)
    {
        if(*p_data!=0)          //若像素值不为0
            *p_data=255;         //将其置为255
        p_data++;
    }
}

/***************************************************************/           
/*函数名称：GuDing(int YuZhi)                                  */      
/*函数类型：void                                               */    
/*参数说明：YuZhi 给定阈值                                     */       
/*功能：对图像进行固定阀值运算。                               */
/***************************************************************/
void HuiDuBianHuanDib::GuDing(int YuZhi)//对图像进行固定阀值运算
{
    LPBYTE p_data=this->GetData(); //原图数据区指针
    int width=this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int height=this->GetHeight(); // 原图高度

    if (!GetRGB())
        RgbToGray();

    for(int i=0;i<height*width;i++)
    {
        if(*p_data>YuZhi)          //灰度值大于给定阈值，置为255
            *p_data=255;
        else
            *p_data=0;             //不大于置为0
        p_data++;
    }
}

/***************************************************************/           
/*函数名称：ShuangYu(int YuZhi1,int YuZhi2,int mode)          */
/*参数：int YuZhi1：阈值1；int YuZhi2：阈值2；int mode：处理方式*/
/*函数类型：void                                              */
/*功能：对图像使用双固定阈值法进行二值化。                     */
/***************************************************************/
void HuiDuBianHuanDib::ShuangYu(int YuZhi1,int YuZhi2,int mode)
{
    LPBYTE p_data=this->GetData (); //原图数据区指针
    int wide=this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int height=this->GetHeight(); // 原图高度

    if (!GetRGB())
        RgbToGray();

    if(mode==0)    //0-255-0型
    {
        for(int i=0;i<height*wide;i++)
        {
            //若该像素的灰度值介于两个阈值之间，这将其置为255
            if(*p_data>=YuZhi1&&*p_data<=YuZhi2)
                *p_data=255;
            else
                *p_data=0;             //不大于置为0
            p_data++;
        }
    }
    if(mode==1)    //255-0-255型
    {
        for(int i=0;i<height*wide;i++)
        {
            //若该像素的灰度值介于两个阈值之间，这将其置为255
            if(*p_data>=YuZhi1&&*p_data<=YuZhi2)
                *p_data=0;
            else
                *p_data=255;             //否则置255
            p_data++;
        }
    }             
}


///***************************************************************/           
/*函数名称：Chuangkoubianhuan(BYTE blow,BYTE bup)                                      
/*函数类型：void                                          
/*变量说明：blow窗口下界 bup窗口上界                                                               
/*功能：对图像进行窗口变换。                    
/***************************************************************/  
void HuiDuBianHuanDib::Chuangkoubianhuan(BYTE bLow, BYTE bUp)
{
    LPBYTE p_data=this->GetData (); //原图数据区指针
    int wide=this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int height=this->GetHeight(); // 原图高度

    for(int i=0;i<height*wide;i++)
    {
        if ((*p_data) < bLow)
        {
            *p_data = 0;            // 直接赋值为0
        }
        else if ((*p_data) > bUp)
        {
            *p_data = 255;          // 直接赋值为255
        }
        else
        {
            *p_data = *p_data; //     变换公式： g = 1.0 * (d - c) / (b - a) * (f - a) + c
        }

        p_data++;
    }
}

///***************************************************************/           
/*函数名称：Zhexianbianhuan(BYTE bX1,BYTE bY1,BYTE bX2,BYTE bY2)                                           */
/*函数类型：void                                              */
/*变量说明：bX1折点一的原始灰度；bX2折点二的原始灰度
/*         bY1折点一的变换灰度；bY2折点二的变换灰度                                                       
/*功能：对图像进行分段线性变换。                      */
/***************************************************************/ 
void HuiDuBianHuanDib::Zhexianbianhuan(BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{ 
    int i;
    // 灰度映射表
    BYTE    bMap[256];
    // 计算灰度映射表
    for (i = 0; i <= bX1; i++)    //[0 —— X1]
    {
        // 判断bX1是否大于0（防止分母为0）
        if (bX1 > 0)
        {
            // 线性变换
            bMap[i] = (BYTE) bY1 * i / bX1;
        }
        else
        {
            // 直接赋值为0
            bMap[i] = 0;
        }
    }
    for (; i <= bX2; i++)        //(X1 —— X2]
    {
        // 判断bX1是否等于bX2（防止分母为0）
        if (bX2 != bX1)
        {
            // 线性变换
            bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
        }
        else
        {
            // 直接赋值为bY1
            bMap[i] = bY1;
        }
    }
    for (; i < 256; i++)        //(X2 —— 256)
    {
        // 判断bX2是否等于255（防止分母为0）
        if (bX2 != 255)
        {
            // 线性变换
            bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
        }
        else
        {
            // 直接赋值为255
            bMap[i] = 255;
        }
    }

    LPBYTE p_data=this->GetData (); //原图数据区指针
    int wide=this->GetDibWidthBytes(); // 原图 4 字节对齐后的宽度
    int height=this->GetHeight(); // 原图高度

    for(int i=0;i<height*wide;i++)
    {
        *p_data++ = bMap[*p_data];
    }
}

///***************************************************************/           
/*函数名称：Fenbujunhenghua()                                           
/*函数类型：void                                          
/*变量说明：无                                                               
/*功能：对图像进行灰度分布均衡化处理。                   
/***************************************************************/  
void HuiDuBianHuanDib::Fenbujunhenghua()
{
    if (GetRGB()) {
        float fps[256] = {}; 
        float temp[256] = {};
        int gps[256] = {}; 

        // 1.直方图
        ZhiFangTu(fps);

        // 2.累积直方图
        for (int i = 0; i < 256; i++) {
            if (i == 0)
                temp[0] = fps[0];
            else
                temp[i] = fps[i] + temp[i - 1];
        
            // 3.f(x, y)映射g(x, y)
            gps[i] = (int)(255.0f * temp[i] + 0.5f);
        }

        // 4.将映射后的灰度写入DIB  
        int width=this->GetWidth();      // 长     
        int height=this->GetHeight(); // 宽
        int lineBytes=this->GetDibWidthBytes(); // 4 字节对齐后的宽度

        LPBYTE p_data=this->GetData (); // 像素指针

        // 5.对各像素进行灰度统计
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                LPBYTE lpSrc = p_data + lineBytes * y + x;
                *lpSrc = gps[*lpSrc];
            }
        }

    }
    else {
        float fps_r[256] = {},  fps_g[256] = {}, fps_b[256] = {};
        float temp_r[256] = {}, temp_g[256] = {}, temp_b[256] = {};
        int gps_r[256] = {},  gps_g[256] = {}, gps_b[256] = {};

        // 1.直方图
        ZhiFangTu(fps_r, fps_g, fps_b);

        // 2.累积直方图
        for (int i = 0; i < 256; i++) {
            if (i == 0) {
                temp_r[0] = fps_r[0];
                temp_g[0] = fps_g[0];
                temp_b[0] = fps_b[0];
            } else {
                temp_r[i] = fps_r[i] + temp_r[i - 1];
                temp_g[i] = fps_g[i] + temp_g[i - 1];
                temp_b[i] = fps_b[i] + temp_b[i - 1];
            }

            // 3.f(x, y)映射g(x, y)
            gps_r[i] = (int)(255.0f * temp_r[i] + 0.5f);
            gps_g[i] = (int)(255.0f * temp_g[i] + 0.5f);
            gps_b[i] = (int)(255.0f * temp_b[i] + 0.5f);
        }

        // 4.将映射后的灰度写入DIB  
        int width=this->GetWidth();      // 长     
        int height=this->GetHeight(); // 宽
        int lineBytes=this->GetDibWidthBytes(); // 4 字节对齐后的宽度

        LPBYTE p_data=this->GetData (); // 像素指针

        // 5.对各像素进行灰度统计
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                LPBYTE lpSrc_b = p_data + lineBytes * y + x * 3;
                LPBYTE lpSrc_g = p_data + lineBytes * y + x * 3 + 1;
                LPBYTE lpSrc_r = p_data + lineBytes * y + x * 3 + 2;
                *lpSrc_r = gps_r[*lpSrc_r];
                *lpSrc_g = gps_g[*lpSrc_g];
                *lpSrc_b = gps_b[*lpSrc_b];
            }
        }

    }
}

///***************************************************************/           
/*函数名称：Pipeibianhuan(BYTE bNum,int *npNu,float*fpPu)                                            */
/*函数类型：void                                              */
/*变量说明：bNum规定直方图灰度级 npNu 直方图映射关系
/*         fpPu灰度分布概率                                                             
/*功能：对图像进行灰度匹配变换。                       */
/***************************************************************/  
void HuiDuBianHuanDib::Pipeibianhuan(BYTE bNum, int *npNu, float *fpPu)
{
    // 循环变量
    LONG i;
    LONG j;
    LONG wide;
    LONG height;
    // 灰度分布密度
    int midu[256],midu2[256],midu3[256];
    // 灰度分布概率
    float gailu[256],gailu2[256],gailu3[256];
    // 中间临时变量
    float temp[256],temp2[256],temp3[256];
    // 指向DIB象素指针
    LPBYTE p_data;
    // 找到DIB图像象素起始位置
    p_data = this->GetData();
    wide=this->GetWidth ();    
    // DIB的高度
    height = GetHeight();     
    if (m_pBitmapInfoHeader->biBitCount<9)        //灰度图像
    {   
        // 对灰度密度分布进行统计    
        Midufenbu(midu);
        // 对灰度分布概率进行统计
        ZhiFangTu(gailu);
        
        // 计算原始累计直方图
        for (i = 0; i < 256; i++)
        {
            if (i == 0)
            {
                temp[0] = gailu[0];             
            }
            else
            {
                temp[i] = temp[i-1] + gailu[i];             
            }
            gailu[i] = temp[i];
        }
        // 计算规定累计直方图
        for (i = 0; i < bNum; i++)
        {
            if (i == 0)
            {
                temp[0] = fpPu[0];
            }
            else
            {
                temp[i] = temp[i-1] + fpPu[i];
            }
            fpPu[i] = temp[i];
        }
        // 确定映射对应关系
        for (i = 0; i < 256; i++)
        {
            // 最接近的规定直方图灰度级
            int m_r = 0;
            // 最小差值
            float min_value_r = 1.0f;
            // 对规定直方图各灰度进行枚举
            for (j = 0; j < bNum; j++)
            {
                // 当前差值
                float now_value = 0.0f;
                //  计算差值
                if (gailu[i] - fpPu[j] >= 0.0f)
                    now_value = gailu[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu[i];
                // 寻找最接近的规定直方图灰度级
                if (now_value < min_value_r)
                {
                    // 最接近的灰度级
                    m_r = j;
                    // 暂存最小差值
                    min_value_r = now_value;
                }
            }
            // 建立灰度映射表
            midu[i] = npNu[m_r];
        }
        // 对各像素进行处理
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                // 对像素进行单映射规则的直方图规定化映射处理
                unsigned char temp = *((unsigned char *)p_data + wide * j + i);
                *((unsigned char *)p_data + wide * j + i) = midu[temp];
            }
        }
    }
    else        //24位彩色
    {
        // 对灰度密度分布进行统计    
        Midufenbu(midu,midu2,midu3);
        // 对灰度分布概率进行统计
        ZhiFangTu(gailu,gailu2,gailu3);
        
        // 计算原始累计直方图
        for (i = 0; i < 256; i++)
        {
            if (i == 0)
            {
                temp[0] = gailu[0];    
                temp2[0] = gailu2[0];
                temp3[0] = gailu3[0];
            }
            else
            {
                temp[i] = temp[i-1] + gailu[i];
                temp2[i] = temp2[i-1] + gailu2[i];    
                temp3[i] = temp3[i-1] + gailu3[i];    
            }
            gailu[i] = temp[i];
            gailu2[i] = temp2[i];
            gailu3[i] = temp3[i];
            
        }
        // 计算规定累计直方图
        for (i = 0; i < bNum; i++)
        {
            if (i == 0)
            {
                temp[0] = fpPu[0];            
            }
            else
            {
                temp[i] = temp[i-1] + fpPu[i];
            }
            fpPu[i] = temp[i];
        }
        // 确定映射对应关系
        for (i = 0; i < 256; i++)
        {
            // 最接近的规定直方图灰度级
            int m_r = 0;
            int m_g = 0;
            int m_b = 0;                 
            // 最小差值
            float min_value_r = 1.0f;
            float min_value_g = 1.0f;
            float min_value_b = 1.0f;
            // 对规定直方图各灰度进行枚举
            for (j = 0; j < bNum; j++)
            {
                // 当前差值
                float now_value = 0.0f;
                //  计算差值
                if (gailu[i] - fpPu[j] >= 0.0f)
                    now_value = gailu[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu[i];
                // 寻找最接近的规定直方图灰度级
                if (now_value < min_value_r)
                {
                    // 最接近的灰度级
                    m_r = j;
                    // 暂存最小差值
                    min_value_r = now_value;
                }
                //  计算差值
                if (gailu2[i] - fpPu[j] >= 0.0f)
                    now_value = gailu2[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu2[i];
                // 寻找最接近的规定直方图灰度级
                if (now_value < min_value_g)
                {
                    // 最接近的灰度级
                    m_g = j;
                    // 暂存最小差值
                    min_value_g = now_value;
                }
                //  计算差值
                if (gailu3[i] - fpPu[j] >= 0.0f)
                    now_value = gailu3[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu3[i];
                // 寻找最接近的规定直方图灰度级
                if (now_value < min_value_b)
                {
                    // 最接近的灰度级
                    m_b = j;
                    // 暂存最小差值
                    min_value_b = now_value;
                }
            }
            // 建立灰度映射表
            midu[i] = npNu[m_r];
            midu2[i] = npNu[m_g];
            midu3[i] = npNu[m_b];
        }
        // 对各像素进行处理
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                // 对像素进行单映射规则的直方图规定化映射处理
                unsigned char temp = *((unsigned char *)p_data + wide * j*3 + i*3);
                *((unsigned char *)p_data + wide * j*3 + i*3) = midu[temp];
                unsigned char temp2 = *((unsigned char *)p_data + wide * j*3 + i*3+1);
                *((unsigned char *)p_data + wide * j*3 + i*3+1) = midu[temp2];
                unsigned char temp3 = *((unsigned char *)p_data + wide * j*3 + i*3+2);
                *((unsigned char *)p_data + wide * j*3 + i*3+2) = midu[temp3];
            }
        }
    }
}

///***************************************************************/           
/*函数名称：Midufenbu()                                        
/*函数类型：void                                      
/*功能：对图像进行密度分布变换。            
/***************************************************************/  
void HuiDuBianHuanDib::Midufenbu(int *nNs_R)
{
    // 循环变量
    LONG i;
    LONG j;
    
    LONG wide;
    LONG height;
    //变量初始化
    memset(nNs_R,0,sizeof(int) * 256);
    // 指向DIB象素指针
    LPBYTE p_data;
    // 找到DIB图像象素起始位置
    p_data = this->GetData();        
    wide=this->GetWidth ();
    // DIB的高度
    height = GetHeight();
    // 对各像素进行灰度转换
    for (j = 0; j < height; j ++)
    {
        for (i = 0; i <wide; i ++)
        {
            // 对各像素进行灰度统计
            unsigned char temp = *((unsigned char *)p_data + wide * j + i);
            nNs_R[temp]++;        
        }
    } 
}

void HuiDuBianHuanDib::Midufenbu(int *nNs_R, int *nNs_G, int *nNs_B)
{
    // 循环变量
    LONG i;
    LONG j;
    LONG wide;
    LONG height;
    //变量初始化
    memset(nNs_R,0,sizeof(int) * 256);
    memset(nNs_G,0,sizeof(int) * 256);
    memset(nNs_B,0,sizeof(int) * 256);
    // 指向DIB象素指针
    LPBYTE p_data;
    // 找到DIB图像象素起始位置
    p_data = this->GetData();        
    wide=this->GetWidth ();
    // DIB的高度
    height = GetHeight();
    // 对各像素进行灰度转换
    for (j= 0;j < height;j ++)
    {
        for (i = 0; i < wide; i ++)
        {
            // 对各像素进行灰度统计
            unsigned char temp = *((unsigned char *)p_data + wide * j*3 + i*3);
            unsigned char temp2 = *((unsigned char *)p_data + wide * j*3 + i*3+1);
            unsigned char temp3 = *((unsigned char *)p_data + wide * j*3 + i*3+2);
            nNs_R[temp]++;        
            nNs_G[temp2]++;
            nNs_B[temp3]++;
        }
    } 
}

///***************************************************************/           
/*函数名称：Shuipingtouying()                                        
/*函数类型：void                                      
/*功能：对图像进行水平投影。            
/***************************************************************/  
void HuiDuBianHuanDib::Shuipingtouying()
{
    //对图像进行二值化
    GuDing(128);

    int width=this->GetWidth(); // 原图宽度
    int height=this->GetHeight(); // 原图高度
    int lineBytes = GetDibWidthBytes(); // 原图 4 字节对齐后的宽度

    LPBYTE p_data=this->GetData (); //原图数据区指针

    LONG lBlackNum;
    LPBYTE temp = new BYTE[lineBytes * height];
    memset(temp, 255, lineBytes * height);

    for(int y = 0; y < height; y++) {
        lBlackNum = 0;

        // 统计每行黑像素个数
        for (int x = 0; x < lineBytes; x++) {
            // 排除因 4 字节对齐引入的多余像素
            bool beyond = GetRGB() ? x >= width : x >= width * 3;
            if (beyond) 
                continue;

            if(p_data[lineBytes * y + x] == 0)
                lBlackNum++;
        }

        // 绘制每行
        for (int x = 0; x < lBlackNum; x++) {
            temp[lineBytes * y + x] = 0;
        }
    }

    memcpy(p_data, temp, lineBytes * height);
    delete []temp;    
}

///***************************************************************/           
/*函数名称：Chuizhitouying()                                      
/*函数类型：void                                      
/*功能：对图像进行垂直投影。            
/***************************************************************/  
void HuiDuBianHuanDib::Chuizhitouying()
{
    //对图像进行二值化
    GuDing(128);

    int width=this->GetWidth(); // 原图宽度
    int height=this->GetHeight(); // 原图高度
    int lineBytes = GetDibWidthBytes(); // 原图 4 字节对齐后的宽度

    LPBYTE p_data=this->GetData (); //原图数据区指针

    LONG lBlackNum;
    LPBYTE temp = new BYTE[lineBytes * height];
    memset(temp, 255, lineBytes * height);

    for(int x = 0; x < lineBytes; x++) {
        lBlackNum = 0;

        // 统计每列黑像素个数
        for (int y = 0; y < height; y++) {
            // 排除因 4 字节对齐引入的多余像素
            bool beyond = GetRGB() ? x >= width : x >= width * 3;
            if (beyond) 
                continue;

            if(p_data[lineBytes * y + x] == 0)
                lBlackNum++;
        }

        // 绘制每列
        for (int y = 0; y < lBlackNum; y++) {
            temp[lineBytes * y + x] = 0;
        }
    }

    memcpy(p_data, temp, lineBytes * height);
    delete []temp;

    //LPBYTE  p_data, p_datatemp;     //原图数据区指针
    //int wide,height;                //原图长、宽
    //int i,j;                        //循环变量
    //long int lBlackNum;                //图像中每行内黑点的个数
    //p_datatemp=this->GetData();
    //p_data=p_datatemp;
    //if(m_pBitmapInfoHeader->biBitCount<9)    
    //    wide=this->GetWidth ();
    //else
    //    wide=this->GetDibWidthBytes();
    //height=this->GetHeight ();
    ////对图像进行二值化
    //for(j=0;j<height;j++)
    //{
    //    for(i=0;i<wide;i++)
    //    { 
    //        if (*p_data>128)
    //            *p_data=255;
    //        else 
    //            *p_data=0;
    //        p_data++;
    //    }
    //}
    //p_data=p_datatemp;
    //LPBYTE  temp=new BYTE[wide*height];        //新图像缓冲区
    //memset( temp,255,wide*height);            //设定新图像初值为255
    //for(i=0;i<wide;i++)
    //{
    //    lBlackNum=0;
    //    for(j=0;j<height;j++)
    //    {
    //        
    //        if(p_data[wide*j+i]==0)
    //            lBlackNum++;
    //    }
    //    for(j=0;j<lBlackNum;j++)
    //        temp[wide*j+i]=0;
    //}
    //memcpy(p_data, temp,wide*height);
    //delete  temp;        
}

///**************************************************************/           
/*函数名称：ZhiFangTu(float *tongji)                               
/*函数类型：void                                         
/*变量说明：tongji 灰度分布密度统计                                                        
/*功能：对图像进行灰度直方图统计。                     
/***************************************************************/
void HuiDuBianHuanDib::ZhiFangTu(float *tongji)
{
    int huidu[256] = {};  // 灰度计数   
    int width=this->GetWidth();      // 长     
    int height=this->GetHeight(); // 宽
    int lineBytes=this->GetDibWidthBytes(); // 4 字节对齐后的宽度

    LPBYTE p_data=this->GetData(); // 像素指针

    // 对各像素进行灰度统计
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE fxy = *(p_data + lineBytes * y + x);
            huidu[fxy]++; // 灰度统计计数
        }
    }

    // 计算灰度分布密度
    for(int i = 0; i < 256; i++) {
        tongji[i] = huidu[i] / (height * width * 1.0f);
    }
}

///*************************************************************/           
/*函数名称：ZhiFangTu(float *tongji_r, float *tongji_g, float *tongji_b)                               
/*函数类型：void                                         
/*变量说明：tongji_r R灰度分布密度统计
           tongji_g G灰度分布密度统计
           tongji_b B灰度分布密度统计
/*功能：对24位彩色图像进行灰度概率统计。                     
/***************************************************************/
void HuiDuBianHuanDib::ZhiFangTu(float *tongji_r, float *tongji_g, float *tongji_b)
{ 
    ASSERT(GetRGB() == NULL);

    int width=this->GetWidth();      // 长     
    int height=this->GetHeight(); // 宽
    int lineBytes=this->GetDibWidthBytes(); // 4 字节对齐后的宽度

    LPBYTE p_data=this->GetData(); // 像素指针

    // 灰度统计
    int huidu_r[256], huidu_g[256], huidu_b[256];
    memset(huidu_r, 0, sizeof(huidu_r));
    memset(huidu_g, 0, sizeof(huidu_g));
    memset(huidu_b, 0, sizeof(huidu_b));

    // 对r、g、b三通道分别进行灰度统计
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE temp_b = p_data[lineBytes*y + x*3] ;
            BYTE temp_g = p_data[lineBytes*y + x*3 + 1] ;
            BYTE temp_r = p_data[lineBytes*y + x*3 + 2] ;

            huidu_r[temp_r]++;
            huidu_g[temp_g]++;
            huidu_b[temp_b]++;
        }
    }

    // 计算灰度分布密度
    for(int i = 0; i < 256; i++) {
        if (tongji_r)
            tongji_r[i] = huidu_r[i] / (height * width * 1.0f);

        if (tongji_g)
            tongji_g[i] = huidu_g[i] / (height * width * 1.0f);
        
        if (tongji_b)
            tongji_b[i] = huidu_b[i] / (height * width * 1.0f);
    }
}

// 显示通道直方图、图像
void HuiDuBianHuanDib::ShowChannelZhiFangTu(float *tongji_r, float *tongji_g, float *tongji_b)
{
    ZhiFangTu(tongji_r, tongji_g, tongji_b);

    int width=this->GetWidth();      // 长     
    int height=this->GetHeight(); // 宽
    int lineBytes=this->GetDibWidthBytes(); // 4 字节对齐后的宽度
    LPBYTE p_data=this->GetData(); // 像素指针

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LPBYTE b = p_data + lineBytes * y + x * 3;
            LPBYTE g = p_data + lineBytes * y + x * 3 + 1;
            LPBYTE r = p_data + lineBytes * y + x * 3 + 2;

            if (!tongji_r)
                *r = 0;
            if (!tongji_g)
                *g = 0;
            if (!tongji_b)
                *b = 0;
        }
    }
}

void HuiDuBianHuanDib::RedZhiFangTu(float *tongji)
{
    ShowChannelZhiFangTu(tongji, NULL, NULL);
}

void HuiDuBianHuanDib::GreenZhiFangTu(float *tongji)
{
    ShowChannelZhiFangTu(NULL, tongji, NULL);
}

void HuiDuBianHuanDib::BlueZhiFangTu(float *tongji)
{
    ShowChannelZhiFangTu(NULL, NULL, tongji);
}

// 彩色图转灰度图
void HuiDuBianHuanDib::RgbToGray()
{
    ASSERT(GetRGB() == NULL);

    int width=this->GetWidth();      // 长     
    int height=this->GetHeight(); // 宽
    int lineBytes=this->GetDibWidthBytes(); // 4 字节对齐后的宽度

    LPBYTE p_data=this->GetData(); // 像素指针

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LPBYTE pb = p_data + lineBytes*y + x*3;
            LPBYTE pg = p_data + lineBytes*y + x*3 + 1;
            LPBYTE pr = p_data + lineBytes*y + x*3 + 2;

            // rgb加权平均
            BYTE gray = 0.11 * (*pr) + 0.59 * (*pg) + 0.3 * (*pb); // g = 0.11 * r + 0.59 * g + 0.3b;
            *pb = gray;
            *pg = gray;
            *pr = gray;
        }
    }
}

void HuiDuBianHuanDib::RgbToGrayFast()
{
    ASSERT(GetRGB() == NULL);

    int width=this->GetWidth();      // 长     
    int height=this->GetHeight(); // 宽
    int lineBytes=this->GetDibWidthBytes(); // 4 字节对齐后的宽度

    LPBYTE p_data=this->GetData(); // 像素指针

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LPBYTE pb = p_data + lineBytes*y + x*3;
            LPBYTE pg = p_data + lineBytes*y + x*3 + 1;
            LPBYTE pr = p_data + lineBytes*y + x*3 + 2;

            int gray = max(*pb, *pg, *pr);   // rgb最大值，不需要实数计算
            *pb = gray;
            *pg = gray;
            *pr = gray;
        }
    }
}
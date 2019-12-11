// JisuanProcessDib.cpp: implementation of the JisuanProcessDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSplit.h"
#include "JisuanProcessDib.h"
#include "SquareDlg.h"
#include "LINEDLG.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JisuanProcessDib::JisuanProcessDib()
{
    count = 0;
}

JisuanProcessDib::~JisuanProcessDib()
{
}

/***************************************************************/
/*函数名称：erzhihua(int T)                                    */
/*函数类型：void                                               */
/*参数：int T，用户给定的阈值                                   */
/*功能：对图像使用固定阈值法进行二值化。                         */
/***************************************************************/
void JisuanProcessDib::erzhihua(int T)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    LPBYTE p_data = GetData();

    if (GetRGB()) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE *p_temp = p_data + lineBytes * (height - y - 1) + x;
                *p_temp = (*p_temp < T) ? 0 : 255;
            }
        }
    }
    else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE *p_temp = p_data + lineBytes * (height - y - 1) + x * 3;
                if (*p_temp < T) {
                    *p_temp++ = 0;
                    *p_temp++ = 0;
                    *p_temp   = 0;
                } else {
                    *p_temp++ = 255;
                    *p_temp++ = 255;
                    *p_temp   = 255;
                }
            }
        }
    }
}

/***************************************************************/
/*函数名称：xiaochugulidianHEI()                               */
/*函数类型：void                                               */
/*功能：消除孤立黑点。                                         */
/***************************************************************/
void JisuanProcessDib::xiaochugulidianHEI()
{
    LPBYTE p_data=this->GetData ();   //取得原图的数据区指针
    int wide=this->GetWidth ();  //取得原图的数据区宽度
    int height=this->GetHeight ();   //取得原图的数据区高度
    if(m_pBitmapInfoHeader->biBitCount<9)    //灰度图像
    {
        for(int j=1;j<height-1;j++)    // 每行
            for(int i=1;i<wide-1;i++)    // 每列
            {
                int temp=0;    
                if(*(p_data+(height-j-1)*wide+i)==0)//本身为黑点
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide+i+n-1);
                        }
                    if(temp>=255*6)///周围8个中点有大于等于6个白点
                        *(p_data+(height-j-1)*wide+i)=255;
                }
            }
    }
    else    //24位彩色
    {
        for(int j=1;j<height-1;j++)    // 每行
            for(int i=1;i<wide-1;i++)    // 每列
            {
                int temp=0;    
                if(*(p_data+(height-j-1)*wide*3+i*3)==0)//本身为黑点
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide*3+(i+n-1)*3);
                        }
                    if(temp>=255*6)///周围8个中点有大于等于6个白点
                    {
                        *(p_data+(height-j-1)*wide*3+i*3)=255;
                        *(p_data+(height-j-1)*wide*3+i*3+1)=255;
                        *(p_data+(height-j-1)*wide*3+i*3+2)=255;
                    }
                }
            }
    }
}

/***************************************************************/
/*函数名称：xiaochugulidianBAI()                               */
/*函数类型：void                                               */
/*功能：消除孤立白点。                                         */
/***************************************************************/
void JisuanProcessDib::xiaochugulidianBAI()
{
    LPBYTE p_data=this->GetData ();   //取得原图的数据区指针
    int wide=this->GetWidth ();  //取得原图的数据区宽度
    int height=this->GetHeight ();   //取得原图的数据区高度
    if(m_pBitmapInfoHeader->biBitCount<9)    //灰度图像
    {
        for(int j=1;j<height-1;j++)    // 每行
            for(int i=1;i<wide-1;i++)    // 每列
            {
                int temp=0;            ;
                if(*(p_data+(height-j-1)*wide+i)==255)//本身为白点
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide+i+n-1);
                        }
                    if(temp<=255*3)///周围8个点中有小于等于2个白点
                        *(p_data+(height-j-1)*wide+i)=0;
                }
            }
    }
    else    //24位彩色
    {
        for(int j=1;j<height-1;j++)    // 每行
            for(int i=1;i<wide-1;i++)    // 每列
            {
                int temp=0;            ;
                if(*(p_data+(height-j-1)*wide*3+i*3)==255)//本身为白点
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide*3+(i+n-1)*3);
                        }
                    if(temp<=255*3)///周围8个点中有小于等于2个白点
                    {
                        *(p_data+(height-j-1)*wide*3+i*3)=0;
                        *(p_data+(height-j-1)*wide*3+i*3+1)=0;
                        *(p_data+(height-j-1)*wide*3+i*3+2)=0;
                    }
                }
            }
    }
}

/***************************************************************/
/*函数名称：biaoji(int T)                                       */
/*函数类型：int                                                */
/*函数参数：int T 二值化阈值                                    */
/*         LPBYTE pdata 标记指针                               */
/*功能：二值化，对图像标记,划分成不同的连通区域。                  */
/***************************************************************/
int JisuanProcessDib::biaoji(int T, LPBYTE pdata)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size]; // 标记每个像素，用于划分连通区域
    memset(p_temp, 255, size);
    memset(flag, 0, sizeof(flag));

    int signID = 0;
    int signCount = 0;
    bool stop = false;

    if (GetRGB()) {
        this->erzhihua(T); ///图像二值化

        // 从左到右标号
        for (int y = 1; y < height - 1; y++) { // 每行
            if (stop) // 判断连通区是否太多
                break;
            for (int x = 1; x < width - 1; x++) { // 每列
                if (signCount > 250) {
                    /*AfxMessageBox("连通区数目太多,请增大阈值");*/
                    stop = true;
                    break;
                }

                int x0 = x;
                int y0 = height - y - 1;
                int indexCurrent = lineBytes * y0 + x0;
                if (p_data[indexCurrent] == 0) {// 若当前点为黑点
                    // 像素坐标
                    int xRightUp = x + 1;
                    int yRightUp = height - (y - 1) - 1;
                    int xUp = x;
                    int yUp = height - (y - 1) - 1;
                    int xLeftUp = x - 1;
                    int yLeftUp = height - (y - 1) - 1;
                    int xLeft = x - 1;
                    int yLeft = height - y - 1;
                    int indexRightUp = lineBytes * yRightUp + xRightUp;
                    int indexUp = lineBytes * yUp + xUp;
                    int indexLeftUp = lineBytes * yLeftUp + xLeftUp;
                    int indexLeft = lineBytes * yLeft + xLeft;

                    // 标记
                    if (p_data[indexRightUp] == 0) { //右上
                        signID = p_temp[indexRightUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;

                        // 1.如果左前与右上标记不同，并且是连通的，修改标记为右上
                        if (p_data[indexLeft] == 0 && p_temp[indexLeft] != signID) { //左前
                            int oldSign = p_temp[indexLeft];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//左前

                        // 2.如果左上与右上标记不同，并且是连通的，修改标记为右上
                        if (p_data[indexLeftUp] == 0 && p_temp[indexLeftUp] != signID) { //左上
                            int oldSign = p_temp[indexLeftUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//左上

                        // 3.如果正上与右上标记不同，并且是连通的，修改标记为右上
                        if (p_data[indexUp] == 0 && p_temp[indexUp] != signID) { //正上
                            int oldSign = p_temp[indexUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//正上
                    }//end//右上
                    else if (p_data[indexUp] == 0) { //正上
                        signID = p_temp[indexUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeftUp] == 0) { //左上
                        signID = p_temp[indexLeftUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeft] == 0) { //左前
                        signID = p_temp[indexLeft];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else { // 没有
                        signCount++;
                        signID = signCount;
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                } //end if
            } //end 每列
        } //end 每行
    }
    else {
        this->RgbToGray();
        //// 由于已转化为24位灰度，故只统计 B 通道 ////
        this->erzhihua(T); ///图像二值化

        // 从左到右标号
        for (int y = 1; y < height - 1; y++) { // 每行
            if (stop) // 判断连通区是否太多
                break;
            for (int x = 1; x < width - 1; x++) { // 每列
                if (signCount > 250) {
                    AfxMessageBox("连通区数目太多,请增大阈值");
                    stop = true;
                    break;
                }

                int x0 = x;
                int y0 = height - y - 1;
                int indexCurrent = lineBytes * y0 + x0 * 3;
                if (p_data[indexCurrent] == 0) {// 若当前点为黑点
                    // 像素坐标
                    int xRightUp = x + 1;
                    int yRightUp = height - (y - 1) - 1;
                    int xUp = x;
                    int yUp = height - (y - 1) - 1;
                    int xLeftUp = x - 1;
                    int yLeftUp = height - (y - 1) - 1;
                    int xLeft = x - 1;
                    int yLeft = height - y - 1;
                    int indexRightUp = lineBytes * yRightUp + xRightUp * 3;
                    int indexUp = lineBytes * yUp + xUp * 3;
                    int indexLeftUp = lineBytes * yLeftUp + xLeftUp * 3;
                    int indexLeft = lineBytes * yLeft + xLeft * 3;

                    // 标记
                    if (p_data[indexRightUp] == 0) { //右上
                        signID = p_temp[indexRightUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;

                        // 1.如果左前与右上标记不同，并且是连通的，修改标记为右上
                        if (p_data[indexLeft] == 0 && p_temp[indexLeft] != signID) { //左前
                            int oldSign = p_temp[indexLeft];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n * 3;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//左前

                        // 2.如果左上与右上标记不同，并且是连通的，修改标记为右上
                        if (p_data[indexLeftUp] == 0 && p_temp[indexLeftUp] != signID) { //左上
                            int oldSign = p_temp[indexLeftUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n * 3;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//左上

                        // 3.如果正上与右上标记不同，并且是连通的，修改标记为右上
                        if (p_data[indexUp] == 0 && p_temp[indexUp] != signID) { //正上
                            int oldSign = p_temp[indexUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n * 3;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//正上
                    }//end//右上
                    else if (p_data[indexUp] == 0) { //正上
                        signID = p_temp[indexUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeftUp] == 0) { //左上
                        signID = p_temp[indexLeftUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeft] == 0) { //左前
                        signID = p_temp[indexLeft];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else { // 没有
                        signCount++;
                        signID = signCount;
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                } //end if
            } //end 每列
        } //end 每行
    }

    count = 0;
    if (!stop) {
        // 区域统计
        for (int i = 1; i <= signCount; i++) {
            if (flag[i] != 0)
                ++count;
        }

        // 区域位置
        for(int i = 0;i < 255; i++)
            pppp[i].pp_area = 0;

        int dibFactor = GetRGB() ? 1 : 3;
        //int dibFactor = (m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;
        for (int t = 1; t <= signCount; t++) {
            pppp[t].pp_number = t;
            for (int y = 1; y < height - 1; y++) {
                for (int x = 1; x < width - 1; x++) {
                    if (p_temp[lineBytes * y + x * dibFactor] == t) { // 查找每个区域的位置
                        pppp[t].pp_x = x;
                        pppp[t].pp_y = height - y - 1;
                        pppp[t].pp_area = flag[t];
                        break;
                    }
                }

                if (pppp[t].pp_area != 0)
                    break;
            }
        }

        if (pdata != nullptr) 
            memcpy(pdata, p_temp, size);
    }

    delete p_temp;
    return signCount;
}

/***************************************************************/
/*函数名称：ClearSMALL(int T1, int T2)                          */
/*函数类型：int                                                 */
/*参数：int T1，二值化阈值                                       */
/*     int T2，面积过滤阈值                                      */
/*功能：消除面积小于给定的阈值的小区域。                           */
/***************************************************************/
int JisuanProcessDib::ClearSMALL(int T1, int T2)
{ 
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];

    int count = biaoji(T1, p_temp); // 标记
    if (count > 0) {
        if(m_pBitmapInfoHeader->biBitCount < 9) {   //灰度图像
            for (int i = 1; i < count; i++) {
                if(flag[i] > 0 && flag[i] < T2) { // 判断连通区的面积（像素个数）是否消除
                    for(int y = 1; y < height - 1; y++) {
                        for(int x = 1; x < width - 1; x++) {
                            if (p_temp[lineBytes * (height - y - 1) + x] == i) {// 标号
                                p_data[lineBytes * (height - y - 1) + x] = 255;
                                pppp[i].pp_area = 0;
                            }
                        } 
                    }
                }
            }
        }
        else { //24位彩色
            for (int i = 1; i < count; i++) {
                if(flag[i] < T2) { // 判断连通区的面积（像素个数）是否消除
                    for(int y = 1; y < height - 1; y++) {
                        for(int x = 1; x < width - 1; x++) {
                            if (p_temp[lineBytes * (height - y - 1) + x * 3] == i) {// 标号
                                p_data[lineBytes * (height - y - 1) + x * 3] = 255;
                                p_data[lineBytes * (height - y - 1) + x * 3 + 1] = 255;
                                p_data[lineBytes * (height - y - 1) + x * 3 + 2] = 255;
                                pppp[i].pp_area = 0;
                            }
                        } 
                    }
                }
            }
        }
    }

    delete p_temp;
    return count;
}

/***************************************************************/
/*函数名称：Borderline(int T)                                  */
/*函数类型：int                                                */
/*函数类型：int                                                */
/*功能：对每个连通区进行边界跟踪，提取边界，输出周长。             */
/***************************************************************/
int JisuanProcessDib::Borderline(int T)
{
    int count = biaoji(T); // 标记
    if (count > 0) {
        if (m_pBitmapInfoHeader->biBitCount == 24)
            Lunkuotiqu2(T);
        else
            Lunkuotiqu(T); // 调用边界跟踪
        
        count = biaoji(T); // 重新标记

        // 区域周长
        for(int i = 0;i < 255; i++) {
            pppp[i].pp_line = pppp[i].pp_area;
            pppp[i].pp_area = 0;
        }
    }

    return count;
}

//将24位彩色图像转换为24位灰度图
void JisuanProcessDib::RgbToGray()
{
    if (GetRGB())
        return;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x += 3) {
            BYTE* pbyBlue = p_data++;   //得到蓝色值
            BYTE* pbyGreen = p_data++;  //得到绿色值
            BYTE* pbyRed = p_data++;    //得到红色值
            BYTE r = *pbyRed;
            BYTE g = *pbyGreen;
            BYTE b = *pbyBlue;
            int gray = (30 * r + 59 * g + 11 * b) /100;
            *pbyBlue = gray;   
            *pbyGreen = gray; 
            *pbyRed = gray;
        }
    }
}

//保存未处理的原图像
void JisuanProcessDib::Baoliu(LPBYTE temp)
{
    // 指向DIB象素指针
    LPBYTE p_data;
    // 指向缓存图像的指针
    LPBYTE    lpDst;
    // 找到DIB图像象素起始位置
    p_data= GetData();
    // DIB的宽度
    LONG wide = GetDibWidthBytes();
    // DIB的高度
    LONG height = GetHeight();    
    // 初始化新分配的内存，设定初始值为255
    lpDst = (LPBYTE)temp;
    memset(lpDst, (BYTE)255, wide * height);
    memcpy(lpDst,p_data,wide*height);
}

///***************************************************************/           
/*函数名称：Lunkuotiqu(int T)                                      
/*函数类型：void  
/*函数参数：int T 二值化阈值
/*功能：对图像进行轮廓提取（对二值图，采用掏空内部点法）。            
/***************************************************************/ 
void JisuanProcessDib::Lunkuotiqu(int T)
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

void JisuanProcessDib::Lunkuotiqu2(int T)
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
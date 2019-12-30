#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "ZhengJiaoBianHuanDib.h"
#include "malloc.h"
#include "MainFrm.h"
#include "DSplitView.h"
#include "Cdib.h"

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 



#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 3.14159265359
#define PI 3.1415926535 
 
ZhengJiaoBianHuanDib::ZhengJiaoBianHuanDib()
{
}
ZhengJiaoBianHuanDib::~ZhengJiaoBianHuanDib()
{
}

CNumber ZhengJiaoBianHuanDib::Add(const CNumber &c1, const CNumber &c2)
{
    CNumber c;
    c.re = c1.re + c2.re;
    c.im = c1.im + c2.im;
    return c;
}
CNumber ZhengJiaoBianHuanDib::Sub(const CNumber &c1, const CNumber &c2)
{
    CNumber c;
    c.re = c1.re - c2.re;
    c.im = c1.im - c2.im;
    return c;
}
CNumber ZhengJiaoBianHuanDib::Mul(const CNumber &c1, const CNumber &c2)
{
    CNumber c;
    c.re = c1.re * c2.re - c1.im * c2.im;
    c.im = c1.re * c2.im + c2.re * c1.im;
    return c;
}


/*************************************************************************
 *
 * 函数名称：FFT(const CNumber *t, const CNumber *f, int r)
 *
 * 参数:
 *   CNumber * t                - 指向时域值的指针
 *   CNumber * f                - 指向频域值的指针
 *   r                         －2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   此函数实现一维快速傅立叶变换。
 *
 ************************************************************************/
void ZhengJiaoBianHuanDib::FFT(const CNumber *t, CNumber *f, int r)
{
    long count; // 傅立叶变换点数
    int i, j, k, p, bfsize;
    CNumber *w, *x, *a, *b; // 复数结构类型的指针变量，其中w指向加权系数
    double angle; // 计算加权系数所用的角度值

    count = 1 << r; // 计算傅立叶变换点数
    w = new CNumber[count / 2];
    a = new CNumber[count];
    b = new CNumber[count];

    // 计算加权系数
    for (i = 0; i < count / 2; i++)
    {
        angle = - i * pi * 2 / count;
        w[i].re = cos(angle);
        w[i].im = sin(angle);
    }

    memcpy(a, t, sizeof(CNumber) * count);

    // 采用频率分解法进行蝶形运算
    for (k = 0; k < r; k++) // 迭代级数(1, 2, 3...)
    {
        for (j = 0; j < (1 << k); j++) // 迭代每个DFT(1, 2, 4...)
        {
            bfsize = 1 << (r - k); // 迭代点数(N, N/2, N/4...)
            for (i = 0; i < bfsize / 2; i++) // 折半迭代(N/2, N/4, N/8...)
            {
                p = j * bfsize;
                b[i + p] = Add(a[i + p], a[i + p + bfsize / 2]); // 前一半
                b[i + p + bfsize / 2] = Mul(Sub(a[i + p], a[i + p + bfsize / 2]), w[i * (1 << k)]); // 后一半
            }
        }

        // 第1级迭代的输出作为第2级的输入，最终的迭代结果存放于a中
        x = a;
        a = b;
        b = x;
    }

    // 将乱序的变换序列重新排序
    for (j = 0; j < count; j++)
    {
        p = 0;
        for (i = 0; i < r; i++) // 遍历二进制位
        {
            if (j & (1 << i))
            {
                p += 1 << (r - i - 1); // 比特翻转
            }
        } //  for i
        f[j] = a[p];
    } // for j

    delete [] w;
    delete [] a;
    delete [] b;
}

///////////////////////////////////////////////
//此函数用来实现图像的傅立叶变换（原始变换）
//两次调用快速傅立叶变换FFT()实现二维傅立叶变换
///////////////////////////////////////////////
void ZhengJiaoBianHuanDib::QuickFourier()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = WIDTHBYTES(width * 8);   // 取得原图的每行字节数（4字节对齐）

    LPBYTE p_data = GetData(); // 指向DIB像素指针

    LONG w = 1; // 进行傅立叶变换的宽度（2的整数次方）
    LONG h = 1; // 进行傅立叶变换的高度（2的整数次方）

    int wp = 0; // 宽方向迭代次数
    int hp = 0; // 高方向迭代次数
    
    CNumber *t; // 时域数据
    CNumber *f; // 频域数据

    // 计算进行傅立叶变换的宽度（2的整数次方）
    while (w * 2 <= width)
    {
        w *= 2;
        wp++;
    }
    // 计算进行傅立叶变换的高度（2的整数次方）
    while (h * 2 <= width)
    {
        h *= 2;
        hp++;
    }

    // 分配数据存储器空间
    t = new CNumber[w * h];
    f = new CNumber[w * h];

    // 给时域赋值
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            t[x + w * y].re = p_data[dibWidth * (height - y - 1) + x];
            t[x + w * y].im = 0;
        }
    }
    
    // 在垂直方向上进行快速傅立叶变换
    for (int y = 0; y < h; y++)
    {
        this->FFT(&t[y * w], &f[y * w], wp);
    }
    
    // 转换变换结果
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            t[y + h * x] = f[x + w * y]; // 将DIB按列排列
        }
    }

    // 在水平方向上进行快速傅立叶变换
    for (int x = 0; x < w; x++)
    {
        this->FFT(&t[x * h], &f[x * h], hp);
    }
    
    // 傅里叶变换变换结果
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int index = y + x * h;
            int temp = sqrt(f[index].re * f[index].re + f[index].im * f[index].im) / 100; // 能量
            if (temp > 255)
                temp=255;
            p_data[dibWidth * (height - y - 1) + x] = (BYTE)(temp); 
        }
    }

    delete [] t;
    delete [] f;
}

///////////////////////////////////////////////
//二维傅立叶变换原点平移（实现一：时域 * 平移因子）
///////////////////////////////////////////////
void ZhengJiaoBianHuanDib::QuickFourier1()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = WIDTHBYTES(width * 8);   // 取得原图的每行字节数（4字节对齐）

    LPBYTE p_data = GetData(); // 指向DIB像素指针

    LONG w = 1; // 进行傅立叶变换的宽度（2的整数次方）
    LONG h = 1; // 进行傅立叶变换的高度（2的整数次方）

    int wp = 0; // 宽方向迭代次数
    int hp = 0; // 高方向迭代次数

    CNumber *t; // 时域数据
    CNumber *f; // 频域数据

    // 计算进行傅立叶变换的宽度（2的整数次方）
    while (w * 2 <= width)
    {
        w *= 2;
        wp++;
    }
    // 计算进行傅立叶变换的高度（2的整数次方）
    while (h * 2 <= width)
    {
        h *= 2;
        hp++;
    }

    // 分配数据存储器空间
    t = new CNumber[w * h];
    f = new CNumber[w * h];

    // 给时域赋值
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            /////////////////////////////////
            int deltaXY = (x + y) % 2 ? -1 : 1; // (-1)^(x + y) 平移因子
            /////////////////////////////////
            t[x + w * y].re = p_data[dibWidth * (height - y - 1) + x] * deltaXY;
            t[x + w * y].im = 0;
        }
    }

    // 在垂直方向上进行快速傅立叶变换
    for (int y = 0; y < h; y++)
    {
        this->FFT(&t[y * w], &f[y * w], wp);
    }

    // 转换变换结果
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            t[y + h * x] = f[x + w * y]; // 将DIB按列排列
        }
    }

    // 在水平方向上进行快速傅立叶变换
    for (int x = 0; x < w; x++)
    {
        this->FFT(&t[x * h], &f[x * h], hp);
    }

    // 傅里叶变换变换结果
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int index = y + x * h;
            int temp = sqrt(f[index].re * f[index].re + f[index].im * f[index].im) / 100; // 能量
            if (temp > 255)
                temp=255;
            p_data[dibWidth * (height - y - 1) + x] = (BYTE)(temp); 
        }
    }

    delete [] t;
    delete [] f;
}

///////////////////////////////////////////////
//二维傅立叶变换原点平移（实现二：对原始变换直接转换）
///////////////////////////////////////////////
void ZhengJiaoBianHuanDib::QuickFourier2()
{
    LONG width = GetWidth();     // DIB的宽度
    LONG height = GetHeight();   // DIB的高度
    LONG dibWidth = WIDTHBYTES(width * 8);   // 取得原图的每行字节数（4字节对齐）

    LPBYTE p_data = GetData(); // 指向DIB像素指针

    LONG w = 1; // 进行傅立叶变换的宽度（2的整数次方）
    LONG h = 1; // 进行傅立叶变换的高度（2的整数次方）

    int wp = 0; // 宽方向迭代次数
    int hp = 0; // 高方向迭代次数

    CNumber *t; // 时域数据
    CNumber *f; // 频域数据

    // 计算进行傅立叶变换的宽度（2的整数次方）
    while (w * 2 <= width)
    {
        w *= 2;
        wp++;
    }
    // 计算进行傅立叶变换的高度（2的整数次方）
    while (h * 2 <= width)
    {
        h *= 2;
        hp++;
    }

    // 分配数据存储器空间
    t = new CNumber[w * h];
    f = new CNumber[w * h];

    // 给时域赋值
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            t[x + w * y].re = p_data[dibWidth * (height - y - 1) + x];
            t[x + w * y].im = 0;
        }
    }

    // 在垂直方向上进行快速傅立叶变换
    for (int y = 0; y < h; y++)
    {
        this->FFT(&t[y * w], &f[y * w], wp);
    }

    // 转换变换结果
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            t[y + h * x] = f[x + w * y]; // 将DIB按列排列
        }
    }

    // 在水平方向上进行快速傅立叶变换
    for (int x = 0; x < w; x++)
    {
        this->FFT(&t[x * h], &f[x * h], hp);
    }

    ///////////////////////////////////////////////////////////
    // 将傅里叶变换后的原点移动到中心 (0, 0) -> (w/2, h/2)
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int index = y + x * h;
            int temp = sqrt(f[index].re * f[index].re + f[index].im * f[index].im) / 100; // 能量
            if (temp > 255)
                temp=255;
            int W = (x < w / 2) ?  (x + w / 2) : (x - w / 2);
            int H = (y < h / 2) ?  (y + h / 2) : (y - h / 2);
            p_data[dibWidth * (height - H - 1) + W] = (BYTE)(temp); // 水平垂直镜像
        }
    }
    ///////////////////////////////////////////////////////////

    delete [] t;
    delete [] f;
}


/*************************************************************************
 *
 * 函数名称：FCT(const double *t, double *f, int r)
 *
 * 参数:
 *   double * t                - 指向时域值的指针
 *   double * f                - 指向频域值的指针
 *   r                        －2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速离散余弦变换。该函数利用2N点的快速付立叶变换
 * 来实现离散余弦变换。
 *
 ************************************************************************/
void ZhengJiaoBianHuanDib::FCT(const double *t, double *f, int r)
{
    // 离散余弦变换点数
    long    count;    
    // 循环变量
    int        i;    
    // 中间变量
    double    dTemp;
    CNumber *X;    
    // 计算离散余弦变换点数
    count = 1<<r;
    // 分配内存
    X=new CNumber[count*2];
    // 赋初值为0
    memset(X, 0, sizeof(CNumber) * count * 2);
    // 将时域点写入数组X
    for(i=0;i<count;i++)
    {
        X[i].re=t[i];
        X[i].im=0;
    }
    // 调用快速付立叶变换
    FFT(X,X,r+1);
    // 调整系数
    dTemp = 1/sqrt((double)count);
    // 求F[0]
    f[0] = X[0].re * dTemp;
    dTemp *= sqrt((double)2);
    // 求F[u]    
    for(i = 1; i < count; i++)
    {
        f[i]=(X[i].re * cos(i*PI/(count*2)) + X[i].im * sin(i*PI/(count*2))) * dTemp;
    }    
    // 释放内存
    delete X;
}

/*************************************************************************
 *
 * 函数名称：
 *   WALSH()
 *
 * 参数:
 *   double * t                - 指向时域值的指针
 *   double * f                - 指向频域值的指针
 *   r                        －2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速沃尔什-哈达玛变换。
 *
 ************************************************************************/

void ZhengJiaoBianHuanDib::WALSH(double *t, double *f, int r)
{
    // 沃尔什-哈达玛变换点数
    long   count;    
    // 循环变量
    int        i,j,k;    
    // 中间变量
    int        bfsize,p;
    double *X1,*X2,*X;    
    // 计算快速沃尔什变换点数
    count = 1 << r;
    // 分配运算所需的数组
    X1 = new double[count];
    X2 = new double[count];
    // 将时域点写入数组X1
    memcpy(X1, t, sizeof(double) * count);
    // 蝶形运算
    for(k = 0; k < r; k++)
    {
        for(j = 0; j < 1<<k; j++)
        {
            bfsize = 1 << (r-k);
            for(i = 0; i < bfsize / 2; i++)
            {
                p = j * bfsize;
                X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
                X2[i + p + bfsize / 2] = X1[i + p] - X1[i + p + bfsize / 2];
            }
        }
        // 互换X1和X2  
        X = X1;
        X1 = X2;
        X2 = X;
    }
    // 调整系数
    for(j = 0; j < count; j++)
    {
        p = 0;
        for(i = 0; i < r; i++)
        {
            if (j & (1<<i))
            {
                p += 1 << (r-i-1);
            }
        }
        f[j] = X1[p] / count;
    }    
    // 释放内存
    delete X1;
    delete X2;
}


/*************************************************************************
 *
 * 函数名称：DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
 *
 * 参数:
 *   LPBYTE lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *  bool               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行离散余弦变换。
 *
 ************************************************************************/
bool ZhengJiaoBianHuanDib::DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
{
    // 指向源图像的指针
    LPBYTE    lpSrc;
    // 循环变量
    LONG    i;
    LONG    j;
    // 进行付立叶变换的宽度和高度（2的整数次方）
    LONG    w;
    LONG    h;
    // 中间变量
    double    dTemp;
    int        wp;
    int        hp;
    // 图像每行的字节数
    LONG    lLineBytes;
    // 计算图像每行的字节数
    lLineBytes = WIDTHBYTES(lWidth * 8);
    // 赋初值
    w = 1;
    h = 1;
    wp = 0;
    hp = 0;
    // 计算进行离散余弦变换的宽度和高度（2的整数次方）
    while(w * 2 <= lWidth)
    {
        w *= 2;
        wp++;
    }
    while(h * 2 <= lHeight)
    {
        h *= 2;
        hp++;
    }
    // 分配内存
    double *f = new double[w * h];
    double *F = new double[w * h];
    // 行
    for(j = 0; j < h; j++)
    {
        // 列
        for(i = 0; i < w; i++)
        {
            // 指向DIB第j行，第i个象素的指针
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
            // 给时域赋值
            f[i + j * w] = *(lpSrc);
        }
    }
    
    for(j = 0; j < h; j++)
    {
        // 对y方向进行离散余弦变换
        FCT(&f[w * j], &F[w * j], wp);
    }
    // 保存计算结果
    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++)
        {
            f[i * h + j] = F[i + w * j];
        }
    }
    for(i = 0; i < w; i++)
    {
        // 对x方向进行离散余弦变换
        FCT(&f[i * h], &F[i * h], hp);
    }
    // 行
    for(j = 0; j < h; j++)
    {
        // 列
        for(i = 0; i < w; i++)
        {
            // 计算频谱
            dTemp = fabs(F[i*h+j]);
            // 判断是否超过255
            if (dTemp > 255)
            {
                // 对于超过的，直接设置为255
                dTemp = 255;
            }
            // 指向DIB第y行，第x个象素的指针
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
            // 更新源图像
            * (lpSrc) = (BYTE)(dTemp);
        }
    }    
    // 释放内存
    delete f;
    delete F;
    //返回
    return true;
}



/*************************************************************************
 *
 * 函数名称：
 *   DIBWalsh()
 *
 * 参数:
 *   LPBYTE lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *  bool               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行沃尔什-哈达玛变换。于上面不同的是，此处是将二维
 * 矩阵转换成一个列向量，然后对该列向量进行一次一维沃尔什-哈达玛变换。
 *
 ************************************************************************/

bool ZhengJiaoBianHuanDib::DIBWalsh(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
{    
    // 指向源图像的指针
    LPBYTE    lpSrc;
    // 循环变量
    LONG    i;
    LONG    j;
    // 进行付立叶变换的宽度和高度（2的整数次方）
    LONG    w;
    LONG    h;
    // 中间变量
    double    dTemp;
    int        wp;
    int        hp;
    // 图像每行的字节数
    LONG    lLineBytes;
    // 计算图像每行的字节数
    lLineBytes = WIDTHBYTES(lWidth * 8);
    // 赋初值
    w = 1;
    h = 1;
    wp = 0;
    hp = 0;
    // 计算进行离散余弦变换的宽度和高度（2的整数次方）
    while(w * 2 <= lWidth)
    {
        w *= 2;
        wp++;
    }    
    while(h * 2 <= lHeight)
    {
        h *= 2;
        hp++;
    }
    // 分配内存
    double *f = new double[w * h];
    double *F = new double[w * h];
    // 列
    for(j = 0; j < w; j++)
    {
        // 行
        for(i = 0; i < h; i++)
        {
            // 指向DIB第j行，第i个象素的指针
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
            // 给时域赋值
            f[i + j * w] = *(lpSrc);
        }
    }    
    // 调用快速沃尔什－哈达玛变换
    WALSH(f, F, wp + hp);
    // 列
    for(j = 0; j < w; j++)
    {
        // 行
        for(i = 0; i < h; i++)
        {
            // 计算频谱
            dTemp = fabs(F[j * w + i] * 1000);
            // 判断是否超过255
            if (dTemp > 255)
            {
                // 对于超过的，直接设置为255
                dTemp = 255;
            }
            // 指向DIB第j行，第i个象素的指针
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
            // 更新源图像
            * (lpSrc) = (BYTE)(dTemp);
        }
    }    
    //释放内存
    delete f;
    delete F;
    //返回
    return true;
}

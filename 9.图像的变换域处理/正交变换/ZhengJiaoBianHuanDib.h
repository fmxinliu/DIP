#ifndef __ZhengJiaoBianHuanDib_H
#define __ZhengJiaoBianHuanDib_H

#include "Cdib.h"

struct CNumber
{
    double re;
    double im;
};
class ZhengJiaoBianHuanDib : public CDib
{
public:
    ZhengJiaoBianHuanDib();
    ~ZhengJiaoBianHuanDib();

public:  
    void QuickFourier();//此函数用来实现图像的傅立叶变换（原始傅里叶变换）
    void QuickFourier1();//此函数用来实现图像的傅立叶变换（原点平移，实现1）
    void QuickFourier2();//此函数用来实现图像的傅立叶变换（原点平移，实现2）
    bool DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);//此函数实现图像的离散余弦变换
    bool DIBWalsh(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);//该函数用来实现图像的沃尔什-哈达玛变换。

private:
    CNumber Add(const CNumber &c1, const CNumber &c2);//实现复数的加法运算
    CNumber Sub(const CNumber &c1, const CNumber &c2);//实现复数的减法运算
    CNumber Mul(const CNumber &c1, const CNumber &c2);//实现复数的乘法运算
    void FFT(const CNumber *t, CNumber *f, int power); //此函数实现一维快速傅立叶变换
    void LiSan(double *t, double *f, int r);//此函数实现离散余弦变换
    void WALSH(double *t, double *f, int r);//该函数用来实现快速沃尔什-哈达玛变换。
};
#endif
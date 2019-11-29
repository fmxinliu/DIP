#ifndef __BingXingBianJieDib_H
#define __BingXingBianJieDib_H
#include "Cdib.h"
 
typedef struct{
    int Value;
    int Dist;
    int AngleNumber;
}    MaxValue;

typedef struct{
    int Height;
    int Width;
}    Seed;

typedef struct{
    int Height;
    int Width;
}    Point;

class BingXingBianJieDib :public CDib
{

public:
    BingXingBianJieDib();
    ~BingXingBianJieDib();

public:
    void Lunkuogenzong(CPoint SeedPoint);
    void Lunkuotiqu(CPoint SeedPoint);
    void Qiyuzengzhang(CPoint point);
    void Zhongzitianchong(CPoint SeedPoint);
    void Lunkuogenzong(int T);
    void Lunkuotiqu(int T);
    void Lunkuotiqu2(int T);
    void BanYuZhi(int Yuzhi);  // 阈值或半阈值分割法，需要人工根据灰度直方图选取阈值（一般为波谷），分割前景和背景
    void Yuzhifenge(int Yuzhi);
    void Diedaifazhi(); // 迭代阈值法，指定初始阈值，由程序自动迭代计算阈值
    void Fenbutongji(int *tongji);
    void Zhifangtu(float *tongji);
    void RgbToGray();
};
#endif
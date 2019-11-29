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
    void BanYuZhi(int Yuzhi);  // ��ֵ�����ֵ�ָ����Ҫ�˹����ݻҶ�ֱ��ͼѡȡ��ֵ��һ��Ϊ���ȣ����ָ�ǰ���ͱ���
    void Yuzhifenge(int Yuzhi);
    void Diedaifazhi(); // ������ֵ����ָ����ʼ��ֵ���ɳ����Զ�����������ֵ
    void Fenbutongji(int *tongji);
    void Zhifangtu(float *tongji);
    void RgbToGray();
};
#endif
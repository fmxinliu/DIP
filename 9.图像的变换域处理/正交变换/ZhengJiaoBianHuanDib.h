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
    void QuickFourier();//�˺�������ʵ��ͼ��ĸ���Ҷ�任��ԭʼ����Ҷ�任��
    void QuickFourier1();//�˺�������ʵ��ͼ��ĸ���Ҷ�任��ԭ��ƽ�ƣ�ʵ��1��
    void QuickFourier2();//�˺�������ʵ��ͼ��ĸ���Ҷ�任��ԭ��ƽ�ƣ�ʵ��2��
    bool DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);//�˺���ʵ��ͼ�����ɢ���ұ任
    bool DIBWalsh(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight);//�ú�������ʵ��ͼ����ֶ�ʲ-������任��

private:
    CNumber Add(const CNumber &c1, const CNumber &c2);//ʵ�ָ����ļӷ�����
    CNumber Sub(const CNumber &c1, const CNumber &c2);//ʵ�ָ����ļ�������
    CNumber Mul(const CNumber &c1, const CNumber &c2);//ʵ�ָ����ĳ˷�����
    void FFT(const CNumber *t, CNumber *f, int power); //�˺���ʵ��һά���ٸ���Ҷ�任
    void LiSan(double *t, double *f, int r);//�˺���ʵ����ɢ���ұ任
    void WALSH(double *t, double *f, int r);//�ú�������ʵ�ֿ����ֶ�ʲ-������任��
};
#endif
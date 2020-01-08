#ifndef __PinYuLuBoDib_H
#define __PinYuLuBoDib_H
#include "Cdib.h"
struct CNumber
{
    double re;
    double im;
};
class PinYuLuBoDib :public CDib
{

public:
    PinYuLuBoDib();
    ~PinYuLuBoDib();

public:
     void QFC(CNumber* t,CNumber* f,int power);//ʵ�ֿ��ٸ���Ҷ�任
     void QuickFourier();//ʵ��ͼ����ٸ���Ҷ�任
     void FirstQuickFourier();//ʵ��ͼ����ٸ���Ҷ�任(δ����ƽ��)
     void fourier(double *data, int height, int width, int isign);//ʵ�ֶ�ά����Ҷ�任
     void BWFilterL(int u,int v,int n);//������˹��ͨ�˲�
     void BWFilterH(int u,int v,int n);//������˹��ͨ�˲�
     void PerfectFilterL(int u,int v);//�����ͨ�˲���
     void PerfectFilterH(int u,int v);//�����ͨ�˲���
     void TLFilter(int u,int v,int u1,int v1);//���ε�ͨ�˲�
     void THFilter(int u,int v,int u1,int v1);//���θ�ͨ�˲�
     void ZLFilter(int u,int v,int n);//ָ����ͨ�˲�
     void ZHFilter(int u,int v,int n);//ָ����ͨ�˲�
private:
    CNumber Add(const CNumber &c1, const CNumber &c2);//ʵ�ָ����ļӷ�����
    CNumber Sub(const CNumber &c1, const CNumber &c2);//ʵ�ָ����ļ�������
    CNumber Mul(const CNumber &c1, const CNumber &c2);//ʵ�ָ����ĳ˷�����
};
#endif
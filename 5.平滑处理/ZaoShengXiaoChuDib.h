#ifndef __ZaoShengXiaoChuDib_H
#define __ZaoShengXiaoChuDib_H
#include "Cdib.h"

// 平滑滤波（卷积运算）
class ZaoShengXiaoChuDib :public CDib
{
public:
    ZaoShengXiaoChuDib();
    ~ZaoShengXiaoChuDib();

public:
    void Chaoxian(int T);
    void GuDing(int YuZhi);    //固定阈值法二值化
    void HeiBaiDianZaoSheng();    //消除黑白点噪声
    void GuliHaiDian(int connec); //消除孤立黑像素点
    void threethree();//3*3均值滤波
    void nn(int n);//n*n均值滤波
    void nnzhong(int n);//n*n中值滤波
    void shizi(int n); //十字型中值滤波器
    void nnzuida(int n); //n*n最大值滤波
    void suijizaosheng();//产生随机噪声
    void jiaoyanzaosheng();//产生椒盐噪声
    void jubupingjun();//有选择的局部平均化 
protected: 
};
#endif
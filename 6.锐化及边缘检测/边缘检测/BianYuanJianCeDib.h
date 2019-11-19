#ifndef __BianYuanJianCeDib_H
#define __BianYuanJianCeDib_H
#include "Cdib.h"
class BianYuanJianCeDib :public CDib
{

public:
    BianYuanJianCeDib();
    ~BianYuanJianCeDib();
public:

    void Krisch();
    void Guasslaplacian();
    void Laplacian(bool onlyborder = true);
    void Sobel();
    void PreWitt();
    void Robert();
    void Template(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef);
    void Templat(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef);
    void Templat24bit(BYTE* m_pdata,int DibWidth,int height,int tempH,int tempW,int tempMX,int tempMY,float *fpArray,float fCoef);

protected:
};
#endif
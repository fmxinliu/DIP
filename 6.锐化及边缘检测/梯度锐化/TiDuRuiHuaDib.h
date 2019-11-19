#ifndef __TiDuRuiHuaDib_H
#define __TiDuRuiHuaDib_H
#include "Cdib.h"


class TiDuRuiHuaDib :public CDib
{
 
public:
    void TiduzhiDaiti();
    void Erzhirihua(int T);
    void Menxianruihua(int T);
    void GuDingRuiHua(int T, bool erase);
    TiDuRuiHuaDib();
    ~TiDuRuiHuaDib();

    void Threshold();
    void RgbToGray();
    void RgbToGrayFast();
    BYTE AvgGray();

protected:
     
};
#endif 
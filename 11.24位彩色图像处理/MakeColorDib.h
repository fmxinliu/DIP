// MakeColorDib.h: interface for the MakeColorDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAKECOLORDIB_H__C1036995_2726_4538_BDAC_B392A7192AD9__INCLUDED_)
#define AFX_MAKECOLORDIB_H__C1036995_2726_4538_BDAC_B392A7192AD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Cdib.h"

class MakeColorDib : public CDib  
{
public:
    MakeColorDib();
    virtual ~MakeColorDib();

public:
    void Mosaic();
    void LowLVBObig();
    void ShuangXiangGROW();
    void ChuiZhiGROW();
    void ShuiPingGROW();
    void LowLVBO();
    void HighLVBO(int m_GaoTong);
    void Sharp();
    void Spread();
    void Embossment();
    void NeonLight();
    void Smoothness();
    void PaintColor(int red, int green, int blue);
    void Exposal();
    void ContrastAlter(int increment);
    void LightReverse();
    void LightAlter(int light);
    void MakeGray();
};

#endif // !defined(AFX_MAKECOLORDIB_H__C1036995_2726_4538_BDAC_B392A7192AD9__INCLUDED_)

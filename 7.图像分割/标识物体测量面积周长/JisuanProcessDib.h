// JisuanProcessDib.h: interface for the JisuanProcessDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JISUANPROCESSDIB_H__6385E9FA_7E01_4785_9F75_56E9F77F4702__INCLUDED_)
#define AFX_JISUANPROCESSDIB_H__6385E9FA_7E01_4785_9F75_56E9F77F4702__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Cdib.h"

typedef struct{
    int Height;
    int Width;
}Point;

struct object{
    int pp_x;
    int pp_y;
    int pp_area;
    int pp_line;
    int pp_number;
};


class JisuanProcessDib : public CDib  
{
public:
    void Baoliu(LPBYTE temp);
    void RgbToGray();
    int  Borderline(int T);
    void Lunkuotiqu(int T);
    void Lunkuotiqu2(int T);
    int  ClearSMALL(int T1, int T2);
    int  biaoji(int T, LPBYTE pdata = nullptr);
    void xiaochugulidianBAI();
    void xiaochugulidianHEI();
    void erzhihua(int yuzhi_gray);
             JisuanProcessDib();
    virtual ~JisuanProcessDib();

////////////////////////////////////////////////////
    object pppp[255]; // 区域标号位置
    int flag[255]; // 区域面积
    int count; // 区域个数
////////////////////////////////////////////////////

};

#endif // !defined(AFX_JISUANPROCESSDIB_H__6385E9FA_7E01_4785_9F75_56E9F77F4702__INCLUDED_)

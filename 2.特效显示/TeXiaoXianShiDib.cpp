#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "TeXiaoXianShiDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"

 
 
TeXiaoXianShiDib::TeXiaoXianShiDib()
{
    
}
TeXiaoXianShiDib::~TeXiaoXianShiDib()
{
}

LONG TeXiaoXianShiDib::LineBytes()
{
    DWORD dwLineBytes;

    int nWidth = GetWidth();
    int nBits = m_pBitmapInfoHeader->biBitCount;

    switch (nBits)
    {
    case 1:
        dwLineBytes = (nWidth + 7) / 8;
        break;
    case 4:
        dwLineBytes = (nWidth + 1) / 2;
        break;
    case 24:
        dwLineBytes = nWidth * 3;
        break;
    case 32:
        dwLineBytes = nWidth * 4;
        break;
    default:
        dwLineBytes = nWidth;
        break;
    }

    // 一个扫描行需 4 字节对齐
    while (dwLineBytes % 4) {
        dwLineBytes++;
    }
    
    return dwLineBytes;
}



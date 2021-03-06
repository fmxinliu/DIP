// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2

IMPLEMENT_DYNCREATE(CDynSplitView2, CView)

CDynSplitView2::CDynSplitView2()
{state2=0;
}

void CDynSplitView2::clearmem()
{
    CDSplitDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    pDoc->statedoc=0;
    state2=1;
    CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
    long int size=CDib1->GetHeight()*CDib1->GetDibWidthBytes();
    memcpy(CDibNew1->m_pData, CDib1->m_pData, size);
}

CPalette * CDynSplitView2::CreateBitmapPalette(TuXiangHeChengDib * pBitmap)
{
    struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } palette = { 0x300, 256 };
        
    LPRGBQUAD pRGBTable = pBitmap->GetRGB();
    UINT numberOfColors = pBitmap->GetNumberOfColors();
        
    for(UINT x=0; x<numberOfColors; ++x)
    {
        palette.aEntries[x].peRed =
            pRGBTable[x].rgbRed;
        palette.aEntries[x].peGreen =
            pRGBTable[x].rgbGreen;
        palette.aEntries[x].peBlue =
            pRGBTable[x].rgbBlue;
        palette.aEntries[x].peFlags = 0;
    }
        
        hPalette.CreatePalette((LPLOGPALETTE)&palette);
    //CPalette hPale=&hPalette;
    return &hPalette;
}

CDynSplitView2::~CDynSplitView2()
{
}
CDSplitDoc* CDynSplitView2::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDSplitDoc)));
    return (CDSplitDoc*)m_pDocument;
}


BEGIN_MESSAGE_MAP(CDynSplitView2, CView)
    //{{AFX_MSG_MAP(CDynSplitView2)
    ON_WM_ERASEBKGND()
    ON_COMMAND(ID_FILESAVE, OnFilesave)
    ON_COMMAND(ID_Add, OnAdd)
    ON_COMMAND(ID_Sub, OnSub)
    ON_COMMAND(ID_Yuyunsuan, OnYuyunsuan)
    ON_COMMAND(ID_Huoyunsuan, OnHuoyunsuan)
    ON_COMMAND(ID_Feiyunsuan, OnFeiyunsuan)
    ON_COMMAND(ID_Huofei, OnHuofei)
    ON_COMMAND(ID_Yufei, OnYufei)
    ON_COMMAND(ID_Yihuo, OnYihuo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 drawing

void CDynSplitView2::OnDraw(CDC* pDC)
{    
    CDSplitDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if(!pDoc ->statedoc&&state2==1)
    {
        int m_scale=1;
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
        int scaledWidth = (int)(bitmapWidth * m_scale);
        int scaledHeight = (int)(bitmapHeight * m_scale);
        if (CDibNew1->GetRGB()) // Has a color table
        {
            CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
            ::StretchDIBits(pDC->GetSafeHdc(),0,0,scaledWidth,scaledHeight,
                0,0,bitmapWidth,bitmapHeight,
                pBitmapData,pBitmapInfo,
                DIB_RGB_COLORS, SRCCOPY);
            pDC->SelectPalette(hOldPalette, true);
            hPalette->DeleteObject();
        }
        else
        {
            StretchDIBits(pDC->GetSafeHdc(),
                0, 0, scaledWidth, scaledHeight,
                0, 0, bitmapWidth, bitmapHeight,
                pBitmapData, pBitmapInfo,
                DIB_RGB_COLORS, SRCCOPY);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 diagnostics

#ifdef _DEBUG
void CDynSplitView2::AssertValid() const
{
    CView::AssertValid();
}

void CDynSplitView2::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 message handlers

BOOL CDynSplitView2::OnEraseBkgnd(CDC* pDC) 
{
    CRect rect;
    GetClientRect(&rect);
    pDC->FillSolidRect(&rect,::GetSysColor(COLOR_WINDOW));
    return TRUE;
}


void CDynSplitView2::OnFilesave() 
{
    // TODO: Add your command handler code here
    CFileDialog dlg(FALSE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("位图文件(*.BMP)|*.BMP|"));    
    if(IDOK==dlg.DoModal())
    {
        CString  filename;
        filename.Format ("%s",dlg.GetPathName() );    
        CDibNew1->SaveFile(filename);
        state2=1;
    }
}


/*加运算消息映射*/
void CDynSplitView2::OnAdd() 
{
    handleDib(&TuXiangHeChengDib::Add);
}


/*减运算消息映射*/
void CDynSplitView2::OnSub() 
{
    handleDib(&TuXiangHeChengDib::Sub);
}


/*与运算消息映射*/
void CDynSplitView2::OnYuyunsuan() 
{
    handleDib(&TuXiangHeChengDib::Yuyunsuan);
}


/*或运算消息映射*/
void CDynSplitView2::OnHuoyunsuan() 
{
    handleDib(&TuXiangHeChengDib::Huoyunsuan);
}


/*非运算消息映射*/
void CDynSplitView2::OnFeiyunsuan() 
{
    // TODO: Add your command handler code here
    clearmem();
    CDibNew1->Feiyunsuan();//调用非运算函数进行运算
    Invalidate();
}


/*或非运算消息映射*/
void CDynSplitView2::OnHuofei() 
{
    handleDib(&TuXiangHeChengDib::Huofei);
}


/*与非运算消息映射*/
void CDynSplitView2::OnYufei() 
{
    handleDib(&TuXiangHeChengDib::Yufei);
}


/*与或运算消息映射*/
void CDynSplitView2::OnYihuo() 
{
    handleDib(&TuXiangHeChengDib::Yihuo);
}

void CDynSplitView2::handleDib(void (TuXiangHeChengDib::*pf)(LPBYTE p_data, LPBYTE p_dataBK, int width, int height))
{
    AfxMessageBox("选择背景图");
    CFileDialog dlg(TRUE, "bmp", "*.bmp");    
    if(dlg.DoModal() == IDOK)
    {
        clearmem();
        filename.Format ("%s",dlg.GetPathName()); 
        CDib1->LoadFile(filename);

        LPBYTE p_data = CDibNew1->GetData();
        LPBYTE p_dataBK = CDib1->GetData(); 
        int width = CDibNew1->GetWidth();
        int height = CDibNew1->GetHeight();

        if (CDib1->GetWidth() != width || 
            CDib1->GetHeight() != height ||
            CDibNew1->m_pBitmapInfoHeader->biBitCount != CDib1->m_pBitmapInfoHeader->biBitCount)
        {
            AfxMessageBox("图像不一致，不能运算");
        }
        else
        {
            (CDibNew1->*pf)(p_data, p_dataBK, width, height);  // pf为指向类成员函数的函数指针，调用方法
            Invalidate();
        }
    }  
}
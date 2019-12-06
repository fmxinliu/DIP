// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "JisuanProcessDib.h"
#include "YuZhi.h"
#include "DELSMALL.h"
#include "SquareDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2

IMPLEMENT_DYNCREATE(CDynSplitView2, CView)

CDynSplitView2::CDynSplitView2()
{
    count = 0;
    state2 = 0;
    state3 = 0;
}

void CDynSplitView2::clearmem()
{
    CDSplitDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    pDoc ->statedoc=0;
    state2=1;
    CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
    long int  size=CDib1->GetHeight()*CDib1->GetDibWidthBytes();
    memcpy(CDibNew1->m_pData,CDib1->m_pData,size);
}

CPalette * CDynSplitView2::CreateBitmapPalette(JisuanProcessDib * pBitmap)
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
    ON_COMMAND(ID_AREAJISUAN, OnAreajisuan)
    ON_COMMAND(ID_XIAOCHUSMALL, OnXiaochusmall)
    ON_COMMAND(ID_FOLLOWLINE, OnFollowline)
    ON_COMMAND(ID_A, OnMarkPart)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 drawing

void CDynSplitView2::OnDraw(CDC* pDC)
{    
    CDSplitDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if(!pDoc->statedoc&&state2==1)
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
            ::StretchDIBits(pDC->GetSafeHdc(),0, 0, bitmapWidth, bitmapHeight,
                0, 0, bitmapWidth, bitmapHeight,
                pBitmapData, pBitmapInfo,
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

        // 绘制标号
        if (state3 == 1 && count != 0)
        {
            CClientDC dc(this);   
            dc.SetTextColor(100);
            CString ss_Area[255];
            for(int i = 1; i <= count; i++) {
                if(CDibNew1->pppp[i].pp_area != 0) {
                    ss_Area[i].Format("%d", CDibNew1->pppp[i].pp_number);
                    dc.TextOut(CDibNew1->pppp[i].pp_x, CDibNew1->pppp[i].pp_y, ss_Area[i]);
                }
            }
        }

        // 绘制面积
        if (state3 == 2 && count != 0)
        {
            CClientDC dc(this);
            dc.SetTextColor(100);
            CString ss_Area[255];
            for(int i = 1; i <= count; i++) {
                if(CDibNew1->pppp[i].pp_area != 0)
                    ss_Area[i].Format("%d", CDibNew1->pppp[i].pp_area);
                dc.TextOut(CDibNew1->pppp[i].pp_x, CDibNew1->pppp[i].pp_y, ss_Area[i]);
            }
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
    CFileDialog dlg(FALSE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("位图文件(*.BMP)|*.BMP|"));    
    if(IDOK==dlg.DoModal())
    {
        CString  filename;
        filename.Format ("%s",dlg.GetPathName() );    
        CDibNew1->SaveFile(filename);
        state2=1;
    }
}

//面积消除消息函数
void CDynSplitView2::OnXiaochusmall() 
{
    clearmem();

    LPBYTE temp;
    int i,j;
    int wide,height;

    CYuZhi Dlg;
    Dlg.DoModal();
    yuzhi_gray=Dlg.m_gray;
    if(CDibNew1->m_pBitmapInfoHeader->biBitCount==24)
    {
         wide=CDibNew1->GetDibWidthBytes();
         height=CDibNew1->GetHeight();
         temp = new BYTE[wide*height];
         memset(temp, (BYTE)255, wide * height);
         CDibNew1->Baoliu(temp);
         CDibNew1->RgbToGray();
    }
    CDibNew1->erzhihua(yuzhi_gray); //图像二值化

    if(CDibNew1->m_pBitmapInfoHeader->biBitCount==24)
    {
        LPBYTE  lpSrc,lpDst,temp2;
        lpSrc=CDibNew1->GetData();
        lpDst=temp;
        temp2=lpSrc;
        for(j=0;j<height;j++)
            for(i=0;i<wide;i++)
            {    
                *lpSrc=*lpDst+*lpSrc;
                if(*lpSrc>255)
                    *lpSrc=255;
                lpSrc++;
                lpDst++;
                }
            lpSrc=temp2;
    }
    int m_value;
    DELSMALL  Dlg1;
    Dlg1.DoModal();
    m_value=Dlg1.m_delsmall;
    CDibNew1->ClearSMALL(m_value); //消除小区域     
     Invalidate();
}


//计算周长消息函数
void CDynSplitView2::OnFollowline() 
{
    clearmem();
    LPBYTE temp;
    int i,j;
    int wide,height;

    CYuZhi Dlg;
    Dlg.DoModal();
    yuzhi_gray=Dlg.m_gray;
    if(CDibNew1->m_pBitmapInfoHeader->biBitCount==24)
    {
         wide=CDibNew1->GetDibWidthBytes();
         height=CDibNew1->GetHeight();
         temp = new BYTE[wide*height];
         memset(temp, (BYTE)255, wide * height);

         CDibNew1->Baoliu(temp);
         CDibNew1->RgbToGray();
    }

    CDibNew1->erzhihua(yuzhi_gray);//图像二值化
    CDibNew1->xiaochugulidianHEI();//消除孤立点黑像素
    CDibNew1->Borderline();//边界跟踪
    CClientDC dc(this);   
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
                dc.SelectPalette(hPalette, true);
            dc.RealizePalette();
            ::StretchDIBits(dc.GetSafeHdc(),0, 0, scaledWidth, scaledHeight,
                0, 0, bitmapWidth, bitmapHeight,
                pBitmapData, pBitmapInfo,
                DIB_RGB_COLORS, SRCCOPY);
            dc.SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
        }
        else
        {
            ::StretchDIBits(dc.GetSafeHdc(),0, 0, scaledWidth, scaledHeight,
                0, 0, bitmapWidth, bitmapHeight,
                pBitmapData, pBitmapInfo,
                DIB_RGB_COLORS, SRCCOPY);
        }
    }

    dc.SetTextColor(100);
    CString ss_line[255];
    for( i=0;i<255;i++)
    {
        if(CDibNew1->pppp[i].pp_line!=0)
        ss_line[i].Format("%d",CDibNew1->pppp[i].pp_line);
        dc.TextOut(CDibNew1->pppp[i].pp_x,CDibNew1->pppp[i].pp_y,ss_line[i]);
    }
}

//显示标记消息函数
void CDynSplitView2::OnMarkPart() 
{
    clearmem();
 
    CYuZhi dlg;
    dlg.m_gray = 100;
    dlg.DoModal();

    count = 0;
    count = CDibNew1->biaoji(dlg.m_gray); //标记、计算像素区
    if (count) {
        state3 = 1;
        Invalidate();
    }
    else {
        AfxMessageBox("连通区数目太多,请增大阈值"); 
    }
}

//计算面积消息函数
void CDynSplitView2::OnAreajisuan() 
{
    clearmem();

    CYuZhi dlg;
    dlg.m_gray = 100;
    dlg.DoModal();

    count = 0;
    count = CDibNew1->biaoji(dlg.m_gray); //标记、计算像素区
    if (count) {
        state3 = 2;
        Invalidate();
    }
    else {
        AfxMessageBox("连通区数目太多,请增大阈值"); 
    }
}

// 提示区域面积
void CDynSplitView2::ShowSquare(int count)
{
    if (count != 0) {
        int signID = 0;  // 标记id
        int areaSum = 0; // 总的像素面积
        int fg[255] = {0}; // 各个区域的面积
        memset(fg, 0, sizeof(fg)); //初始化赋值都为0

        // 区域统计
        for (int i = 1; i <= count; i++) {
            if (CDibNew1->flag[i] != 0) {
                fg[signID] = CDibNew1->flag[i];
                ++signID;
                areaSum += CDibNew1->flag[i];
            }
        }

        SquareDlg dlg; // 输出对话框
        dlg.m_number = signID; // 输出连通区域个数
        dlg.m_squareALL = areaSum; // 输出连通区域的总积
        CString ss[20];
        // 在对话框里输出每个连通区的面积（区域像素个数）
        for(int i = 0; i < signID; i++) {
            ss[i].Format("连通区：%3d，该区面积:%10.0d\r\n", i+1, fg[i]);
            dlg.m_ShuChu += ss[i];
        }
        dlg.DoModal();
    }
}
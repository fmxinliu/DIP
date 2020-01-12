// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "LightDlg.h"
#include "ontrastDlg.h"
#include "PaintColorDlg.h"
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
    state2=0;
    m_byRed = m_byGreen = m_byBlue = 255;
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
    memcpy(CDibNew1->m_pData,CDib1->m_pData,size);
}

CPalette * CDynSplitView2::CreateBitmapPalette(MakeColorDib * pBitmap)
{
    struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } palette = { 0x300, 256 };
        
    LPRGBQUAD pRGBTable = pBitmap->GetRGB();
    UINT numberOfColors = pBitmap->GetNumberOfColors();

    if(numberOfColors!=24)
    {
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
    ON_COMMAND(ID_MakegGray, OnMakeGray)
    ON_COMMAND(ID_LightAlter, OnLightAlter)
    ON_COMMAND(ID_LightReverse, OnLightReverse)
    ON_COMMAND(ID_ContrastAlter, OnContrastAlter)
    ON_COMMAND(ID_Exposal, OnExposal)
    ON_COMMAND(ID_PaintColor, OnPaintColor)
    ON_COMMAND(ID_Smoothness, OnSmoothness)
    ON_COMMAND(ID_NeonLight, OnNeonLight)
    ON_COMMAND(ID_Embossment, OnEmbossment)
    ON_COMMAND(ID_Spread, OnSpread)
    ON_COMMAND(ID_Sharp, OnSharp)
    ON_COMMAND(ID_High_LVBO, OnHighLVBO)
    ON_COMMAND(ID_Low_LVBO, OnLowLVBO)
    ON_COMMAND(ID_ShuiPing_GROW, OnShuiPingGROW)
    ON_COMMAND(ID_ChuiZhi_GROW, OnChuiZhiGROW)
    ON_COMMAND(ID_ShuangXiang_GROW, OnShuangXiangGROW)
    ON_COMMAND(ID_High_LVBOnormal, OnHighLVBOnormal)
    ON_COMMAND(ID_High_LVBOexcessive, OnHighLVBOexcessive)
    ON_COMMAND(ID_Low_LVBObig, OnLowLVBObig)
    ON_COMMAND(ID_Mosaic, OnMosaic)
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
    CFileDialog dlg(FALSE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("λͼ�ļ�(*.BMP)|*.BMP|"));    
    if(IDOK==dlg.DoModal())
    {
        CString  filename;
        filename.Format ("%s",dlg.GetPathName() );    
        CDibNew1->SaveFile(filename);
        state2=1;
    }
}


void CDynSplitView2::OnMakeGray()   //�Ҷȵ��� 
{
    clearmem();
    CDibNew1->MakeGray();
    Invalidate();
}

void CDynSplitView2::OnLightAlter()   //���ȵ���
{
    LightDlg dlg; 
    dlg.m_Light=0;
    dlg.DoModal(); 

    clearmem();
    CDibNew1->LightAlter(dlg.m_Light);
    Invalidate();
}

void CDynSplitView2::OnLightReverse()  //����ȡ��
{
    clearmem();
    CDibNew1->LightReverse();
    Invalidate();
}

void CDynSplitView2::OnContrastAlter()   //�Աȶȵ���
{
    ContrastDlg dlg;
    dlg.m_Increment=0;
    dlg.DoModal();

    clearmem();
    CDibNew1->ContrastAlter(dlg.m_Increment);
    Invalidate();
}

void CDynSplitView2::OnExposal()   //ͼ���ع�
{
    clearmem();
    CDibNew1->Exposal();
    Invalidate();
}

void CDynSplitView2::OnPaintColor()  //ͼ����ɫ
{
    PaintColorDlg dlg;
    dlg.m_Red = m_byRed;
    dlg.m_Green = m_byGreen;
    dlg.m_Blue = m_byBlue;

    if(dlg.DoModal() == IDOK)
    {
        m_byRed=dlg.m_Red;
        m_byGreen=dlg.m_Green;
        m_byBlue=dlg.m_Blue; 
    }

    clearmem();
    CDibNew1->PaintColor(m_byRed, m_byGreen, m_byBlue);
    Invalidate();    
}

void CDynSplitView2::OnSmoothness()  //ͼ��ƽ��
{
    clearmem();
    CDibNew1->Smoothness();
    Invalidate();
}

void CDynSplitView2::OnNeonLight() //ͼ���޺�
{
    clearmem();   //ȡ��ԭʼͼ��Ŀ����ļ�
    CDibNew1->NeonLight();   //����ͼ���޺纯��
    Invalidate();           //����ˢ�º���    
}

void CDynSplitView2::OnEmbossment()   //ͼ�񸡵�
{
    clearmem();
    CDibNew1->Embossment();
    Invalidate();  
}

void CDynSplitView2::OnSpread() //ͼ����ɢ
{
    clearmem();
    CDibNew1->Spread();
    Invalidate();
}

void CDynSplitView2::OnSharp()
{
    clearmem();
    CDibNew1->Sharp();
    Invalidate(); 
}

void CDynSplitView2::OnHighLVBO() ///��ͨ�˲�(����)
{  
    clearmem();
    CDibNew1->HighLVBO(1);
    Invalidate();
}

void CDynSplitView2::OnHighLVBOnormal() ///��ͨ�˲����еȣ�
{
    clearmem();
    CDibNew1->HighLVBO(2);
    Invalidate();
}

void CDynSplitView2::OnHighLVBOexcessive() ///��ͨ�˲���������
{ 

    clearmem();
    CDibNew1->HighLVBO(3);
    Invalidate();
}

void CDynSplitView2::OnLowLVBO() //��ͨ�˲�(3x3)
{
    clearmem();
    CDibNew1->LowLVBO();
    Invalidate();
}

void CDynSplitView2::OnLowLVBObig()   //��ͨ�˲�(5x5)
{
    clearmem();
    CDibNew1->LowLVBObig();
    Invalidate();
}

void CDynSplitView2::OnShuiPingGROW() ///ˮƽ��ǿ
{
    clearmem();
    CDibNew1->ShuiPingGROW();
    Invalidate();
}

void CDynSplitView2::OnChuiZhiGROW() ///��ֱ��ǿ
{
    clearmem();
    CDibNew1->ChuiZhiGROW();
    Invalidate();
}

void CDynSplitView2::OnShuangXiangGROW() ///˫����ǿ
{
    clearmem();
    CDibNew1->ShuangXiangGROW();
    Invalidate();
}

void CDynSplitView2::OnMosaic() // ������
{
    clearmem();
    CDibNew1->Mosaic();
    Invalidate();
}

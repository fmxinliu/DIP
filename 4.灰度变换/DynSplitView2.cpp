// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "DlgZhiFangTu.h"
#include "DlgChuangkoubianhuan.h" 
#include "DlgZhexianbianhuan.h" 
#include "DlgGuDingFaZhi.h"
#include "DialogShuangYu.h" 
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
}


/*��ʼ��ͼ������*/
void CDynSplitView2::clearmem()
{
    CDSplitDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    pDoc ->statedoc=0;
    state2=1;
    CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
    long int  size=CDib1->GetHeight()*CDib1->GetWidth();
    memcpy(CDibNew1->m_pData,CDib1->m_pData,size);
}

CPalette * CDynSplitView2::CreateBitmapPalette(HuiDuBianHuanDib * pBitmap)
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
    ON_COMMAND(ID_Zhifangtutongji, OnZhifangtutongji)
    ON_COMMAND(ID_FanSeBianHuan, OnFanSeBianHuan)
    ON_COMMAND(ID_Chuangkoubianhuan, OnChuangkoubianhuan)
    ON_COMMAND(ID_ZheXianBianHuan, OnZheXianBianHuan)
    ON_COMMAND(ID_Fenbujunhenghua, OnFenbujunhenghua)
    ON_COMMAND(ID_Pipeibianhuan, OnPipeibianhuan)
    ON_COMMAND(ID_Yuantuzhifangtu, OnYuantuzhifangtu)
    ON_COMMAND(ID_Shuipingtouying, OnShuipingtouying)
    ON_COMMAND(ID_Chuizhitouying, OnChuizhitouying)
    ON_COMMAND(ID_Fei0, OnFei0)
    ON_COMMAND(ID_GUDING, OnGuding)
    ON_COMMAND(ID_ShuanGuDingFa, OnShuanGuDingFa)
    ON_COMMAND(ID_RedZhiFangtu, OnRedZhiFangtu)
    ON_COMMAND(ID_BlueZhiFangtu, OnBlueZhiFangtu)
    ON_COMMAND(ID_GreenZhifangtu, OnGreenZhifangtu)
    ON_COMMAND(ID_RgbToGray, &CDynSplitView2::OnRgbtogray)
    ON_COMMAND(ID_RgbToGrayFast, &CDynSplitView2::OnRgbtograyfast)
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
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int m_scale=1;
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
        filename.Format ("%s",dlg.GetPathName() );    
        CDibNew1->SaveFile(filename);
        state2=1;
    }
}

//��ɫ�任��Ϣӳ��
void CDynSplitView2::OnFanSeBianHuan() 
{
    state3=0;

    clearmem2();
    CDibNew1->FanSeBianHuan();                                //���÷�ɫ�任������
    Invalidate();                                            //����ˢ�º���
}

//�Ҷȴ��ڱ任��Ϣӳ��
void CDynSplitView2::OnChuangkoubianhuan() 
{
    state3=0;
    
    // �����Ի���
    CDlgChuangkoubianhuan  dlg;
    
    // ��������
    BYTE    bLow;    
    // ��������
    BYTE    bUp;
    
    // ��ʼ������ֵ
    dlg.m_bLow = 50;
    dlg.m_bUp = 200;
    
    // ��ʾ�Ի�����ʾ�û��趨����������
    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    // ��ȡ�û��趨�Ĵ���������
    bLow = dlg.m_bLow;
    bUp = dlg.m_bUp;
        
    clearmem2();
    CDibNew1->Chuangkoubianhuan(bLow, bUp);             //���ô��ڱ任������
    Invalidate();    
}

//���߱任��Ϣӳ��
void CDynSplitView2::OnZheXianBianHuan() 
{
    state3=0;
    
    // �����Ի���
    CDlgZhexianbianhuan dlgPara;
    
    // ��1����
    BYTE    bX1;
    BYTE    bY1;    
    // ��2����
    BYTE    bX2;
    BYTE    bY2;
    
    // ��ʼ������ֵ
    dlgPara.m_bX1 = 50;
    dlgPara.m_bY1 = 30;
    dlgPara.m_bX2 = 200;
    dlgPara.m_bY2 = 220;
    
    // ��ʾ�Ի�����ʾ�û��趨����λ��
    if (dlgPara.DoModal() != IDOK)
    {
        return;
    } 

    // ��ȡ�û����趨
    bX1 = dlgPara.m_bX1;
    bY1 = dlgPara.m_bY1;
    bX2 = dlgPara.m_bX2;
    bY2 = dlgPara.m_bY2;
    
    // ����Zhexianbianhuan()�������лҶ�����     
    clearmem2();
    CDibNew1->Zhexianbianhuan(bX1, bY1, bX2, bY2);             //�������߱任������
    Invalidate();           //����ˢ�º���
}

//ͼ��Ҷȷֲ����⻯��Ϣӳ��
void CDynSplitView2::OnFenbujunhenghua() 
{
    state3=0;
    
    clearmem2();
    CDibNew1->Fenbujunhenghua();             //���÷ֲ����⻯������
    Invalidate();    
}

//ƥ��任��Ϣӳ��
void CDynSplitView2::OnPipeibianhuan() 
{    
    clearmem();
    int n;
    state3=0;
    
    // ����Ҷȷֲ��ܶ�
    CDibNew1->ZhiFangTu(midu2);             //���ûҶ�ͳ�ƴ�����
    
    // �����Ի���
    CDlgZhiFangTu*  dlg2;
    dlg2=new CDlgZhiFangTu(this);
    dlg2->Create(IDD_DLG_zhifangtufenbu);
    
    // ���ݻҶȷֲ��ܶ����ݸ������
    for (n = 0; n <256; n++)
        dlg2->m_fIntensity[n] = midu2[n];
    
    //--------ֱ��ͼ���� ��ʼ------------------------------------------
    int nu[64];
    float pu[64];
    
    float a=1.0f/(32.0f*63.0f);
    for(int i=0;i<64;i++)
    {
        nu[i]=i*4;
        pu[i]=a*i;
    }
    //--------ֱ��ͼ���� ����------------------------------------------
    if(CDibNew1->m_pBitmapInfoHeader->biBitCount<9)        //�Ҷ�ͼ��
    {
        clearmem();
        CDibNew1->Pipeibianhuan(64,nu, pu);             //����ƥ��仯������
        Invalidate();           //����ˢ�º���
    }
    else        //24λ��ɫ
    {
        clearmem2();
        CDibNew1->Pipeibianhuan(64,nu, pu);             //����ƥ��仯������
        Invalidate();           //����ˢ�º���
    }
}

//ˮƽͶӰ��Ϣӳ��
void CDynSplitView2::OnShuipingtouying()
{
    int i;
    state3=0;

    clearmem2();
    CDibNew1->Shuipingtouying();             //����ˮƽͶӰ������
    Invalidate();
}

//��ֱͶӰ��Ϣӳ��
void CDynSplitView2::OnChuizhitouying()  
{
    clearmem();
    int i;
    state3=0;
    
    // ����Ҷȷֲ��ܶ�
    CDibNew1->ZhiFangTu(midu2);             //���ûҶ�ͳ�ƴ�����
    
    // �����Ի���
    CDlgZhiFangTu*  dlg2;     
    dlg2=new CDlgZhiFangTu(this);
    dlg2->Create(IDD_DLG_zhifangtufenbu);
    
    // ���ݻҶȷֲ��ܶ����ݸ������
    for (i = 0; i <256; i++)
        dlg2->m_fIntensity[i] = midu2[i];
    if(CDibNew1->m_pBitmapInfoHeader->biBitCount<9)        //�Ҷ�ͼ��
    {
        clearmem();
        CDibNew1->Chuizhitouying();             //���ô�ֱͶӰ������
        Invalidate();
    }
    else        //24λ��ɫ
    {
        clearmem2();
        CDibNew1->Chuizhitouying();             //���ô�ֱͶӰ������
        Invalidate();
    }
}
 
//����ȡһ���Ҷȱ任��Ϣӳ��
void CDynSplitView2::OnFei0() 
{
    state3=0;

    clearmem2();
    CDibNew1->Fei0();             //���÷���ȡһ������
    Invalidate();
}

//�̶���ֵ�Ҷȱ任��Ϣӳ��
void CDynSplitView2::OnGuding() 
{
    int i;
    state3=0;
   
    CDlgGuDingFaZhi dlg;
    if(IDOK==dlg.DoModal())
    {
        clearmem2();
        CDibNew1->GuDing(dlg.m_Yuzhi);
        Invalidate();
    }
}

void CDynSplitView2::OnShuanGuDingFa() 
{
    state3=0;

    CDialogShuangYu dlg;
    if(IDOK==dlg.DoModal())
    {
        clearmem2();
        CDibNew1->ShuangYu(dlg.m_ZhiYu1,dlg.m_ZhiYu2,dlg.m_mode);
        Invalidate();
    }
}

void CDynSplitView2::clearmem2()
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

//ԭͼֱ��ͼ��ʾ
void CDynSplitView2::OnYuantuzhifangtu()  
{
    clearmem2();
    // ����ɫ�����ĻҶȷֲ��ܶ�
    float midu[256];

    // ����Ҷȷֲ��ܶ�      
    CDibNew1->ZhiFangTu(midu);             //���ûҶ�ͳ�ƴ�����

    // �����Ի���
    CDlgZhiFangTu*  dlg3;
    dlg3=new CDlgZhiFangTu(this); // ������ڴ��ɶԻ����Լ�ȥ�ͷ�
    dlg3->Create(IDD_DLG_zhifangtufenbu);

    // ���ݻҶȷֲ��ܶ����ݸ������
    for (int i = 0; i < 256; i++)
        dlg3->m_fIntensity[i] = midu[i];

    // ��ʾ�Ի���
    dlg3->ShowWindow(SW_RESTORE);
}

//�����ֱ��ͼ��ʾ
void CDynSplitView2::OnZhifangtutongji() 
{
    // ����ɫ�����ĻҶȷֲ��ܶ�
    float midu[256];

    // ����Ҷȷֲ��ܶ�
    CDSplitDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    pDoc ->statedoc=0;
    state2=1;

    CDibNew1->ZhiFangTu(midu);             //���ûҶ�ͳ�ƴ�����

    // �����Ի���
    CDlgZhiFangTu*  dlg;     
    dlg=new CDlgZhiFangTu(this);
    dlg->Create(IDD_DLG_zhifangtufenbu);
    // ���ݻҶȷֲ��ܶ����ݸ������
    for (int i = 0; i <256; i++)
        dlg->m_fIntensity[i] = midu[i];

    // ��ʾ������ֱ��ͼ
    dlg->ShowWindow(SW_RESTORE);
}

void CDynSplitView2::OnRedZhiFangtu() 
{
    clearmem2();
    // ����ɫ�����ĻҶȷֲ��ܶ�
    float midu[256];
    
    // ����Ҷȷֲ��ܶ�      
    CDibNew1->RedZhiFangTu(midu);             //���ûҶ�ͳ�ƴ�����
    
    // �����Ի���
    CDlgZhiFangTu*  dlg3;
    dlg3=new CDlgZhiFangTu(this);
    dlg3->Create(IDD_DLG_zhifangtufenbu);
    
    // ���ݻҶȷֲ��ܶ����ݸ������
    for (int i = 0; i < 256; i++)
        dlg3->m_fIntensity[i] = midu[i];
    
    // ��ʾ�Ի���
    dlg3->ShowWindow(SW_RESTORE);
    Invalidate();
}

void CDynSplitView2::OnBlueZhiFangtu() 
{
    clearmem2();
    // ����ɫ�����ĻҶȷֲ��ܶ�
    float midu[256];
    
    // ����Ҷȷֲ��ܶ�      
    CDibNew1->BlueZhiFangTu(midu);             //���ûҶ�ͳ�ƴ�����
    
    // �����Ի���
    CDlgZhiFangTu*  dlg3;
    dlg3=new CDlgZhiFangTu(this);
    dlg3->Create(IDD_DLG_zhifangtufenbu);
    
    // ���ݻҶȷֲ��ܶ����ݸ������
    for (int i = 0; i < 256; i++)
        dlg3->m_fIntensity[i] = midu[i];
    
    // ��ʾ�Ի���
    dlg3->ShowWindow(SW_RESTORE);
    Invalidate();
}

void CDynSplitView2::OnGreenZhifangtu() 
{
    clearmem2();
    // ����ɫ�����ĻҶȷֲ��ܶ�
    float midu[256];
    
    // ����Ҷȷֲ��ܶ�      
    CDibNew1->GreenZhiFangTu(midu);             //���ûҶ�ͳ�ƴ�����
    
    // �����Ի���
    CDlgZhiFangTu*  dlg3;
    dlg3=new CDlgZhiFangTu(this);
    dlg3->Create(IDD_DLG_zhifangtufenbu);
    
    // ���ݻҶȷֲ��ܶ����ݸ������
    for (int i = 0; i < 256; i++)
        dlg3->m_fIntensity[i] = midu[i];
    
    // ��ʾ�Ի���
    dlg3->ShowWindow(SW_RESTORE);
    Invalidate();
}


// rgb -> gray
void CDynSplitView2::OnRgbtogray()
{
    clearmem2();
    if (!CDibNew1->GetRGB())
    {
        CDibNew1->RgbToGray();
    }

    Invalidate();
}


void CDynSplitView2::OnRgbtograyfast()
{
    clearmem2();
    if (!CDibNew1->GetRGB())
    {
        CDibNew1->RgbToGrayFast();
    }

    Invalidate();
}

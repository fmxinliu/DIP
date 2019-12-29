#if !defined(AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_)
#define AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DynSplitView2.h : header file
//
#include"DSplitDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 view
#include "ZhengJiaoBianHuanDib.h"
#include "CDIB.h"

class CDynSplitView2 : public CView
{
protected:
    CDynSplitView2();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CDynSplitView2)

// Attributes
public:
    CDSplitDoc* GetDocument();

// Operations
public:
    void clearmem2();
    CPalette *CreateBitmapPalette( ZhengJiaoBianHuanDib* pBitmap);
    CString filename;
    ZhengJiaoBianHuanDib *CDibNew1;
    CDib *CDib1;
    CPalette hPalette;
    LPBITMAPINFO p_bihed;
    int state2;
    void clearmem();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDynSplitView2)
    protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CDynSplitView2();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    //{{AFX_MSG(CDynSplitView2)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnFilesave();
    afx_msg void OnQuickForuier();
    afx_msg void OnLisanyuxuan();
    afx_msg void OnWalsh();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnQuickforuierOrigin();
    afx_msg void OnQuickforuierTransimpl1();
    afx_msg void OnQuickforuierTransimpl2();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_)

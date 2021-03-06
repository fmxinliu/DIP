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
#include "TuXiangHeChengDib.h"
#include "CDIB.h"

typedef void (*PF)(LPBYTE p_data, LPBYTE p_dataBK, int width, int height);

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
        CPalette *CreateBitmapPalette(TuXiangHeChengDib* pBitmap);
        CString filename;
        TuXiangHeChengDib *CDibNew1;
        CDib *CDib1,*CDib2;
        CPalette hPalette;
        int state2;
        void clearmem();
        void handleDib(void (TuXiangHeChengDib::*pf)(LPBYTE p_data, LPBYTE p_dataBK, int width, int height));

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
    afx_msg void OnAdd();
    afx_msg void OnSub();
    afx_msg void OnYuyunsuan();
    afx_msg void OnHuoyunsuan();
    afx_msg void OnFeiyunsuan();
    afx_msg void OnHuofei();
    afx_msg void OnYufei();
    afx_msg void OnYihuo();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNSPLITVIEW2_H__76E949DA_9CAD_11D1_907F_00A024782894__INCLUDED_)

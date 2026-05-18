
// ThrowingAppView.h : interface of the CThrowingAppView class
//

#pragma once
#include "ThrowingAppDoc.h"
#include "resultsdialog.h"
#include "afxwin.h"
// class CThrowingAppDoc; // Forward declaration


class CThrowingAppView : public CScrollView
{
protected: // create from serialization only
	CThrowingAppView();
	DECLARE_DYNCREATE(CThrowingAppView)

// Attributes
public:
	CThrowingAppDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
protected:
	virtual void OnInitialUpdate(void);  // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CThrowingAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	double height; // Height
	static const double gravity; // Gravitational constant, or Gravity of Earth
	double time;  // Time
	double v0; // Initial velocity
	double zheight; // Height of the medium
	double flightTime; // Flight time
	void Calculate(void);
	double phi0;  // Throw angle
public:
	afx_msg void OnOptionsSetparameters();
protected:
	double flightDistance; // Flight Distance
	double scaleX;
	double scaleY;
	
public:
	CResultsDialog m_resultsDlg;
	void ShowResultsDialog(void);
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateScrollSize(void);
	void DrawXAxisTicks(CDC* pDC, const CRect& rectClient);
	CFont m_headerFont;
public:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
protected:
	void PrintPageHeader(CDC* pDC, CPrintInfo* pInfo, CString& sHeader);
public:
	afx_msg void OnOptionsEstimateangle();
};

#ifndef _DEBUG  // debug version in ThrowingAppView.cpp
inline CThrowingAppDoc* CThrowingAppView::GetDocument() const
   { return reinterpret_cast<CThrowingAppDoc*>(m_pDocument); }
#endif


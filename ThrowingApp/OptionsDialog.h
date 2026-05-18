#pragma once
#include "afxcolorbutton.h"
#include "afxwin.h"


// COptionsDialog dialog

class COptionsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(COptionsDialog)

public:
	COptionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_h;
	double m_phi0;
	double m_v0;
	double m_scaleX;
	double m_scaleY;
	COLORREF m_color;
protected:
	CMFCColorButton m_colorDisplay;
public:
	afx_msg void OnBnClickedChooseColor();	
protected:
	CEdit m_edtInitialVelocity;
	CEdit m_edtThrowAngle;
	CEdit m_edtLaunchHeight;
	CEdit m_edtScaleX;
	CEdit m_edtScaleY;
	double GetPositiveDoubleFromEdit(CEdit& editControl);
public:
	afx_msg void OnBnClickedOk();
};

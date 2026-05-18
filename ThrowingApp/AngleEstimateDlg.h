#pragma once
#include "afxwin.h"


// CAngleEstimateDlg dialog

class CAngleEstimateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAngleEstimateDlg)

public:
	CAngleEstimateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAngleEstimateDlg();

// Dialog Data
	enum { IDD = IDD_THROW_ESTIMATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	const double dPii;
	double m_dFlightDistance;
	double m_dInitialVelocity;
	double m_dThrowAngleEstimated;
	double m_dThrowAngle;
	double m_dInitialVelocityEstimated;
	CString m_szFlightDistance;
	CString m_szInitialVelocity;
	CString m_szEstimatedThrowAngle;
	CString m_szThrowAngle;
	CString m_szEstimatedInitialVelocity;
	CEdit m_edtFlightDistance;
	CEdit m_edtInitialVelocity;
	CEdit m_edtEstimatedThrowAngle;
	CEdit m_edtThrowAngle;
	CEdit m_edtEstimatedInitialVelocity;
	CEdit m_edtLaunchHeight;
	double m_dLaunchHeight;
	CString m_szLaunchHeight;
public:
	afx_msg void OnBnClickedEstimateThrowAngleButton();
	afx_msg void OnBnClickedEstimateInitialVelocityButton();
protected:
	const double dGravity;
	double sec(double dX);
	double GetPositiveDoubleFromEdit(CEdit& editControl);
};

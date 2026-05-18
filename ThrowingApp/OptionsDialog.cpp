// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ThrowingApp.h"
#include "OptionsDialog.h"
#include "afxdialogex.h"
#include <string>
#include <stdexcept>


// COptionsDialog dialog

IMPLEMENT_DYNAMIC(COptionsDialog, CDialogEx)

COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(COptionsDialog::IDD, pParent)
	, m_h(1.75)
	, m_phi0(35)
	, m_v0(20)
	, m_scaleX(50)
	, m_scaleY(50)
	, m_color(RGB(0,0,255))
{

}

COptionsDialog::~COptionsDialog()
{
}

void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_H, m_h);
	DDX_Text(pDX, IDC_EDIT_PHI0, m_phi0);
	DDX_Text(pDX, IDC_EDIT_V0, m_v0);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_scaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_scaleY);
	DDX_Control(pDX, IDC_COLOR_DISPLAY, m_colorDisplay);
	DDX_Control(pDX, IDC_EDIT_V0, m_edtInitialVelocity);
	DDX_Control(pDX, IDC_EDIT_PHI0, m_edtThrowAngle);
	DDX_Control(pDX, IDC_EDIT_H, m_edtLaunchHeight);
	DDX_Control(pDX, IDC_EDIT_SCALEX, m_edtScaleX);
	DDX_Control(pDX, IDC_EDIT_SCALEY, m_edtScaleY);
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CHOOSE_COLOR, &COptionsDialog::OnBnClickedChooseColor)
	ON_BN_CLICKED(IDOK, &COptionsDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// COptionsDialog message handlers


void COptionsDialog::OnBnClickedChooseColor()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(m_color);
	if (dlg.DoModal() == IDOK)
	{
		m_color = dlg.GetColor();
		Invalidate(); // Trigger a repaint to show the new color
	}
}



double COptionsDialog::GetPositiveDoubleFromEdit(CEdit& editControl)
{
	CString strValue;
	editControl.GetWindowTextW(strValue);

	if ( strValue.IsEmpty()){
		// Handle empty input (e.g., show an error message)
		AfxMessageBox(_T("Input cannot be empty."), MB_ICONERROR);
		return 0.0;
	}

	// Convert CString to std::string
	std::string stringValue = CT2A(strValue);

	try {
		double value = std::stod(stringValue);
		if ( value <= 0.0 ) {
			throw std::runtime_error("Input must be a positive number.");
		}
		return value;
	} catch ( const std::exception& e) {
		// Handle the exception (e.g., display an error message)
		AfxMessageBox(_T("Error: ") + CString(e.what()), MB_ICONERROR);
		return 0.0;
	}	
}


void COptionsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_h = GetPositiveDoubleFromEdit(m_edtLaunchHeight);
	m_phi0 = GetPositiveDoubleFromEdit(m_edtThrowAngle);
	m_v0 = GetPositiveDoubleFromEdit(m_edtInitialVelocity);
	m_scaleX = GetPositiveDoubleFromEdit(m_edtScaleX);
	m_scaleY = GetPositiveDoubleFromEdit(m_edtScaleY);
	CDialogEx::OnOK();
}

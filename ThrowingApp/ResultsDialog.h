#pragma once
#include "afxwin.h"
#include <vector>
#include "ThrowingAppDoc.h"

// CResultsDialog dialog

class CResultsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CResultsDialog)

public:
	CResultsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultsDialog();

// Dialog Data
// #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESULTS_DIALOG };
// #endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	// Implementation
	virtual BOOL OnInitDialog(); // Add 

	DECLARE_MESSAGE_MAP()
public:
	
	void UpdateResults(const std::vector<ThrowParams>& throws);
	void AppendTextWithColor(const CString& text, COLORREF color, int throwNumber);
private:
	CRichEditCtrl m_resultsEdit;  // Use CRichEditCtrl instead of CEdit
public:
	void ClearResults(void);
	afx_msg void OnBnClickedButtonPrint();
protected:
	void OnFilePrint(void);	
	void PrintRichEditContents(void);
	void OnFilePrintSetup(void);
};

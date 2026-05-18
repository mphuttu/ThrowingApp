// ResultsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ThrowingApp.h"
#include "ResultsDialog.h"
#include "afxdialogex.h"
#include "ThrowingAppDoc.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>


// CResultsDialog dialog

IMPLEMENT_DYNAMIC(CResultsDialog, CDialogEx)

CResultsDialog::CResultsDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResultsDialog::IDD, pParent)
{

}

CResultsDialog::~CResultsDialog()
{
}

void CResultsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULTS_RICHEDIT, m_resultsEdit);
}


BEGIN_MESSAGE_MAP(CResultsDialog, CDialogEx)
	ON_COMMAND(ID_FILE_PRINT, &CResultsDialog::OnFilePrint) // Menu item handler
	ON_COMMAND(ID_FILE_PRINTSETUP, &CResultsDialog::OnFilePrintSetup) // Menu item handler
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CResultsDialog::OnBnClickedButtonPrint) // Button click handler
END_MESSAGE_MAP()


// CResultsDialog message handlers

BOOL CResultsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if ( !AfxInitRichEdit2())
	{
		AfxMessageBox(_T("Failed to initialize RichEdit 2.0"));
		return FALSE;
	}
	if ( !AfxInitRichEdit())
	{
		AfxMessageBox(_T("Failed to initialize RichEdit 1.0"));
		return FALSE;
	}

	// Set up the RichEdit control
	// Set up the RichEdit control
    ASSERT(m_resultsEdit.GetSafeHwnd());  // Ensure the RichEdit control was created
	if ( !m_resultsEdit.GetSafeHwnd() )
	{
		AfxMessageBox(_T("Failed to create RichEdit control."));
		return FALSE;
	}
	// Set options for this rich edit control
	m_resultsEdit.SetOptions(ECOOP_SET, ECO_AUTOWORDSELECTION | 
      ECO_AUTOVSCROLL | ECO_WANTRETURN | ECO_SAVESEL);
   
	m_resultsEdit.DragAcceptFiles(TRUE);
   // GenerateRichText();
    m_resultsEdit.SetBackgroundColor(FALSE, RGB(255, 255, 255)); // White background
    m_resultsEdit.SetEventMask(ENM_CHANGE | ENM_SELCHANGE | ENM_KEYEVENTS | ENM_MOUSEEVENTS);
	m_resultsEdit.SetWindowTextW(_T("Initial Results...\r\n"));
	m_resultsEdit.ShowWindow(SW_SHOW);

	// Initialize your dialog controls here
	if (!::IsWindow(m_resultsEdit.m_hWnd))
	{
		AfxMessageBox(_T("Error: Rich Edit Control not initialized"));
		return FALSE; // If the control is not initialized, return FALSE to abort.
	}

	// Initialize your dialog controls here
	// For example, you can set the initial text the edit control:
	// Safely set the text, ensuring the pointer is valid
	try
	{
		m_resultsEdit.SetWindowTextW(_T("Results\n"));
	}

	catch (CInvalidArgException* e)
	{
		e->Delete();
		AfxMessageBox(_T("Failed to set text in Rich Edit Control due to invalid argument."));
	}

	return TRUE;  // Return TRUE unless you set the focus to the control
	              // EXCEPTION: OCX Property Pages should return FALSE.
}

void CResultsDialog::AppendTextWithColor(const CString& text, COLORREF color, int throwNumber)
{
	// Set up the RichEdit control
    ASSERT(m_resultsEdit.GetSafeHwnd());  // Ensure the RichEdit control was created

	CHARFORMAT2 cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR | CFM_SIZE | CFM_BOLD;
	cf.crTextColor = RGB(0,0,0); // Default to black for text
	cf.yHeight = 200; // Text height (font size)

	if (throwNumber == 1)
	{
		// Add the Header Row
		m_resultsEdit.SetSel(-1,-1);
		m_resultsEdit.ReplaceSel(_T("Nr.\tColor\tLaunch Height\tInitial Velocity\tThrow Angle\tLength of the Throw\r\n"));
	}

	// Set Number
	CString line;
	line.Format(_T("%d\t"), throwNumber);
	m_resultsEdit.SetSel(-1,-1);
	m_resultsEdit.ReplaceSel(line);

	// Add Color
	m_resultsEdit.SetSel(-1,-1);
	cf.crTextColor = color;
	m_resultsEdit.SetSelectionCharFormat(cf);
	m_resultsEdit.ReplaceSel(_T("■\t")); // Square to represent color

	// Add Data
	m_resultsEdit.SetSel(-1,-1);
	cf.crTextColor = RGB(0,0,0); // Reset text color to black
	m_resultsEdit.SetSelectionCharFormat(cf);
	m_resultsEdit.ReplaceSel(text + _T("\r\n"));
	// Set the selection to the end of the text
	//CHARRANGE range;
	//m_resultsEdit.GetSel(range);

	//// Append the text at the end
	//range.cpMin = range.cpMax = -1;
	//m_resultsEdit.SetSel(range);

	//

	//m_resultsEdit.SetSelectionCharFormat(cf);
	//m_resultsEdit.ReplaceSel(text);
}

void CResultsDialog::UpdateResults(const std::vector<ThrowParams>& throws)
{
	// Set up the RichEdit control
	// Ensure the RichEdit control was created
    ASSERT(m_resultsEdit.GetSafeHwnd());  // Ensure the RichEdit control was created

	// Clear any existing text
	m_resultsEdit.SetWindowTextW(_T(""));
	
	// Set up the CHARFORMAT2 for the header
	CHARFORMAT2 cfHeader;
	memset(&cfHeader, 0, sizeof(cfHeader));
	cfHeader.cbSize = sizeof(cfHeader);
	cfHeader.dwMask = CFM_BOLD | CFM_SIZE | CFM_COLOR | CFM_FACE; // Mask for bold, size, color, and font
	cfHeader.dwEffects = CFE_BOLD; // Bold effect
	cfHeader.yHeight = 180; // Font size (in twips, 20 twips = 1 point, so 180 = 9pt)
	cfHeader.crTextColor = RGB(0,0,0); // Black color for the header font
	_tcscpy_s(cfHeader.szFaceName, _T("Arial")); // Set font name

	// Apply the header formatting and insert the header text
	m_resultsEdit.SetSel(-1,-1);
	m_resultsEdit.SetSelectionCharFormat(cfHeader);

	// Add the header 
	CString headerText;
	headerText.Format(_T("Nr.\tColor\tLaunch Height\tInitial Velocity\tThrow Angle\tLength of the Throw\r\n"));
	// m_resultsEdit.SetSel(-1,-1);
	m_resultsEdit.ReplaceSel(headerText);

	// Reset the font to normal (non-bold) and smaller size for the data rows
	CHARFORMAT2 cfData;
	memset(&cfData, 0, sizeof(cfData));
	cfData.cbSize = sizeof(cfData);
	cfData.dwMask = CFM_BOLD | CFM_SIZE | CFM_COLOR | CFM_FACE;
	cfData.dwEffects = 0; // No bold
	cfData.yHeight = 180; // Smaller font size (9pt)
	cfData.crTextColor = RGB(0,0,0); // Black color for the data text
	_tcscpy_s(cfData.szFaceName, _T("Times New Roman")); // Set font name
	
	int throwNumber = 1;
	for (const auto& t : throws)
	{
		// Insert throw data with standard formatting
		m_resultsEdit.SetSel(-1,-1);
		m_resultsEdit.SetSelectionCharFormat(cfData);

		// Format the resultText for each throw
		CString resultText;
		resultText.Format(_T("%d\t"), throwNumber);
		m_resultsEdit.ReplaceSel(resultText);

		// Add the color indicator (a small square)
		CHARFORMAT2 cfColor;
		memset(&cfColor, 0, sizeof(cfColor));
		cfColor.cbSize = sizeof(cfColor);
		cfColor.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
		cfColor.crTextColor = t.color;
		cfColor.yHeight = 200;
		_tcscpy_s(cfColor.szFaceName, _T("Arial")); // Ensure a compatible font

		// Move caret to the end and insert color square
		m_resultsEdit.SetSel(-1, -1);  // Adjust this to set the size of the square
		m_resultsEdit.SetSelectionCharFormat(cfColor);
		m_resultsEdit.ReplaceSel(_T("■\t"));

		// Reset the color to black for the rest of the text
		m_resultsEdit.SetSel(-1,-1);
		m_resultsEdit.SetSelectionCharFormat(cfData);

		// Add the rest of the data
		CString dataText;

		dataText.Format(_T("%.2f m\t\t%.2f m/s\t\t%.2f°\t\t%.2f m\r\n"),
			t.height, t.initialVelocity, t.throwAngle, t.flightDistance);

		// Append text with color
		// AppendTextWithColor(resultText, t.color, throwNumber);
		// resultText += dataText;

		// m_resultsEdit.SetSel(-1,-1);
		m_resultsEdit.ReplaceSel(dataText);

		++throwNumber;
	}
}
void CResultsDialog::ClearResults(void)
{
	// Set up the RichEdit control
    ASSERT(m_resultsEdit.GetSafeHwnd());  // Ensure the RichEdit control was created
	m_resultsEdit.SetWindowTextW(_T(""));
}


void CResultsDialog::OnBnClickedButtonPrint()
{
	// TODO: Add your control notification handler code here
	PrintRichEditContents();
	// Printing Logic
	//CDC dc;
	//CPrintDialog printDlg(FALSE);

	//if (printDlg.DoModal() == IDCANCEL) return;

	//dc.Attach(printDlg.GetPrinterDC());
	//dc.m_bPrinting = TRUE;

	//CString strTitle;
	//strTitle.LoadStringW(AFX_IDS_APP_TITLE);

	//DOCINFO di;
	//::ZeroMemory(&di, sizeof(DOCINFO));
	//di.cbSize = sizeof(DOCINFO);
	//di.lpszDocName = strTitle;

	//BOOL bPrintingOK = dc.StartDocW(&di);

	//CFont font;
	//font.CreatePointFont(100, _T("Arial"));

	//CFont* pOldFont = dc.SelectObject(&font);

	//dc.StartPage();

	//// Print the contents of the RichEdit control or other results
	//CString strText;
	//m_resultsEdit.GetWindowTextW(strText);

	//dc.TextOutW(100,100, strText);

	//dc.EndPage();
	//dc.EndDoc();

	//dc.SelectObject(pOldFont);
	//dc.DeleteDC();
}


void CResultsDialog::OnFilePrint(void)
{
	// Handle the print command here
	// You should directly call the same code as in OnBnClickedButtonPrint()
	// OnBnClickedButtonPrint();
	PrintRichEditContents();
}





void CResultsDialog::PrintRichEditContents(void)
{
	CDC dc;
	CPrintDialog printDlg(FALSE);

	if (printDlg.DoModal() == IDCANCEL)
		return;

	dc.Attach(printDlg.GetPrinterDC()); // Get and attach a printer DC
	dc.m_bPrinting = TRUE;

	CString strTitle;
	strTitle.LoadStringW(AFX_IDS_APP_TITLE);

	DOCINFO di;
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = strTitle;

	BOOL bPrintingOK = dc.StartDocW(&di);

	CRect rect;
	dc.GetClipBox(&rect); // Get the clipping box

	// Adjust the mapping mode
	dc.SetMapMode(MM_TEXT);

	// Calculate teh margins and the printable area
	int nHorizRes = dc.GetDeviceCaps(HORZRES);
	int nVertRes = dc.GetDeviceCaps(VERTRES);
	int nLogPixelsX = dc.GetDeviceCaps(LOGPIXELSX);
	int nLogPixelsY = dc.GetDeviceCaps(LOGPIXELSY);

	// Margins (in inches)
	double marginLeft = 0.5; // 0.5 inch margin
	double marginRight = 0.5;
	double marginTop = 0.5;
	double marginBottom = 0.5;

	// Convert margins to device units
	int nMarginLeft = static_cast<int>(marginLeft * nLogPixelsX);
	int nMarginRight = static_cast<int>(marginRight * nLogPixelsX);
	int nMarginTop = static_cast<int>(marginTop * nLogPixelsY);
	int nMarginBottom = static_cast<int>(marginBottom * nLogPixelsY);

	// Set the rect to the printable area minus margins
	rect.left = nMarginLeft;
	rect.top = nMarginTop;
	rect.right = nHorizRes - nMarginRight;
	rect.bottom = nVertRes - nMarginBottom;

	FORMATRANGE fr;
	fr.hdc = dc.m_hDC;
	fr.hdcTarget = dc.m_hDC;
	fr.rc = rect;
	fr.rcPage = rect;
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;

	// Start the document
	m_resultsEdit.FormatRange(&fr, TRUE);

	m_resultsEdit.DisplayBand(&rect);

	// End the document
	m_resultsEdit.FormatRange(NULL, FALSE);

	dc.EndDoc();
	dc.Detach();

}


void CResultsDialog::OnFilePrintSetup(void)
{
	CPrintDialog printDlg(TRUE);
	printDlg.DoModal();
}

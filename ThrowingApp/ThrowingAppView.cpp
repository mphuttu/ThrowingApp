
// ThrowingAppView.cpp : implementation of the CThrowingAppView class
//

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ThrowingApp.h"
#endif

#include "ThrowingAppDoc.h"
#include "ThrowingAppView.h"
#include "OptionsDialog.h"
#include "ResultsDialog.h"
#include "AngleEstimateDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThrowingAppView

IMPLEMENT_DYNCREATE(CThrowingAppView, CScrollView)

const double CThrowingAppView::gravity = 9.80665;

BEGIN_MESSAGE_MAP(CThrowingAppView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPTIONS_SETPARAMETERS, &CThrowingAppView::OnOptionsSetparameters)
	ON_COMMAND(ID_OPTIONS_ESTIMATEANGLE, &CThrowingAppView::OnOptionsEstimateangle)
END_MESSAGE_MAP()

// CThrowingAppView construction/destruction

CThrowingAppView::CThrowingAppView()
	: height(1.75)
	, time(0)
	, v0(9.40)
	, zheight(0)
	, flightTime(0)
	, phi0(25)
	, flightDistance(0)
	, scaleX(50.0)
	, scaleY(50.0)
{
	// TODO: add construction code here
	// m_resultsDlg.Create(IDD_RESULTS_DIALOG, this);
	// m_resultsDlg.ShowWindow(SW_SHOW);
	// Set up the RichEdit control
    //ASSERT(m_resultsEdit.GetSafeHwnd());  // Ensure the RichEdit control was created
	ShowResultsDialog();
}

CThrowingAppView::~CThrowingAppView()
{
}

BOOL CThrowingAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CThrowingAppView drawing

void CThrowingAppView::OnDraw(CDC* pDC)
{
	CThrowingAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Save the current state of the device context
	int nDC = pDC->SaveDC();

	// Create font for axis labels
	CFont aFont;

	if ( aFont.CreateFontW( 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, FF_MODERN, 0) ){
		pDC->SelectObject( &aFont);
	} else {
		AfxMessageBox(_T("Unable to create font") );
		return;
	}

	//pDC->SetMapMode(MM_TEXT);
	// TODO: add draw code for native data here
	CRect rectClient;
	GetClientRect(&rectClient);
	
	//Set viewport origin to bottom left corner of the window
	CPoint ptBottomLeft(0, -850);
	pDC->LPtoDP(&ptBottomLeft);
	pDC->SetViewportOrg( ptBottomLeft);
	// Adjust origin to the bottom left of the client area
	int originX = 50; // Padding from the left
	int originY = rectClient.Height() - 50; // Padding from the bottom

	// Base co-ordinates for axes
	const CPoint ORIGIN(100,100);
	originX = ORIGIN.x;
	originY = ORIGIN.y;
	const CPoint Y_EXTENT( ORIGIN.x, ORIGIN.y + 650);
	const CPoint X_EXTENT(ORIGIN.x + 1900, ORIGIN.y);

	// Draw Cartesian coordinate axes
	CPen AxisPen;
	if (AxisPen.CreatePen(PS_SOLID, 1, RGB(0,0,0)) ) { // Black Pen for axes
		pDC->SelectObject( &AxisPen);
	} else {
		AfxMessageBox( _T("Unable to create Axis Pen") );
		return;
	}

	CPen* pOldPen = pDC->SelectObject(&AxisPen);

	// X axis
	/*pDC->MoveTo(originX, originY);
	pDC->LineTo(rectClient.Width() - 10, originY);*/


	// Y axis
	/*pDC->MoveTo(originX, originY);
	pDC->LineTo(originX, 10);*/

	// Draw axes
	pDC->MoveTo ( Y_EXTENT);
	pDC->LineTo( ORIGIN );
	pDC->LineTo( X_EXTENT);
	//// Draw X axis ticks
	//for (int i = 0; i < rectClient.Width() / scaleX; ++i)
	//{
	//	int x = ORIGIN.x + static_cast<int>(i * scaleX);
	//	pDC->MoveTo(x, ORIGIN.y -5);
	//	pDC->LineTo(x, ORIGIN.y + 5);
	//	CString str;
	//	str.Format(_T("%d"), i);
	//	pDC->TextOutW( x- 3, ORIGIN.y - 7, str);
	//}

	// Draw the X axis ticks
	DrawXAxisTicks(pDC, rectClient);

	// Draw Y axis ticks
	for (int i = 0; i < rectClient.Height() / scaleY; ++i)
	{
		int y = ORIGIN.y + static_cast<int>(i * scaleY);
		pDC->MoveTo(ORIGIN.x -5, y);
		pDC->LineTo(ORIGIN.x +5, y);
		CString str;
		str.Format(_T("%d"), i);
		pDC->TextOutW(ORIGIN.x - 20, y +4, str);
	}

	pDC->SelectObject(pOldPen);

	// Draw each throw
	int throwIndex = 0;
	const auto& throws = pDoc->GetThrows();
	for (const auto& t : throws) 
	{
		// Draw the flight path
		CPen GraphPen(PS_SOLID, 1, t.color); // Pen with the chosen color for the throw
		pOldPen = pDC->SelectObject(&GraphPen);

		// Calculate the points for the flight path
		double timeIncrement = 0.1; // Adjust for smoother curve
		double rad_phi0 = t.throwAngle * M_PI / 180.0;
		double flightTime = (t.initialVelocity * sin(rad_phi0) +
                            sqrt(pow(t.initialVelocity * sin(rad_phi0), 2) + 2 * gravity * t.height)) / gravity;
		double currentX = 0.0;
		double currentY = 0.0;

		for (double currentTime = 0.0; currentTime <= flightTime; currentTime += timeIncrement)
		{
			/*double  currentX = v0 * currentTime * cos(rad_phi0);
			double  currentY = height + v0 * currentTime * sin(rad_phi0) - 0.5 * gravity * currentTime * currentTime;*/
			currentX = t.initialVelocity * currentTime * cos(rad_phi0);
			currentY = t.height + t.initialVelocity * currentTime * sin(rad_phi0) - 0.5 * gravity * currentTime * currentTime;

			int drawX = originX + static_cast<int>(currentX * scaleX);
			int drawY = originY + static_cast<int>(currentY * scaleY);

			if (currentTime == 0.0 )
			{
				pDC->MoveTo(drawX, drawY);
			}
			else
			{
				pDC->LineTo(drawX, drawY);
			}
		}

		pDC->SelectObject(pOldPen);
		throwIndex++;		
	} // end of Draw each throw

	
	
	//CString resultText;
	//resultText.Format(_T("Starting height (h): %.2f m\nInitial velocity (v0): %.2f m/s\nInitial throw angle (phi0): %.2f degrees\nThe result (flight distance): %.2f m"), height, v0, phi0, flightDistance);
	//// pDC->TextOutW(originX + static_cast<int>(flightDistance * scaleX) + 20, 50, resultText);
	//CRect resultRect(originX + static_cast<int>(flightDistance * scaleX/2) + 20, 550, rectClient.Width() - 30, -rectClient.Height() +10);
	//pDC->DrawText(resultText, &resultRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

	// Display initial conditions and results
		CFont bFont;
		if ( bFont.CreateFontW( 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, FF_ROMAN , _T("Times New Roman") ) ){
			pDC->SelectObject( &bFont);
		} else {
			AfxMessageBox(_T("Unable to create font") );
			return;
		}
	int infoX = static_cast<int>(0.2 * rectClient.Width()); // Right side padding
	int infoY =  800; // Top padding
	int lineHeight = 30; // Line height for each throw's info

	// CString allResults;
	// m_resultsDlg.m_resultsEdit.SetWindowTextW(_T("")); // Clear the previous results
	// Update the results dialog with the accumulated text
	ShowResultsDialog();

	m_resultsDlg.ClearResults();
	if ( m_resultsDlg.m_hWnd != NULL ) // Check if the dialog window is already created
	{
		m_resultsDlg.UpdateResults(pDoc->GetThrows());
	}
	//for (const auto& t : pDoc->GetThrows() )
	//{
	//	
	//	// Draw the color indicator
	//	/*
	//	CPen ColorPen(PS_SOLID, 2, t.color);
	//	pOldPen = pDC->SelectObject(&ColorPen);
	//	
	//	pDC->MoveTo(infoX - 20, infoY + 5);
	//	pDC->LineTo(infoX - 5, infoY + 5);
	//	pDC->SelectObject(pOldPen);*/

	//	// Display throw information
	//	// Build the result text
	//	CString resultText;
	//	// double rad_phi0 = t.throwAngle * M_PI / 180.0;
	//	// double flightTime = (1 / gravity) * (sqrt(pow(t.initialVelocity * sin(rad_phi0), 2) + 2 * gravity * t.height) - t.initialVelocity * sin(rad_phi0));
	//	//double flightDistance = t.initialVelocity * flightTime * cos(rad_phi0);
	//	// Calculate();
	//	//double rad_phi0 = t.throwAngle * M_PI / 180.0;
	//	//double sin_phi0 = sin(t.throwAngle * M_PI / 180.0);
	//	//// zheight = height + v0 * time * sin_phi0 - 0.5 * gravity * time * time;
	//	//double flightTime = (t.initialVelocity * sin(rad_phi0) + 
	//	//	sqrt(pow(t.initialVelocity * sin(rad_phi0),2)  + 2 * gravity * t.height)) / gravity;
	//	//// flightTime = ( 2 * v0 * sin_phi0) / gravity;
	//	//double flightDistance = t.initialVelocity * flightTime * cos(rad_phi0);
	//	//resultText.Format(_T("Launch height: %.2f m; Initial velocity: %.2f m/s;  Throw Angle: %.2f°; Length of the Throw: %.2f m\n"), t.height ,t.initialVelocity, t.throwAngle, flightDistance);
	//	//CRect resultRect(infoX, infoY, rectClient.Width() -10 , infoY - lineHeight ); // Adjust rect to fit text
	//	// pDC->DrawText(resultText, &resultRect, DT_LEFT | DT_TOP | DT_WORDBREAK);
	//	// infoY -= lineHeight ; // Move down for next throw
	//	
	//	// Append the result text with the color to the dialog
	//	/*m_resultsDlg.AppendTextWithColor(resultText, t.color);
	//	allResults += resultText;	*/	
	//}
	
	// m_resultsDlg.UpdateResults(allResults);
	// m_resultsDlg.SetWindowPos(NULL, rectClient.right -300, rectClient.top + 50, 400, rectClient.Height() -100, SWP_NOZORDER);
	// Restore the original device context
	pDC->RestoreDC( nDC );
}


// CThrowingAppView printing


void CThrowingAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CThrowingAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	CScrollView::OnPreparePrinting(pInfo);
	// pInfo->SetMaxPage(1); // Or the number you need
	CThrowingAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// int nPages = pDoc->CalcNumPages();
	return DoPreparePrinting(pInfo);
	
}

void CThrowingAppView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	// Create a font for the header
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));		// Clear out structure
	lf.lfHeight = 16;						// Request a 16-pixel-high font
	lf.lfWeight = FW_BOLD;					// Bold font
	_tcscpy_s(lf.lfFaceName, _T("Arial"));	// Reques Arial typeface

	m_headerFont.CreateFontIndirectW(&lf);

	/*
	// Retrieve the printable area from the print info
    CRect rectPrintArea = pInfo->m_rectDraw;

    // Calculate the content size
    double contentWidth = flightDistance * scaleX; // X * scaleX;
    double contentHeight = height * scaleY; // h * scaleY;

    // Determine scaling factors to fit content in printable area
    double scaleX = static_cast<double>(rectPrintArea.Width()) / contentWidth;
    double scaleY = static_cast<double>(rectPrintArea.Height()) / contentHeight;
    double scale = min(scaleX, scaleY); // Use smaller scale factor

    // Set the mapping mode to isotropic
    pDC->SetMapMode(MM_ISOTROPIC);

    // Set window extent to the scaled content size
    pDC->SetWindowExt(static_cast<int>(contentWidth), static_cast<int>(contentHeight));

    // Set viewport extent to fit the printable area scaled accordingly
    pDC->SetViewportExt(static_cast<int>(rectPrintArea.Width() / scale), 
                        static_cast<int>(rectPrintArea.Height() / scale));

    // Set viewport origin to align with the printable area
    pDC->SetViewportOrg(rectPrintArea.left, rectPrintArea.top);
	*/
}

void CThrowingAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing

	// Clean up the font
	if (m_headerFont.GetSafeHandle())
	{
		m_headerFont.DeleteObject();
	}
}

void CThrowingAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CThrowingAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CThrowingAppView diagnostics

#ifdef _DEBUG
void CThrowingAppView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CThrowingAppView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CThrowingAppDoc* CThrowingAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CThrowingAppDoc)));
	return (CThrowingAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CThrowingAppView message handlers


void CThrowingAppView::Calculate(void)
{
	double rad_phi0 = phi0 * M_PI / 180.0;
	double sin_phi0 = sin(phi0 * M_PI / 180.0);
	zheight = height + v0 * time * sin_phi0 - 0.5 * gravity * time * time;
	// Climb height is 
	flightTime = (v0 * sin(rad_phi0) + sqrt(pow(v0 * sin(rad_phi0),2) + 2 * gravity * height)) / gravity;
	// flightTime = ( 2 * v0 * sin_phi0) / gravity;
	flightDistance = v0 * flightTime * cos(rad_phi0);
	Invalidate();
}


void CThrowingAppView::OnOptionsSetparameters()
{
	// TODO: Add your command handler code here
	COptionsDialog dlg;
	dlg.m_h = height;
	dlg.m_phi0 = phi0;
	dlg.m_v0 = v0;
	dlg.m_scaleX = scaleX;
	dlg.m_scaleY = scaleY;

	if (dlg.DoModal() == IDOK)
	{
		height = dlg.m_h;
		phi0 = dlg.m_phi0;
		v0 = dlg.m_v0;
		scaleX = dlg.m_scaleX;
		scaleY = dlg.m_scaleY;
		Calculate();
		GetDocument()->AddThrow(dlg.m_h, dlg.m_v0, dlg.m_phi0, dlg.m_color);
		UpdateScrollSize();
	}
}


void CThrowingAppView::OnInitialUpdate(void)
{
	CScrollView::OnInitialUpdate();

	// Set the initial scroll sizes
	UpdateScrollSize();
	/*CThrowingAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;*/

	//CSize docSize = pDoc->GetDocSize();
	// CSize sizeTotal;
	// TODO: calculate the total size of this view
	// sizeTotal.cx = 1900;
	// sizeTotal.cy = 850;
	// SetScrollSizes(MM_LOENGLISH, sizeTotal);
	//SetScrollSizes(MM_LOENGLISH, docSize );
}


void CThrowingAppView::ShowResultsDialog(void)
{
	if ( !AfxInitRichEdit())
	{
		AfxMessageBox(_T("Failed to initialize RichEdit 1.0"));
		return;
	}

	if ( !m_resultsDlg.m_hWnd) // Check if the dialog is not created
	{
		m_resultsDlg.Create(IDD_RESULTS_DIALOG, this); // Create the dialog
	}
	/*CRect rectClient;
	GetClientRect(&rectClient);*/
	m_resultsDlg.ShowWindow(SW_SHOW);  // Show the dialog (SW_SHOW)	
	::SetForegroundWindow(m_resultsDlg.GetSafeHwnd());
	// m_resultsDlg.SetWindowPos(NULL, rectClient.right - 300, rectClient.top + 50, 300, rectClient.Height() - 100, SWP_NOZORDER);
    // Update the results immediately when the dialog is shown
    // m_resultsDlg.UpdateResults(GetDocument()->GetThrows());
}


void CThrowingAppView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// Call the base class's OnUpdate to ensure default behavior
	CScrollView::OnUpdate(pSender, lHint, pHint);

	// Get the document associated with this view
	CThrowingAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get the document size dynamically and update the scroll sizes
	CSize docSize = pDoc->GetDocSize();
	SetScrollSizes(MM_LOENGLISH, docSize);

	// Invalidate the view to trigger a redraw
	Invalidate();
}


void CThrowingAppView::UpdateScrollSize(void)
{
	CThrowingAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Calculate the maximum flight distance
	double maxFlightDistance = 0.0;

	for (const auto& t : pDoc->GetThrows() )
	{
		if (t.flightDistance > maxFlightDistance )
		{
			maxFlightDistance = t.flightDistance;
		}
	}

	// Convert flight distance to logical units (MM_LOENGLISH, where 1 unit = 0.01 inch)
	int width = static_cast<int>(maxFlightDistance * 100.0); // Convert meters to logical units
	int height = 850; // Fixed height or based on your requirements

	// Set the scroll sizes based on the max flight distance
	CSize sizeTotal(width, height);
	SetScrollSizes(MM_LOENGLISH, sizeTotal);
}


void CThrowingAppView::DrawXAxisTicks(CDC* pDC, const CRect& rectClient)
{
	CThrowingAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Adjust origin to the bottom left of the client area
	// Base co-ordinates for axes
	const CPoint ORIGIN(100,100);

	// Calculate the maximum flight distance
	double maxFlightDistance = 0.0;
	for (const auto& t : pDoc->GetThrows() )
	{
		if (t.flightDistance > maxFlightDistance )
		{
			maxFlightDistance = t.flightDistance;
		}
	}

	double xAxisLength = max(maxFlightDistance, ORIGIN.x + 1900);

	// Determine the number of ticks needed based on the max flight distance
	int numTicks = static_cast<int> (xAxisLength / scaleX ) + 1;

	// Draw X axis ticks
	for (int i = 0; i <= numTicks; ++i)
	{
		int x = ORIGIN.x + static_cast<int>(i * scaleX);
		pDC->MoveTo(x, ORIGIN.y - 5);
		pDC->LineTo(x, ORIGIN.y +5 );

		// Format and draw the tick label
		CString str;
		str.Format(_T("%d"), i);
		pDC->TextOutW( x - 3, ORIGIN.y - 7, str); // Adjusted y-position to avoid overlap with ticks
	}
}


void CThrowingAppView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	/*
	// Calculate the header position (adjust as needed)
	int headerHeight = 50; // Example height
	CRect headerRect(pInfo->m_rectDraw);
	headerRect.bottom = headerRect.top + headerHeight;

	// Draw your header content (e.g., title and page number)
	CString headerText = _T("Graphical Throw Results");
	pDC->DrawText(headerText, headerRect, DT_CENTER | DT_VCENTER);

	// Adjust the drawing rectangle for the main content
	pInfo->m_rectDraw.top += headerHeight;
	*/

	/*
	// Call the base class implementation
	CScrollView::OnPrint(pDC, pInfo);

	// Print the header on each page
	CFont* pOldFont = pDC->SelectObject(&m_headerFont);

	CString headerText = _T("Graphical Throw Results");

	// Calculate the header position (top center)
	CRect rcHeader(pInfo->m_rectDraw);
	rcHeader.bottom = rcHeader.top + 100; // 100 units for the header height

	// Draw the header text
	pDC->DrawText(headerText, &rcHeader, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// Print the page number
	CString pageNumber;
	pageNumber.Format(_T("Page %d"), pInfo->m_nCurPage);

	// Position the page number at the bottom center of the header area
	CRect rcPageNumber(pInfo->m_rectDraw);
	rcPageNumber.top = rcHeader.bottom + 10;
	rcPageNumber.bottom = rcPageNumber.top + 50;

	pDC->DrawText(pageNumber, &rcPageNumber, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// Restore the old font
	pDC->SelectObject(pOldFont);

	// Call the base class implementation
	CScrollView::OnPrint(pDC, pInfo);
	*/
	CThrowingAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Print the header on each page
	CFont* pOldFont = pDC->SelectObject(&m_headerFont);

	CString headerText = _T("Graphical Throw Results");

	// Print the page header and adjust the DC window origin
	CString sDocTitle = headerText + _T(" -- ") + pDoc->GetTitle();
	PrintPageHeader(pDC, pInfo, sDocTitle);

	// Restore the old font
	pDC->SelectObject(pOldFont);
	// Print the document data
	OnDraw(pDC);
}


void CThrowingAppView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// Call the base class implementation
	CScrollView::OnPrepareDC(pDC, pInfo);

	// Adjust the scale factor for print preview
	/*
	if (pInfo->m_bPreview) {
		// Example: Shrink the page by 80% for preview
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(100,100); // Adjust as needed
		pDC->SetViewportExt(pInfo->m_rectDraw.Width() * 80/100, 
			pInfo->m_rectDraw.Height() * 80 /100);
	}
	*/
}


void CThrowingAppView::PrintPageHeader(CDC* pDC, CPrintInfo* pInfo,  CString& sHeader)
{
	// Draw the header text aligned left
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(0, -25, sHeader);

	// Draw a line across the page just below the header text
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int y = -35 - tm.tmHeight;
	pDC->MoveTo(0,y);
	pDC->LineTo(pInfo->m_rectDraw.right, y);

	// Adjust the drawing rect to not include the header
	y -= 25;
	pInfo->m_rectDraw.top += y;
}


void CThrowingAppView::OnOptionsEstimateangle()
{
	// TODO: Add your command handler code here
	CAngleEstimateDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		// AfxMessageBox(_T("Dialog was opened!"));
	}
}

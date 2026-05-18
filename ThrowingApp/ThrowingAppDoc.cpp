
// ThrowingAppDoc.cpp : implementation of the CThrowingAppDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ThrowingApp.h"
#endif

#include "ThrowingAppDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CThrowingAppDoc

IMPLEMENT_DYNCREATE(CThrowingAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CThrowingAppDoc, CDocument)
END_MESSAGE_MAP()


// CThrowingAppDoc construction/destruction

CThrowingAppDoc::CThrowingAppDoc()
{
	// TODO: add one-time construction code here
	// Add initial throw with default parameters
	// m_throws.push_back( {1.75, 20.0, 45.0, RGB(255, 0, 0) } ); // Does not work with Visual Studio 2012
	/*ThrowParams initialThrow;
	initialThrow.height = 1.75;
	initialThrow.initialVelocity = 20.0;
	initialThrow.throwAngle = 45.0;
	initialThrow.color = RGB(255, 0, 0);*/
	m_throws.push_back(ThrowParams(1.75,20.0,45.0,RGB(255,0,0)));
}

CThrowingAppDoc::~CThrowingAppDoc()
{
}

BOOL CThrowingAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	// Clear existing throws and add default throw
	m_throws.clear();
	// m_throws.push_back({1.75, 20.0, 45.0, RGB(255, 0, 0) }); // Does not work in the VS 2012
	/*ThrowParams initialThrow;
	initialThrow.height = 1.75;
	initialThrow.initialVelocity = 20.0;
	initialThrow.throwAngle = 45.0;
	initialThrow.color = RGB(255, 0, 0);
	m_throws.push_back(initialThrow);*/

	return TRUE;
}




// CThrowingAppDoc serialization

void CThrowingAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here

		// Store the number of throws
		int numThrows = static_cast<int>(m_throws.size());
		ar << numThrows;

		// int size = static_cast<UINT> (m_throws.size() );
		// ar << size;
		// Store each throw's parameters including the new flightDistance
		for ( const auto& t: m_throws)
		{
			// ar << t.height << t.initialVelocity << t.throwAngle << t.color << t.flightDistance;
			// ar << t;
			ar << t.height;
			ar << t.initialVelocity;
			ar << t.throwAngle;
			ar << t.color;
			ar << t.flightDistance; // New line to store the flightDistance
		}
	}
	else
	{
		// TODO: add loading code here
		// Load the number of throws
		int numThrows;
		ar >> numThrows;

		m_throws.clear(); // Clear existing data before loading new data

		//UINT size;
		//ar >> size;
		//m_throws.resize(size);
		//for (auto& t : m_throws )
		//{
		//	//ar >> t.height >> t.initialVelocity >> t.throwAngle >> t.color >> t.flightDistance;
		//	ar >> t;
		//}
		for ( int i = 0; i < numThrows; ++i)
		{
			ThrowParams t;
			ar >> t.height;
			ar >> t.initialVelocity;
			ar >> t.throwAngle;
			ar >> t.color;
			ar >> t.flightDistance;  // New line to load flightDistance

			m_throws.push_back(t); // Add the throw to the collection.
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CThrowingAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CThrowingAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CThrowingAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CThrowingAppDoc diagnostics

#ifdef _DEBUG
void CThrowingAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CThrowingAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CThrowingAppDoc commands


void CThrowingAppDoc::AddThrow(double height, double initialVelocity, double throwAngle, COLORREF color)
{
	/*ThrowParams newThrow;
	newThrow.height = height;
	newThrow.initialVelocity = initialVelocity;
	newThrow.throwAngle = throwAngle;
	newThrow.color = color;*/
	m_throws.push_back(ThrowParams(height,initialVelocity, throwAngle, color));
	// m_throws.push_back( {height, initialVelocity, throwAngle, color } ); // Does not work in VS2012
	SetModifiedFlag(TRUE);	
	UpdateAllViews(NULL);
}


const std::vector<ThrowParams>& CThrowingAppDoc::GetThrows(void) const
{
	//TODO: insert return statement here
	return m_throws;
}


CSize CThrowingAppDoc::GetDocSize(void) const
{
	// Calculate the size of the document based on its contents
	// This example assumes you have a vector of throws that you need to 
	// account for in the size.
	int width = 1900;  // Default or minimum width
	int height = 850;  // Default or minimum height

	for (const auto& ThrowParams : m_throws)
	{
		// Adjust width and height based on the properties of each throw
		// This could depend on the range of the throws, how much space each throw occupies, etc.
		// For simplicity, let's assume we increase width based on the number of throws:
		width += 1;  // Example: add 10 units for each throw
	}
	return CSize(width, height);
}

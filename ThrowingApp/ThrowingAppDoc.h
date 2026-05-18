
// ThrowingAppDoc.h : interface of the CThrowingAppDoc class
//
#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>

struct ThrowParams {
	double height;
	double initialVelocity;
	double throwAngle;
	COLORREF color;
	double flightDistance;

	// Default constructor
    ThrowParams() 
        : height(0.0), initialVelocity(0.0), throwAngle(0.0), color(RGB(0, 0, 0)), flightDistance(0.0)
    {
    }

	ThrowParams(double h, double v0, double angle, COLORREF c)
		: height(h), initialVelocity(v0), throwAngle(angle), color(c)
	{
		// Calculate flight distance
		const double gravity = 9.80665; // Gravitational constant, or Gravity of Earth
		const double dPii = 3.14159265358979323846;
        double rad_phi0 = throwAngle * dPii / 180.0;
        double sin_phi0 = sin(throwAngle * dPii / 180.0);
        double flightTime = (initialVelocity * sin(rad_phi0) + 
                             sqrt(pow(initialVelocity * sin(rad_phi0), 2) + 
                             2 * gravity * height)) / gravity;
        flightDistance = initialVelocity * flightTime * cos(rad_phi0);
	}

	 friend CArchive& operator<<(CArchive& ar, const ThrowParams& t)
     {
            ar << t.height << t.initialVelocity << t.throwAngle << t.color;
            return ar;
     }

     friend CArchive& operator>>(CArchive& ar, ThrowParams& t)
     {
            ar >> t.height >> t.initialVelocity >> t.throwAngle >> t.color;
            return ar;
     }
};


class CThrowingAppDoc : public CDocument
{
protected: // create from serialization only
	CThrowingAppDoc();
	DECLARE_DYNCREATE(CThrowingAppDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CThrowingAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	std::vector<ThrowParams> m_throws;
public:
	void AddThrow(double height, double initialVelocity, double throwAngle, COLORREF color);
	const std::vector<ThrowParams>& GetThrows(void) const;
	CSize GetDocSize(void) const;
};

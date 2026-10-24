
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

	static double ComputeFlightDistance(double heightValue, double velocityValue, double angleDegrees)
	{
		if (!std::isfinite(heightValue) || !std::isfinite(velocityValue) || !std::isfinite(angleDegrees))
		{
			return 0.0;
		}

		if (heightValue < 0.0 || velocityValue <= 0.0 || angleDegrees <= 0.0 || angleDegrees >= 90.0)
		{
			return 0.0;
		}

		const double gravity = 9.80665;
		const double pi = 3.14159265358979323846;
		const double radPhi0 = angleDegrees * pi / 180.0;
		const double flightTime = (velocityValue * sin(radPhi0) +
			sqrt(pow(velocityValue * sin(radPhi0), 2) + 2.0 * gravity * heightValue)) / gravity;
		return velocityValue * flightTime * cos(radPhi0);
	}

	// Default constructor
    ThrowParams() 
        : height(0.0), initialVelocity(0.0), throwAngle(0.0), color(RGB(0, 0, 0)), flightDistance(0.0)
    {
    }

	ThrowParams(double h, double v0, double angle, COLORREF c)
		: height(h), initialVelocity(v0), throwAngle(angle), color(c), flightDistance(ComputeFlightDistance(h, v0, angle))
	{
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

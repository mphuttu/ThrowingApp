// AngleEstimateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThrowingApp.h"
#include "AngleEstimateDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>


// CAngleEstimateDlg dialog

IMPLEMENT_DYNAMIC(CAngleEstimateDlg, CDialogEx)

CAngleEstimateDlg::CAngleEstimateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAngleEstimateDlg::IDD, pParent)	
	, m_dFlightDistance(23.56)
	, m_dInitialVelocity(0)
	, m_dThrowAngleEstimated(0)
	, m_dThrowAngle(0)
	, m_dInitialVelocityEstimated(0)
	, dPii(3.1415926535897932384626433832795)
	, m_szFlightDistance(_T("23.56"))
	, m_szInitialVelocity(_T(""))
	, m_szEstimatedThrowAngle(_T(""))
	, m_szThrowAngle(_T(""))
	, m_szEstimatedInitialVelocity(_T(""))
	, m_dLaunchHeight(0)
	, m_szLaunchHeight(_T(""))
	, dGravity(9.80665)
{

}

CAngleEstimateDlg::~CAngleEstimateDlg()
{
}

void CAngleEstimateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FLIGHT_DISTANCE_EDIT, m_edtFlightDistance);
	DDX_Control(pDX, IDC_GIVEN_INITIAL_VELOCITY_EDIT, m_edtInitialVelocity);
	DDX_Control(pDX, IDC_ESTIMATED_THROW_ANGLE_EDIT, m_edtEstimatedThrowAngle);
	DDX_Control(pDX, IDC_GIVEN_THROW_ANGLE_EDIT, m_edtThrowAngle);
	DDX_Control(pDX, IDC_ESTIMATED_INITIAL_VELOCITY_EDIT, m_edtEstimatedInitialVelocity);
	DDX_Control(pDX, IDC_LAUNCH_HEIGHT_EDIT, m_edtLaunchHeight);
}


BEGIN_MESSAGE_MAP(CAngleEstimateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ESTIMATE_THROW_ANGLE_BUTTON, &CAngleEstimateDlg::OnBnClickedEstimateThrowAngleButton)
	ON_BN_CLICKED(IDC_ESTIMATE_INITIAL_VELOCITY_BUTTON, &CAngleEstimateDlg::OnBnClickedEstimateInitialVelocityButton)
END_MESSAGE_MAP()





void CAngleEstimateDlg::OnBnClickedEstimateThrowAngleButton()
{
	// TODO: Add your control notification handler code here
	// Function: OnBnClickedEstimateThrowAngleButton
	// Description: Calculates from the flight distance, height and
	// the given initial velocity the estimated throw angle.

	// Given Flight Distance
	//m_edtFlightDistance.GetWindowTextW(m_szFlightDistance);    // Copy edit value to CString
	//std::string strFlightDistance =  CT2A(m_szFlightDistance); // Converse CString to string
	//m_dFlightDistance = std::stod(strFlightDistance);          // Converse string to double
	m_dFlightDistance = GetPositiveDoubleFromEdit(m_edtFlightDistance);
	if ( m_dFlightDistance <= 0.0 ) {
		AfxMessageBox(_T("Give a positive real number!"));
		return;
	}

	// Given Launch Height
	//m_edtLaunchHeight.GetWindowTextW(m_szLaunchHeight);        // Copy edit value to CString
	//std::string strLaunchHeight = CT2A(m_szLaunchHeight);      // Converse CString to string
	//m_dLaunchHeight = std::stod(strLaunchHeight);              // Converse string to double
	m_dLaunchHeight = GetPositiveDoubleFromEdit(m_edtLaunchHeight);
	if ( m_dLaunchHeight <= 0.0 ) {
		AfxMessageBox(_T("Give a positive real number!"));
		return;
	}

	// Given Initial Velocity
	//m_edtInitialVelocity.GetWindowTextW(m_szInitialVelocity);    // Copy edit value to CString
	//std::string strInitialVelocity = CT2A(m_szInitialVelocity);  // Converse CString to string
	//m_dInitialVelocity = std::stod(strInitialVelocity);          // Converse string to double
	m_dInitialVelocity = GetPositiveDoubleFromEdit(m_edtInitialVelocity);
	if ( m_dInitialVelocity <= 0.0 ) {
		AfxMessageBox(_T("Give a positive real number!"));
		return;
	}

	// Set some shorter variable names
	double a = m_dFlightDistance;
	double b = dGravity;
	double c = m_dInitialVelocity;
	double d = m_dLaunchHeight;

	// Mathematica Expression
	/*ArcCos[(
    Sqrt[(a^2 b^2 b*d c^2)/(a^2 b^2 c^4 + b*d^2 c^4) + (a^2 b^2 c^4)/(
     a^2 b^2 c^4 + b*d^2 c^4) - 
     Sqrt[-a^4 b^4 c^4 (a^2 b^2 - 2 b*d c^2 - c^4)]/(
     a^2 b^2 c^4 + b*d^2 c^4)]/Sqrt[2])*/

	// Calculate intermediate expressions
    // double numerator = (a * a * b * b * b * d * c * c) / (a * a * b * b * c * c * c * c + b * d * d * c * c * c * c);
    // double denominator = (a * a * b * b * c * c * c * c + b * d * d * c * c * c * c);
    // double sqrtTerm = -a * a * b * b * c * c * c * c * (a * a * b * b - 2 * b * d * c * c - c * c) / (a * a * b * b * c * c * c * c + b * d * d * c * c * c * c);

	double dCoeff = a/c;
	double dNomOne = std::pow(c,2) + b * d;
	double dNomTwo = std::sqrt(std::pow(c,4) + 2*b*std::pow(c,2)*d -std::pow(a,2)*std::pow(b,2));
	double dDenom = 2*(std::pow(a,2) + std::pow(d,2));
	double dSqrtTerm = (dNomOne + dNomTwo)/dDenom;
	double dAllTogether = dCoeff * std::sqrt(dSqrtTerm);
	
	// Compute the final result
	double dThrowAngleEstimatedRadians = std::acos(dAllTogether);
	m_dThrowAngleEstimated = dThrowAngleEstimatedRadians * (180.0 / dPii);
	CString szThrowAngleEstimated;
	szThrowAngleEstimated.Format(_T("%.2f"), m_dThrowAngleEstimated); // Format with 2 decimal places (adjust as needed)

	// Set the result to the Edit Control
	m_edtEstimatedThrowAngle.SetWindowTextW(szThrowAngleEstimated);

}


void CAngleEstimateDlg::OnBnClickedEstimateInitialVelocityButton()
{
	// TODO: Add your control notification handler code here
	// Function: OnBnClickedEstimateThrowAngleButton
	// Description: Calculates from the flight distance, height and
	// the given initial velocity the estimated throw angle.

	// Given Flight Distance
	//m_edtFlightDistance.GetWindowTextW(m_szFlightDistance);    // Copy edit value to CString
	//std::string strFlightDistance =  CT2A(m_szFlightDistance); // Converse CString to string
	//m_dFlightDistance = std::stod(strFlightDistance);          // Converse string to double
	m_dFlightDistance = GetPositiveDoubleFromEdit(m_edtFlightDistance);
	if ( m_dFlightDistance <= 0.0 ) {
		AfxMessageBox(_T("Give a positive real number!"));
		return;
	}

	// Given Launch Height
	//m_edtLaunchHeight.GetWindowTextW(m_szLaunchHeight);        // Copy edit value to CString
	//std::string strLaunchHeight = CT2A(m_szLaunchHeight);      // Converse CString to string
	//m_dLaunchHeight = std::stod(strLaunchHeight);              // Converse string to double
	m_dLaunchHeight = GetPositiveDoubleFromEdit(m_edtLaunchHeight);
	if ( m_dLaunchHeight <= 0.0 ) {
		AfxMessageBox(_T("Give a positive real number!"));
		return;
	}

	// Given Throw Angle
	//m_edtThrowAngle.GetWindowTextW(m_szThrowAngle);    // Copy edit value to CString
	//std::string strThrowAngle = CT2A(m_szThrowAngle);  // Converse CString to string
	//m_dThrowAngle = std::stod(strThrowAngle);          // Converse string to double
	m_dThrowAngle = GetPositiveDoubleFromEdit(m_edtThrowAngle);
	if ( m_dThrowAngle <= 0.0 ) {
		AfxMessageBox(_T("Give a positive real number!"));
		return;
	}

	// Set some shorter variable names
	double a = m_dFlightDistance;
	double b = dGravity; // Gravitational acceleration on Earth, approximation ~9.80665 m/s^2
	double cDeg = m_dThrowAngle;
	double d = m_dLaunchHeight;

	// Convert the Throw Angle from degrees to radians
	double c = (dPii/180) * cDeg;

	// Mathematica Expression
	// v0 -> (a b Sqrt[Sec[c]])/Sqrt[2 bd Cos[c] + 2 a b Sin[c]]

	// Calculate in parts
	double dNomOne = a * b;
	double dNomTwo = sec(c);
	double dNom = dNomOne * std::sqrt(dNomTwo);
	double dDenomOne = b * d * std::cos(c);
	double dDenomTwo = a * b * std::sin(c);
	double dDenomTerm = 2 * (dDenomOne + dDenomTwo);
	double dDenom = std::sqrt(dDenomTerm);

	// Initial velocity
	m_dInitialVelocityEstimated = dNom / dDenom;
	CString szInitialVelocityEstimated;
	szInitialVelocityEstimated.Format(_T("%.2f"), m_dInitialVelocityEstimated);

	// Set the text into the CEDIT control
	m_edtEstimatedInitialVelocity.SetWindowTextW(szInitialVelocityEstimated);

}


double CAngleEstimateDlg::sec(double dX)
{
	if (std::cos(dX) == 0.0 ) {
		throw std::runtime_error("Division by zero in a secant function!");
	}
	return 1.0 / std::cos(dX);
}


double CAngleEstimateDlg::GetPositiveDoubleFromEdit(CEdit& editControl)
{
	CString strValue;	
	editControl.GetWindowTextW(strValue);
	std::string stringValue = CT2A(strValue);

	if ( strValue.IsEmpty() ) {
		// Handle empty input (e.g., show an error message)
		AfxMessageBox(_T("Input cannot be empty!"), MB_ICONERROR);
	return 0.0; // Default value (you can adjust as needed)
	}

	try {		
		double value = std::stod(stringValue);
		if (value <= 0.0) {
			throw std::runtime_error("Input must be a positive number.");
		}
		return value;
	} catch (const std::exception& e) {
		// Handle the exception (e.g., display an error message)
		AfxMessageBox(_T("Error: ") + CString(e.what()), MB_ICONERROR);
		return 0.0; // Default value (you can adjust as needed)
	}
}

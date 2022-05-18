// Copyright (C) 2012-Present Andrew S. Bantly
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#pragma once
#include <map>
#include "ImageButton.h"
#include "EditColors.h"
#include "Resource.h"
#include "afxwin.h"

// CNewFractal dialog

// Storeds fractal coordinates
class CFractalCoordinate
{
public:
	CFractalCoordinate() : m_csName(_T("Default")),m_iWidth(640), m_iHeight(480), m_iIterations(256),m_nMaxCalc(256),m_dXMin(-2.0),m_dXMax(2.0),m_dYMin(-2.0),m_dYMax(2.0),m_dP(0.0),m_dQ(0.0) {};
	CFractalCoordinate(CString csName,int iWidth,int iHeight,int iIterations,int nMaxCalc,double dXMin,double dXMax,double dYMin,double dYMax,double dP,double dQ) : m_csName(csName),m_iWidth(iWidth),m_iHeight(iHeight),m_iIterations(iIterations),m_nMaxCalc(nMaxCalc),m_dXMin(dXMin),m_dXMax(dXMax),m_dYMin(dYMin),m_dYMax(dYMax),m_dP(dP),m_dQ(dQ) {};
	CFractalCoordinate(const CFractalCoordinate & FractalCoordinate) {*this = FractalCoordinate;}
	~CFractalCoordinate() {};
	CFractalCoordinate & operator = (const CFractalCoordinate & FractalCoordinate)
	{
		if (this != &FractalCoordinate)
		{
			m_csName = FractalCoordinate.m_csName;
			m_iWidth = FractalCoordinate.m_iWidth;
			m_iHeight = FractalCoordinate.m_iHeight;
			m_iIterations = FractalCoordinate.m_iIterations;
			m_nMaxCalc = FractalCoordinate.m_nMaxCalc;
			m_dXMin = FractalCoordinate.m_dXMin;
			m_dXMax = FractalCoordinate.m_dXMax;
			m_dYMin = FractalCoordinate.m_dYMin;
			m_dYMax = FractalCoordinate.m_dYMax;
			m_dP = FractalCoordinate.m_dP;
			m_dQ = FractalCoordinate.m_dQ;
		}
		return *this;
	}

	CString GetName() {return m_csName;}
	int GetWidth() {return m_iWidth;}
	int GetHeight() {return m_iHeight;}
	int GetIterations() {return m_iIterations;}
	int GetMaxCalc() {return m_nMaxCalc;}
	double GetXMin() {return m_dXMin;}
	double GetXMax() {return m_dXMax;}
	double GetYMin() {return m_dYMin;}
	double GetYMax() {return m_dYMax;}
	double GetP() {return m_dP;}
	double GetQ() {return m_dQ;}

protected:
	CString m_csName;
	int m_iWidth;
	int m_iHeight;
	int m_iIterations;
	int m_nMaxCalc;
	double m_dXMin;
	double m_dXMax;
	double m_dYMin;
	double m_dYMax;
	double m_dP;
	double m_dQ;
};

class CNewFractal : public CDialog
{
	DECLARE_DYNAMIC(CNewFractal)

public:
	CNewFractal(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewFractal();
	void UpdateColor(std::vector<std::vector<BYTE> > vecRGB);
	void InitColors() {SetColors();}

// Dialog Data
	enum { IDD = IDD_NEW_FRACTAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	std::vector<CFractalCoordinate> m_vCoordinates,m_vDragonCoordinates,m_vPhoenixCoordinates,m_vPoints,m_vDragonPoints,m_vPhoenixPoints,m_vPhoenix2Coordinates;
	int m_iFractalType;
	UINT m_iMode,m_iIterations,m_nMaxCalc;
	BOOL m_bRed,m_bGreen,m_bBlue,m_bDropUnused,m_bSmooth,m_bSortOrder,m_bGradient,m_bBaseColor,m_bOrbitColor,m_bTrapX,m_bTrapY;
	COLORREF m_RGB1,m_RGB2,m_RGB3,m_RGB4,m_RGB5,m_RGB6;
	int m_RR,m_RG,m_RB;
	UINT m_iWidth,m_iHeight;
	double m_dXMin,m_dXMax,m_dYMin,m_dYMax,m_dP,m_dQ,m_dStep;
	double m_dDDVXMin,m_dDDVXMax,m_dDDVYMin,m_dDDVYMax;
	double m_dXScale,m_dYScale,m_dBailRadius,m_dTrapX,m_dTrapY;
	CEdit m_XMin,m_YMin,m_XMax,m_YMax,m_P,m_Q,m_Step;
	CStatic m_Static_XMin,m_Static_XMax,m_Static_YMin,m_Static_YMax,m_Static_P,m_Static_Q,m_Static_Step;
	BOOL m_bBase;
	COLORREF m_RGBBase;
	CComboBox m_Range;
	int m_iCurSel;
	CImageButton m_BtnDonate,m_BtnChooseImage,m_BtnChooseImage2,m_BtnBase,m_BtnRnd,m_BtnRGB1,m_BtnRGB2,m_BtnRGB3,m_BtnRGB4,m_BtnRGB5,m_BtnRGB6,m_BtnStepColor,m_BtnEditColors;
	CButton m_BtnLT,m_BtnGT;
	CGradient m_Gradient;
	CString m_csImage,m_csImage2;
	BOOL m_bUseImage,m_bUseImage2,m_bBlendBase,m_bBlendFractal;
	CButton m_BtnBlendBase,m_BtnBlendFractal;
	CEdit m_Alpha,m_Alpha2;
	int m_iAlpha,m_iAlpha2;
	CStatic m_Static_Alpha,m_Static_Alpha2;
	COLORREF m_RGBStepColor;
	CStatic m_StaticBase,m_StaticFractal;
	CButton m_RadioBase,m_RadioBaseAD,m_RadioFractal,m_RadioFractalAD;
	CEditColors m_EditColors;
	CEdit m_TrapX,m_TrapY;
	BOOL m_bGCR,m_bGCG,m_bGCB;
	CEdit m_GCR, m_GCG, m_GCB;
	double m_dGCR,m_dGCG,m_dGCB;

protected:
	std::vector<CFractalCoordinate> & GetCoordinates();
	void RedrawButtons();
	void SetColors();
	COLORREF MakeRGB(COLORREF crRGB);
	void ChangeColor(COLORREF & crRGB,CImageButton & BtnRGB);
	BOOL ChooseImage(LPCTSTR lpszTitle,CString & csNewImage);

protected:
	afx_msg void OnComboRange();
	afx_msg void OnSelChangeComboRange();
	afx_msg void OnCheckGradient();
	afx_msg void OnCheckBase();
	afx_msg void OnBase();
	afx_msg void OnRandom();
	afx_msg void OnReverse();
	afx_msg void OnReset();
	afx_msg void OnTrapX();
	afx_msg void OnTrapY();
	afx_msg void OnRGB1();
	afx_msg void OnRGB2();
	afx_msg void OnRGB3();
	afx_msg void OnRGB4();
	afx_msg void OnRGB5();
	afx_msg void OnRGB6();
	afx_msg void OnDropUnused();
	afx_msg void OnSmooth();
	afx_msg void OnClickColor();
	afx_msg void OnChangeScaleColor();
	afx_msg void OnChooseImage();
	afx_msg void OnChooseImage2();
	afx_msg void OnStepColor();
	afx_msg void OnEditColors();
	afx_msg void OnKillFocusEditIter();
	afx_msg void OnClickGamma();
	afx_msg void OnClickDonate();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

public:
	UINT GetWidth() {return m_iWidth;}
	UINT GetHeight() {return m_iHeight;}
	UINT GetMode() {return m_iMode;}
	UINT GetIterations() {return m_iIterations;}
	UINT GetMaxCalc() {return m_nMaxCalc;}
	BOOL GetRed() {return m_bRed;}
	BOOL GetGreen() {return m_bGreen;}
	BOOL GetBlue() {return m_bBlue;}
	BOOL GetDropUnused() {return m_bDropUnused;}
	BOOL GetSmooth() {return m_bSmooth;}
	BOOL GetSortOrder() {return m_bSortOrder;}
	BOOL GetGradient() {return m_bGradient;}
	BOOL GetBaseColor() {return m_bBaseColor;}
	BOOL GetOrbitColor() {return m_bOrbitColor;}
	COLORREF GetRGB1() {return m_RGB1;}
	COLORREF GetRGB2() {return m_RGB2;}
	COLORREF GetRGB3() {return m_RGB3;}
	COLORREF GetRGB4() {return m_RGB4;}
	COLORREF GetRGB5() {return m_RGB5;}
	COLORREF GetRGB6() {return m_RGB6;}
	int GetRR() {return m_RR;}
	int GetRG() {return m_RG;}
	int GetRB() {return m_RB;}
	double GetXMin() {return m_dXMin;}
	double GetXMax() {return m_dXMax;}
	double GetYMin() {return m_dYMin;}
	double GetYMax() {return m_dYMax;}
	double GetP() {return m_dP;}
	double GetQ() {return m_dQ;}
	double GetXScale() {return m_dXScale;}
	double GetYScale() {return m_dYScale;}
	double GetBailRadius() {return m_dBailRadius;}
	double GetStep() {return m_dStep;}
	BOOL GetBase() {return m_bBase;}
	COLORREF GetRGBBase() {return m_RGBBase;}
	CString GetImage() {return m_csImage;}
	BOOL GetUseImage() {return m_bUseImage;}
	CString GetImage2() {return m_csImage2;}
	BOOL GetUseImage2() {return m_bUseImage2;}
	int GetFractalType() {return m_iFractalType;}
	int GetAlpha() {return m_iAlpha;}
	int GetAlpha2() {return m_iAlpha2;}
	BOOL GetBlendBase() {return m_bBlendBase;}
	BOOL GetBlendFractal() {return m_bBlendFractal;}
	COLORREF GetRGBStepColor() {return m_RGBStepColor;}
	std::vector<std::vector<BYTE> > GetRGB() {return m_EditColors.GetColorMap();}
	BOOL GetTrapX() {return m_bTrapX;}
	double GetTrapXValue() {return m_dTrapX;}
	BOOL GetTrapY() {return m_bTrapY;}
	double GetTrapYValue() {return m_dTrapY;}
	BOOL GetGCR() {return m_bGCR;}
	double GetGCRValue() {return m_dGCR;}
	BOOL GetGCG() {return m_bGCG;}
	double GetGCGValue() {return m_dGCG;}
	BOOL GetGCB() {return m_bGCB;}
	double GetGCBValue() {return m_dGCB;}

	void SetWidth(UINT iWidth) {m_iWidth = iWidth;}
	void SetHeight(UINT iHeight) {m_iHeight = iHeight;}
	void SetMode(UINT iMode) {m_iMode = iMode;}
	void SetIterations(UINT iIterations) {m_iIterations = iIterations;}
	void SetMaxCalc(UINT nMaxCalc) {m_nMaxCalc = nMaxCalc;}
	void SetRed(BOOL bRed) {m_bRed = bRed;} 
	void SetGreen(BOOL bGreen) {m_bGreen = bGreen;} 
	void SetBlue(BOOL bBlue) {m_bBlue = bBlue;}
	void SetDropUnused(BOOL bDropUnused) {m_bDropUnused = bDropUnused;}
	void SetSmooth(BOOL bSmooth) {m_bSmooth = bSmooth;}
	void SetSortOrder(BOOL bSortOrder) {m_bSortOrder = bSortOrder;}
	void SetGradient(BOOL bGradient) {m_bGradient = bGradient;}
	void SetBaseColor(BOOL bBaseColor) {m_bBaseColor = bBaseColor;}
	void SetOrbitColor(BOOL bOrbitColor) {m_bOrbitColor = bOrbitColor;}
	void SetRGB1(COLORREF RGB) {m_RGB1 = RGB;}
	void SetRGB2(COLORREF RGB) {m_RGB2 = RGB;}
	void SetRGB3(COLORREF RGB) {m_RGB3 = RGB;}
	void SetRGB4(COLORREF RGB) {m_RGB4 = RGB;}
	void SetRGB5(COLORREF RGB) {m_RGB5 = RGB;}
	void SetRGB6(COLORREF RGB) {m_RGB6 = RGB;}
	void SetRR(int RR) {m_RR = RR;} 
	void SetRB(int RB) {m_RB = RB;} 
	void SetRG(int RG) {m_RG = RG;} 
	void SetXMin(double dXMin) {m_dXMin = dXMin;}
	void SetXMax(double dXMax) {m_dXMax = dXMax;}
	void SetYMin(double dYMin) {m_dYMin = dYMin;}
	void SetYMax(double dYMax) {m_dYMax = dYMax;}
	void SetDDVXMin(double dXMin) {m_dDDVXMin = dXMin;}
	void SetDDVXMax(double dXMax) {m_dDDVXMax = dXMax;}
	void SetDDVYMin(double dYMin) {m_dDDVYMin = dYMin;}
	void SetDDVYMax(double dYMax) {m_dDDVYMax = dYMax;}
	void SetP(double dP) {m_dP = dP;}
	void SetQ(double dQ) {m_dQ = dQ;}
	void SetXScale(double dXScale) {m_dXScale = dXScale;}
	void SetYScale(double dYScale) {m_dYScale = dYScale;}
	void SetBailRadius(double dBailRadius) {m_dBailRadius = dBailRadius;}
	void SetStep(double dStep) {m_dStep = dStep;}
	void SetBase(BOOL bBase) {m_bBase = bBase;}
	void SetRGBBase(COLORREF RGB) {m_RGBBase = RGB;}
	void SetImage(CString csImage) {m_csImage = csImage;}
	void SetUseImage(BOOL bUseImage) {m_bUseImage = bUseImage;}
	void SetImage2(CString csImage2) {m_csImage2 = csImage2;}
	void SetUseImage2(BOOL bUseImage2) {m_bUseImage2 = bUseImage2;}
	void SetFractalType(int iFractalType) {m_iFractalType = iFractalType;}
	void SetAlpha(int iAlpha) {m_iAlpha = iAlpha;}
	void SetAlpha2(int iAlpha2) {m_iAlpha2 = iAlpha2;}
	void SetBlendBase(BOOL bBlendBase) {m_bBlendBase = bBlendBase;}
	void SetBlendFractal(BOOL bBlendFractal) {m_bBlendFractal = bBlendFractal;}
	void SetRGBStepColor(COLORREF RGBStepColor) {m_RGBStepColor = RGBStepColor;}
	void SetTrapX(BOOL bTrapX) {m_bTrapX = bTrapX;}
	void SetTrapXValue(double dTrapX) {m_dTrapX = dTrapX;}
	void SetTrapY(BOOL bTrapY) {m_bTrapY = bTrapY;}
	void SetTrapYValue(double dTrapY) {m_dTrapY = dTrapY;}
	void SetGCR(BOOL bGCR) {m_bGCR = bGCR;}
	void SetGCRValue(double dGCR) {m_dGCR = dGCR;}
	void SetGCG(BOOL bGCG) {m_bGCG = bGCG;}
	void SetGCGValue(double dGCG) {m_dGCG = dGCG;}
	void SetGCB(BOOL bGCB) {m_bGCB = bGCB;}
	void SetGCBValue(double dGCB) {m_dGCB = dGCB;}
};

// Dialog to select the type of fractal that is created
class CNewFractalType : public CDialog
{
	DECLARE_DYNAMIC(CNewFractalType)

public:
	CNewFractalType(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewFractalType();

public:
	int GetChoice() {return m_iChoice;}
	void SetChoice(int iChoice) {m_iChoice = iChoice;}

// Dialog Data
	enum { IDD = IDD_NEW_FRACTAL_TYPE };

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CString m_csText;
	int m_iChoice;
};

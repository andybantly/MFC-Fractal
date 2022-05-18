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
#include "BitmapIcon.h"
#include "FractalBase.h"
#include "cpu.h"

class CFractalDoc : public CDocument
{
protected: // create from serialization only
	CFractalDoc();
	DECLARE_DYNCREATE(CFractalDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);

protected:
	void SaveImage(CArchive & ar,CString & csPictureName);
	void LoadImage(CArchive & ar,CString & csPictureName);

// Implementation
public:
	virtual ~CFractalDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void RenderFractal();
	void RenderFractal(double dLastXMin,double dLastXMax,double dLastYMin,double dLastYMax,int iNewWidth,std::vector<std::vector<BYTE> > RGB);
	CFractalBase * ChooseFractal(CFractalParm & FractalParm);
	void SortDrop(std::vector<CIteration> Iterations);
	void SetFractalTitle();
	void AddTextToDoc(CDecorativeText & DecorativeText,int iReplaceText = FALSE);
	void AddPictureToDoc(CDecorativePicture & DecorativePicture,int iReplacePicture = FALSE);

	CDIBFrame & GetDoubleBuffer() {return m_DoubleBufferDIB;}
	CDIBFrame & GetBaseImage() {return m_BaseDIB;}
	CDIBFrame & GetFractalImage() {return m_FractalDIB;}
	CDIBFrame & GetFractal() {return m_DisplayDIB;}
	std::vector<CDecorativeText> & GetDecorativeText() {return m_vDecorativeText;}
	std::vector<CDecorativePicture> & GetDecorativePicture() {return m_vDecorativePicture;}
	std::vector<std::vector<BYTE> > & GetRGB() {return m_RGB;}

public: // properties
	double GetXMin();
	double GetXMax();
	double GetYMin();
	double GetYMax();
	double GetP();
	double GetQ();
	LONG GetWidth();
	LONG GetHeight();
	double GetXScale();
	double GetYScale();
	double GetBailRadius();
	LONG GetMode();
	LONG GetIterations();
	LONG GetMaxCalc();
	VARIANT_BOOL GetRed();
	VARIANT_BOOL GetGreen();
	VARIANT_BOOL GetBlue();
	VARIANT_BOOL GetDropUnused();
	VARIANT_BOOL GetSmooth();
	VARIANT_BOOL GetSortOrder();
	VARIANT_BOOL GetGradient();
	VARIANT_BOOL GetBase();
	VARIANT_BOOL GetBaseColor();
	VARIANT_BOOL GetOrbitColor();
	COLORREF GetRGB1();
	COLORREF GetRGB2();
	COLORREF GetRGB3();
	COLORREF GetRGB4();
	COLORREF GetRGB5();
	COLORREF GetRGB6();
	COLORREF GetRGBBase();
	LONG GetRR();
	LONG GetRG();
	LONG GetRB();
	double GetStep();
	COLORREF GetRGBStepColor();
	BSTR GetImage();
	VARIANT_BOOL GetUseImage();
	BSTR GetImage2();
	VARIANT_BOOL GetUseImage2();
	LONG GetFractalType();
	LONG GetAlpha();
	LONG GetAlpha2();
	VARIANT_BOOL GetBlendBase();
	VARIANT_BOOL GetBlendFractal();
	BSTR GetFileName();
	VARIANT_BOOL GetTrapX();
	double GetTrapXValue();
	VARIANT_BOOL GetTrapY();
	double GetTrapYValue();
	VARIANT_BOOL GetGCR();
	double GetGCRValue();
	VARIANT_BOOL GetGCG();
	double GetGCGValue();
	VARIANT_BOOL GetGCB();
	double GetGCBValue();
	IDispatch * GetPicture();
	
	void SetXMin(double dXMin);
	void SetXMax(double dXMax);
	void SetYMin(double dYMin);
	void SetYMax(double dYMax);
	void SetP(double dP);
	void SetQ(double dQ);
	void SetWidth(LONG iWidth);
	void SetHeight(LONG iHeight);
	void SetXScale(double dXScale);
	void SetYScale(double dYScale);
	void SetBailRadius(double dBailRadius);
	void SetMode(LONG iMode);
	void SetIterations(LONG iIterations);
	void SetMaxCalc(LONG nMaxCalc);
	void SetRed(VARIANT_BOOL bRed);
	void SetGreen(VARIANT_BOOL bGreen);
	void SetBlue(VARIANT_BOOL bBlue);
	void SetDropUnused(VARIANT_BOOL bDropUnused);
	void SetSmooth(VARIANT_BOOL bSmooth);
	void SetSortOrder(VARIANT_BOOL bSortOrder);
	void SetGradient(VARIANT_BOOL bGradient);
	void SetBase(VARIANT_BOOL bBase);
	void SetBaseColor(VARIANT_BOOL bBaseColor);
	void SetOrbitColor(VARIANT_BOOL bOrbitColor);
	void SetRGB(std::vector<std::vector<BYTE> > & vRGB) {m_RGB = vRGB;}
	void SetRGB1(COLORREF RGB);
	void SetRGB2(COLORREF RGB);
	void SetRGB3(COLORREF RGB);
	void SetRGB4(COLORREF RGB);
	void SetRGB5(COLORREF RGB);
	void SetRGB6(COLORREF RGB);
	void SetRR(LONG RR);
	void SetRG(LONG RG);
	void SetRB(LONG RB);
	void SetStep(double dStep);
	void SetRGBStepColor(COLORREF RGBStepColor);
	void SetRGBBase(COLORREF RGB);
	void SetImage(LPCTSTR Image);
	void SetUseImage(VARIANT_BOOL bUseImage);
	void SetImage2(LPCTSTR Image2);
	void SetUseImage2(VARIANT_BOOL bUseImage2);
	void SetFractalType(LONG iFractalType);
	void SetAlpha(LONG iAlpha);
	void SetAlpha2(LONG iAlpha2);
	void SetBlendBase(VARIANT_BOOL bBlendBase);
	void SetBlendFractal(VARIANT_BOOL bBlendFractal);
	void SetFileName(LPCTSTR FileName);
	void SetTrapX(VARIANT_BOOL bTrapX);
	void SetTrapXValue(double dTrapX);
	void SetTrapY(VARIANT_BOOL bTrapY);
	void SetTrapYValue(double dTrapY);
	void SetGCR(VARIANT_BOOL bGCR);
	void SetGCRValue(double dGCR);
	void SetGCG(VARIANT_BOOL bGCG);
	void SetGCGValue(double dGCG);
	void SetGCB(VARIANT_BOOL bGCB);
	void SetGCBValue(double dGCB);
	void SetPicture(IDispatch *);

public: // methods
	void Render();
	void RenderZoom(double dLastXMin,double dLastXMax,double dLastYMin,double dLastYMax,int iNewWidth);

public:
	void SetOpen(BOOL bOpen) {m_bOpen = bOpen;}

protected:
	void PrimeRender();

protected:
	BOOL m_bOpen;
	CPU m_CPU;
	CDIBFrame m_BaseDIB,m_FractalDIB,m_DisplayDIB,m_DoubleBufferDIB;
	BOOL m_bEmbedImage;
	CString m_csPathFileName;

protected:
	// Fractal rendering
	UINT m_iFractalType;
	UINT m_iWidth,m_iHeight,m_iMode,m_iIterations,m_nMaxCalc;
	BOOL m_bRed,m_bGreen,m_bBlue,m_bDropUnused,m_bSmooth,m_bSortOrder,m_bGradient,m_bBase,m_bBaseColor,m_bOrbitColor;
	COLORREF m_RGB1,m_RGB2,m_RGB3,m_RGB4,m_RGB5,m_RGB6,m_RGBBase;
	int m_RR,m_RG,m_RB;
	double m_dXScale,m_dYScale,m_dBailRadius,m_dXMin,m_dXMax,m_dYMin,m_dYMax,m_dP,m_dQ,m_dStep;
	std::vector<std::vector<BYTE> > m_RGB;
	CString m_csImage,m_csImage2;
	BOOL m_bUseImage,m_bUseImage2;
	int m_iAlpha,m_iAlpha2;
	BOOL m_bBlendBase,m_bBlendFractal;
	COLORREF m_RGBStepColor;
	std::vector<CDecorativeText> m_vDecorativeText;
	std::vector<CDecorativePicture> m_vDecorativePicture;
	BOOL m_bTrapX,m_bTrapY;
	double m_dTrapX,m_dTrapY;
	BOOL m_bGCR,m_bGCG,m_bGCB;
	double m_dGCR,m_dGCG,m_dGCB;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();

	// Generated OLE dispatch map functions
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
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

#include <set>

class CFractalParm
{
public:
	CFractalParm() : m_iIterations(0) {};
	CFractalParm(int iMode,int iIterations,int nMaxCalc,int MC,int MR,double deltaP,double deltaQ,double XMin,double YMin,double XMax,double YMax,double dP,double dQ,double dXScale,double dYScale,double dBailRadius,double dStep,std::vector<std::vector<BYTE> > & vecRGB,BOOL bGradient,int iFractalType,BOOL bBaseColor,BOOL bOrbitColor,std::vector<LONG> * pvecEnergy,BOOL bTrapX,double dTrapX,BOOL bTrapY,double dTrapY)
	{
		SetFractalType(iFractalType);
		SetMode(iMode);
		SetIterations(iIterations);
		SetMaxCalc(nMaxCalc);
		SetMC(MC);
		SetMR(MR);
		SetDeltaP(deltaP);
		SetDeltaQ(deltaQ);
		SetXMin(XMin);
		SetYMin(YMin);
		SetXMax(XMax);
		SetYMax(YMax);
		SetDP(dP);
		SetDQ(dQ);
		SetXScale(dXScale);
		SetYScale(dYScale);
		SetBailRadius(dBailRadius);
		SetStep(dStep);
		SetRGB(vecRGB);
		SetGradient(bGradient);
		SetBaseColor(bBaseColor);
		SetOrbitColor(bOrbitColor);
		SetEnergy(pvecEnergy);
		SetTrapX(bTrapX);
		SetTrapXValue(dTrapX);
		SetTrapY(bTrapY);
		SetTrapYValue(dTrapY);
		SetQ();
	}
	CFractalParm(CFractalParm & FractalParm) {*this = FractalParm;}
	~CFractalParm(){};
	CFractalParm & CFractalParm::operator = (CFractalParm & FractalParm)
	{
		if (this != &FractalParm)
		{
			SetFractalType(FractalParm.GetFractalType());
			SetMode(FractalParm.GetMode());
			SetIterations(FractalParm.GetIterations());
			SetMaxCalc(FractalParm.GetMaxCalc());
			SetMC(FractalParm.GetMC());
			SetMR(FractalParm.GetMR());
			SetDeltaP(FractalParm.GetDeltaP());
			SetDeltaQ(FractalParm.GetDeltaQ());
			SetXScale(FractalParm.GetXScale());
			SetYScale(FractalParm.GetYScale());
			SetBailRadius(FractalParm.GetBailRadius());
			SetXMin(FractalParm.GetXMin());
			SetYMin(FractalParm.GetYMin());
			SetRGB(FractalParm.GetRGB());
			SetXMax(FractalParm.GetXMax());
			SetYMax(FractalParm.GetYMax());
			SetDP(FractalParm.GetDP());
			SetDQ(FractalParm.GetDQ());
			SetStep(FractalParm.GetStep());
			SetGradient(FractalParm.GetGradient());
			SetBaseColor(FractalParm.GetBaseColor());
			SetOrbitColor(FractalParm.GetOrbitColor());
			SetEnergy(FractalParm.GetEnergy());
			SetTrapX(FractalParm.GetTrapX());
			SetTrapXValue(FractalParm.GetTrapXValue());
			SetTrapY(FractalParm.GetTrapY());
			SetTrapYValue(FractalParm.GetTrapYValue());
			SetQ();
		}
		return *this;
	}

public:
	int GetIterations() const {return m_iIterations;}
	int GetMode() const {return m_iMode;}
	int GetMaxCalc() const {return m_nMaxCalc;}
	int GetMC() const {return m_MC;}
	int GetMR() const {return m_MR;}
	double * GetQ() {return &m_Q[0];}
	double GetDeltaP() const {return m_deltaP;}
	double GetDeltaQ() const {return m_deltaQ;}
	double GetXMin() const {return m_XMin;}
	double GetYMin() const {return m_YMin;}
	double GetXMax() const {return m_XMax;}
	double GetYMax() const {return m_YMax;}
	double GetDP() const {return m_dP;}
	double GetDQ() const {return m_dQ;}
	double GetXScale() const {return m_dXScale;}
	double GetYScale() const {return m_dYScale;}
	double GetBailRadius() const {return m_dBailRadius;}
	double GetStep() const {return m_dStep;}

	std::vector<std::vector<BYTE> > & GetRGB() {return m_RGB;}

	BOOL GetGradient() const {return m_bGradient;}
	int GetFractalType() const {return m_iFractalType;}
	BOOL GetBaseColor() const {return m_bBaseColor;}
	BOOL GetOrbitColor() const {return m_bOrbitColor;}
	std::vector<LONG> * GetEnergy() {return m_pvecEnergy;}
	BOOL GetTrapX() const {return m_bTrapX;}
	double GetTrapXValue() const {return m_dTrapX;}
	BOOL GetTrapY() const {return m_bTrapY;}
	double GetTrapYValue() const {return m_dTrapY;}

	void SetIterations(int iIterations) {m_iIterations = iIterations;}
	void SetMode(int iMode) {m_iMode = iMode;}
	void SetMaxCalc(int nMaxCalc) {m_nMaxCalc = nMaxCalc;}
	void SetMC(int iMC) {m_MC = iMC;}
	void SetMR(int iMR) {m_MR = iMR;}
	void SetDeltaP(double deltaP) {m_deltaP = deltaP;}
	void SetDeltaQ(double deltaQ) {m_deltaQ = deltaQ;}
	void SetXMin(double dXMin) {m_XMin = dXMin;}
	void SetYMin(double dYMin) {m_YMin = dYMin;}
	void SetXMax(double dXMax) {m_XMax = dXMax;}
	void SetYMax(double dYMax) {m_YMax = dYMax;}
	void SetDP(double dP) {m_dP = dP;}
	void SetDQ(double dQ) {m_dQ = dQ;}
	void SetXScale(double dXScale) {m_dXScale = dXScale;}
	void SetYScale(double dYScale) {m_dYScale = dYScale;}
	void SetBailRadius(double dBailRadius) {m_dBailRadius = dBailRadius;}
	void SetStep(double dStep) {m_dStep = dStep;}
	void SetRGB(std::vector<std::vector<BYTE> > & vecRGB) {m_RGB = vecRGB;}
	void SetGradient(BOOL bGradient) {m_bGradient = bGradient;}
	void SetFractalType(int iFractalType) {m_iFractalType = iFractalType;}
	void SetBaseColor(BOOL bBaseColor) {m_bBaseColor = bBaseColor;}
	void SetOrbitColor(BOOL bOrbitColor) {m_bOrbitColor = bOrbitColor;}
	void SetEnergy(std::vector<LONG> * pvecEnergy) {m_pvecEnergy = pvecEnergy;}
	void SetTrapX(BOOL bTrapX) {m_bTrapX = bTrapX;}
	void SetTrapXValue(double dTrapX) {m_dTrapX = dTrapX;}
	void SetTrapY(BOOL bTrapY) {m_bTrapY = bTrapY;}
	void SetTrapYValue(double dTrapY) {m_dTrapY = dTrapY;}

protected:
	void SetQ()
	{
		m_Q.resize(GetMR() + 1);
		m_Q[1] = GetYMax();
		for (int R = 2;R <= GetMR();++R)
			m_Q[R] = m_Q[R - 1] - GetDeltaQ();
	}

private:
	int m_iFractalType,m_iIterations,m_nMaxCalc,m_MC,m_MR;
	double m_deltaP,m_deltaQ,m_XMin,m_YMin,m_XMax,m_YMax,m_dP,m_dQ,m_dStep;
	std::vector<std::vector<BYTE> > m_RGB;
	BOOL m_bGradient,m_bBaseColor,m_bOrbitColor;
	std::vector<LONG> * m_pvecEnergy;
	std::vector<double> m_Q;
	int m_iMode;
	double m_dXScale,m_dYScale,m_dBailRadius;
	BOOL m_bTrapX,m_bTrapY;
	double m_dTrapX,m_dTrapY;
};

// Base class for decorative elements that contains the rectangular coordinates
class CDecorative
{
public:
	CDecorative() : m_Rect(0,0,0,0) {};
	CDecorative(CRect Rect)
	{
		// Decorative rectangle
		m_Rect = Rect;
	}
	CDecorative(const CDecorative & rhs) {*this = rhs;}
	~CDecorative() {};
	CDecorative & CDecorative::operator = (const CDecorative & rhs)
	{
		if (this != &rhs)
		{
			// Decorative rectangle
			SetRect(rhs.GetRect());
		}
		return *this;
	}

	bool operator < (const CDecorative & rhs) const
	{
		return m_Rect < rhs.m_Rect;
	}

	bool operator == (const CDecorative & rhs) const
	{
		return m_Rect == rhs.m_Rect ? true : false;
	}

	CRect GetRect() const {return m_Rect;}
	void SetRect(CRect Rect) {m_Rect = Rect;}

protected:
	CRect m_Rect;
};

// Container for 1 element of some decorative text
class CDecorativeText : public CDecorative
{
	typedef CDecorative baseclass;
public:
	CDecorativeText() : 
		CDecorative(), 
			m_csFontName(L"Calibri"), m_iPtSize(12),
			m_TextRGB(RGB(255,255,255)), m_BackRGB((0,0,0)), m_bNoBackColor(FALSE),
			m_iWeight(0), m_bItalics(FALSE), m_iOpacity(50) {};
	CDecorativeText(CRect Rect,CString csFontName,int iWeight,BOOL bItalics,UINT iPtSize,UINT iOpacity,COLORREF TextRGB,COLORREF BackRGB,BOOL bNoBackColor,CString csText) :
		CDecorative(Rect),
			m_csFontName(csFontName), m_iPtSize(iPtSize),
			m_TextRGB(TextRGB), m_BackRGB(BackRGB), m_bNoBackColor(bNoBackColor),
			m_iWeight(iWeight), m_bItalics(bItalics), m_iOpacity(iOpacity),
			m_csText(csText) {};
	CDecorativeText(CString csFontName,int iWeight,BOOL bItalics,UINT iPtSize,UINT iOpacity,COLORREF TextRGB,COLORREF BackRGB,BOOL bNoBackColor,CString csText) :
		CDecorative(),
			m_csFontName(csFontName), m_iPtSize(iPtSize),
			m_TextRGB(TextRGB), m_BackRGB(BackRGB), m_bNoBackColor(bNoBackColor),
			m_iWeight(iWeight), m_bItalics(bItalics), m_iOpacity(iOpacity),
			m_csText(csText) {};
	CDecorativeText(const CDecorativeText & rhs) {*this = rhs;}
	~CDecorativeText() {};
	CDecorativeText & CDecorativeText::operator = (const CDecorativeText & rhs)
	{
		if (this != &rhs)
		{
			baseclass::operator = (rhs);
			SetFontName(rhs.GetFontName());
			SetWeight(rhs.GetWeight());
			SetItalics(rhs.GetItalics());
			SetPtSize(rhs.GetPtSize());
			SetOpacity(rhs.GetOpacity());
			SetTextRGB(rhs.GetTextRGB());
			SetBackRGB(rhs.GetBackRGB());
			SetText(rhs.GetText());
			SetNoBackColor(rhs.GetNoBackColor());
		}
		return *this;
	}

	bool operator < (const CDecorativeText & rhs) const
	{
		return baseclass::operator < (rhs);
	}

	bool operator == (const CDecorativeText & rhs) const
	{
		return baseclass::operator ==(rhs);
	}

	CString GetFontName() const {return m_csFontName;}
	int GetWeight() const {return m_iWeight;}
	BOOL GetItalics() const {return m_bItalics;}
	UINT GetPtSize() const {return m_iPtSize;}
	UINT GetOpacity() const {return m_iOpacity;}
	COLORREF GetTextRGB() const {return m_TextRGB;}
	COLORREF GetBackRGB() const {return m_BackRGB;}
	CString GetText() const {return m_csText;}
	BOOL GetNoBackColor() const {return m_bNoBackColor;}

	void SetFontName(CString csFontName) {m_csFontName = csFontName;}
	void SetWeight(int iWeight) {m_iWeight = iWeight;}
	void SetItalics(BOOL bItalics) {m_bItalics = bItalics;}
	void SetPtSize(UINT iPtSize) {m_iPtSize = iPtSize;}
	void SetOpacity(UINT iOpacity) {m_iOpacity = iOpacity;}
	void SetTextRGB(COLORREF TextRGB) {m_TextRGB = TextRGB;}
	void SetBackRGB(COLORREF BackRGB) {m_BackRGB = BackRGB;}
	void SetText(CString csText) {m_csText = csText;}
	void SetNoBackColor(BOOL bNoBackColor) {m_bNoBackColor = bNoBackColor;}

protected:
	CString m_csFontName;
	BOOL m_bItalics,m_bNoBackColor;
	int m_iWeight;
	COLORREF m_TextRGB,m_BackRGB;
	UINT m_iPtSize,m_iOpacity;
	CString m_csText;
};

// Container for 1 element of a decorative picture
class CDecorativePicture : public CDecorative
{
	typedef CDecorative baseclass;
public:
	CDecorativePicture() : CDecorative(), m_iOpacity(50), m_bTransparentColor(FALSE), m_crTransparentColor(RGB(0,128,0)) {};
	CDecorativePicture(CRect Rect,CString csPictureName,UINT iOpacity, BOOL bTransparentColor, COLORREF crTransparentColor) : 
		CDecorative(Rect), 
			m_csPictureName(csPictureName), m_iOpacity(iOpacity),
			m_bTransparentColor(bTransparentColor), m_crTransparentColor(crTransparentColor) {};
	CDecorativePicture(CString csPictureName,UINT iOpacity, BOOL bTransparentColor, COLORREF crTransparentColor) : 
		CDecorative(), 
			m_csPictureName(csPictureName), m_iOpacity(iOpacity),
			m_bTransparentColor(bTransparentColor), m_crTransparentColor(crTransparentColor) {};
	CDecorativePicture(const CDecorativePicture & rhs) {*this = rhs;}
	~CDecorativePicture() {};
	CDecorativePicture & CDecorativePicture::operator = (const CDecorativePicture & rhs)
	{
		if (this != &rhs)
		{
			baseclass::operator = (rhs);
			SetPictureName(rhs.GetPictureName());
			SetOpacity(rhs.GetOpacity());
			SetTransparentColorFlag(rhs.GetTransparentColorFlag());
			SetTransparentColor(rhs.GetTransparentColor());
		}
		return *this;
	}

	bool operator < (const CDecorativePicture & rhs) const
	{
		return baseclass::operator < (rhs);
	}

	bool operator == (const CDecorativePicture & rhs) const
	{
		return baseclass::operator ==(rhs);
	}

	CString GetPictureName() const {return m_csPictureName;}
	UINT GetOpacity() const {return m_iOpacity;}
	BOOL GetTransparentColorFlag() const {return m_bTransparentColor;}
	COLORREF GetTransparentColor() const {return m_crTransparentColor;}

	void SetPictureName(CString csPictureName) {m_csPictureName = csPictureName;}
	void SetOpacity(UINT iOpacity) {m_iOpacity = iOpacity;}
	void SetTransparentColorFlag(BOOL bTransparentColor) {m_bTransparentColor = bTransparentColor;}
	void SetTransparentColor(COLORREF crTransparentColor) {m_crTransparentColor = crTransparentColor;}

protected:
	CString m_csPictureName;
	UINT m_iOpacity;
	BOOL m_bTransparentColor;
	COLORREF m_crTransparentColor;
};

class CIteration
{
public:
	CIteration() : m_nIteration(0),m_iIteration(0),m_crRGB(0) {}
	CIteration & operator = (const CIteration & rhs)
	{
		if (this != &rhs)
		{
			m_nIteration = rhs.m_nIteration;
			m_iIteration = rhs.m_iIteration;
			m_crRGB = rhs.m_crRGB;
		}
		return *this;
	}
	CIteration & operator = (int nIteration)
	{
		m_nIteration = nIteration;
		return *this;
	}
	bool operator < (const CIteration & rhs) const
	{
		return m_nIteration < rhs.m_nIteration || (m_nIteration == rhs.m_nIteration && m_iIteration < rhs.m_iIteration);
	}
	bool operator > (const CIteration & rhs) const
	{
		return m_nIteration > rhs.m_nIteration || (m_nIteration == rhs.m_nIteration && m_iIteration > rhs.m_iIteration);
	}
	CIteration(const CIteration & rhs) {*this = rhs;}
	operator int() const {return m_nIteration;}
	int operator ++ () {return ++m_nIteration;}

	void SetIteration(int iIteration) {m_iIteration = iIteration;}
	void SetRGB(COLORREF crRGB) {m_crRGB = crRGB;}

	int GetIteration() {return m_iIteration;}
	int GetRGB() {return m_crRGB;}

private:
	int m_nIteration;
	int m_iIteration;
	COLORREF m_crRGB;
};

template <class T>
struct LessThan : public std::binary_function<const T &,const T &,bool>
{
	bool operator()(const T & LHS,const T & RHS) const
	{
		return LHS < RHS;
	}
};

template <class T>
struct GreaterThan : public std::binary_function<const T &,const T &,bool>
{
	bool operator()(const T & LHS,const T & RHS) const
	{
		return LHS > RHS;
	}
};
/*
Andrew Scott Bantly's Software License Agreement
Version 1.0

Copyright(C) 2012 Andrew Scott Bantly

1. Permission and Restrictions:
   - This software is proprietary and is provided "as is."
   - No individual or organization may copy, modify, distribute, or use this software or its derivatives without prior written consent from Andrew Scott Bantly.
   - Usage in commercial, educational, or personal projects is strictly prohibited without explicit permission from Andrew Scott Bantly

2. Limited Access :
   - Access to the source code is granted for reference purposes only.
   - Redistribution of the source code or its compiled binaries, whether modified or unmodified, is expressly forbidden.

3. Liability Disclaimer :
   - This software is provided without warranty of any kind, either express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, or noninfringement.
   - Under no circumstances shallAndrew Scott Bantly be liable for any damages, including incidental, special, or consequential damages arising out of or in connection with the use of this software.

4. Violation of Terms :
   - Any unauthorized use, modification, or distribution of this software will result in immediate revocation of any implied permissions and may lead to legal action.

5. Jurisdiction :
   - This license shall be governed by the laws of The United States of America.
*/

#pragma once

#include <ostream>
#include <math.h>
#include "FractalParm.h"
#include "corecrt_math_defines.h"

extern CComAutoCriticalSection g_MTBlock;

const double g_d2Pi = 2.0 * M_PI;

// Class for common calculations
class CCalcBase
{
public:
	CCalcBase(CFractalParm Parm) : m_Parm(Parm), m_iMode(1), m_dXScale(1), m_dYScale(1), m_dBailRadius(4), m_bTrapX(FALSE), m_bTrapY(FALSE), m_bTrap(FALSE), m_dTrapScaleX(0.0), m_dTrapScaleY(0.0)
	{
		m_iMode = Parm.GetMode();
		m_dXScale = Parm.GetXScale();
		m_dYScale = Parm.GetYScale();
		m_dBailRadius = Parm.GetBailRadius();

		double dColors = (double)Parm.GetIterations();
		m_bTrapX = Parm.GetTrapX();
		m_dTrapEpsilonX = Parm.GetTrapXValue();
		m_bTrapY = Parm.GetTrapY();
		m_dTrapEpsilonY = Parm.GetTrapYValue();
		if (m_bTrapX)
			m_dTrapScaleX = dColors / m_dTrapEpsilonX;
		if (m_bTrapY)
			m_dTrapScaleY = dColors / m_dTrapEpsilonY;
		m_bTrap = m_bTrapX | m_bTrapY;
	};
	~CCalcBase(){};

protected:
	bool Test(double dXSQ, double dYSQ, int iMode = 0) /* final */ // try to keep this baby as fast as possible.  virtualizing will slow by 35% or more
	{
		switch (m_iMode)
		{
		case 1:
			return (m_dXScale * dXSQ + m_dYScale * dYSQ) < m_dBailRadius;
		case 2:
			return (m_dXScale * dXSQ - m_dYScale * dYSQ) < m_dBailRadius;
		case 3:
			return (m_dYScale * dYSQ - m_dXScale * dXSQ) < m_dBailRadius;
		case 4:
			return (abs(m_dXScale * dXSQ - m_dYScale * dYSQ)) < m_dBailRadius;
		default:
			return false;
		}
		return false;
	}

	double Radius()
	{
		return sqrt(m_XSQ + m_YSQ);
	}

	int Step(int J,double dR)
	{
		return max((int)((double)J - dR),1);
	}

	int Calc2Rad(double dRadius,double dRadScale)
	{
		if (dRadius == 0.0)
			return 0;
		double theta = atan2(m_Y / dRadius,m_X / dRadius);
		if (theta < 0)
			theta += g_d2Pi;
		return (int)(theta * dRadScale);
	}

	int TrapCalc(double X,double Y)
	{
		if (m_bTrap)
		{
			int iTrapX = TrapCalcX(X);
			int	iTrapY = TrapCalcY(Y);
			return iTrapX | iTrapY;
		}
		return 0;
	}

	int TrapCalcX(double X)
	{
		if (m_bTrapX && (abs(X) < m_dTrapEpsilonX))
			return (int)(abs(X) * m_dTrapScaleX);
		return 0;
	}

	int TrapCalcY(double Y)
	{
		if (m_bTrapY && (abs(Y) < m_dTrapEpsilonY))
			return (int)(abs(Y) * m_dTrapScaleY);
		return 0;
	}

protected:
	CFractalParm m_Parm;

protected:
	double m_X;
	double m_Y;
	double m_XSQ;
	double m_YSQ;
	BOOL m_bTrapX,m_bTrapY,m_bTrap;
	double m_dTrapEpsilonX,m_dTrapEpsilonY;
	double m_dTrapScaleX,m_dTrapScaleY;

protected:
	std::vector<double> m_vecXPoint;
	std::vector<double> m_vecYPoint;

protected:
	int m_iMode;
	double m_dXScale;
	double m_dYScale;
	double m_dBailRadius;
};
/* -- possibly future function but would need devirtualization
class CCalcBaseMode1 : public CCalcBase
{
public:
	CCalcBaseMode1(CFractalParm Parm) : CCalcBase(Parm)	{};
	virtual ~CCalcBaseMode1(){};

protected:
	virtual bool Test(double dXSQ, double dYSQ) final
	{
		return (m_dXScale * dXSQ + m_dYScale * dYSQ) < m_dBailRadius;
	}
};

class CCalcBaseMode2 : public CCalcBase
{
public:
	CCalcBaseMode2(CFractalParm Parm) : CCalcBase(Parm)	{};
	virtual ~CCalcBaseMode2(){};

protected:
	virtual bool Test(double dXSQ, double dYSQ) final
	{
		return (m_dXScale * dXSQ - m_dYScale * dYSQ) < m_dBailRadius;
	}
};

class CCalcBaseMode3 : public CCalcBase
{
public:
	CCalcBaseMode3(CFractalParm Parm) : CCalcBase(Parm)	{};
	virtual ~CCalcBaseMode3(){};

protected:
	virtual bool Test(double dXSQ, double dYSQ) final
	{
		return (m_dYScale * dYSQ - m_dXScale * dXSQ) < m_dBailRadius;
	}
};

class CCalcBaseMode4 : public CCalcBase
{
public:
	CCalcBaseMode4(CFractalParm Parm) : CCalcBase(Parm)	{};
	virtual ~CCalcBaseMode4(){};

protected:
	virtual bool Test(double dXSQ, double dYSQ) final
	{
		return (abs(m_dXScale * dXSQ - m_dYScale * dYSQ)) < m_dBailRadius;
	}
};
*/
// Class for calculating the mandelbrot set
class CFractalBase
{
public:
	CFractalBase(CFractalParm FractalParm);
	virtual ~CFractalBase() {};
	virtual void operator() (int RowBeg, int RowEnd) = 0;
	virtual void Apply(ULONG Iteration, int Column, int Row) = 0;

protected:
	CFractalParm m_FractalParm;
};

// Base derived class for holding the canvas (DIB + colors)
class CFractalCanvas : public CFractalBase
{
public:
	CFractalCanvas(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CFractalCanvas() {};
	std::vector<CIteration> GetIterations();
	virtual void Apply(ULONG Iteration, int Column, int Row) final
	{
		// Check for color scaling (calculations <> color palette)
		LONG Color = Iteration;
		if (Color == m_nMaxCalc)
			Color = 0; // Base color
		else
		{
			if (m_bModulo)
				Color = Iteration % m_nMaxIterations + 1;

			// Increment the count of this iteration
			++m_Iterations[Color];
		}

		// Set the pixel and track the palette for palette animation
		m_DisplayDIB.SetPixelAndPaletteIndex(Column - 1, Row - 1, m_RGB[0][Color], m_RGB[1][Color], m_RGB[2][Color], Color);
	}

protected:
	CDIBFrame & m_DisplayDIB;
	std::vector<std::vector<BYTE> > m_RGB;
	int m_nMaxIterations,m_nMaxCalc;
	bool m_bModulo;
	std::vector<CIteration> m_Iterations;
};

// Driver thread that carries out the function
class CDriveMultiThreadedFractal : public CWinThread
{
	DECLARE_DYNCREATE(CDriveMultiThreadedFractal)

private:
	CDriveMultiThreadedFractal() : m_phHandle(NULL) {};

public:
	CDriveMultiThreadedFractal(HANDLE * phHandle,CFractalBase * pFractal);
	virtual ~CDriveMultiThreadedFractal();
	virtual BOOL InitInstance();
	virtual BOOL PumpMessage();
	virtual int ExitInstance();

protected:
	HANDLE m_hPump;
	bool m_bPumpMessage;
	HANDLE * m_phHandle;
	CFractalBase * m_pFractal;

public:
	CFractalBase * GetFractal() {return m_pFractal;}

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnDoWork(WPARAM wParam,LPARAM lParam);
	afx_msg void OnEndThread(WPARAM wParam,LPARAM lParam);
};
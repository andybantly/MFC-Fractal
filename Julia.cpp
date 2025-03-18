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

#include "stdafx.h"
#include <math.h>
#include <float.h>
#include "FractalDoc.h"
#include "Julia.h"

/////////////////////////////////////////////////////////////////////////////

CRenderJuliaFractal::CRenderJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJulia(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderJuliaFractal::~CRenderJuliaFractal()
{
}

void CRenderJuliaFractal::operator() (int RowBeg,int RowEnd)
{
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaY);

			// Apply the iteration
			Apply(iIter,C,R);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepJuliaFractal::CRenderStepJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJulia(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepJuliaFractal::~CRenderStepJuliaFractal()
{
}

void CRenderStepJuliaFractal::operator() (int RowBeg,int RowEnd)
{
	double dStep = m_FractalParm.GetStep();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(iIter,Step(C,dR),Step(R,dR));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderADJuliaFractal::CRenderADJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJulia(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderADJuliaFractal::~CRenderADJuliaFractal()
{
}

void CRenderADJuliaFractal::operator() (int RowBeg,int RowEnd)
{
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Calculate angular decomposition color
			if ((bBaseColor && iIter == nMaxCalc) || (bOrbitColor && iIter != nMaxCalc))
				iIter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(iIter,C,R);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepADJuliaFractal::CRenderStepADJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJulia(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepADJuliaFractal::~CRenderStepADJuliaFractal()
{
}

void CRenderStepADJuliaFractal::operator() (int RowBeg,int RowEnd)
{
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && iIter == nMaxCalc) || (bOrbitColor && iIter != nMaxCalc))
				iIter = Calc2Rad(dRadius,dRadScale);

			// Apply the the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(iIter,Step(C,dR),Step(R,dR));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderJuliaDragonFractal::CRenderJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderJuliaDragonFractal::~CRenderJuliaDragonFractal()
{
}

void CRenderJuliaDragonFractal::operator() (int RowBeg,int RowEnd)
{
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Apply the iteration
			Apply(iIter,C,R);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepJuliaDragonFractal::CRenderStepJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepJuliaDragonFractal::~CRenderStepJuliaDragonFractal()
{
}

void CRenderStepJuliaDragonFractal::operator() (int RowBeg,int RowEnd)
{
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dStep = m_FractalParm.GetStep();

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(iIter,Step(C,dR),Step(R,dR));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderADJuliaDragonFractal::CRenderADJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderADJuliaDragonFractal::~CRenderADJuliaDragonFractal()
{
}

void CRenderADJuliaDragonFractal::operator() (int RowBeg,int RowEnd)
{
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Calculate angular decomposition color
			if ((bBaseColor && iIter == nMaxCalc) || (bOrbitColor && iIter != nMaxCalc))
				iIter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(iIter,C,R);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepADJuliaDragonFractal::CRenderStepADJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepADJuliaDragonFractal::~CRenderStepADJuliaDragonFractal()
{
}

void CRenderStepADJuliaDragonFractal::operator() (int RowBeg,int RowEnd)
{
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && iIter == nMaxCalc) || (bOrbitColor && iIter != nMaxCalc))
				iIter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(iIter,Step(C,dR),Step(R,dR));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderJuliaPhoenixFractal::CRenderJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderJuliaPhoenixFractal::~CRenderJuliaPhoenixFractal()
{
}

void CRenderJuliaPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Apply the iteration
			Apply(iIter,C,R);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepJuliaPhoenixFractal::CRenderStepJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepJuliaPhoenixFractal::~CRenderStepJuliaPhoenixFractal()
{
}

void CRenderStepJuliaPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	double dStep = m_FractalParm.GetStep();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(iIter,Step(C,dR),Step(R,dR));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderADJuliaPhoenixFractal::CRenderADJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderADJuliaPhoenixFractal::~CRenderADJuliaPhoenixFractal()
{
}

void CRenderADJuliaPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Calculate angular decomposition color
			if ((bBaseColor && iIter == nMaxCalc) || (bOrbitColor && iIter != nMaxCalc))
				iIter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(iIter,C,R);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepADJuliaPhoenixFractal::CRenderStepADJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepADJuliaPhoenixFractal::~CRenderStepADJuliaPhoenixFractal()
{
}

void CRenderStepADJuliaPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();
	double XMax = m_FractalParm.GetXMax();
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double YMin = m_FractalParm.GetYMin();
	double deltaX = (XMax - XMin) / (double)MC;
	double deltaY = (YMax - YMin) / (double)MR;
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1; C <= MC; ++C)
	{
		for (int R = RowBeg; R <= RowEnd; ++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc, C, R, XMin, YMax, Q, P, deltaX, deltaY);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && iIter == nMaxCalc) || (bOrbitColor && iIter != nMaxCalc))
				iIter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(iIter,Step(C,dR),Step(R,dR));
		}
	}
}
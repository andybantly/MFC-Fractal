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
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

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
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

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
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

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
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

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
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

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
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

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
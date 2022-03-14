// Copyright (C) 2012-2016 Andrew S. Bantly
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaY);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaY);

			// Calculate angular decomposition color
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(Iter,C,R);
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaY);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(dRadius,dRadScale);

			// Apply the the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

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

CRenderStepJuliaDragonFractal::CRenderStepJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CJuliaDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepJuliaDragonFractal::~CRenderStepJuliaDragonFractal()
{
}

void CRenderStepJuliaDragonFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaY);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaY);

			// Calculate angular decomposition color
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(Iter,C,R);
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaP();
	double deltaY = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaY);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaQ();
	double deltaXi = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int iIter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaXi);

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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaQ();
	double deltaXi = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaXi);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaQ();
	double deltaXi = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaXi);

			// Calculate angular decomposition color
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(Iter,C,R);
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
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double XMin = m_FractalParm.GetXMin();
	double YMax = m_FractalParm.GetYMax();
	double deltaX = m_FractalParm.GetDeltaQ();
	double deltaXi = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetDP();
	double Q = m_FractalParm.GetDQ();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / (2.0 * atan(1.0) * 4.0);

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit
			int Iter = Calc(nMaxCalc,C,R,XMin,YMax,Q,P,deltaX,deltaXi);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}
	}
}
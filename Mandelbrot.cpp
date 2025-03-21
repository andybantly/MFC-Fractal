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
#include "Mandelbrot.h"

/////////////////////////////////////////////////////////////////////////////

CRenderMandelbrotFractal::CRenderMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrot(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderMandelbrotFractal::~CRenderMandelbrotFractal()
{
}

void CRenderMandelbrotFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Apply the iteration
			Apply(Calc(nMaxCalc,P,Q[R]),C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepMandelbrotFractal::CRenderStepMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrot(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepMandelbrotFractal::~CRenderStepMandelbrotFractal()
{
}

void CRenderStepMandelbrotFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Apply the ridge/stair effect
			double dR = Radius() * dStep;

			// Apply the iteration
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderADMandelbrotFractal::CRenderADMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrot(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderADMandelbrotFractal::~CRenderADMandelbrotFractal()
{
}

void CRenderADMandelbrotFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(Iter,C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepADMandelbrotFractal::CRenderStepADMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrot(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepADMandelbrotFractal::~CRenderStepADMandelbrotFractal()
{
}

void CRenderStepADMandelbrotFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderMandelbrotDragonFractal::CRenderMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderMandelbrotDragonFractal::~CRenderMandelbrotDragonFractal()
{
}

void CRenderMandelbrotDragonFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Apply the iteration
			Apply(Calc(nMaxCalc,P,Q[R]),C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepMandelbrotDragonFractal::CRenderStepMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepMandelbrotDragonFractal::~CRenderStepMandelbrotDragonFractal()
{
}

void CRenderStepMandelbrotDragonFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderADMandelbrotDragonFractal::CRenderADMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderADMandelbrotDragonFractal::~CRenderADMandelbrotDragonFractal()
{
}

void CRenderADMandelbrotDragonFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)nMaxCalc / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(Iter,C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepADMandelbrotDragonFractal::CRenderStepADMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotDragon(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepADMandelbrotDragonFractal::~CRenderStepADMandelbrotDragonFractal()
{
}

void CRenderStepADMandelbrotDragonFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)nMaxCalc / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderMandelbrotPhoenixFractal::CRenderMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderMandelbrotPhoenixFractal::~CRenderMandelbrotPhoenixFractal()
{
}

void CRenderMandelbrotPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Apply the the escape orbit count iteration
			Apply(Calc(nMaxCalc,P,Q[R]),C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepMandelbrotPhoenixFractal::CRenderStepMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepMandelbrotPhoenixFractal::~CRenderStepMandelbrotPhoenixFractal()
{
}

void CRenderStepMandelbrotPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderADMandelbrotPhoenixFractal::CRenderADMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderADMandelbrotPhoenixFractal::~CRenderADMandelbrotPhoenixFractal()
{
}

void CRenderADMandelbrotPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(Iter,C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepADMandelbrotPhoenixFractal::CRenderStepADMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepADMandelbrotPhoenixFractal::~CRenderStepADMandelbrotPhoenixFractal()
{
}

void CRenderStepADMandelbrotPhoenixFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int CX = 1,C = 1;CX <= MC;++CX)
	{
		for (int RY = RowBeg,R = RowBeg;RY <= RowEnd;++RY,R = RY,C = CX)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderMandelbrotPhoenix2Fractal::CRenderMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix2(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderMandelbrotPhoenix2Fractal::~CRenderMandelbrotPhoenix2Fractal()
{
}

void CRenderMandelbrotPhoenix2Fractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Apply the escape orbit count iteration
			Apply(Calc(nMaxCalc,P,Q[R]),C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepMandelbrotPhoenix2Fractal::CRenderStepMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix2(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepMandelbrotPhoenix2Fractal::~CRenderStepMandelbrotPhoenix2Fractal()
{
}

void CRenderStepMandelbrotPhoenix2Fractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Apply the ridge/stair effect iteration
			double dR = Radius() * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderADMandelbrotPhoenix2Fractal::CRenderADMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix2(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderADMandelbrotPhoenix2Fractal::~CRenderADMandelbrotPhoenix2Fractal()
{
}

void CRenderADMandelbrotPhoenix2Fractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(Radius(),dRadScale);

			// Apply the iteration
			Apply(Iter,C,R);
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderStepADMandelbrotPhoenix2Fractal::CRenderStepADMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CMandelBrotPhoenix2(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
}

CRenderStepADMandelbrotPhoenix2Fractal::~CRenderStepADMandelbrotPhoenix2Fractal()
{
}

void CRenderStepADMandelbrotPhoenix2Fractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = RowEnd - RowBeg + 1;
	BOOL bBaseColor = m_FractalParm.GetBaseColor();
	BOOL bOrbitColor = m_FractalParm.GetOrbitColor();
	double dStep = m_FractalParm.GetStep();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dRadScale = (double)(nMaxCalc) / g_d2Pi;

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the escape orbit count
			int Iter = Calc(nMaxCalc,P,Q[R]);

			// Calculate angular decomposition color
			double dRadius = Radius();
			if ((bBaseColor && Iter == nMaxCalc) || (bOrbitColor && Iter != nMaxCalc))
				Iter = Calc2Rad(dRadius,dRadScale);

			// Apply the ridge/stair effect iteration
			double dR = dRadius * dStep;
			Apply(Iter,Step(C,dR),Step(R,dR));
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderBuddhaBrotBaseFractal::CRenderBuddhaBrotBaseFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CBuddhaBrot(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
	m_bModulo = true;
}

CRenderBuddhaBrotBaseFractal::~CRenderBuddhaBrotBaseFractal()
{
}

void CRenderBuddhaBrotBaseFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double deltaQ = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dXMin = m_FractalParm.GetXMin();
	double dYMin = m_FractalParm.GetYMin();
	std::vector<LONG> & vecEnergy = *(m_FractalParm.GetEnergy());
	LONG * pEnergy = &vecEnergy[0];

	// Allocate the storage for tracking the path and occurrences at the point in the path
	m_vecXPoint.resize(nMaxCalc);
	m_vecYPoint.resize(nMaxCalc);
	double * pXPoint = &m_vecXPoint[0];
	double * pYPoint = &m_vecYPoint[0];

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the iteration
			int Iter = Calc(nMaxCalc,P,Q[R]);
			if (Iter == nMaxCalc)
			{
				for (int iPt = 0;iPt < Iter;++iPt)
				{
					int C1 = (int)((pXPoint[iPt] - dXMin) / deltaP);
					if (C1 >= 0 && C1 < MC)
					{
						int R1 = (int)((pYPoint[iPt] - dYMin) / deltaQ);
						if (R1 >= 0 && R1 < MR)
							InterlockedIncrement(&pEnergy[R1 * MC + C1]);
					}
				}
			}
		}

		// Update the P value
		P += deltaP;
	}
}

/////////////////////////////////////////////////////////////////////////////

CRenderBuddhaBrotOrbitFractal::CRenderBuddhaBrotOrbitFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CBuddhaBrot(FractalParm), CFractalCanvas(FractalParm,DisplayDIB)
{
	m_bModulo = true;
}

CRenderBuddhaBrotOrbitFractal::~CRenderBuddhaBrotOrbitFractal()
{
}

void CRenderBuddhaBrotOrbitFractal::operator() (int RowBeg,int RowEnd)
{
	// The range of this portion of the Fractal
	int MC = m_FractalParm.GetMC();
	int MR = m_FractalParm.GetMR();

	// Local copies of member variables
	double * Q = m_FractalParm.GetQ();
	double deltaP = m_FractalParm.GetDeltaP();
	double deltaQ = m_FractalParm.GetDeltaQ();
	double P = m_FractalParm.GetXMin();
	int nMaxIter = m_FractalParm.GetIterations();
	int nMaxCalc = m_FractalParm.GetMaxCalc();
	double dXMin = m_FractalParm.GetXMin();
	double dYMin = m_FractalParm.GetYMin();
	std::vector<LONG> & vecEnergy = *(m_FractalParm.GetEnergy());
	LONG * pEnergy = &vecEnergy[0];

	// Allocate the storage for tracking the path and occurrences at the point in the path
	m_vecXPoint.resize(nMaxCalc);
	m_vecYPoint.resize(nMaxCalc);
	double * pXPoint = &m_vecXPoint[0];
	double * pYPoint = &m_vecYPoint[0];

	// Calculate the set values for each pixel across the columns and down the rows
	for (int C = 1;C <= MC;++C)
	{
		for (int R = RowBeg;R <= RowEnd;++R)
		{
			// Calculate the iteration
			int Iter = Calc(nMaxCalc,P,Q[R]);
			for (int iPt = 0;iPt < Iter;++iPt)
			{
				int C1 = (int)((pXPoint[iPt] - dXMin) / deltaP);
				if (C1 >= 0 && C1 < MC)
				{
					int R1 = (int)((pYPoint[iPt] - dYMin) / deltaQ);
					if (R1 >= 0 && R1 < MR)
						InterlockedIncrement(&pEnergy[R1 * MC + C1]);
				}
			}
		}

		// Update the P value
		P += deltaP;
	}
}
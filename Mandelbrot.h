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

#include "FractalBase.h"

class CMandelBrot : public CCalcBase
{
public:
	CMandelBrot(CFractalParm FractalParm) : CCalcBase(FractalParm) {};
	~CMandelBrot(){};

protected:
	int Calc(int nMaxCalc,double dP,double dQ)
	{
		double X = 0.0;
		double Y = 0.0;
		double XSQ = 0.0;
		double YSQ = 0.0;
		int nCalc = 0,nTrapCalc = 0;

		while ((nCalc < nMaxCalc) && Test(XSQ,YSQ))
		{
			XSQ = X * X;
			YSQ = Y * Y;
			Y = 2 * X * Y + dQ;
			X = XSQ - YSQ + dP;

			if (nTrapCalc = TrapCalc(X,Y))
			{
				nCalc = nTrapCalc;
				break;
			}
			nCalc++;
		}
		m_X = X;
		m_Y = Y;
		m_XSQ = XSQ;
		m_YSQ = YSQ;
		return nCalc;
	}
};

class CMandelBrotDragon : public CCalcBase
{
public:
	CMandelBrotDragon(CFractalParm FractalParm) : CCalcBase(FractalParm){};
	~CMandelBrotDragon(){};

protected:
	int Calc(int nMaxCalc,double dP,double dQ)
	{
		double X = 0.5;
		double Y = 0.0;
		double XSQ = 0.0;
		double YSQ = 0.0;
		double XTemp,YTemp;
		int nCalc = 0,nTrapCalc = 0;
		while ((nCalc < nMaxCalc) && Test(XSQ,YSQ))
		{
			XSQ = X * X;
			YSQ = Y * Y;
			XTemp = (Y - X) * (Y + X) + X;
			YTemp = X * Y;
			YTemp = YTemp + YTemp - Y;
			X = dP * XTemp + dQ * YTemp;
			Y = dQ * XTemp - dP * YTemp;

			if (nTrapCalc = TrapCalc(X,Y))
			{
				nCalc = nTrapCalc;
				break;
			}
			nCalc++;
		}
		m_X = X;
		m_Y = Y;
		m_XSQ = XSQ;
		m_YSQ = YSQ;
		return nCalc;
	}
};

class CMandelBrotPhoenix : public CCalcBase
{
public:
	CMandelBrotPhoenix(CFractalParm FractalParm) : CCalcBase(FractalParm){};
	~CMandelBrotPhoenix(){};

protected:
	int Calc(int nMaxCalc,double dP,double dQ)
	{
		double X = -0.5 * dQ;
		double Xi = 0.0;
		double Y = 0.0;
		double Yi = 0.0;
		double XSQ = 0.0;
		double XiSQ = 0.0;
		double XTemp,XiTemp;
		int nCalc = 0,nTrapCalc = 0;
		while ((nCalc < nMaxCalc) && Test(XSQ,XiSQ))
		{
			XSQ = X * X;
			XiSQ = Xi * Xi;
			XTemp = XSQ - XiSQ + dP + dQ * Y;
			XiTemp = 2 * X * Xi + dQ * Yi;
			Y = X;
			Yi = Xi;
			X = XTemp;
			Xi = XiTemp;

			if (nTrapCalc = TrapCalc(X,Y))
			{
				nCalc = nTrapCalc;
				break;
			}
			nCalc++;
		}
		m_X = X;
		m_Y = Y;
		m_XSQ = XSQ;
		m_YSQ = XiSQ;
		return nCalc;
	}
};

class CMandelBrotPhoenix2 : public CCalcBase
{
public:
	CMandelBrotPhoenix2(CFractalParm FractalParm) : CCalcBase(FractalParm){};
	~CMandelBrotPhoenix2(){};

protected:
	int Calc(int nMaxCalc,double dP,double dQ)
	{
		double X = 0.0;
		double Xi = 0.0;
		double Y = 0.0;
		double Yi = 0.0;
		double XSQ = 0.0;
		double XiSQ = 0.0;
		double XT,XiT;
		int nCalc = 0,nTrapCalc = 0;
		while ((nCalc < nMaxCalc) && Test(XSQ,XiSQ))
		{
			XSQ = X * X;
			XiSQ = Xi * Xi;
			XT = XSQ - XiSQ + dP + dQ * Y;
			XiT = 2.0 * X * Xi + dQ * Y;
			Y = X;
			Yi = Xi;
			X = XT;
			Xi = XiT;

			if (nTrapCalc = TrapCalc(X,Y))
			{
				nCalc = nTrapCalc;
				break;
			}
			nCalc++;
		}
		m_X = X;
		m_Y = Y;
		m_XSQ = XSQ;
		m_YSQ = XiSQ;
		return nCalc;
	}
};

class CBuddhaBrot : public CCalcBase
{
public:
	CBuddhaBrot(CFractalParm FractalParm) : CCalcBase(FractalParm){};
	~CBuddhaBrot(){};

protected:
	int Calc(int nMaxCalc,double dP,double dQ)
	{
		double X = 0.0;
		double Y = 0.0;
		double XSQ = 0.0;
		double YSQ = 0.0;
		int nCalc = 0,nTrapCalc = 0;
		double * pXPoint = &m_vecXPoint[0];
		double * pYPoint = &m_vecYPoint[0];

		while ((nCalc < nMaxCalc) && Test(XSQ,YSQ))
		{
			XSQ = X * X;
			YSQ = Y * Y;
			Y = 2 * X * Y + dQ;
			X = XSQ - YSQ + dP;

			if (nTrapCalc = TrapCalcX(X))
			{
				pYPoint[nCalc] = Y;
				pXPoint[nTrapCalc] = X;
				nCalc = nTrapCalc;
				break;
			}

			if (nTrapCalc = TrapCalcY(Y))
			{
				pXPoint[nCalc] = X;
				pYPoint[nTrapCalc] = Y;
				nCalc = nTrapCalc;
				break;
			}

			// Track the points
			pXPoint[nCalc] = X;
			pYPoint[nCalc] = Y;

			nCalc++;
		}
		m_X = X;
		m_Y = Y;
		m_XSQ = XSQ;
		m_YSQ = YSQ;
		return nCalc;
	}
};

// Class for rendering the mandelbrot fractal without angular decomposition (main case)
class CRenderMandelbrotFractal : public CFractalCanvas, public CMandelBrot
{
public:
	CRenderMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderMandelbrotFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot fractal with angular decomposition
class CRenderStepMandelbrotFractal : public CFractalCanvas, public CMandelBrot
{
public:
	CRenderStepMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepMandelbrotFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot fractal with angular decomposition
class CRenderADMandelbrotFractal : public CFractalCanvas, public CMandelBrot
{
public:
	CRenderADMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderADMandelbrotFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot fractal with angular decomposition
class CRenderStepADMandelbrotFractal : public CFractalCanvas, public CMandelBrot
{
public:
	CRenderStepADMandelbrotFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepADMandelbrotFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot dragon fractal without angular decomposition (main case)
class CRenderMandelbrotDragonFractal : public CFractalCanvas, public CMandelBrotDragon
{
public:
	CRenderMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderMandelbrotDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot dragon fractal without angular decomposition (main case)
class CRenderStepMandelbrotDragonFractal : public CFractalCanvas, public CMandelBrotDragon
{
public:
	CRenderStepMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepMandelbrotDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot dragon fractal with angular decomposition
class CRenderADMandelbrotDragonFractal : public CFractalCanvas, public CMandelBrotDragon
{
public:
	CRenderADMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderADMandelbrotDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot dragon fractal with angular decomposition
class CRenderStepADMandelbrotDragonFractal : public CFractalCanvas, public CMandelBrotDragon
{
public:
	CRenderStepADMandelbrotDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepADMandelbrotDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal without angular decomposition (main case)
class CRenderMandelbrotPhoenixFractal : public CFractalCanvas, public CMandelBrotPhoenix
{
public:
	CRenderMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderMandelbrotPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal without angular decomposition (main case)
class CRenderStepMandelbrotPhoenixFractal : public CFractalCanvas, public CMandelBrotPhoenix
{
public:
	CRenderStepMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepMandelbrotPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal with angular decomposition
class CRenderADMandelbrotPhoenixFractal : public CFractalCanvas, public CMandelBrotPhoenix
{
public:
	CRenderADMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderADMandelbrotPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal with angular decomposition
class CRenderStepADMandelbrotPhoenixFractal : public CFractalCanvas, public CMandelBrotPhoenix
{
public:
	CRenderStepADMandelbrotPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepADMandelbrotPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal without angular decomposition (main case)
class CRenderMandelbrotPhoenix2Fractal : public CFractalCanvas, public CMandelBrotPhoenix2
{
public:
	CRenderMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderMandelbrotPhoenix2Fractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal without angular decomposition (main case)
class CRenderStepMandelbrotPhoenix2Fractal : public CFractalCanvas, public CMandelBrotPhoenix2
{
public:
	CRenderStepMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepMandelbrotPhoenix2Fractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal with angular decomposition
class CRenderADMandelbrotPhoenix2Fractal : public CFractalCanvas, public CMandelBrotPhoenix2
{
public:
	CRenderADMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderADMandelbrotPhoenix2Fractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot phoenix fractal with angular decomposition
class CRenderStepADMandelbrotPhoenix2Fractal : public CFractalCanvas, public CMandelBrotPhoenix2
{
public:
	CRenderStepADMandelbrotPhoenix2Fractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepADMandelbrotPhoenix2Fractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot fractal without angular decomposition (main case)
class CRenderBuddhaBrotBaseFractal : public CFractalCanvas, public CBuddhaBrot
{
public:
	CRenderBuddhaBrotBaseFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderBuddhaBrotBaseFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the mandelbrot fractal without angular decomposition (main case)
class CRenderBuddhaBrotOrbitFractal : public CFractalCanvas, public CBuddhaBrot
{
public:
	CRenderBuddhaBrotOrbitFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderBuddhaBrotOrbitFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};
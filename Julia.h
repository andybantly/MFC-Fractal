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
#pragma once

#include "FractalBase.h"

class CJulia : public CCalcBase
{
public:
	CJulia(CFractalParm FractalParm) : CCalcBase(FractalParm) {};
	~CJulia(){};

protected:
	int Calc(int nMaxCalc,int C,int R,double XMin,double YMax,double dQ,double dP,double deltaX,double deltaY)
	{
		double X = XMin + ((double)C * deltaX);
		double Y = YMax - ((double)R * deltaY);
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

class CJuliaDragon : public CCalcBase
{
public:
	CJuliaDragon(CFractalParm FractalParm) : CCalcBase(FractalParm){};
	~CJuliaDragon(){};

protected:
	int Calc(int nMaxCalc,int C,int R,double XMin,double YMax,double dQ,double dP,double deltaX,double deltaY)
	{
		double X = XMin + ((double)C * deltaX);
		double Y = YMax - ((double)R * deltaY);
		double XSQ = 0.0;
		double YSQ = 0.0;
		double TSQ,TXY,YT;
		int nCalc = 0,nTrapCalc = 0;
		while ((nCalc < nMaxCalc) && Test(XSQ,YSQ))
		{
			XSQ = X * X;
			YSQ = Y * Y;
			TSQ = YSQ - XSQ;
			TXY = X * Y;
			TXY += TXY;
			YT = dQ * (TSQ + X) - dP * (TXY - Y);
			X = dP * (TSQ + X) + dQ * (TXY - Y);
			Y = YT;
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

class CJuliaPhoenix : public CCalcBase
{
public:
	CJuliaPhoenix(CFractalParm FractalParm) : CCalcBase(FractalParm){};
	~CJuliaPhoenix(){};

protected:
	int Calc(int nMaxCalc,int C,int R,double XMin,double YMax,double dQ,double dP,double deltaX,double deltaXi)
	{
		double X = YMax - ((double)R * deltaX);
		double Xi = XMin + ((double)C * deltaXi);
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

// Class for rendering the julia fractal without angular decomposition (main case)
class CRenderJuliaFractal : public CFractalCanvas, public CJulia
{
public:
	CRenderJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderJuliaFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia fractal without angular decomposition (main case)
class CRenderStepJuliaFractal : public CFractalCanvas, public CJulia
{
public:
	CRenderStepJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepJuliaFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the fractal with angular decomposition
class CRenderADJuliaFractal : public CFractalCanvas, public CJulia
{
public:
	CRenderADJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderADJuliaFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the fractal with angular decomposition
class CRenderStepADJuliaFractal : public CFractalCanvas, public CJulia
{
public:
	CRenderStepADJuliaFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepADJuliaFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia dragon fractal without angular decomposition (main case)
class CRenderJuliaDragonFractal : public CFractalCanvas, public CJuliaDragon
{
public:
	CRenderJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderJuliaDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia dragon fractal without angular decomposition (main case)
class CRenderStepJuliaDragonFractal : public CFractalCanvas, public CJuliaDragon
{
public:
	CRenderStepJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepJuliaDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia dragon fractal with angular decomposition
class CRenderADJuliaDragonFractal : public CFractalCanvas, public CJuliaDragon
{
public:
	CRenderADJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderADJuliaDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia dragon fractal with angular decomposition
class CRenderStepADJuliaDragonFractal : public CFractalCanvas, public CJuliaDragon
{
public:
	CRenderStepADJuliaDragonFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepADJuliaDragonFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia phoenix fractal without angular decomposition (main case)
class CRenderJuliaPhoenixFractal : public CFractalCanvas, public CJuliaPhoenix
{
public:
	CRenderJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderJuliaPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia phoenix fractal without angular decomposition (main case)
class CRenderStepJuliaPhoenixFractal : public CFractalCanvas, public CJuliaPhoenix
{
public:
	CRenderStepJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepJuliaPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia phoenix fractal with angular decomposition
class CRenderADJuliaPhoenixFractal : public CFractalCanvas, public CJuliaPhoenix
{
public:
	CRenderADJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderADJuliaPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

// Class for rendering the julia phoenix fractal with angular decomposition
class CRenderStepADJuliaPhoenixFractal : public CFractalCanvas, public CJuliaPhoenix
{
public:
	CRenderStepADJuliaPhoenixFractal(CFractalParm FractalParm,CDIBFrame & DisplayDIB);
	virtual ~CRenderStepADJuliaPhoenixFractal();
	virtual void operator() (int RowBeg,int RowEnd);
};

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

#include "BitmapIcon.h"
#include "cpu.h"
#include "resource.h"

class CColorOp
{
public:
	CColorOp(CDIBFrame & DisplayDIB,CDIBFrame & BaseDIB,CDIBFrame & FractalDIB,UINT_PTR nIDEvent,int iColor = 0,BOOL bBlendBaseDIB = FALSE,BOOL bBlendFracDIB = FALSE,int iAlpha = 0,int iAlpha2 = 0) : m_DisplayDIB(DisplayDIB), m_BaseDIB(BaseDIB), m_FractalDIB(FractalDIB), m_nIDEvent(nIDEvent), m_iColor(iColor), m_bBlendBaseDIB(bBlendBaseDIB), m_bBlendFracDIB(bBlendFracDIB), m_iAlpha(iAlpha), m_iAlpha2(iAlpha2) {}
	~CColorOp(){};
	void operator() (int iRowBeg,int iRowEnd);

public:
	UINT_PTR m_nIDEvent;
	CDIBFrame & m_DisplayDIB;
	CDIBFrame & m_BaseDIB;
	CDIBFrame & m_FractalDIB;
	int m_iRndBeg,m_iRndEnd;
	int m_iColor;
	BOOL m_bBlendBaseDIB,m_bBlendFracDIB;
	int m_iAlpha,m_iAlpha2;

private:
	CColorOp(CColorOp & ColorOp);
	CColorOp & operator = (CColorOp & ColorOp);
};

// Driver thread that carries out the function
class CDriveMultiThreadedColor : public CWinThread
{
	DECLARE_DYNCREATE(CDriveMultiThreadedColor)

private:
	CDriveMultiThreadedColor() : m_phHandle(NULL) {};

public:
	CDriveMultiThreadedColor(HANDLE * phHandle,CColorOp * pColorOp);
	virtual ~CDriveMultiThreadedColor();
	virtual BOOL InitInstance();
	virtual BOOL PumpMessage();
	virtual int ExitInstance();

protected:
	HANDLE m_hPump;
	bool m_bPumpMessage;
	HANDLE * m_phHandle;
	CColorOp * m_pColorOp;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnDoWork(WPARAM wParam,LPARAM lParam);
	afx_msg void OnEndThread(WPARAM wParam,LPARAM lParam);
};

class CChangeColor
{
public:
	CChangeColor(CDIBFrame & DisplayDIB,CDIBFrame & BaseDIB,CDIBFrame & FractalDIB,UINT_PTR nIDEvent,int iColor = 0,BOOL bBlendBaseDIB = FALSE,BOOL bBlendFracDIB = FALSE,int iAlpha = 0,int iAlpha2 = 0);
	~CChangeColor();
	void ChangeColor();
	UINT_PTR GetEvent() {return m_nIDEvent;}

private:
	CChangeColor(CChangeColor & ChangeColor);
	CChangeColor & operator = (CChangeColor & ChangeColor);

private:
	CDIBFrame & m_DisplayDIB;
	CDIBFrame & m_BaseDIB;
	CDIBFrame & m_FractalDIB;
	BOOL m_bBlendBaseDIB,m_bBlendFracDIB;
	int m_iAlpha,m_iAlpha2;
	CDriveMultiThreadedColor ** m_ppDriveMultiThreadedColor;
	HANDLE m_arrHandle[MAXTHREADS];
	CPU m_CPU;
	int m_nTotalThreads;
	UINT_PTR m_nIDEvent;
};
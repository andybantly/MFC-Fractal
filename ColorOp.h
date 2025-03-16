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
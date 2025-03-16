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
   - Under no circumstances shall[Your Name or Organization] be liable for any damages, including incidental, special, or consequential damages arising out of or in connection with the use of this software.

4. Violation of Terms :
   - Any unauthorized use, modification, or distribution of this software will result in immediate revocation of any implied permissions and may lead to legal action.

5. Jurisdiction :
   - This license shall be governed by the laws of The United States of America.
*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _ATL_APARTMENT_THREADED 
#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <vector>
#include <memory>
#include <functional>

#include <float.h>
#include <math.h>

#include <limits>

#ifdef _DEBUG
#define MAXTHREADS 4
#else
#define MAXTHREADS 64
#endif
#define WM_DOWORK (WM_APP + 100)
#define WM_ENDTHREAD (WM_APP + 101)

void DebugMsg(const TCHAR * pwszFormat,...);
void DebugLastError();

// Debugging timer used for detecting the duration of an operation
class CDuration
{
   const TCHAR * p;
   clock_t start;
   clock_t finish;
public:
   CDuration(const TCHAR * sz) : p(sz),start(clock()),finish(start) {};
   ~CDuration()
   {
      finish = clock();
      double duration = (double)(finish - start) / CLOCKS_PER_SEC;
      DebugMsg(_T("%s Duration = %f seconds\n"),p,duration);
   }
};

// Blocking mechanism
class CBlock
{
public:
	CBlock(CComAutoCriticalSection & ref) : m_ref(ref) {ref.Lock();}
	~CBlock() {m_ref.Unlock();}
private:
	CComAutoCriticalSection & m_ref;
	CBlock(); // no
	CBlock(const CBlock & rhs); // no
	CBlock& operator=(const CBlock & rhs); // no
};

// Helper class to generate a random number in the range from [RangeMin,RangeMax]
class CRandom
{
public:
	CRandom(bool bQuick = true) : m_bQuick(false)
	{
		SetRangeMin(DBL_MIN);
		SetRangeMax(DBL_MAX);
		SeedQuick(bQuick);
	}
	CRandom(double dRangeMin,double dRangeMax,bool bQuick = true) : m_bQuick(false)
	{
		SetRangeMin(dRangeMin);
		SetRangeMax(dRangeMax);
		SeedQuick(bQuick);
	}

	double NextRandomRange() {return RandomRange(m_dRangeMin,m_dRangeMax);}
	double NextRandom() {return Next();}

	void SetRangeMin(double dRangeMin) {m_dRangeMin = dRangeMin;}
	void SetRangeMax(double dRangeMax) {m_dRangeMax = dRangeMax;}

private:

	void SeedQuick(bool bQuick)
	{
		if (bQuick)
		{
			m_dQuickMax = (double)(DWORD)(-1);
			m_dwQuickSeed = (DWORD)(Next() * m_dQuickMax);
			m_bQuick = bQuick;
		}
		else
		{
			srand((unsigned)time(NULL));
		}
	}

	double RandomRange(double dRangeMin,double dRangeMax)
	{
		return floor((dRangeMin + (Next() * ((dRangeMax - dRangeMin) + 1.0))));
	}

	double Next()
	{
		double dRandom;
		if (m_bQuick)
		{
			m_dwQuickSeed = 1664525L * m_dwQuickSeed + 1013904223L;
			dRandom = (double)m_dwQuickSeed / m_dQuickMax;
		}
		else
			dRandom = (double)rand() / RAND_MAX;
		return dRandom;
	}

private:
	double m_dRangeMin,m_dRangeMax;
	bool m_bQuick;
	DWORD m_dwQuickSeed;
	double m_dQuickMax;
};

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

// Gamma correction function (default value causes no gamma correction)
BYTE GammaCorrect(BYTE B,double dExp = 1.0);

// RGB functions that use gamma correction (default values cause no gamma correction)
COLORREF GRGB(BYTE R,BYTE G,BYTE B,double dRExp = 1.0,double dGExp = 1.0,double dBExp = 1.0);
COLORREF GRGB(COLORREF RGB,double dRExp = 1.0,double dGExp = 1.0,double dBExp = 1.0);
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

#include "stdafx.h"
#include "resource.h"

CComAutoCriticalSection g_MTBlock;

void DebugMsg(const TCHAR * pwszFormat,...)
{
	va_list arglist;
	va_start(arglist, pwszFormat);
	int nBufSize = _vsctprintf(pwszFormat,arglist) + 1;
	if (nBufSize)
	{
		std::vector<TCHAR> Buffer(nBufSize);
		TCHAR * pBuffer = &Buffer[0];
		_vstprintf_s(pBuffer,nBufSize,pwszFormat,arglist);
		va_end(arglist);
		OutputDebugString(pBuffer);
	}
}

// Breaks down "GetLastError"
void DebugLastError()
{
	LPVOID lpMsgBuf;
	DWORD dwLastError = GetLastError(); 
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,dwLastError,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&lpMsgBuf,0,NULL);
	DebugMsg(L"%s\n",lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// Gamma correcting function
BYTE GammaCorrect(BYTE B,double dExp)
{
	double d = 1.0;
	if (1.0 != dExp && dExp > 0.0 && dExp < 5.0)
		return (BYTE)(pow(((double)B / 255.0),dExp) * 255.0);
	return B;
}

// An RGB function with gamma correction
COLORREF GRGB(BYTE R,BYTE G,BYTE B,double dRExp,double dGExp,double dBExp)
{
	R = GammaCorrect(R,dRExp);
	G = GammaCorrect(G,dGExp);
	B = GammaCorrect(B,dBExp);
	return RGB(R,G,B);
}

// An RGB function with gamma correction
COLORREF GRGB(COLORREF RGB,double dRExp,double dGExp,double dBExp)
{
	return GRGB(GetRValue(RGB),GetGValue(RGB),GetBValue(RGB),dRExp,dGExp,dBExp);
}

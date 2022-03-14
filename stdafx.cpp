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

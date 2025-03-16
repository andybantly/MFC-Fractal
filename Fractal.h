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

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "NewFractal.h"
#include "resource.h"       // main symbols

// CFractalApp:
// See Fractal.cpp for the implementation of this class
//

class CFractalApp : public CWinAppEx
{
public:
	CFractalApp();

public:
	CNewFractal m_Mandelbrot;
	CNewFractal m_Julia;
	CNewFractal m_MandelbrotDragon;
	CNewFractal m_MandelbrotPhoenix;
	CNewFractal m_JuliaDragon;
	CNewFractal m_JuliaPhoenix;
	CNewFractal m_MandelbrotPhoenix2;
	CNewFractal m_Buddhabrot;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	COleTemplateServer m_server;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);

public:
	void SetZoomNew(int iFractalType);
	BOOL GetZoomNew(int & iFractalType);
	void SetLastChoice(int iLastChoice) {m_iLastChoice = iLastChoice;}
	int GetLastChoice() {return m_iLastChoice;}
	CNewFractal & GetFractalDialog(int iFractalType);
	CString GetFractalDialogTitle(int iFractalType);
	BOOL IsJuliaFractal(int iFractalType);
	int GetMandelJulia(int iFractalType);
	BOOL IsAutomation();
	BOOL ChooseImage(LPCTSTR lpszTitle,CString & csNewImage);
	INT_PTR ChangeColor(COLORREF & crRGB,CImageButton & BtnRGB);
	void CreatePointFont(CFont & Font, CDC * pDC, LONG lfWeight, BYTE lfItalic, BYTE lfCharSet, BYTE lfQuality, CString csFontName, int iPtSize);

protected:
	BOOL m_bZoomNew;
	int m_iFractalType;
	int m_iLastChoice;
	BOOL m_bAutomation;
};

extern CFractalApp theApp;

class CFractalFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFractalFileDialog)

public:
	CFractalFileDialog(BOOL bOpenFileDialog = FALSE, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszTitle = NULL,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATE | OFN_ENABLESIZING,
		LPCTSTR lpszFilter = NULL,
		BOOL bSaveAs = FALSE,
		CWnd * pParentWnd = NULL);

public:
	BOOL GetEmbedded() {return m_bEmbedded;}

protected:
	BOOL m_bSaveAs;
	BOOL m_bEmbedded;

protected:
	virtual void DoDataExchange(CDataExchange * pDX);

protected:
	afx_msg void OnEmbedded();

	DECLARE_MESSAGE_MAP()
};

// Image file dialog
class CImageFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CImageFileDialog)
public:
	CImageFileDialog(BOOL bOpenFileDialog = FALSE, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszTitle = NULL,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATE | OFN_ENABLESIZING,
		LPCTSTR lpszFilter = NULL,
		CWnd * pParentWnd = NULL);
protected:
	virtual void DoDataExchange(CDataExchange * pDX);
	DECLARE_MESSAGE_MAP()
	CString m_csTitle;
};
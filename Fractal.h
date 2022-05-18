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
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

// TextEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Fractal.h"
#include "TextEdit.h"

// CTextEdit

IMPLEMENT_DYNAMIC(CTextEdit, CEdit)

CTextEdit::CTextEdit()
{
	// Set the default color information
	m_crTextColorRGB = RGB(255,255,255);
	m_crBackColorRGB = RGB(0,0,0);
	m_BackGround.CreateSolidBrush(m_crBackColorRGB);

	// Set the default font information
	m_iPtSize = 12 * 10;
	m_iBold = 0;
	m_bItalics = FALSE;
	m_csFontName = L"Calibri";

	// Create the default font
	TextEditFont();
	m_bSetFont = false;
}

// Change the font
void CTextEdit::UpdateFont(CString csFontName,int iPtSize,int iBold,BOOL bItalics)
{
	m_iPtSize = iPtSize * 10;
	m_iBold = iBold;
	m_bItalics = bItalics;
	m_csFontName = csFontName;

	// Create the default font
	TextEditFont();
	m_bSetFont = false;
}

void CTextEdit::SetTextColorRGB(COLORREF crTextColorRGB)
{
	m_crTextColorRGB = crTextColorRGB;
}

void CTextEdit::SetBackColorRGB(COLORREF crBackColorRGB)
{
	m_crBackColorRGB = crBackColorRGB;
	m_BackGround.DeleteObject();
	m_BackGround.CreateSolidBrush(m_crBackColorRGB);
}

CTextEdit::~CTextEdit()
{
}

// Create the font for use in the edit control
void CTextEdit::TextEditFont()
{
	// Create the font for the edit control
	CWnd * pWnd = CWnd::FromHandle(m_hWnd);
	if (!pWnd)
		return;
	CDC * pDC = pWnd->GetDC();
	theApp.CreatePointFont(m_Font, pDC, m_iBold, m_bItalics, DEFAULT_CHARSET, DEFAULT_QUALITY, m_csFontName, m_iPtSize);
}

CString CTextEdit::GetFontName()
{
	return m_csFontName;
}

int CTextEdit::GetPtSize()
{
	return m_iPtSize;
}

int CTextEdit::GetWeight()
{
	return m_iBold;
}

BOOL CTextEdit::GetItalics()
{
	return m_bItalics;
}

BEGIN_MESSAGE_MAP(CTextEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

// CTextEdit message handlers
HBRUSH CTextEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// Change any attributes of the DC here
	pDC->SetTextColor(m_crTextColorRGB);
	pDC->SetBkColor(m_crBackColorRGB);
	pDC->SetBkMode(OPAQUE);
	if (!m_bSetFont)
	{
		SetFont(&m_Font);
		m_bSetFont = true;
	}

	// Return a non-NULL brush if the parent's handler should not be called
	return m_BackGround;
}

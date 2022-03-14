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

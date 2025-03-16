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

#include "stdafx.h"
#include "ImageButton.h"
#include "comutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CImageButton, CButton)

CImageButton::CImageButton() : CButton(), m_bHasFocus(FALSE), m_bMouseOver(FALSE), m_bImage(FALSE), m_bRect(FALSE), m_bText(FALSE), m_iWidth(0), m_iHeight(0)
{
	// First detect uxtheme.dll
	m_hUxTheme = LoadLibrary(_T("uxtheme.dll"));
	if (m_hUxTheme != NULL)
	{
		OpenThemeData = (PFNOpenThemeData)GetProcAddress(m_hUxTheme,"OpenThemeData");
		DrawThemeBackground = (PFNDrawThemeBackground)GetProcAddress(m_hUxTheme,"DrawThemeBackground");
		DrawThemeText = (PFNDrawThemeText)GetProcAddress(m_hUxTheme,"DrawThemeText");
		CloseThemeData = (PFNCloseThemeData)GetProcAddress(m_hUxTheme,"CloseThemeData");
	}
}

CImageButton::~CImageButton()
{
	// Free the theme library
	if (m_hUxTheme)
	{
		FreeLibrary(m_hUxTheme);
		m_hUxTheme = NULL;
	}
}

void CImageButton::OnNMThemeChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	InvalidateRect(NULL);
	*pResult = 0;
}

// Create the button by subclassing it (button must be owner draw)
void CImageButton::CreateButton()
{
	// Intercept all the buttons control messages
	SubclassDlgItem(m_nButtonId,m_pWnd);
}

// Create the bitmap button
BOOL CImageButton::CreateBtmp(CWnd * pWnd,int nBitmapId,int nButtonId,const TCHAR * pszText,COLORREF crTextColor,eBitmapAlignment eAlignment)
{
	// Initialize
	m_crTextColor = (crTextColor == COLOR_BTNTEXT ? ::GetSysColor(COLOR_BTNTEXT) : crTextColor);
	m_eAlignment = eAlignment;

	// Initialize the Bitmap Icon
	m_bImage = TRUE;
	m_BitmapIcon.Load(nBitmapId);
	m_iWidth = m_BitmapIcon.GetWidth();
	m_iHeight = m_BitmapIcon.GetHeight();;

	// Create a new button with the proper style settings to replace the button on the form
	m_pWnd = pWnd;
	m_nButtonId = nButtonId;
	CreateButton();

	// Set the window text
	if (pszText)
		SetWindowText(pszText);

	return TRUE;
}

// Used for creating a button with an icon for the image
BOOL CImageButton::CreateIcon(CWnd * pWnd,int nIconId,int nButtonId,const TCHAR * pszText,COLORREF crTextColor,eBitmapAlignment eAlignment)
{
	// Initialize
	m_crTextColor = crTextColor;
	m_eAlignment = eAlignment;

	// Initialize the Bitmap Icon
	m_bImage = TRUE;
	m_BitmapIcon.LoadIcon(nIconId);
	m_iWidth = m_BitmapIcon.GetWidth();
	m_iHeight = m_BitmapIcon.GetHeight();;

	// Create a new button with the proper style settings to replace the button on the form
	m_pWnd = pWnd;
	m_nButtonId = nButtonId;
	CreateButton();

	// Set the window text
	if (pszText)
		SetWindowText(pszText);

	return TRUE;
}

// Used for creating a colored region (like for buttons that select color)
BOOL CImageButton::CreateRect(CWnd * pWnd,int nButtonId,COLORREF crFillColor,const TCHAR * pszText,eBitmapAlignment eAlignment,int iWidth,int iHeight)
{
	// Initialize
	m_crFillColor = crFillColor;
	m_crTextColor = COLOR_BTNTEXT;
	m_eAlignment = eAlignment;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_bRect = TRUE;

	// Create a new button with the proper style settings to replace the button on the form
	m_pWnd = pWnd;
	m_nButtonId = nButtonId;
	CreateButton();

	// Set the window text
	if (pszText)
		SetWindowText(pszText);

	return TRUE;
}

// Used for creating a button with text only
BOOL CImageButton::CreateText(CWnd * pWnd,int nButtonId,const TCHAR * pszText,COLORREF crTextColor)
{
	m_crTextColor = crTextColor;
	m_eAlignment = LEFT;
	m_iWidth = 0;
	m_iHeight = 0;
	m_bText = TRUE;

	// Create a new button with the proper style settings to replace the button on the form
	m_pWnd = pWnd;
	m_nButtonId = nButtonId;
	CreateButton();

	// Set the window text
	if (pszText)
		SetWindowText(pszText);

	return TRUE;
}

COLORREF CImageButton::SetFillColor(COLORREF crFillColor)
{
	COLORREF cr = m_crFillColor;
	m_crFillColor = crFillColor;
	RedrawWindow();
	return cr;
}

void CImageButton::UpdateBitmap(int nBitmapId)
{
	m_BitmapIcon.Load(nBitmapId);
}

void CImageButton::UpdateIcon(int nIconId)
{
	m_BitmapIcon.LoadIcon(nIconId);
}

BEGIN_MESSAGE_MAP(CImageButton, CButton)
	//{{AFX_MSG_MAP(CImageButton)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_MOUSELEAVE,&CImageButton::OnMouseLeave)
	ON_WM_NCHITTEST()
	ON_NOTIFY_REFLECT(NM_THEMECHANGED, &CImageButton::OnNMThemeChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageButton message handlers

void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// This code only works with buttons
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	// Attach to the buttons device context
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	// Test for visual styles
	HWND hButton = lpDrawItemStruct->hwndItem;
	HTHEME hTheme = NULL;
	if (m_hUxTheme != NULL)
		hTheme = OpenThemeData(hButton,L"Button");

	// Set the button class
	UINT uPart = DFC_BUTTON;
	if (hTheme)
		uPart = BP_PUSHBUTTON;

	// Build up the button style
	UINT uStyle = 0;
	if (!hTheme)
		uStyle = DFCS_BUTTONPUSH;

	// Test the mouse over flag for making the button hot
	if (hTheme && m_bMouseOver)
		uStyle |= PBS_HOT;

	// If drawing a selected button, add the pushed style to DrawFrameControl
	BOOL bPushed = lpDrawItemStruct->itemState & ODS_SELECTED;
	if (bPushed)
	{
		if (hTheme)
			uStyle |= PBS_PRESSED;
		else
			uStyle |= DFCS_PUSHED;
	}

	// If drawing a disabled button, add the disabled style to DrawFrameControl
	BOOL bDisabled = lpDrawItemStruct->itemState & ODS_DISABLED;
	if (bDisabled)
	{
		if (hTheme)
			uStyle |= PBS_DISABLED;
		else
			uStyle |= DFCS_INACTIVE;
	}

	// Draw the button frame
	if (hTheme == NULL)
		::DrawFrameControl(lpDrawItemStruct->hDC,&lpDrawItemStruct->rcItem,uPart,uStyle);
	else
		DrawThemeBackground(hTheme,lpDrawItemStruct->hDC,uPart,uStyle,&lpDrawItemStruct->rcItem,NULL);

	// Get the button's text.
	CString csButtonText;
	GetWindowText(csButtonText);
	_bstr_t btButtonText = csButtonText.AllocSysString();

	// Adjust the text rectange (down and right) if the button is depressed
	CRect ButtonRect = lpDrawItemStruct->rcItem;

	// Draw the transparent bitmap to the button
	int nOff = bPushed ? 1 : 0;

	// Rectangle X starting point
	int nRX = 10 + nOff;

	// Test for a button with text
	if (csButtonText.GetLength())
	{
		CSize Size;
		CString csTextExtent = csButtonText;
		csTextExtent.Remove('&');
		Size = dc.GetOutputTextExtent(csTextExtent);
		int nSizeX = Size.cx;
		int nTotHeight = Size.cy;

		int nTotWidth = nSizeX + m_iWidth + 4;
		int nX = ((ButtonRect.Width() - 6) >> 1) - (nTotWidth >> 1);

		int nYTop = (ButtonRect.Height() - 4) >> 1;
		int nY = nYTop - (nTotHeight >> 1) + 1;

		if (m_bText)
		{
			nX = nX + 1;
			nY = nY + 1;
		}

		// Get the top of the rectangle for blt'ing
		int nYBmp = nYTop - (m_iHeight >> 1) + 1;

		// Set the text color
		COLORREF crTextColor = (!bDisabled ? m_crTextColor : ::GetSysColor(COLOR_GRAYTEXT));
		COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, crTextColor);

		// Output the text using the custom rectangle area
		int nTxtStart = nX + nOff + (m_eAlignment == LEFT ? m_iWidth + 4 : 0);
		CRect TextRect = CRect(nTxtStart,nY + nOff,nTxtStart + nSizeX,nY + nTotHeight + nOff);

		if (hTheme == NULL)
			dc.DrawText(csButtonText,&TextRect,DT_SINGLELINE);
		else
			DrawThemeText(hTheme,lpDrawItemStruct->hDC,uPart,uStyle,btButtonText,btButtonText.length(),DT_SINGLELINE,0,&TextRect);
		::SetTextColor(lpDrawItemStruct->hDC, crOldColor);

		// Output the bitmap
		int nBmpStart = (m_eAlignment == LEFT ? nX + nOff : nTxtStart + nSizeX + 4);
		if (m_bImage)
		{
			UINT nFlags = DST_ICON;
			if (bDisabled)
				nFlags |= DSS_DISABLED;
			dc.DrawState(CPoint(nBmpStart,nYBmp + nOff + 1),CSize(m_iWidth,m_iHeight),m_BitmapIcon,nFlags,(HBRUSH)0);
		}
		else if (m_bRect)
		{
			CRect Rect(nRX,nYBmp + nOff + 1,nRX + m_iWidth,nYBmp + nOff + m_iHeight + 1);
			CBrush Brush;
			if (bDisabled)
				Brush.CreateSolidBrush(GetSysColor(COLOR_GRAYTEXT));
			else
				Brush.CreateSolidBrush(m_crFillColor);
			dc.FillRect(&Rect,&Brush);
		}
	}
	else
	{
		// Bitmap or Icon only, centered to the button
		int nX = (ButtonRect.Width() >> 1) - (m_iWidth >> 1);
		int nY = (ButtonRect.Height() >> 1) - (m_iHeight >> 1);
		if (m_bImage)
		{
			UINT nFlags = DST_ICON;
			if (bDisabled)
				nFlags |= DSS_DISABLED;
			dc.DrawState(CPoint(nX + nOff,nY + nOff),CSize(m_iWidth,m_iHeight),m_BitmapIcon,nFlags,(HBRUSH)0);
		}
		else
		{
			CRect Rect(nRX + nOff,nY + nOff,nRX + nOff + m_iWidth,nY + nOff + m_iHeight);
			CBrush Brush(m_crFillColor);
			dc.FillRect(&Rect,&Brush);
		}
	}

	// Draw the focus rectangle
	if (m_bHasFocus)
		dc.DrawFocusRect(CRect(ButtonRect.left + 3,ButtonRect.top + 3,ButtonRect.right - 3,ButtonRect.bottom - 3));

	// Close the buttons theme data
	if (hTheme)
		CloseThemeData(hTheme);

	dc.Detach();
}

void CImageButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	m_bHasFocus = FALSE;
	InvalidateRect(NULL);
	RedrawWindow();
}

void CImageButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	m_bHasFocus = TRUE;
	InvalidateRect(NULL);
	RedrawWindow();
}

LRESULT CImageButton::OnNcHitTest(CPoint point)
{
	if (!m_bMouseOver)
	{
		m_bMouseOver = TRUE;
		InvalidateRect(NULL);
		TRACKMOUSEEVENT Track;
		Track.cbSize = sizeof(TRACKMOUSEEVENT);
		Track.dwFlags = TME_LEAVE;
		Track.dwHoverTime = 0;
		Track.hwndTrack = GetSafeHwnd();
		TrackMouseEvent(&Track);
	}

	return CButton::OnNcHitTest(point);
}

LRESULT CImageButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	if (m_bMouseOver)
	{
		m_bMouseOver = FALSE;
		InvalidateRect(NULL);
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CGradient

IMPLEMENT_DYNAMIC(CGradient, CImageButton)

CGradient::CGradient() : CImageButton(), m_parrRGB(NULL), m_nRGB(0)
{
}

// Create the gradient control
BOOL CGradient::CreateGradient(CWnd * pWnd,int nButtonId,COLORREF arrRGB[],int nRGB,int R,int G,int B,BOOL bBase,COLORREF RGBBase)
{
	// Initialize the colors
	m_nRGB = nRGB;
	m_vRGB.resize(nRGB);
	m_parrRGB = &m_vRGB[0];
	memcpy(m_parrRGB,&arrRGB[0],sizeof(COLORREF) * nRGB);
	m_R = R;
	m_G = G;
	m_B = B;
	m_bBase = bBase;
	m_RGBBase = RGBBase;

	// Create a new button with the proper style settings to replace the button on the form
	m_pWnd = pWnd;
	m_nButtonId = nButtonId;
	CreateButton();

	return TRUE;
}

void CGradient::UpdateColors(COLORREF arrRGB[],int nRGB,int R,int G,int B,BOOL bBase,COLORREF RGBBase)
{
	// Update the colors
	m_nRGB = nRGB;
	m_vRGB.resize(nRGB);
	m_parrRGB = &m_vRGB[0];
	memcpy(m_parrRGB,&arrRGB[0],sizeof(COLORREF) * nRGB);
	m_R = R;
	m_G = G;
	m_B = B;
	m_bBase = bBase;
	m_RGBBase = RGBBase;
	InvalidateRect(NULL,TRUE);
}

CGradient::~CGradient()
{
}

BEGIN_MESSAGE_MAP(CGradient, CImageButton)
	//{{AFX_MSG_MAP(CGradient)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradient message handlers

// Draw the chalk board using double buffering to prevent screen flicker
void CGradient::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// Get the enabled/disabled status
	BOOL bDisabled = lpDrawItemStruct->itemState & ODS_DISABLED;

	// Attach to the controls device context for custom drawing
	CDC DC;
	DC.Attach(lpDrawItemStruct->hDC);

	// Create a memory device context
	CDC MemDC;
	MemDC.CreateCompatibleDC(&DC);
	CDC * pDC = &MemDC;

	// Get the controls rectangle
	CRect Rect;
	GetClientRect(Rect);

	// Create an offscreen bitmap
	CBitmap MemBMP;
	MemBMP.CreateCompatibleBitmap(&DC,Rect.Width(),Rect.Height());

	// Replace the default bitmap
	CBitmap * pLastBMP = pDC->SelectObject(&MemBMP);

	// Create the DIB for the colors
	CDIBFrame Gradient(Rect.Width(),Rect.Height());
	Gradient.GradientFill(m_parrRGB,m_nRGB,m_R,m_G,m_B);

	// Copy to the destination
	Gradient.DrawTo(pDC->m_hDC);

	if (m_bBase)
	{
		CPen Pen(PS_SOLID,1,m_RGBBase);
		CPen * pPen = pDC->SelectObject(&Pen);
		int nX = min(3,Rect.right - 1);
		for (int iX = 0;iX < nX;++iX)
		{
			pDC->MoveTo(iX,Rect.top);
			pDC->LineTo(iX,Rect.bottom);
		}
		pDC->SelectObject(pPen);
	}

	// Draw the offscreen buffer to the screen
	DC.BitBlt(0,0,Rect.Width(),Rect.Height(),pDC,0,0,SRCCOPY);

	// Restore the default bitmap
	pDC->SelectObject(pLastBMP);

	// Detach from the controls device context because we don't want to delete it
	DC.Detach();
}

BOOL CGradient::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


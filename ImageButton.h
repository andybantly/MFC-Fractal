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

// Handle Windows XP theming in a backwords compatible way
typedef HTHEME (WINAPI* PFNOpenThemeData)(HWND,LPCWSTR);
typedef HRESULT (WINAPI* PFNDrawThemeBackground)(HTHEME,HDC,int,int,const RECT *,const RECT *);
typedef HRESULT (WINAPI* PFNDrawThemeText)(HTHEME,HDC,int,int,LPCWSTR,int,DWORD,DWORD,const RECT *);
typedef HRESULT (WINAPI* PFNCloseThemeData)(HTHEME);

/////////////////////////////////////////////////////////////////////////////
// CImageButton window

class CImageButton : public CButton
{
	DECLARE_DYNAMIC(CImageButton)

protected:
	HMODULE m_hUxTheme;
	PFNOpenThemeData OpenThemeData;
	PFNDrawThemeBackground DrawThemeBackground;
	PFNDrawThemeText DrawThemeText;
	PFNCloseThemeData CloseThemeData;

// Construction
public:
	enum eBitmapAlignment {LEFT,RIGHT};
	CImageButton();

// Attributes
public:

// Operations
public:
	BOOL CreateBtmp(CWnd * pWnd,int nBitmapId,int nButtonId,const TCHAR * pszText = NULL,COLORREF crTextColor = COLOR_BTNTEXT,eBitmapAlignment = LEFT);
	BOOL CreateIcon(CWnd * pWnd,int nIconId,int nButtonId,const TCHAR * pszText = NULL,COLORREF crTextColor = COLOR_BTNTEXT,eBitmapAlignment eAlignment = LEFT);
	BOOL CreateRect(CWnd * pWnd,int nButtonId,COLORREF crFillColor,const TCHAR * pszText = NULL,eBitmapAlignment eAlignment = LEFT,int iWidth = 24,int iHeight = 12);
	BOOL CreateText(CWnd * pWnd,int nButtonId,const TCHAR * pszText = NULL,COLORREF crTextColor = COLOR_BTNTEXT);
	COLORREF SetFillColor(COLORREF crFillColor);
	void UpdateBitmap(int nBitmapId);
	void UpdateIcon(int nIconId);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageButton)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	afx_msg void OnNMThemeChanged(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

protected:
	void CreateButton();

// Implementation
public:
	virtual ~CImageButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	BOOL m_bHasFocus;
	BOOL m_bMouseOver;

	CWnd * m_pWnd;
	int m_nButtonId;
	COLORREF m_crTextColor;
	eBitmapAlignment m_eAlignment;

	// Rectangle fill color
	COLORREF m_crFillColor;
	int m_iWidth;
	int m_iHeight;

	// Bitmap-Icon
	CBitmapIcon m_BitmapIcon;

	// Image
	BOOL m_bImage;

	// Filled Rectangle
	BOOL m_bRect;

	// Text button
	BOOL m_bText;

	DECLARE_MESSAGE_MAP()
};

// The custom control for showing the colors of the gradient
class CGradient : public CImageButton
{
	DECLARE_DYNAMIC(CGradient)

public:
	CGradient();

	// Member data
protected:
	int m_nRGB;
	COLORREF * m_parrRGB;
	std::vector<COLORREF> m_vRGB;
	int m_R,m_G,m_B;
	COLORREF m_RGBBase;
	BOOL m_bBase;

	// Attributes
public:

	// Operations
public:
	BOOL CreateGradient(CWnd * pWnd,int nButtonId,COLORREF arrRGB[],int nRGB,int R,int G,int B,BOOL bBase,COLORREF RGBBase);
	void UpdateColors(COLORREF arrRGB[],int nRGB,int R,int G,int B,BOOL bBase,COLORREF RGBBase);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradient)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CGradient();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// Generated message map functions
protected:
	//{{AFX_MSG(CGradient)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
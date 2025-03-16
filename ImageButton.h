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
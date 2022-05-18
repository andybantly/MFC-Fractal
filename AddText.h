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
//

#pragma once
#include "ImageButton.h"
#include "TextEdit.h"
#include "FontCombo.h"
#include "Gripper.h"
#include "FractalParm.h"
#include "Resource.h"

// CAddText dialog

class CAddText : public CDialog
{
	DECLARE_DYNAMIC(CAddText)

public:
	CAddText(CWnd * pParent = NULL);   // standard constructor
	CAddText(CDecorativeText DecorativeText,CWnd * pParent = NULL);
	virtual ~CAddText();

	void GetTextFont(CString & csFontName,int & iWeight,BOOL & bItalics,COLORREF & TextRGB,COLORREF & BackRGB,UINT & iPtSize,UINT & iOpacity,BOOL & bNoBackColor);
	CString GetText();
	CDecorativeText GetDecorativeText();

// Dialog Data
	enum { IDD = IDD_ADDTEXT };

protected:
	CTextEdit m_EditText;
	CString m_csEditText;
	CFontComboBox m_Font;

	CDecorativeText m_DecorativeText;
	CString m_csFontName;
	CImageButton m_TextColor, m_BackColor;
	COLORREF m_TextColorRGB,m_BackColorRGB;
	BOOL m_bItalics;
	UINT m_iOpacity,m_iPtSize,m_iWeight;
	BOOL m_bNoBackColor;
	CSliderCtrl m_WeightSlider;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnSize(UINT nType,int cx,int cy);
	virtual void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

protected:
	CRect m_DialogRect;
	CGripper m_Gripper;
	CPoint m_ptMinTrack,m_ptMaxTrack;
	bool m_bInitDialog;

protected:
	void UpdateFont();

	afx_msg void OnTextColor();
	afx_msg void OnBackColor();
	afx_msg void OnAttribute();
	afx_msg void OnHScroll(UINT nSBCode,UINT nPos,CScrollBar * pScrollBar);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnTimedAttribute();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};
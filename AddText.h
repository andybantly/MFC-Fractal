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
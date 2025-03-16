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

#include "stdafx.h"
#include "Fractal.h"
#include "FontCombo.h"
#include <atlimage.h>

// CFontComboBox

IMPLEMENT_DYNAMIC(CFontComboBox, CComboBox)

CFontComboBox::CFontComboBox()
{
	m_Reg.LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TTR));
	m_Sel.LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TTR));

	// Adjust the colors to match window and highlight colors
	for (int y = 0; y < 18; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			COLORREF cr = m_Reg.GetPixel(x, y);
			if (cr == 0)
			{
				m_Reg.SetPixel(x, y, GetSysColor(COLOR_WINDOW));
				m_Sel.SetPixel(x, y, GetSysColor(COLOR_HIGHLIGHT));
			}
			else
			{
				m_Reg.SetPixel(x, y, 0);
				m_Sel.SetPixel(x, y, GetSysColor(COLOR_WINDOW));
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CFontComboBox, CComboBox)
END_MESSAGE_MAP()

void CFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (GetCount())
	{
		BOOL bSel = lpDrawItemStruct->itemState & ODS_SELECTED;
		CRect ItemRect(lpDrawItemStruct->rcItem);
		CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->FillSolidRect(ItemRect, GetSysColor(bSel ? COLOR_HIGHLIGHT : COLOR_WINDOW));
		pDC->SetTextColor(GetSysColor(bSel ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

		if (lpDrawItemStruct->itemID != (UINT)-1)
		{
			CString csFontName;
			GetLBText(lpDrawItemStruct->itemID,csFontName);

			int nWidth = 18;
			int nHeight = 16;

			tagFontAttr & FontAttr = m_mapFontAttr[csFontName];
			if (FontAttr.first)
			{
				if (bSel)
				{
					if (!m_Sel.IsNull())
						m_Sel.BitBlt(lpDrawItemStruct->hDC, ItemRect.left, ItemRect.top, 16, 16, 0, 0, SRCCOPY);
				}
				else
				{
					if (!m_Reg.IsNull())
						m_Reg.BitBlt(lpDrawItemStruct->hDC, ItemRect.left, ItemRect.top, 16, 16, 0, 0, SRCCOPY);
				}
			}

			if (FontAttr.second.first)
				pDC->ExtTextOut(ItemRect.left + nWidth + 3, ItemRect.top, ETO_CLIPPED, ItemRect, csFontName, NULL);
			else
			{
				// Create the font for the edit control
				int iPtSize = FontAttr.second.second;
				bool bContinue = true;
				do
				{
					CFont Font;
					theApp.CreatePointFont(Font, pDC, FW_NORMAL, 0, DEFAULT_CHARSET, DEFAULT_QUALITY, csFontName, --iPtSize);
					HGDIOBJ hObj = pDC->SelectObject(Font);
					CSize szFont = pDC->GetTextExtent(csFontName);
					if (szFont.cy <= 18)
					{
						FontAttr.second.second = iPtSize + 1;
						pDC->ExtTextOut(ItemRect.left + nWidth + 3, ItemRect.top, ETO_CLIPPED, ItemRect, csFontName, NULL);
					}
					pDC->SelectObject(hObj);
					bContinue = szFont.cy > 18 ? true : false;
				} while (bContinue);
			}
		}
	}
}

void CFontComboBox::AddString(BOOL bTT, BYTE lfCharSet, LPCTSTR lpszString)
{
	CString csFontName(lpszString);
	if (m_mapFontAttr.find(csFontName) == m_mapFontAttr.end())
		m_mapFontAttr[csFontName] = tagFontAttr(bTT, tagCSPT(lfCharSet, 250));
	CComboBox::AddString(lpszString);
}
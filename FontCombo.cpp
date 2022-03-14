// Copyright (C) 2012-2016 Andrew S. Bantly
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
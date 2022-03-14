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
#pragma once
#include <set>

// CFontComboBox
class CFontComboBox : public CComboBox
{
	typedef std::pair<BYTE, int> tagCSPT;
	typedef std::pair<BOOL, tagCSPT > tagFontAttr;
	typedef std::map<CString, tagFontAttr > tagMapFontAttr;
	DECLARE_DYNAMIC(CFontComboBox)

public:
	CFontComboBox();
	void SetBmp(int uiBmp, int uiWidth = 16, int uiHeight = 16);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void AddString(BOOL bTT, BYTE lfCharSet, LPCTSTR lpszString);

protected:
	tagMapFontAttr m_mapFontAttr; // TT, Charset, Point Size
	CImage m_Reg,m_Sel;
	int m_nCount;
	DECLARE_MESSAGE_MAP()
};
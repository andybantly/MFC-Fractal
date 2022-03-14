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
#include "ImageButton.h"
#include "FractalParm.h"
#include "Resource.h"

// CAddPicture dialog
class CAddPicture : public CDialog
{
	DECLARE_DYNAMIC(CAddPicture)

public:
	CAddPicture(CWnd* pParent = NULL);   // standard constructor
	CAddPicture(CDecorativePicture & DecorativePicture,CWnd * pParent = NULL);
	virtual ~CAddPicture();
	CDecorativePicture GetDecorativePicture();

// Dialog Data
	enum { IDD = IDD_ADDPICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	CDecorativePicture m_DecorativePicture;
	CImageButton m_Picture;
	CImageButton m_TransparentColor;
	CString m_csPictureName;
	CEdit m_Opacity;
	CStatic m_Static_Opacity;
	UINT m_iOpacity;
	BOOL m_bTransparentColor;
	COLORREF m_crTransparentColor;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnChoosePicture();
	afx_msg void OnTransparentColor();
	afx_msg void OnChangeTransparentColor();
};

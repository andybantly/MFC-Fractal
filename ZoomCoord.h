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
#pragma once
#include "Resource.h"
#include "afxwin.h"

// CZoomCoord dialog

class CZoomCoord : public CDialog
{
	DECLARE_DYNAMIC(CZoomCoord)

public:
	CZoomCoord(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZoomCoord();

// Dialog Data
	enum { IDD = IDD_ZOOM_TO_COORDS };

public:
	void SetZoomCoord(CString csZoomCoord) {m_csZoomCoord = csZoomCoord;}
	int GetChoice() {return m_iChoice;}
	BOOL FlyInto() {return m_bCheckFlyFrames;}
	UINT FlyFrames() {return m_nFlyFrames;}
	BOOL FlyPixels() {return m_bByPixels;}

protected:
	CString m_csZoomCoord;
	int m_iChoice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedRadioNew();
	afx_msg void OnBnClickedRadioExisting();
	afx_msg void OnBnClickedCheckPixels();
	afx_msg void OnBnClickedCheckFlyframes();

protected:
	CEdit m_FlyFrames;
	CStatic m_Static_FlyFrames;
	UINT m_nFlyFrames;
	CButton m_Check_FlyFrames,m_Existing,m_New;
	BOOL m_bCheckFlyFrames;
	BOOL m_bByPixels;
	CButton m_ByPixels;
};

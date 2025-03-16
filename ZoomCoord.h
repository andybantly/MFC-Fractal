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

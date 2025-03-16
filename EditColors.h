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

#pragma once
#include "afxcmn.h"
#include "resource.h"

// CEditColors dialog

class CEditColors : public CDialog
{
	DECLARE_DYNAMIC(CEditColors)

	CListCtrl m_Colors;
	CImageList m_ImageList;
	CDIBFrame m_Gradient;
	int m_nColors;
	std::vector<COLORREF> m_vecColors;
	BYTE m_RR,m_GG,m_BB;
	BOOL m_bR,m_bG,m_bB;
	std::vector<std::vector<BYTE> > m_RGB;
	bool m_bChange;

public:
	CEditColors(CWnd * pParent = NULL);   // standard constructor
	virtual ~CEditColors();
	void SetColors(int nColors);
	void SetRGB(COLORREF RGB1,COLORREF RGB2,BYTE RR,BYTE GG,BYTE BB,BOOL bR,BOOL bG,BOOL bB);
	void SetRGB(COLORREF RGB1,COLORREF RGB2,COLORREF RGB3,COLORREF RGB4,COLORREF RGB5,COLORREF RGB6,BYTE RR,BYTE GG,BYTE BB,BOOL bR,BOOL bG,BOOL bB);
	void UpdateColor(std::vector<std::vector<BYTE> > vecRGB);
	void BuildColorMap();
	std::vector<std::vector<BYTE> > GetColorMap();

// Dialog Data
	enum { IDD = IDD_EDIT_COLORS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnNMDblclkListColors(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	HBITMAP MakeColor(BYTE R,BYTE G,BYTE B,CString & csColor);
	void BuildImageList();
};

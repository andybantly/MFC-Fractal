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
#include "ZoomCoord.h"


// CZoomCoord dialog

IMPLEMENT_DYNAMIC(CZoomCoord, CDialog)

CZoomCoord::CZoomCoord(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomCoord::IDD, pParent), m_iChoice(1), m_nFlyFrames(60), m_bCheckFlyFrames(FALSE), m_bByPixels(FALSE)
{
}

CZoomCoord::~CZoomCoord()
{
}

void CZoomCoord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_COORD, m_csZoomCoord);
	DDX_Control(pDX, IDC_RADIO_NEW, m_New);
	DDX_Control(pDX, IDC_RADIO_EXISTING, m_Existing);
	DDX_Control(pDX, IDC_EDIT_FLYFRAMES, m_FlyFrames);
	DDX_Control(pDX, IDC_STATIC_FLYFRAMES, m_Static_FlyFrames);
	DDX_Text(pDX, IDC_EDIT_FLYFRAMES, m_nFlyFrames);
	DDV_MinMaxUInt(pDX, m_nFlyFrames, 1, 1000);
	DDX_Control(pDX, IDC_CHECK_FLYFRAMES, m_Check_FlyFrames);
	DDX_Check(pDX, IDC_CHECK_FLYFRAMES, m_bCheckFlyFrames);
	DDX_Check(pDX, IDC_CHECK_PIXELS, m_bByPixels);
	DDX_Control(pDX, IDC_CHECK_PIXELS, m_ByPixels);
}

BOOL CZoomCoord::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();
	m_New.SetCheck(1);
	return bRet;
}

BEGIN_MESSAGE_MAP(CZoomCoord, CDialog)
	ON_BN_CLICKED(IDC_RADIO_NEW, &CZoomCoord::OnBnClickedRadioNew)
	ON_BN_CLICKED(IDC_RADIO_EXISTING, &CZoomCoord::OnBnClickedRadioExisting)
	ON_BN_CLICKED(IDC_CHECK_PIXELS, &CZoomCoord::OnBnClickedCheckPixels)
	ON_BN_CLICKED(IDC_CHECK_FLYFRAMES, &CZoomCoord::OnBnClickedCheckFlyframes)
END_MESSAGE_MAP()

// CZoomCoord message handlers


void CZoomCoord::OnBnClickedRadioNew()
{
	m_iChoice = 1;
	m_Check_FlyFrames.EnableWindow(FALSE);
	m_Static_FlyFrames.EnableWindow(FALSE);
	m_ByPixels.EnableWindow(FALSE);
	m_FlyFrames.EnableWindow(FALSE);
}

void CZoomCoord::OnBnClickedRadioExisting()
{
	m_iChoice = 2;
	m_Check_FlyFrames.EnableWindow();
	m_Static_FlyFrames.EnableWindow();
	m_ByPixels.EnableWindow();
	m_FlyFrames.EnableWindow();
}

void CZoomCoord::OnBnClickedCheckPixels()
{
	UpdateData();
	m_FlyFrames.EnableWindow(!m_bByPixels);
}

void CZoomCoord::OnBnClickedCheckFlyframes()
{
	UpdateData();
}

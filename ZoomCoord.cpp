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

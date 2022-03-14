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
#include "AddPicture.h"

// CAddPicture dialog

IMPLEMENT_DYNAMIC(CAddPicture, CDialog)

CAddPicture::CAddPicture(CWnd* pParent /*=NULL*/)
	: CDialog(CAddPicture::IDD, pParent)
{
	m_iOpacity = 50;
	m_bTransparentColor = FALSE;
	m_crTransparentColor = RGB(0,128,0);
}

CAddPicture::CAddPicture(CDecorativePicture & DecorativePicture,CWnd * pParent /*=NULL*/)
: CDialog(CAddPicture::IDD, pParent)
{
	m_DecorativePicture = DecorativePicture;
	m_csPictureName = m_DecorativePicture.GetPictureName();
	m_iOpacity = m_DecorativePicture.GetOpacity();
	m_bTransparentColor = m_DecorativePicture.GetTransparentColorFlag();
	m_crTransparentColor = m_DecorativePicture.GetTransparentColor();
}

CAddPicture::~CAddPicture()
{
}

void CAddPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PICTURENAME, m_csPictureName);
	DDX_Control(pDX, IDC_STATIC_OPACITY, m_Static_Opacity);
	DDX_Control(pDX, IDC_EDIT_OPACITY, m_Opacity);
	DDX_Text(pDX, IDC_EDIT_OPACITY, m_iOpacity);
	DDV_MinMaxUInt(pDX, m_iOpacity, 0, 100);
	DDX_Check(pDX, IDC_CHECK_TRANSPARENT_COLOR,m_bTransparentColor);
	if (pDX->m_bSaveAndValidate)
	{
		m_DecorativePicture.SetPictureName(m_csPictureName);
		m_DecorativePicture.SetOpacity(m_iOpacity);
		m_DecorativePicture.SetTransparentColorFlag(m_bTransparentColor);
		m_DecorativePicture.SetTransparentColor(m_crTransparentColor);
	}
}

BOOL CAddPicture::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();
	m_Picture.CreateBtmp(this,IDB_PICTURE,IDC_CHOOSE_PICTURE,_T("Choose Picture"));
	m_TransparentColor.CreateRect(this,IDC_TRANSPARENT_COLOR,m_crTransparentColor);
	m_TransparentColor.EnableWindow(m_bTransparentColor);
	return bRet;
}

CDecorativePicture CAddPicture::GetDecorativePicture()
{
	return m_DecorativePicture;
}

BEGIN_MESSAGE_MAP(CAddPicture, CDialog)
	ON_BN_CLICKED(IDC_CHOOSE_PICTURE, &CAddPicture::OnChoosePicture)
	ON_BN_CLICKED(IDC_CHECK_TRANSPARENT_COLOR, &CAddPicture::OnTransparentColor)
	ON_BN_CLICKED(IDC_TRANSPARENT_COLOR, &CAddPicture::OnChangeTransparentColor)
END_MESSAGE_MAP()

// CAddPicture message handlers
void CAddPicture::OnChoosePicture()
{
	// Choose the image
	CString csPictureName;
	if (theApp.ChooseImage(_T("Add a Picture"),csPictureName))
	{
		CImage Image;
		if (SUCCEEDED(Image.Load(csPictureName)))
		{
			m_crTransparentColor = Image.GetPixel(0,0);
			m_TransparentColor.SetFillColor(m_crTransparentColor);
			m_csPictureName = csPictureName;
			UpdateData(FALSE);
		}
	}
}

void CAddPicture::OnTransparentColor()
{
	UpdateData();
	m_TransparentColor.EnableWindow(m_bTransparentColor);
}

void CAddPicture::OnChangeTransparentColor()
{
	theApp.ChangeColor(m_crTransparentColor,m_TransparentColor);
}
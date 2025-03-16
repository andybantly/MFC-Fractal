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
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

// EditColors.cpp : implementation file
//

#include "stdafx.h"
#include "Fractal.h"
#include "EditColors.h"
#include "BitmapIcon.h"

// CEditColors dialog

IMPLEMENT_DYNAMIC(CEditColors, CDialog)

CEditColors::CEditColors(CWnd * pParent) 
: CDialog(CEditColors::IDD, pParent), m_nColors(0), m_bChange(false)
{
}

CEditColors::~CEditColors()
{
}

void CEditColors::SetColors(int nColors)
{
	if (m_nColors != nColors)
		m_bChange = true;
	m_nColors = nColors;
}

void CEditColors::SetRGB(COLORREF RGB1,COLORREF RGB2,BYTE RR,BYTE GG,BYTE BB,BOOL bR,BOOL bG,BOOL bB)
{
	if (m_vecColors.size() != 2)
		m_bChange = true;
	m_vecColors.resize(2);

	RGB1 = RGB(bR ? GetRValue(RGB1) : 0,bG ? GetGValue(RGB1) : 0,bB ? GetBValue(RGB1) : 0);
	RGB2 = RGB(bR ? GetRValue(RGB2) : 0,bG ? GetGValue(RGB2) : 0,bB ? GetBValue(RGB2) : 0);

	RR = bR ? RR : 0;
	GG = bG ? GG : 0;
	BB = bB ? BB : 0;

	if (m_RR != RR || m_GG != GG || m_BB != BB || 
		m_bR != bR || m_bG != bG || m_bB != bB ||
		RGB1 != m_vecColors[0] || RGB2 != m_vecColors[1])
		m_bChange = true;

	m_RR = RR;
	m_GG = GG;
	m_BB = BB;

	m_bR = bR;
	m_bG = bG;
	m_bB = bB;

	m_vecColors[0] = RGB1;
	m_vecColors[1] = RGB2;

	if (m_bChange)
	{
		m_RGB.clear();
		m_bChange = false;
	}
}

void CEditColors::SetRGB(COLORREF RGB1,COLORREF RGB2,COLORREF RGB3,COLORREF RGB4,COLORREF RGB5,COLORREF RGB6,BYTE RR,BYTE GG,BYTE BB,BOOL bR,BOOL bG,BOOL bB)
{
	if (m_vecColors.size() != 6)
		m_bChange = true;
	m_vecColors.resize(6);

	RGB1 = RGB(bR ? GetRValue(RGB1) : 0,bG ? GetGValue(RGB1) : 0,bB ? GetBValue(RGB1) : 0);
	RGB2 = RGB(bR ? GetRValue(RGB2) : 0,bG ? GetGValue(RGB2) : 0,bB ? GetBValue(RGB2) : 0);
	RGB3 = RGB(bR ? GetRValue(RGB3) : 0,bG ? GetGValue(RGB3) : 0,bB ? GetBValue(RGB3) : 0);
	RGB4 = RGB(bR ? GetRValue(RGB4) : 0,bG ? GetGValue(RGB4) : 0,bB ? GetBValue(RGB4) : 0);
	RGB5 = RGB(bR ? GetRValue(RGB5) : 0,bG ? GetGValue(RGB5) : 0,bB ? GetBValue(RGB5) : 0);
	RGB6 = RGB(bR ? GetRValue(RGB6) : 0,bG ? GetGValue(RGB6) : 0,bB ? GetBValue(RGB6) : 0);

	RR = bR ? RR : 0;
	GG = bG ? GG : 0;
	BB = bB ? BB : 0;

	if (m_RR != RR || m_GG != GG || m_BB != BB || 
		m_bR != bR || m_bG != bG || m_bB != bB ||
		RGB1 != m_vecColors[0] || RGB2 != m_vecColors[1] || RGB3 != m_vecColors[2] ||
		RGB4 != m_vecColors[3] || RGB5 != m_vecColors[4] || RGB6 != m_vecColors[5])
		m_bChange = true;

	m_RR = RR;
	m_GG = GG;
	m_BB = BB;

	m_bR = bR;
	m_bG = bG;
	m_bB = bB;

	m_vecColors[0] = RGB1;
	m_vecColors[1] = RGB2;
	m_vecColors[2] = RGB3;
	m_vecColors[3] = RGB4;
	m_vecColors[4] = RGB5;
	m_vecColors[5] = RGB6;

	if (m_bChange)
	{
		m_RGB.clear();
		m_bChange = false;
	}
}

void CEditColors::UpdateColor(std::vector<std::vector<BYTE> > vecRGB)
{
	m_RGB = vecRGB;
}

void CEditColors::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_COLORS,m_Colors);
}

// Make the Bitmap and color text
HBITMAP CEditColors::MakeColor(BYTE R,BYTE G,BYTE B,CString & csColor)
{
	// Get the color value
	COLORREF crRGB = RGB(R,G,B);

	// Create a DIB of the color
	CDIBFrame Image(32,32);
	CDC * pImageDC = (CDC *)Image;
	pImageDC->FillSolidRect(1,1,30,30,crRGB);
	
	// Convert to a DDB
	HBITMAP hBitmap = Image.ToDDB();

	// Get the RGB text
	csColor.Format(L"RGB(%d,%d,%d)",R,G,B);
	return hBitmap;
}

// Build the color map
void CEditColors::BuildColorMap()
{
	// Build the palette
	m_Gradient.Init(m_nColors,32);
	m_Gradient.GradientFill(&m_vecColors[0],(int)m_vecColors.size(),m_RR,m_GG,m_BB);

	// Prepare the color map
	m_RGB.resize(3);
	m_RGB[0].resize(m_nColors + 1,0);
	m_RGB[1].resize(m_nColors + 1,0);
	m_RGB[2].resize(m_nColors + 1,0);

	// Create the icons for the image list
	for (int iColor = 0;iColor < m_nColors;++iColor)
	{
		// Get the color
		BYTE R,G,B;
		m_Gradient.GetPixel(iColor,16,R,G,B);

		// Set the color in the color map
		m_RGB[0][iColor + 1] = R;
		m_RGB[1][iColor + 1] = G;
		m_RGB[2][iColor + 1] = B;
	}

	// Make sure the last color is the last color in the range (makes a big difference for binary palette)
	m_RGB[0][m_nColors] = GetRValue(m_vecColors[m_vecColors.size() - 1]);
	m_RGB[1][m_nColors] = GetGValue(m_vecColors[m_vecColors.size() - 1]);
	m_RGB[2][m_nColors] = GetBValue(m_vecColors[m_vecColors.size() - 1]);
}

// Build the image list and text from the color map
void CEditColors::BuildImageList()
{
	// Create the image list
	if (m_ImageList.GetSafeHandle())
		m_ImageList.DeleteImageList();
	m_ImageList.Create(32,32,ILC_COLORDDB,m_nColors,m_nColors);

	// Create the icons for the image list
	for (int iColor = 0;iColor < m_nColors;++iColor)
	{
		// Get the color
		BYTE R,G,B;
		R = m_RGB[0][iColor + 1];
		G = m_RGB[1][iColor + 1];
		B = m_RGB[2][iColor + 1];

		// Get the bitmap and text for the color
		CString csColor;
		CBitmap Bitmap;
		Bitmap.Attach(MakeColor(R,G,B,csColor));

		// Add to the image list
		m_ImageList.Add(&Bitmap,&Bitmap);

		// Add the item, the text, and index to the image list
		m_Colors.InsertItem(iColor,csColor,iColor);
	}

	// Set the image list to the icon view
	m_Colors.SetImageList(&m_ImageList,LVSIL_NORMAL);
}

// Get and optionally build the color map
std::vector<std::vector<BYTE> > CEditColors::GetColorMap()
{
	if (m_RGB.empty())
		BuildColorMap();
	return m_RGB;
}

BOOL CEditColors::OnInitDialog()
{
	BOOL bRes = CDialog::OnInitDialog();

	if (m_nColors)
	{
		if (m_RGB.empty())
		{
			// Build the color map
			BuildColorMap();
		}

		// Build the image list
		BuildImageList();
	}
	else
		EndDialog(IDCANCEL);

	return bRes;
}

BEGIN_MESSAGE_MAP(CEditColors, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COLORS, &CEditColors::OnNMDblclkListColors)
END_MESSAGE_MAP()

// CEditColors message handlers
void CEditColors::OnNMDblclkListColors(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int iItem = pNMItemActivate->iItem;
	if (iItem != -1)
	{
		// Get the color
		BYTE R,G,B;
		R = m_RGB[0][iItem + 1];
		G = m_RGB[1][iItem + 1];
		B = m_RGB[2][iItem + 1];

		// Edit the color
		CColorDialog ClrDlg;
		ClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
		COLORREF crRGB = RGB(R,G,B);
		ClrDlg.m_cc.rgbResult = crRGB;
		if (ClrDlg.DoModal() == IDOK)
		{
			// Get the new color
			crRGB = ClrDlg.GetColor();

			// Get the color components
			R = GetRValue(crRGB);
			G = GetGValue(crRGB);
			B = GetBValue(crRGB);

			// Set the color in the color map (channel filtering happens later for the map)
			m_RGB[0][iItem + 1] = R;
			m_RGB[1][iItem + 1] = G;
			m_RGB[2][iItem + 1] = B;

			// Check for color channel filtering
			if (R & !m_bR)
				AfxMessageBox(L"Red channel will be removed do to red channel settings");
			if (G & !m_bG)
				AfxMessageBox(L"Green channel will be removed do to green channel settings");
			if (B & !m_bB)
				AfxMessageBox(L"Blue channel will be removed do to blue channel settings");

			// Set the color values that make the color
			R = m_bR ? R : 0;
			G = m_bG ? G : 0;
			B = m_bB ? B : 0;

			// Get the bitmap and text
			CString csColor;
			CBitmap Bitmap;
			Bitmap.Attach(MakeColor(R,G,B,csColor));

			// Update the image list
			m_ImageList.Replace(iItem,&Bitmap,&Bitmap);

			// Update the text
			m_Colors.SetItemText(iItem,0,csColor);
		}
	}
	*pResult = 0;
}

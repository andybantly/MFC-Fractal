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
#include "BitmapIcon.h"

// Construct the frame
CDIBFrame::CDIBFrame() : m_x(0), m_y(0), m_nBitCount(32), m_nScale(4),
	m_hBkgFrame(NULL), m_pBkgBits(NULL), m_hLastBkgFrame(NULL), 
	m_pBitmapInfo(NULL), m_pBitmapInfoHdr(NULL), m_bTrackPalette(false)
{
}

// Construct the frame
CDIBFrame::CDIBFrame(int x,int y,int nBitCount) : m_x(x), m_y(y), m_nBitCount(nBitCount),
	m_hBkgFrame(NULL), m_pBkgBits(NULL), m_hLastBkgFrame(NULL), 
	m_pBitmapInfo(NULL), m_pBitmapInfoHdr(NULL), m_bTrackPalette(false)
{
	// Create the frame
	CreateFrame();
}

// Copy/Assignment constructor
CDIBFrame::CDIBFrame(const CDIBFrame & rhs)
{
	*this = rhs;
}

// Deconstruct the frame
CDIBFrame::~CDIBFrame()
{
	// Delete the frame
	DeleteFrame();
}

// Copy/Assignment operator
CDIBFrame & CDIBFrame::operator = (const CDIBFrame & rhs)
{
	if (this != &rhs)
	{
		// Initialize
		m_bTrackPalette = rhs.m_bTrackPalette;
		Init(rhs.m_x,rhs.m_y,rhs.m_bTrackPalette,rhs.m_nBitCount);

		// Copy the color information
		m_Palette = rhs.m_Palette;
		m_RGB = rhs.m_RGB;

		// Copy the source
		memcpy(m_pBkgBits,rhs.m_pBkgBits,GetSizeImage());
	}
	return *this;
}

// Set the dimensions and create the frame
void CDIBFrame::Init(int x,int y,bool bTrackPalette,int nBitCount)
{
	// Test the dimensions
	if (x < 1 || y < 1)
		return;

	// Test for already being initialized
	if (m_FrameDC && m_x == x && m_y == y)
		return;

	// Set the new dimensions
	m_x = x;
	m_y = y;
	m_bTrackPalette = bTrackPalette;
	m_nBitCount = nBitCount;

	// Create the frame
	CreateFrame();
}

// Create the frame
void CDIBFrame::CreateFrame()
{
	// Cleanup the last frame
	DeleteFrame();

	// Set the scale
	if (m_nBitCount == 32)
		m_nScale = 4;
	else if (m_nBitCount == 24)
		m_nScale = 3;
	else if (m_nBitCount == 8)
		m_nScale = 1;
	else if (m_nBitCount == 1)
		m_nScale = 1;

	// Create a DC for the compatible display
	CDC DisplayDC;
	DisplayDC.Attach(GetDC(GetDesktopWindow()));

	// Create the last frame DC
	m_FrameDC.CreateCompatibleDC(&DisplayDC);
	if (m_FrameDC)
	{
		// Calculate the size of the bitmap info structure (header + color table)
		DWORD dwLen = 0;
		if (m_nBitCount == 8)
			dwLen = (DWORD)((WORD)sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
		else if (m_nBitCount == 1)
			dwLen = (DWORD)((WORD)sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD));
		else
			dwLen = (DWORD)((WORD)sizeof(BITMAPINFOHEADER));

		// Allocate the bitmap structure
		m_Buffer.resize(dwLen,0);
		BYTE * pBuffer = &m_Buffer[0];

		// Set up the bitmap info structure for the DIB section
		m_pBitmapInfo = (BITMAPINFO*)pBuffer;
		m_pBitmapInfoHdr = (BITMAPINFOHEADER*)&(m_pBitmapInfo->bmiHeader);
		m_pBitmapInfoHdr->biSize = sizeof(BITMAPINFOHEADER);
		m_pBitmapInfoHdr->biWidth = m_x;
		m_pBitmapInfoHdr->biHeight = m_y;
		m_pBitmapInfoHdr->biPlanes = 1;
		m_pBitmapInfoHdr->biBitCount = m_nBitCount;
		m_pBitmapInfoHdr->biCompression = BI_RGB;
		m_pBitmapInfoHdr->biClrUsed = 0;
		m_pBitmapInfoHdr->biClrImportant = 0;
		m_pBitmapInfoHdr->biSizeImage = ((m_pBitmapInfoHdr->biWidth * m_pBitmapInfoHdr->biBitCount / 8 + 3) & 0xFFFFFFFC) * m_pBitmapInfoHdr->biHeight;

		// 1 meter = 39.3700787 inches
		// 1 inch has a specific amount of X and Y pixels
		int nLPX = DisplayDC.GetDeviceCaps(LOGPIXELSX);
		int nLPY = DisplayDC.GetDeviceCaps(LOGPIXELSY);
		m_pBitmapInfoHdr->biXPelsPerMeter = (LONG)(nLPX * 39.3700787 + 0.5);
		m_pBitmapInfoHdr->biYPelsPerMeter = (LONG)(nLPY * 39.3700787 + 0.5);

		// 256 color gray scale color table embedded in the DIB
		if (m_nBitCount == 8)
		{
			RGBQUAD Colors;
			for (int iColor = 0;iColor < 256;iColor++)
			{
				Colors.rgbBlue = Colors.rgbGreen = Colors.rgbRed = Colors.rgbReserved = iColor;
				m_pBitmapInfo->bmiColors[iColor] = Colors;
			}
			m_pBitmapInfo->bmiHeader.biClrUsed = 256;
		}
		else if (m_nBitCount == 1)
		{
			RGBQUAD OffColor = {255,255,255,0};
			RGBQUAD OnColor = {0,0,0,0};
			m_pBitmapInfo->bmiColors[0] = OffColor;
			m_pBitmapInfo->bmiColors[1] = OnColor;
			m_pBitmapInfo->bmiHeader.biClrUsed = 2;
		}

		// Create the DIB for the frame
		m_hBkgFrame = CreateDIBSection(m_FrameDC,m_pBitmapInfo,DIB_RGB_COLORS,(void**)&m_pBkgBits,NULL,0);

		// Prepare the frame DIB for painting
		m_hLastBkgFrame = (HBITMAP)m_FrameDC.SelectObject(m_hBkgFrame);

		// Initialize the DIB to black
		m_FrameDC.PatBlt(0,0,m_x,m_y,BLACKNESS);

		// Initialize the palette
		if (m_bTrackPalette && m_nBitCount != 1)
			m_Palette.resize(m_x * m_y,0);
	}

	// Delete the dc's
	ReleaseDC(GetDesktopWindow(),DisplayDC.Detach());
}

// Delete the frame
void CDIBFrame::DeleteFrame()
{
	// Check for a frame to cleanup
	if (m_FrameDC)
	{
		// UnSelect the DIB
		m_FrameDC.SelectObject(m_hLastBkgFrame);

		// Delete the DIB for the frame
		DeleteObject(m_hBkgFrame);
		m_pBkgBits = NULL;
		m_pBitmapInfo = NULL;
		m_pBitmapInfoHdr = NULL;
		m_bTrackPalette = false;

		// Delete the last frame DC
		m_FrameDC.DeleteDC();
	}
}

int CDIBFrame::GetWidth()
{
	return m_x;
}

int CDIBFrame::GetHeight()
{
	return m_y;
}

void CDIBFrame::SetPixelAndPaletteIndex(int X, int Y, BYTE R, BYTE G, BYTE B, int iPalette) noexcept
{
	DWORD dwPos = (Y * m_x + X);
	m_Palette[dwPos] = iPalette;

	dwPos *= m_nScale;
	m_pBkgBits[dwPos] = B;
	m_pBkgBits[dwPos + 1] = G;
	m_pBkgBits[dwPos + 2] = R;
}

void CDIBFrame::SetPixel(int X, int Y, BYTE R, BYTE G, BYTE B)
{
	DWORD dwPos = (Y * m_x + X) * m_nScale;
	m_pBkgBits[dwPos] = B;
	m_pBkgBits[dwPos + 1] = G;
	m_pBkgBits[dwPos + 2] = R;
}

void CDIBFrame::SetPixelPaletteIndex(int X, int Y, int iPalette)
{
	DWORD dwPos = (Y * m_x + X);
	m_Palette[dwPos] = iPalette;
}

void CDIBFrame::GetPixelAndPaletteIndex(int X, int Y, BYTE & R, BYTE & G, BYTE & B, int & iPalette)
{
	DWORD dwPos = (Y * m_x + X);
	iPalette = m_Palette[dwPos];

	dwPos *= m_nScale;
	B = m_pBkgBits[dwPos];
	G = m_pBkgBits[dwPos + 1];
	R = m_pBkgBits[dwPos + 2];
}

void CDIBFrame::GetPixel(int X, int Y, BYTE & R, BYTE & G, BYTE & B)
{
	DWORD dwPos = (Y * m_x + X) * m_nScale;
	B = m_pBkgBits[dwPos];
	G = m_pBkgBits[dwPos + 1];
	R = m_pBkgBits[dwPos + 2];
}

void CDIBFrame::GetPixelPaletteIndex(int X,int Y,int & iPalette)
{
	DWORD dwPos = (Y * m_x + X);
	iPalette = m_Palette[dwPos];
}

void CDIBFrame::SetRGB(std::vector<std::vector<BYTE> > & RGB)
{
	m_RGB = RGB;
}

std::vector<std::vector<BYTE> > & CDIBFrame::GetRGB() 
{
	return m_RGB;
}

DWORD CDIBFrame::GetPixelCount()
{
	return GetSizeImage() / m_nScale;
}

void CDIBFrame::DrawTo(HDC hDC,int x,int y,DWORD dwROP,HBRUSH hPatternBrush)
{
	if (hPatternBrush)
		hPatternBrush = (HBRUSH)SelectObject(hDC,hPatternBrush);

	BitBlt(hDC,x,y,m_x,m_y,(HDC)m_FrameDC,0,0,dwROP);

	if (hPatternBrush)
		SelectObject(hDC,hPatternBrush);
}

void CDIBFrame::GradientFill(COLORREF arrRGB[],int nRGB,int R,int G,int B)
{
	// Need an even amount of at least 2 colors
	if (nRGB < 2 || nRGB % 2 == 1)
		return;

	// Define the gradient rectangle
	GRADIENT_RECT gRect;
	gRect.UpperLeft = 0;
	gRect.LowerRight = 1;

	// Get the COLOR16 colors
	int ix = 0;
	double dx = 1.0 / (double)(nRGB - 1);
	int nx = (int)((double)m_x * dx);
	TRIVERTEX Vert[2];
	for (int iRGB = 0;iRGB < nRGB - 1;++iRGB)
	{
		int R1 = GetRValue(arrRGB[iRGB]) << 8;
		int G1 = GetGValue(arrRGB[iRGB]) << 8;
		int B1 = GetBValue(arrRGB[iRGB]) << 8;
		int R2 = GetRValue(arrRGB[iRGB + 1]) << 8;
		int G2 = GetGValue(arrRGB[iRGB + 1]) << 8;
		int B2 = GetBValue(arrRGB[iRGB + 1]) << 8;

		// Define the beginning gradient vertice
		Vert[0].x = ix;
		Vert[0].y = 0;
		Vert[0].Red = R1;
		Vert[0].Green = G1;
		Vert[0].Blue = B1;
		Vert[0].Alpha = 0;

		// Define the ending gradient vertice
		ix += nx;
		if (iRGB + 1 == nRGB - 1)
			ix = m_x;
		Vert[1].x = ix;
		Vert[1].y = m_y;
		Vert[1].Red = R2;
		Vert[1].Green = G2;
		Vert[1].Blue = B2;
		Vert[1].Alpha = 0;

		// Fill the area with the gradient
		::GradientFill((HDC)m_FrameDC,&Vert[0],2,&gRect,1,GRADIENT_FILL_RECT_H);
	}

	if (R || G || B)
	{
		// Scale the gradient with the base scale colors
		for (int X = 0;X < m_x;++X)
		{
			for (int Y = 0;Y < m_y;++Y)
			{
				BYTE R1,G1,B1;
				GetPixel(X,Y,R1,G1,B1);
				R1 += R;
				G1 += G;
				B1 += B;
				SetPixel(X,Y,R1,G1,B1);
			}
		}
	}
}

void CDIBFrame::Fill(COLORREF RGB)
{
	// Fill the area with a solid color
	m_FrameDC.FillSolidRect(0,0,m_x,m_y,RGB);
}

// Blend two frames together to the destination, factoring out the key color of the foreground image
void CDIBFrame::MixFrame(int iMixAmt,CDIBFrame & DIBFore,CDIBFrame & DIBBack)
{
	// Get the foreground color data
	LPSTR pDIBForeBits = (LPSTR)DIBFore;

	// Get the background color data
	LPSTR pDIBBackBits = (LPSTR)DIBBack;

	// Get the source and destination Bytes
	BYTE * pDestBytes = (BYTE *)m_pBkgBits;
	BYTE * pForeBytes = (BYTE *)pDIBForeBits;
	BYTE * pBackBytes = (BYTE *)pDIBBackBits;

	// Alpha blending level (0-255)
	BYTE alpha = iMixAmt;
	BYTE alpha2 = 255 - alpha;

	// Process the DIB
	DWORD dwBytes = GetSizeImage();
	for (DWORD dw = 0;dw < dwBytes;dw += m_nScale)
	{
		// Get the foreground pixel
		BYTE FR,FG,FB;
		FB = pForeBytes[dw];
		FG = pForeBytes[dw + 1];
		FR = pForeBytes[dw + 2];

		// Get the background pixel
		BYTE BR,BG,BB;
		BB = pBackBytes[dw];
		BG = pBackBytes[dw + 1];
		BR = pBackBytes[dw + 2];

		// Get the destination pixel
		BYTE & DB = pDestBytes[dw];
		BYTE & DG = pDestBytes[dw + 1];
		BYTE & DR = pDestBytes[dw + 2];

		// Update by blending the sources to the destination
		DB = (BB * alpha2 + FB * alpha) >> 8;
		DG = (BG * alpha2 + FG * alpha) >> 8;
		DR = (BR * alpha2 + FR * alpha) >> 8;
	}
}

// Convert DIB to a DDB
HBITMAP CDIBFrame::ToDDB()
{
	return CreateDIBitmap((HDC)m_FrameDC,m_pBitmapInfoHdr,CBM_INIT,(const void *)m_pBkgBits,m_pBitmapInfo,DIB_RGB_COLORS);
}

// The Petzold DrawBitmap function
void CDIBFrame::DrawDDBBitmap(HBITMAP hBitmap,int xStart,int yStart,DWORD dwROP,HBRUSH hPatternBrush)
{
	HDC hdc = (HDC)m_FrameDC;
	BITMAP bm;
	HDC hdcMem;
	POINT ptSize,ptOrg;

	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem,hBitmap);
	SetMapMode(hdcMem,GetMapMode(hdc));

	GetObject(hBitmap,sizeof(BITMAP),(LPVOID)&bm);

	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdc,&ptSize,1);

	ptOrg.x = 0;
	ptOrg.y = 0;
	DPtoLP(hdcMem,&ptOrg,1);

	if (hPatternBrush)
		hPatternBrush = (HBRUSH)SelectObject(hdc,hPatternBrush);

	BitBlt(hdc,xStart,yStart,ptSize.x,ptSize.y,hdcMem,ptOrg.x,ptOrg.y,dwROP);

	if (hPatternBrush)
		SelectObject(hdc,hPatternBrush);
	DeleteDC(hdcMem);
}

// Convert the DIB to a DDB
void CDIBFrame::DrawDDBTo(CDIBFrame & Dest,int xStart,int yStart,DWORD dwROP,HBRUSH hPatternBrush)
{
	CBitmap Bitmap;
	Bitmap.Attach(ToDDB());
	Dest.DrawDDBBitmap(Bitmap,xStart,yStart,dwROP,hPatternBrush);
}

// Convert the DIB to a pattern brush by way of DIB to DDB
HBRUSH CDIBFrame::DIBPatternBrush()
{
	CBitmap BMPBrush;
	BMPBrush.Attach(ToDDB());
	CBrush PatternBrush;
	PatternBrush.CreatePatternBrush(&BMPBrush);
	HBRUSH hPatternBrush = (HBRUSH)PatternBrush.Detach();
	return hPatternBrush;
}

CBitmapIcon::CBitmapIcon() : m_nID(0), m_crTransparentColor(RGB(0,0,0))
{
}

CBitmapIcon::CBitmapIcon(UINT nID) : m_nID(nID), m_crTransparentColor(RGB(0,0,0))
{
	Initialize();
}

CBitmapIcon::CBitmapIcon(const CBitmapIcon & rhs)
{
	*this = rhs;
}

CBitmapIcon::~CBitmapIcon()
{
}

CBitmapIcon & CBitmapIcon::operator = (const CBitmapIcon & rhs)
{
	if (this != &rhs)
	{
		m_nID = rhs.m_nID;
		Initialize();
	}
	return *this;
}

void CBitmapIcon::Load(UINT nID)
{
	m_nID = nID;
	Initialize();
}

void CBitmapIcon::LoadIcon(UINT nID)
{
	m_Icon.LoadIcon(nID);
}

UINT CBitmapIcon::GetWidth()
{
	return m_Icon.GetWidth();
}

UINT CBitmapIcon::GetHeight()
{
	return m_Icon.GetHeight();
}

COLORREF CBitmapIcon::GetTransparentColor()
{
	return m_crTransparentColor;
}

CBitmapIcon::operator HICON()
{
	return m_Icon;
}

void CBitmapIcon::Initialize()
{
	m_Bitmap.DeleteObject();
	if (m_Bitmap.LoadBitmap(m_nID))
		ConvertBitmap();
}

void CBitmapIcon::ConvertBitmap(HBITMAP hBitmap)
{
	// Set the bitmap
	CBitmap * pBitmap;
	if (hBitmap == NULL)
		pBitmap = &m_Bitmap;
	else
		pBitmap = CBitmap::FromHandle(hBitmap);

	// Create a memory DC that contains the loaded up bitmap
	CDC ImageDC;
	ImageDC.CreateCompatibleDC(NULL);
	CBitmap * pOldImageBitmap = ImageDC.SelectObject(pBitmap);

	// Get the transparent pixel from the upper left of the bitmap
	m_crTransparentColor = ImageDC.GetPixel(0,0);

	// Get the bitmap information
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	// Create a memory DC for the mask monochrome bitmap
	CDC MaskDC;
	MaskDC.CreateCompatibleDC(NULL);

	// Initialize the "mask" monochrome bitmap
	CBitmap MaskBitmap;
	MaskBitmap.CreateBitmap(bm.bmWidth,bm.bmHeight,1,1,NULL);
	CBitmap * pOldMaskBitmap = MaskDC.SelectObject(&MaskBitmap);

	// Create the "mask" bitmap
	ImageDC.SetBkColor(m_crTransparentColor);
	MaskDC.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&ImageDC,0,0,SRCCOPY);

	// Create a memory DC for the color bitmap
	CDC ColorDC;
	ColorDC.CreateCompatibleDC(NULL);

	// Initialize the "color" bitmap
	CBitmap ColorBitmap;
	ColorBitmap.CreateCompatibleBitmap(&ImageDC,bm.bmWidth,bm.bmHeight);
	CBitmap * pOldOrBitmap = ColorDC.SelectObject(&ColorBitmap);

	// Create the "color" bitmap
	ColorDC.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&ImageDC,0,0,SRCCOPY);
	ColorDC.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&MaskDC,0,0,0x220326);

	// Clean up the image bitmap
	ImageDC.SelectObject(pOldImageBitmap);

	// Clean up the "color" bitmap
	ColorDC.SelectObject(pOldOrBitmap);

	// Clean up the "mask" bitmap
	MaskDC.SelectObject(pOldMaskBitmap);

	// Create the Icon
	m_Icon.CreateIcon((HBITMAP)MaskBitmap,(HBITMAP)ColorBitmap);
}

HICON CBitmapIcon::CreateFromBitmap(HBITMAP hBitmap)
{
	// Convert the bitmap to an icon
	ConvertBitmap(hBitmap);

	// Return the Icon
	return (HICON)m_Icon;
}
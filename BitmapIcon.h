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

class CDIBFrame
{
public:
	CDIBFrame();
	CDIBFrame(int x,int y,int nBitCount = 32);
	CDIBFrame(const CDIBFrame & rhs);
	~CDIBFrame();

public:
	CDIBFrame & operator = (const CDIBFrame & rhs);
	operator HBITMAP () {return m_hBkgFrame;}
	operator HDC () {return (HDC)m_FrameDC;}
	operator CDC * () {return &m_FrameDC;}
	operator CDC & () {return m_FrameDC;}
	operator LPSTR () {return m_pBkgBits;}
	operator LPSTR * () {return &m_pBkgBits;}
	operator BITMAPINFO * () {return m_pBitmapInfo;}
	operator BITMAPINFOHEADER * () {return m_pBitmapInfoHdr;}
	operator bool () {return GetSizeImage() ? true : false;}
	DWORD GetSizeImage() {return m_pBitmapInfoHdr ? m_pBitmapInfoHdr->biSizeImage : 0; }

public:
	void Init(int x,int y,bool bTrackPalette = false,int nBitCount = 32);
	void CreateFrame();
	void DeleteFrame();

	int GetWidth();
	int GetHeight();
	void SetPixelAndPaletteIndex(int X, int Y, BYTE R, BYTE G, BYTE B, int iPalette);
	void SetPixel(int X, int Y, BYTE R, BYTE G, BYTE B);
	void SetPixelPaletteIndex(int X,int Y,int iPalette);
	void GetPixelAndPaletteIndex(int X, int Y, BYTE & R, BYTE & G, BYTE & B, int & iPalette);
	void GetPixel(int X, int Y, BYTE & R, BYTE & G, BYTE & B);
	void GetPixelPaletteIndex(int X,int Y,int & iPalette);
	void SetRGB(std::vector<std::vector<BYTE> > & RGB);
	std::vector<std::vector<BYTE> > & GetRGB();
	DWORD GetPixelCount();
	void DrawTo(HDC hDC,int x = 0,int y = 0,DWORD dwROP = SRCCOPY,HBRUSH hPatternBrush = NULL);
	void GradientFill(COLORREF arrRGB[],int nRGB,int R = 0,int G = 0,int B = 0);
	void Fill(COLORREF RGB);
	void MixFrame(int iMixAmt,CDIBFrame & DIBFore,CDIBFrame & DIBBack);

	// DDB Conversions
	HBITMAP ToDDB();
	void DrawDDBBitmap(HBITMAP hBitmap,int xStart = 0,int yStart = 0,DWORD dwROP = SRCCOPY,HBRUSH hPatternBrush = NULL);
	void DrawDDBTo(CDIBFrame & Dest,int xStart = 0,int yStart = 0,DWORD dwROP = SRCCOPY,HBRUSH hPatternBrush = NULL);
	HBRUSH DIBPatternBrush();

public:
	int m_x,m_y,m_nBitCount;

protected:
	CDC m_FrameDC;
	HBITMAP m_hBkgFrame;
	LPSTR m_pBkgBits;
	HBITMAP m_hLastBkgFrame;
	std::vector<BYTE> m_Buffer;
	BITMAPINFO * m_pBitmapInfo;
	BITMAPINFOHEADER * m_pBitmapInfoHdr;
	int m_nScale;
	bool m_bTrackPalette;
	std::vector<int> m_Palette;
	std::vector<std::vector<BYTE> > m_RGB;
};

class CBitmapIcon
{
	// Wrapper class for the Icon
	class CIcon
	{
	public:
		CIcon() : m_hIcon(NULL), m_iWidth(0), m_iHeight(0) {}
		~CIcon()
		{
			CleanupIcon();
		}

		// Return a handle to the icon
		operator HICON() {return m_hIcon;}
		
		// Create an Icon from two bitmaps
		void CreateIcon(HBITMAP hMaskBitmap,HBITMAP hColorBitmap)
		{
			// Clean up a previous icon
			CleanupIcon();

			// Create an Icon from the "color" and "mask" bitmaps
			m_IconInfo.fIcon = TRUE; // TRUE is icon, FALSE is cursor
			m_IconInfo.xHotspot = 0; // ignored for icon
			m_IconInfo.yHotspot = 0; // ignored for icon
			m_IconInfo.hbmMask = hMaskBitmap;
			m_IconInfo.hbmColor = hColorBitmap;
			m_hIcon = CreateIconIndirect(&m_IconInfo);
			GetDimensions();
		}

		// Load an Icon from a resource
		void LoadIcon(UINT nID)
		{
			CleanupIcon();
			m_hIcon = (HICON)LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(nID),IMAGE_ICON,0,0,LR_DEFAULTCOLOR);
			GetIconInfo(m_hIcon,&m_IconInfo);
			GetDimensions();
		}

		// Clean up the icon
		void CleanupIcon()
		{
			if (m_hIcon)
			{
				DestroyIcon(m_hIcon);
				m_hIcon = NULL;
			}
		}

		void GetDimensions()
		{
			BITMAP BitMap;
			GetObject(m_IconInfo.hbmColor,sizeof(BITMAP),&BitMap);
			m_iWidth = BitMap.bmWidth;
			m_iHeight = BitMap.bmHeight;
		}

		int GetWidth()
		{
			return m_iWidth;
		}

		int GetHeight()
		{
			return m_iHeight;
		}
	private:
		HICON m_hIcon;
		ICONINFO m_IconInfo;
		int m_iWidth,m_iHeight;
	};

public:
	CBitmapIcon();
	CBitmapIcon(UINT nID);
	CBitmapIcon(const CBitmapIcon & rhs);
	~CBitmapIcon();
	CBitmapIcon & operator = (const CBitmapIcon & rhs);

public:
	void Load(UINT nID);
	void LoadIcon(UINT nID);
	UINT GetWidth();
	UINT GetHeight();
	COLORREF GetTransparentColor();
	operator HICON();
	HICON CreateFromBitmap(HBITMAP hBitmap);

protected:
	void Initialize();
	void ConvertBitmap(HBITMAP hBitmap = NULL);

private:
	UINT m_nID;
	CBitmap m_Bitmap;
	COLORREF m_crTransparentColor;
	CIcon m_Icon;
};
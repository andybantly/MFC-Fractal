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
#include "AddText.h"

int CALLBACK FontProc(
	ENUMLOGFONTEX *lpelfe,    // logical-font data
	NEWTEXTMETRICEX *lpntme,  // physical-font data
	DWORD FontType,           // type of font
	LPARAM lParam             // application-defined data
	);

	// CAddText dialog

IMPLEMENT_DYNAMIC(CAddText, CDialog)

CAddText::CAddText(CWnd * pParent /*=NULL*/)
	: CDialog(CAddText::IDD, pParent), m_DecorativeText()
{
	m_csFontName = L"Calibri";
	m_TextColorRGB = RGB(255,255,255);
	m_BackColorRGB = RGB(0,0,0);
	m_iWeight = 0;
	m_bItalics = FALSE;
	m_iOpacity = 50;
	m_iPtSize = 12;
	m_bNoBackColor = FALSE;
	m_bInitDialog = false;
}

CAddText::CAddText(CDecorativeText DecorativeText,CWnd * pParent /*=NULL*/)
: CDialog(CAddText::IDD, pParent)
{
	m_DecorativeText = DecorativeText;
	m_csFontName = DecorativeText.GetFontName();
	m_TextColorRGB = DecorativeText.GetTextRGB();
	m_BackColorRGB = DecorativeText.GetBackRGB();
	m_iWeight = DecorativeText.GetWeight();
	m_bItalics = DecorativeText.GetItalics();
	m_iOpacity = DecorativeText.GetOpacity();
	m_iPtSize = DecorativeText.GetPtSize() / 10;
	m_bNoBackColor = DecorativeText.GetNoBackColor();
	m_csEditText = DecorativeText.GetText();
	m_bInitDialog = false;
}

CAddText::~CAddText()
{
}

// Get the font, colors, and opacity of the editor
void CAddText::GetTextFont(CString & csFontName,int & iWeight,BOOL & bItalics,COLORREF & TextRGB,COLORREF & BackRGB,UINT & iPtSize,UINT & iOpacity,BOOL & bNoBackColor)
{
	// Create the font for the edit control
	csFontName = m_EditText.GetFontName();
	iWeight = m_EditText.GetWeight();
	bItalics = m_EditText.GetItalics();
	TextRGB = m_TextColorRGB;
	BackRGB = m_BackColorRGB;
	iPtSize = m_iPtSize;
	iOpacity = m_iOpacity;
	bNoBackColor = m_bNoBackColor;
}

// Get the text in the editor
CString CAddText::GetText()
{
	return m_csEditText;
}

CDecorativeText CAddText::GetDecorativeText()
{
	return m_DecorativeText;
}

void CAddText::UpdateFont()
{
	m_EditText.UpdateFont(m_csFontName,m_iPtSize,m_iWeight,m_bItalics);
	m_EditText.RedrawWindow();
}

BOOL CAddText::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();

	// Enumerate all styles of all fonts for the ANSI character set
	LOGFONT Logfont;
	Logfont.lfFaceName[0] = '\0';
	Logfont.lfCharSet = DEFAULT_CHARSET;
	HDC hDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);
	EnumFontFamiliesEx(hDC, &Logfont, (FONTENUMPROC)FontProc, (LPARAM)&m_Font, 0);
	DeleteDC(hDC);

	m_Font.SetCurSel(0);
	if (m_Font.FindString(-1,m_csFontName) != LB_ERR)
		m_Font.SetCurSel(m_Font.FindString(-1,m_csFontName));

	m_TextColor.CreateRect(this, IDC_TEXTCOLOR, m_TextColorRGB, _T("Text"));
	m_BackColor.CreateRect(this,IDC_BACKCOLOR,m_BackColorRGB,_T("BackGround"));
	m_EditText.SetTextColorRGB(m_TextColorRGB);
	m_EditText.SetBackColorRGB(m_BackColorRGB);

	m_WeightSlider.SetRange(0, 900);
	m_WeightSlider.SetPos(m_iWeight);
	m_WeightSlider.SetLineSize(100);
	m_WeightSlider.SetPageSize(100);
	m_WeightSlider.SetTic(100);
	m_WeightSlider.SetTicFreq(100);

	// Create the gripper control
	CRect Rect;
	GetClientRect(Rect);
	Rect.left = Rect.right - GetSystemMetrics(SM_CXHSCROLL);
	Rect.top  = Rect.bottom - GetSystemMetrics(SM_CYVSCROLL);
	m_Gripper.Create(WS_CHILD|SBS_SIZEBOX|SBS_SIZEBOXBOTTOMRIGHTALIGN|SBS_SIZEGRIP|WS_VISIBLE,Rect,this,AFX_IDW_SIZE_BOX);
	m_Gripper.SetWindowPos(&wndTop,Rect.left,Rect.top,Rect.Width(),Rect.Height(),0);

	// Get the dialog dimensions
	GetWindowRect(&m_DialogRect);

	// Allow the screen to resized 100% more horizontally and 50% more vertically
	m_ptMinTrack.x = m_DialogRect.Width();
	m_ptMinTrack.y = m_DialogRect.Height();
	m_ptMaxTrack.x = min(m_ptMinTrack.x * 4,GetSystemMetrics(SM_CXSCREEN));
	m_ptMaxTrack.y = min((long)((double)m_ptMinTrack.y * 3),GetSystemMetrics(SM_CYSCREEN));

	// Work in client coordinates
	ScreenToClient(&m_DialogRect);

	// Update the default font
	UpdateFont();

	// Done with initialization
	m_bInitDialog = true;

	return bRet;
}

void CAddText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_EditText);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csEditText);
	DDX_Control(pDX, IDC_FONT, m_Font);
	DDX_CBString(pDX, IDC_FONT, m_csFontName);
	DDX_Control(pDX, IDC_BOLDSLIDER, m_WeightSlider);
	DDX_Check(pDX, IDC_ITALICS, m_bItalics);
	DDX_Text(pDX, IDC_TEXT_OPACITY, m_iOpacity);
	DDV_MinMaxUInt(pDX, m_iOpacity, 0, 100);
	DDX_Text(pDX, IDC_POINTSIZE, m_iPtSize);
	DDV_MinMaxUInt(pDX, m_iPtSize, 1, 75);
	DDX_Check(pDX, IDC_NOBACKGROUND, m_bNoBackColor);
}

BEGIN_MESSAGE_MAP(CAddText, CDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TEXTCOLOR, &CAddText::OnTextColor)
	ON_BN_CLICKED(IDC_BACKCOLOR, &CAddText::OnBackColor)
	ON_CBN_CLOSEUP(IDC_FONT, &CAddText::OnAttribute)
	ON_CBN_SELCHANGE(IDC_FONT, &CAddText::OnAttribute)
	ON_BN_CLICKED(IDC_ITALICS, &CAddText::OnAttribute)
	ON_EN_CHANGE(IDC_TEXT_OPACITY, &CAddText::OnTimedAttribute)
	ON_EN_CHANGE(IDC_POINTSIZE, &CAddText::OnTimedAttribute)
	ON_BN_CLICKED(IDC_NOBACKGROUND, &CAddText::OnAttribute)
	ON_WM_HSCROLL()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

void CAddText::OnGetMinMaxInfo(MINMAXINFO FAR * lpMMI)
{
	// Set the min/max tracking dimensions
	if (!m_bInitDialog)
		return;
	lpMMI->ptMinTrackSize = m_ptMinTrack;
	lpMMI->ptMaxTrackSize = m_ptMaxTrack;
}

void CAddText::OnSize(UINT nType,int cx,int cy)
{
	if (!m_bInitDialog)
		return;

	// Check the gripper window handle
	if (!m_Gripper.GetSafeHwnd())
		return;

	// Turn off drawing
	SetRedraw(FALSE);

	// Get the dialog rectangle
	CRect DialogRect,Rect;
	GetWindowRect(&DialogRect);
	ScreenToClient(&DialogRect);

	// Get the dialog size delta
	CPoint ptDiff(DialogRect.right - m_DialogRect.right,DialogRect.bottom - m_DialogRect.bottom);

	// Resize the text editor
	m_EditText.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	m_EditText.SetWindowPos(NULL,Rect.left,Rect.top,Rect.Width() + ptDiff.x, Rect.Height() + ptDiff.y,SWP_NOZORDER|SWP_NOACTIVATE);

	// Move the font combobox
	m_Font.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	m_Font.SetWindowPos(NULL,Rect.left,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the text color button
	m_TextColor.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	m_TextColor.SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the back color button
	m_BackColor.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	m_BackColor.SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the weight slider
	m_WeightSlider.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	m_WeightSlider.SetWindowPos(NULL, Rect.left, Rect.top + ptDiff.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	// Resize the gripper
	m_Gripper.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	m_Gripper.SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the font text
	CWnd * pWnd = GetDlgItem(IDC_STATIC_FONT);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the bold checkbox
	pWnd = GetDlgItem(IDC_STATIC_WEIGHT);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the italics checkbox
	pWnd = GetDlgItem(IDC_ITALICS);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
		
	// Move the opacity text
	pWnd = GetDlgItem(IDC_STATIC_OPACITY);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
		
	// Move the opacity editor
	pWnd = GetDlgItem(IDC_TEXT_OPACITY);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
		
	// Move the point size text
	pWnd = GetDlgItem(IDC_STATIC_POINTSIZE);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the point size editor
	pWnd = GetDlgItem(IDC_POINTSIZE);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the no background check box
	pWnd = GetDlgItem(IDC_NOBACKGROUND);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Move the OK button
	pWnd = GetDlgItem(IDOK);
	pWnd->GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	pWnd->SetWindowPos(NULL,Rect.left + ptDiff.x,Rect.top + ptDiff.y,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

	// Redraw the resized controls
	SetRedraw();
	Invalidate();
	UpdateWindow();

	// Update the dialog size
	m_DialogRect = DialogRect;
}

// CAddText message handlers
void CAddText::OnTextColor()
{
	if (theApp.ChangeColor(m_TextColorRGB,m_TextColor) == IDOK)
	{
		m_EditText.SetTextColorRGB(m_TextColorRGB);

		// Test for a text color corner case
		if (m_bNoBackColor && m_TextColorRGB == RGB(255,255,255))
		{
			// Adjust background color
			m_BackColorRGB = RGB(0,0,0);
			m_BackColor.SetFillColor(m_BackColorRGB);
			m_EditText.SetBackColorRGB(m_BackColorRGB);
		}
		
		// Update the changes
		m_EditText.RedrawWindow();
	}
}

void CAddText::OnBackColor()
{
	if (theApp.ChangeColor(m_BackColorRGB,m_BackColor) == IDOK)
	{
		m_EditText.SetBackColorRGB(m_BackColorRGB);
		m_EditText.RedrawWindow();
	}
}

// Update the data and redraw the editor
void CAddText::OnAttribute()
{
	if (UpdateData())
	{
		// Toggle the radio buttons
		m_WeightSlider.EnableWindow();

		// Toggle the background button
		m_BackColor.EnableWindow(!m_bNoBackColor);
		if (m_bNoBackColor)
		{
			// Adjust the opacity to the likely intended value
			m_iOpacity = 100;

			// Adjust the foreground color
			if (m_TextColorRGB == RGB(255,255,255))
			{
				m_TextColorRGB = 0;
				m_TextColor.SetFillColor(m_TextColorRGB);
				m_EditText.SetTextColorRGB(m_TextColorRGB);
			}

			// Adjust background color
			m_BackColorRGB = RGB(255,255,255);
			m_BackColor.SetFillColor(m_BackColorRGB);
			m_EditText.SetBackColorRGB(m_BackColorRGB);
			m_EditText.RedrawWindow();

			// Update the screen
			UpdateData(FALSE);
		}

		// Update and redraw the edit control
		UpdateFont();
	}
}

void CAddText::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_BOLDSLIDER)
	{
		m_iWeight = m_WeightSlider.GetPos();
		OnAttribute();
		return;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CAddText::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_FONT)
		lpMeasureItemStruct->itemHeight = 18;
	else
		CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

// Set a timer to ease up on the DDV error messages
void CAddText::OnTimedAttribute()
{
	KillTimer(1);
	SetTimer(1,500,NULL);
}

// Handle the timer so that DDX/DDV can now be carried out
void CAddText::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// Kill the timer
		KillTimer(1);

		// Update the data
		OnAttribute();
	}
}

int CALLBACK FontProc(
  ENUMLOGFONTEX *lpelfe,    // logical-font data
  NEWTEXTMETRICEX *lpntme,  // physical-font data
  DWORD FontType,           // type of font
  LPARAM lParam             // application-defined data
)
{
	CFontComboBox * pcbFont = (CFontComboBox *)lParam;
	CString csFontName;
	CComBSTR ccbFontName;
	if (FontType != TRUETYPE_FONTTYPE)
		ccbFontName = CComBSTR((TCHAR*)&lpelfe->elfLogFont.lfFaceName[0]);
	else
		ccbFontName = CComBSTR((TCHAR*)&lpelfe->elfFullName[0]);
	csFontName = ccbFontName;
	if (csFontName.GetLength())
	{
		if (pcbFont->FindString(-1,csFontName) == LB_ERR)
			pcbFont->AddString(FontType == TRUETYPE_FONTTYPE,lpelfe->elfLogFont.lfCharSet,csFontName);
	}
	return 1;
}
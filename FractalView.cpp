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
#include <math.h>
#include "Fractal.h"
#include "FractalDoc.h"
#include "FractalView.h"
#include "ZoomCoord.h"
#include "MainFrm.h"
#include "AddText.h"
#include "AddPicture.h"
#include "Animate.h"
#include "Winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFractalView
IMPLEMENT_DYNCREATE(CFractalView, CScrollView)

BEGIN_MESSAGE_MAP(CFractalView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CFractalView::OnZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, &CFractalView::OnUpdateZoomOut)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_INITMENUPOPUP() // Necessary for a bug that causes check states not to show correctly
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_IMAGEFILTER_SMOOTH, &CFractalView::OnSmooth)
	ON_COMMAND(ID_CONTEXT_JULIA, &CFractalView::OnJulia)
	ON_COMMAND(ID_CONTEXT_JULIA2X, &CFractalView::OnJulia2X)
	ON_WM_TIMER()
	ON_COMMAND(ID_COLORBLEED_UP, &CFractalView::OnColorBleedUp)
	ON_UPDATE_COMMAND_UI(ID_COLORBLEED_UP, &CFractalView::OnUpdateColorBleedUp)
	ON_COMMAND(ID_COLORBLEED_DOWN, &CFractalView::OnColorBleedDown)
	ON_UPDATE_COMMAND_UI(ID_COLORBLEED_DOWN, &CFractalView::OnUpdateColorBleedDown)
	ON_COMMAND(ID_COLORBLEED_SUBTLEUP, &CFractalView::OnColorBleedSubtleUp)
	ON_UPDATE_COMMAND_UI(ID_COLORBLEED_SUBTLEUP, &CFractalView::OnUpdateColorBleedSubtleUp)
	ON_COMMAND(ID_COLORBLEED_SUBTLEDOWN, &CFractalView::OnColorBleedSubtleDown)
	ON_UPDATE_COMMAND_UI(ID_COLORBLEED_SUBTLEDOWN, &CFractalView::OnUpdateColorBleedSubtleDown)
	ON_COMMAND(ID_COLORBLEED_PALETTEUP, &CFractalView::OnColorBleedPaletteUp)
	ON_UPDATE_COMMAND_UI(ID_COLORBLEED_PALETTEUP, &CFractalView::OnUpdateColorBleedPaletteUp)
	ON_COMMAND(ID_COLORBLEED_PALETTEDOWN, &CFractalView::OnColorBleedPaletteDown)
	ON_UPDATE_COMMAND_UI(ID_COLORBLEED_PALETTEDOWN, &CFractalView::OnUpdateColorBleedPaletteDown)
	ON_COMMAND(ID_DRAGRECTANGLE_ZOOMBOX, &CFractalView::OnDragRectangleZoom)
	ON_UPDATE_COMMAND_UI(ID_DRAGRECTANGLE_ZOOMBOX, &CFractalView::OnUpdateDragRectangleZoom)
	ON_COMMAND(ID_DRAGRECTANGLE_TEXTBOX, &CFractalView::OnDragRectangleText)
	ON_UPDATE_COMMAND_UI(ID_DRAGRECTANGLE_TEXTBOX, &CFractalView::OnUpdateDragRectangleText)
	ON_COMMAND(ID_DRAGRECTANGLE_PICTUREBOX, &CFractalView::OnDragRectanglePicture)
	ON_UPDATE_COMMAND_UI(ID_DRAGRECTANGLE_PICTUREBOX, &CFractalView::OnUpdateDragRectanglePicture)
	ON_COMMAND(ID_CONTEXT_PICTURE_EDIT, &CFractalView::OnEditPicture)
	ON_COMMAND(ID_CONTEXT_PICTURE_DELETE, &CFractalView::OnDeletePicture)
	ON_COMMAND(ID_CONTEXT_TEXT_EDIT, &CFractalView::OnEditText)
	ON_COMMAND(ID_CONTEXT_TEXT_DELETE, &CFractalView::OnDeleteText)
	ON_COMMAND(ID_CONTEXT_EDIT_COLOR, &CFractalView::OnEditColor)
	ON_COMMAND(ID_VIEW_SMALLZOOM, &CFractalView::OnSmallZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLZOOM, &CFractalView::OnUpdateSmallZoom)
	ON_COMMAND(ID_VIEW_CONTINUOUS_SMALLZOOM, &CFractalView::OnContinuousSmallZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTINUOUS_SMALLZOOM, &CFractalView::OnUpdateContinuousSmallZoom)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CFractalView construction/destruction

CFractalView::CFractalView()
{
	// Zoom
	m_bZoomOut = FALSE;
	m_bLButtonDown = FALSE;
	m_bZoomIn = FALSE;
	m_bContinuousSmallZoom = FALSE;

	// Color Animating
	m_pChangeColor = NULL;
	m_pBlendImage = NULL;
	m_bColorBleedUp = FALSE;
	m_bColorBleedDown = FALSE;
	m_bColorBleedSubtleUp = FALSE;
	m_bColorBleedSubtleDown = FALSE;
	m_bColorBleedPaletteUp = FALSE;
	m_bColorBleedPaletteDown = FALSE;

	// Drag rectangle
	m_bDragZoom = TRUE;
	m_bDragText = FALSE;
	m_bDragPicture = FALSE;
	m_bDragPicture = FALSE;

	// Text, picture, and border grabbing
	m_bGrabText = FALSE;
	m_bGrabPicture = FALSE;
	m_bGrabBorder = FALSE;
	m_iGrabPicture = m_iGrabText = 0;
	m_crFGSelect = RGB(255,255,255);
	m_crBGSelect = RGB(0,0,0);
	m_ctClock = clock();
}

CFractalView::~CFractalView()
{
	if (m_pChangeColor)
	{
		delete m_pChangeColor;
		m_pChangeColor = NULL;
	}

	if (m_pBlendImage)
	{
		delete m_pBlendImage;
		m_pBlendImage = NULL;
	}
}

BOOL CFractalView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CFractalView drawing
BOOL CFractalView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

// Handle sizing
void CFractalView::OnSize(UINT nType,int cx,int cy)
{
	// Call the base class to handle the scrolling
	CScrollView::OnSize(nType,cx,cy);

	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Update the view and make sure the view fits
	pDoc->UpdateAllViews(NULL);
}

void CFractalView::OnDraw(CDC* pDC)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Set the new scroll sizes
	CSize FractalSize;
	FractalSize.cx = pDoc->GetWidth();
	FractalSize.cy = pDoc->GetHeight();

	// Handle the changed condition
	if (m_bZoomOut)
	{
		// Zoomed out
		SetScaleToFitSize(FractalSize);
		GetParentFrame()->RecalcLayout();
	}
	else
	{
		// Zoomed in
		SetScrollSizes(MM_TEXT,FractalSize);
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();
	}

	// Get the fractal
	CDIBFrame & DisplayDIB = pDoc->GetFractal();

	// Update the double buffer
	CDIBFrame & DoubleBufferDIB = pDoc->GetDoubleBuffer();
	DisplayDIB.DrawTo(DoubleBufferDIB);

	// Render the pictures
	std::vector<CDecorativePicture> & DecorativePicture = pDoc->GetDecorativePicture();
	std::vector<CDecorativePicture>::iterator itPicture;
	for (itPicture = DecorativePicture.begin();itPicture != DecorativePicture.end();++itPicture)
		DrawDecorativePicture(DoubleBufferDIB,*itPicture);

	// Render the text
	std::vector<CDecorativeText> & DecorativeText = pDoc->GetDecorativeText();
	std::vector<CDecorativeText>::iterator itText;
	for (itText = DecorativeText.begin();itText != DecorativeText.end();++itText)
		DrawDecorativeText(DoubleBufferDIB,*itText);

	// Test for drawing a rectangle
	bool bRect = false;
	CRect Rect;
	if (m_bLButtonDown)
	{
		// Test for dragging out a zoom rectangle
		if (!m_bGrabText && !m_bGrabPicture && !m_bGrabBorder)
		{
			CPoint Pt1 = ScalePoint(m_Pt1);
			CPoint Pt2 = ScalePoint(m_Pt2);
			CRect PtRect(Pt1,Pt2);
			PtRect.NormalizeRect();
			Rect = PtRect;
			bRect = true;
		}
		else if (m_bGrabBorder || m_bGrabPicture || m_bGrabText)
		{
			// Test for dragging a border, picture, or text
			if (m_iGrabPicture)
			{
				std::vector<CDecorativePicture> & vDecorativePicture = pDoc->GetDecorativePicture();
				CDecorativePicture & DecorativePicture = vDecorativePicture[m_iGrabPicture - 1];
				Rect = DecorativePicture.GetRect();
				bRect = true;
			}
			if (!bRect && m_iGrabText)
			{
				std::vector<CDecorativeText> & vDecorativeText = pDoc->GetDecorativeText();
				CDecorativeText & DecorativeText = vDecorativeText[m_iGrabText - 1];
				Rect = DecorativeText.GetRect();
				bRect = true;
			}
		}
	}
	else if (m_bZoomIn) // Test for zooming in to the zoom rectangle
	{
		m_bZoomIn = FALSE;
		CPoint Pt1 = ScalePoint(m_Pt1);
		CPoint Pt2 = ScalePoint(m_Pt2);

		// Zoom into the point
		Zoom(Pt1,Pt2);
	}

	// Test for mousing over an area to show it can be grabbed
	if (!m_bLButtonDown && !bRect)
	{
		int iPicture = 0,iText = 0;
		if (!PointOnBorder(m_Pt2,iPicture,iText))
		{
			iPicture = PointInPicture(m_Pt2);
			iText = PointInText(m_Pt2);
		}

		if (iPicture)
		{
			std::vector<CDecorativePicture> & vDecorativePicture = pDoc->GetDecorativePicture();
			CDecorativePicture & DecorativePicture = vDecorativePicture[iPicture - 1];
			Rect = DecorativePicture.GetRect();
			bRect = true;
		}
		
		if (!bRect && iText)
		{
			std::vector<CDecorativeText> & vDecorativeText = pDoc->GetDecorativeText();
			CDecorativeText & DecorativeText = vDecorativeText[iText - 1];
			Rect = DecorativeText.GetRect();
			bRect = true;
		}
	}

	if (bRect)
	{
		CPen Pen;
		Pen.CreatePen(PS_DOT,1,m_crFGSelect);
		CDC * pDoubleBufferDC = (CDC *)DoubleBufferDIB;
		CPen * pOldPen = pDoubleBufferDC->SelectObject(&Pen);
		pDoubleBufferDC->SetBkColor(m_crBGSelect);
		pDoubleBufferDC->MoveTo(Rect.left,Rect.top);
		pDoubleBufferDC->LineTo(Rect.right,Rect.top);
		pDoubleBufferDC->LineTo(Rect.right,Rect.bottom);
		pDoubleBufferDC->LineTo(Rect.left,Rect.bottom);
		pDoubleBufferDC->LineTo(Rect.left,Rect.top);
		pDoubleBufferDC->SelectObject(pOldPen);
		UpdateSelection();
		SetTimer(2,250,NULL);
	}

	// Render the output
	DoubleBufferDIB.DrawTo(pDC->m_hDC);
}

// Add the custom text to the fractal in the area of the drag rectangle
void CFractalView::DrawDecorativeText(CDC * pDC,CDecorativeText & DecorativeText)
{
	CRect DIBRect = DecorativeText.GetRect();
	CString csFontName = DecorativeText.GetFontName();
	int iWeight = DecorativeText.GetWeight();
	BOOL bItalics = DecorativeText.GetItalics();
	UINT iPtSize = DecorativeText.GetPtSize();
	UINT iOpacity = DecorativeText.GetOpacity();
	COLORREF TextRGB = DecorativeText.GetTextRGB();
	COLORREF BackRGB = DecorativeText.GetBackRGB();
	CString csText = DecorativeText.GetText();
	BOOL bNoBackColor = DecorativeText.GetNoBackColor();

	int x = DIBRect.Width();
	int y = DIBRect.Height();
	if (x == 0 && y == 0)
		return;
	UINT iMaxPtSize = iPtSize;

	// Parse into sentences
	CString csSentence;
	CStringArray csaText;
	int iLastPos = 0,iPos = -1;
	while ((iPos = csText.Find(L"\n",iLastPos)) != -1)
	{
		csSentence = csText.Mid(iLastPos,iPos - iLastPos);
		csaText.Add(csSentence);
		iLastPos = iPos + 1;
	}
	if (iLastPos < csText.GetLength())
	{
		csSentence = csText.Mid(iLastPos);
		csaText.Add(csSentence);
	}

	// Get the number of lines of text
	int nLines = (int)csaText.GetCount();

	// Set background mixing
	BOOL bBkgMix = TRUE;

	// Set the blend level
	UINT iMixAmt = (UINT)((double)iOpacity * 2.55 + 0.5);
	if (iMixAmt > 255)
		iMixAmt = 255;

	// Isolate the full or preview frames
	CDIBFrame DIBFore;
	CDIBFrame DIBBack;
	CDIBFrame DIBMix;

	// Initialize the background image/text frame
	DIBFore.Init(x,y);
	CDC & DCFore = DIBFore;

	// Initialize the backround image frame
	DIBBack.Init(x,y);
	CDC & DCBack = DIBBack;

	// Initialize the mix frame
	DIBMix.Init(x,y);
	CDC & DCMix = DIBMix;

	// Screens rectangle
	CRect Rect(0,0,x,y);

	// Copy the background into the frame for blending
	DCBack.BitBlt(0,0,x,y,pDC,DIBRect.left,DIBRect.top,SRCCOPY);

	// Set the background
	if (bNoBackColor)
		DCMix.BitBlt(0,0,x,y,pDC,DIBRect.left,DIBRect.top,SRCCOPY);
	else
		DCMix.FillSolidRect(&Rect,BackRGB);

	// Mix the frames
	DIBFore.MixFrame(iMixAmt,DIBMix,DIBBack);

	// Create the font for the text
	CFont HorzFont;
	CDC * pForeDC = CDC::FromHandle(DCFore);
	theApp.CreatePointFont(HorzFont, pForeDC, iWeight, bItalics, DEFAULT_CHARSET, DEFAULT_QUALITY, csFontName, iPtSize);

	// Select the font
	CFont * pLastFont = DCFore.SelectObject(&HorzFont);

	// Set the text color and background mode
	COLORREF crTxt = DCFore.SetTextColor(TextRGB);
	DCFore.SetBkMode(TRANSPARENT);

	// Center align the text
	DCFore.SetTextAlign(TA_LEFT);

	// Get the character dimensions
	TEXTMETRIC tm;
	DCFore.GetOutputTextMetrics(&tm);

	// Get the X and Y character sizes
	int cxChar = tm.tmAveCharWidth;
	int cyChar = tm.tmHeight + tm.tmExternalLeading;
	int nLPP = y / cyChar;
	int iTop = y / 2 - nLines * cyChar / 2;

	// Output the text
	for (int iLine = 0;iLine < nLines;++iLine)
	{
		csSentence = csaText.GetAt(iLine);
		CSize Ext = DCFore.GetTextExtent(csSentence);
		int iLeft = x / 2 - Ext.cx / 2;
		DCFore.ExtTextOut(iLeft,iTop,ETO_CLIPPED,&Rect,csSentence,NULL);
		iTop += cyChar;
	}

	// Update the display
	DIBFore.DrawTo(pDC->GetSafeHdc(),DIBRect.left,DIBRect.top);
}

// Add the custom picture to the fractal in the area of the drag rectangle
void CFractalView::DrawDecorativePicture(CDC * pDC,CDecorativePicture & DecorativePicture)
{
	CRect DIBRect = DecorativePicture.GetRect();
	UINT iOpacity = DecorativePicture.GetOpacity();
	CString csPictureName = DecorativePicture.GetPictureName();
	BOOL bTransparentColor = DecorativePicture.GetTransparentColorFlag();
	COLORREF crTransparentColor = DecorativePicture.GetTransparentColor();

	int x = DIBRect.Width();
	int y = DIBRect.Height();
	if (x == 0 && y == 0)
		return;

	// Set background mixing
	BOOL bBkgMix = TRUE;

	// Set the blend level
	UINT iMixAmt = (UINT)((double)iOpacity * 2.55 + 0.5);
	if (iMixAmt > 255)
		iMixAmt = 255;

	// Isolate the full or preview frames
	CDIBFrame DIBFore;
	CDIBFrame DIBBack;
	CDIBFrame DIBPicture;

	// Initialize the background image/text frame
	DIBFore.Init(x,y);
	CDC & DCFore = DIBFore;

	// Initialize the backround image frame
	DIBBack.Init(x,y);
	CDC & DCBack = DIBBack;

	// Screens rectangle
	CRect Rect(0,0,x,y);

	// Copy the background into the frame for blending
	DCBack.BitBlt(0,0,x,y,pDC,DIBRect.left,DIBRect.top,SRCCOPY);

	CImage Image;
	if (SUCCEEDED(Image.Load(csPictureName)))
	{
		DIBPicture.Init(x,y);
		if (bTransparentColor)
		{
			CDIBFrame DIBBackTemp;
			DIBBackTemp.Init(x,y);
			DIBBack.DrawTo(DIBBackTemp);
			Image.TransparentBlt(DIBBackTemp,0,0,x,y,crTransparentColor);
			DIBBackTemp.DrawTo(DIBPicture);
		}
		else
			Image.StretchBlt(DIBPicture,0,0,x,y,SRCCOPY);

		// Mix the image into the frame
		DIBFore.MixFrame(iMixAmt,DIBPicture,DIBBack);
	}

	// Update the display
	DIBFore.DrawTo(pDC->GetSafeHdc(),DIBRect.left,DIBRect.top);
}

// Set the windows cursor with one from the resource or one that is predefined
HCURSOR CFractalView::SetWndCursor(LPTSTR Cursor,BOOL bPredefined)
{
	if (bPredefined)
		m_hCursor = AfxGetApp()->LoadStandardCursor(Cursor);
	else
		m_hCursor = AfxGetApp()->LoadCursor(Cursor);
	return SetCursor(m_hCursor);
}

// Test a client or screen coordinate for intersecting with a border of an image or text
int CFractalView::PointOnBorder(CPoint point,int & iPicture,int & iText)
{
	iPicture = 0;
	iText = 0;

	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	std::vector<CDecorativePicture> & vDecorativePicture = pDoc->GetDecorativePicture();
	std::vector<CDecorativePicture>::iterator itPicture;
	for (itPicture = vDecorativePicture.begin();itPicture != vDecorativePicture.end();++itPicture,++iPicture)
	{
		CDecorativePicture & DecorativePicture = *itPicture;
		CRect Rect = DecorativePicture.GetRect();
		CPoint ptTL = Rect.TopLeft();
		CPoint ptBR = Rect.BottomRight();
		CPoint ptTR = CPoint(ptBR.x,ptTL.y);
		CPoint ptBL = CPoint(ptTL.x,ptBR.y);

		if ((point == ptTL || point == ptBR) || (point == ptTR || point == ptBL) || (ptTL.x == point.x || ptBR.x == point.x) || (ptTL.y == point.y || ptBR.y == point.y))
		{
			iPicture++;
			return 1;
		}
	}
	iPicture = 0;

	std::vector<CDecorativeText> & vDecorativeText = pDoc->GetDecorativeText();
	std::vector<CDecorativeText>::iterator itText;
	for (itText = vDecorativeText.begin();itText != vDecorativeText.end();++itText,++iText)
	{
		CDecorativeText & DecorativeText = *itText;
		CRect Rect = DecorativeText.GetRect();
		CPoint ptTL = Rect.TopLeft();
		CPoint ptBR = Rect.BottomRight();
		CPoint ptTR = CPoint(ptBR.x,ptTL.y);
		CPoint ptBL = CPoint(ptTL.x,ptBR.y);

		if ((point == ptTL || point == ptBR) || (point == ptTR || point == ptBL) || (ptTL.x == point.x || ptBR.x == point.x) || (ptTL.y == point.y || ptBR.y == point.y))
		{
			iText++;
			return 1;
		}
	}
	iText = 0;
	return 0;
}

// Test a client or screen coordinate for intersecting with a block of text
int CFractalView::PointInText(CPoint point)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	int iRet = 0;
	std::vector<CDecorativeText> & vDecorativeText = pDoc->GetDecorativeText();
	std::vector<CDecorativeText>::iterator itText;
	int iText = 0;
	for (itText = vDecorativeText.begin();itText != vDecorativeText.end();++itText,++iText)
	{
		CDecorativeText & DecorativeText = *itText;
		CRect Rect = DecorativeText.GetRect();
		if (Rect.PtInRect(point))
			iRet = iText + 1;
	}
	return iRet;
}

// Test a client or screen coordinate for intersecting with a picture
int CFractalView::PointInPicture(CPoint point)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	int iRet = 0;
	std::vector<CDecorativePicture> & vDecorativePicture = pDoc->GetDecorativePicture();
	std::vector<CDecorativePicture>::iterator itPicture;
	int iPicture = 0;
	for (itPicture = vDecorativePicture.begin();itPicture != vDecorativePicture.end();++itPicture,++iPicture)
	{
		CDecorativePicture & DecorativePicture = *itPicture;
		CRect Rect = DecorativePicture.GetRect();
		if (Rect.PtInRect(point))
			iRet = iPicture + 1;
	}
	return iRet;
}

// Swap the colors to create an ant selection illusion
void CFractalView::UpdateSelection()
{
	clock_t ctClock = clock();
	if (ctClock - m_ctClock > 250)
	{
		std::swap(m_crFGSelect,m_crBGSelect);
		m_ctClock = ctClock;
	}
}

void CFractalView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// Provide a full view of the server with scroll bars
	SetScrollSizes(MM_TEXT,CSize(0,0));

	// Calculate the total size of this view
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Render the fractal to the DIB
	pDoc->RenderFractal();
	pDoc->SetFractalTitle();
}

// CFractalView printing

BOOL CFractalView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFractalView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFractalView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CFractalView diagnostics

#ifdef _DEBUG
void CFractalView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFractalView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFractalDoc* CFractalView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFractalDoc)));
	return (CFractalDoc*)m_pDocument;
}
#endif //_DEBUG


// CFractalView message handlers
// Toggle the zoom
void CFractalView::OnZoomOut()
{
	// Toggle the zoom
	m_bZoomOut = !m_bZoomOut;

	// Calculate the total size of this view
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Update the view and make sure the view fits
	pDoc->UpdateAllViews(NULL);
}

// Toggle the zoom UI (either menu or toolbar)
void CFractalView::OnUpdateZoomOut(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bZoomOut);
}

void CFractalView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Calculate the total size of this view
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get the point
	CPoint Pt = GetScrollPosition() + point;
	m_Pt2 = Pt;

	// Set the cursor
	int iPicture,iText;
	if (PointOnBorder(Pt,iPicture,iText))
	{
		m_hLastCursor = SetWndCursor(IDC_SIZEALL,TRUE);
		m_iGrabPicture = iPicture;
		m_iGrabText = iText;
		m_bGrabBorder = TRUE;
	}
	else if (iPicture = PointInPicture(Pt))
	{
		m_hLastCursor = SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR4));
		m_bGrabPicture = TRUE;
		m_iGrabPicture = iPicture;
	}
	else if (iText = PointInText(Pt))
	{
		m_hLastCursor = SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR4));
		m_bGrabText = TRUE;
		m_iGrabText = iText;
	}
	else if (m_bDragZoom)
		m_hLastCursor = SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR1));
	else if (m_bDragPicture)
		m_hLastCursor = SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR5));
	else if (m_bDragText)
		m_hLastCursor = SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR2));

	if (!m_bLButtonDown)
		m_Pt1 = Pt;
	m_bLButtonDown = TRUE;

	// Track the mouse in case it leaves the document while drawing a zoom rectangle
	TRACKMOUSEEVENT EventTrack;
	EventTrack.cbSize = sizeof(EventTrack);
	EventTrack.dwFlags = TME_LEAVE;
	EventTrack.hwndTrack = m_hWnd;
	EventTrack.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&EventTrack);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CFractalView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Set the cursor
	if (!m_bLButtonDown)
		return;
	if (m_bGrabText || m_bGrabPicture)
	{
		SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR3));
		m_bGrabText = FALSE;
		m_iGrabText = 0;
		m_bGrabPicture = FALSE;
		m_iGrabPicture = 0;
	}
	else
		SetCursor(m_hLastCursor);
	if (m_bGrabBorder)
		m_bGrabBorder = FALSE;
	m_bLButtonDown = FALSE;
	CPoint Pt = GetScrollPosition() + point;
	m_Pt2 = Pt;
	CScrollView::OnLButtonUp(nFlags, point);

	if (!m_bGrabText && !m_bGrabPicture && !m_bGrabBorder)
	{
		CRect Rect(m_Pt1,m_Pt2);
		Rect.NormalizeRect();
		if ((Rect.right - Rect.left) > 2 && (Rect.bottom - Rect.top) > 2)
			m_bZoomIn = TRUE;
	}
	SetWndCursor(IDC_ARROW,1);
	RedrawWindow();
}

void CFractalView::OnMouseMove(UINT nFlags, CPoint point)
{
	// Update the cursor text in the main frame status window
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int iPicture,iText;
	CPoint Pt = GetScrollPosition() + point;
	m_Pt2 = Pt;
	Pt = ScalePoint(Pt);
	
	if (m_bLButtonDown)
	{
		if (m_bGrabBorder)
		{
			SetWndCursor(IDC_SIZEALL,TRUE);
			if (m_iGrabPicture)
			{
				std::vector<CDecorativePicture> & vDecorativePicture = pDoc->GetDecorativePicture();
				CDecorativePicture & DecorativePicture = vDecorativePicture[m_iGrabPicture - 1];
				CRect Rect = DecorativePicture.GetRect();
				CPoint PtDiff = m_Pt2 - m_Pt1;
				CPoint PtCP = Rect.CenterPoint();
				if (m_Pt2.x >= PtCP.x)
					Rect.InflateRect(0,0,PtDiff.x,0);
				else
					Rect.InflateRect(-PtDiff.x,0,0,0);
				if (m_Pt2.y >= PtCP.y)
					Rect.InflateRect(0,0,0,PtDiff.y);
				else
					Rect.InflateRect(0,-PtDiff.y,0,0);
				DecorativePicture.SetRect(Rect);
				m_Pt1 = m_Pt2;
				pDoc->SetModifiedFlag();
				pDoc->SetFractalTitle();
			}
			else if (m_iGrabText)
			{
				std::vector<CDecorativeText> & vDecorativeText = pDoc->GetDecorativeText();
				CDecorativeText & DecorativeText = vDecorativeText[m_iGrabText - 1];
				CRect Rect = DecorativeText.GetRect();
				CPoint PtDiff = m_Pt2 - m_Pt1;
				CPoint PtCP = Rect.CenterPoint();
				if (m_Pt2.x >= PtCP.x)
					Rect.InflateRect(0,0,PtDiff.x,0);
				else
					Rect.InflateRect(-PtDiff.x,0,0,0);
				if (m_Pt2.y >= PtCP.y)
					Rect.InflateRect(0,0,0,PtDiff.y);
				else
					Rect.InflateRect(0,-PtDiff.y,0,0);
				DecorativeText.SetRect(Rect);
				m_Pt1 = m_Pt2;
				pDoc->SetModifiedFlag();
				pDoc->SetFractalTitle();
			}
		}
		else if (m_bGrabText || m_bGrabPicture)
		{
			SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR4));
			if (m_iGrabPicture)
			{
				std::vector<CDecorativePicture> & vDecorativePicture = pDoc->GetDecorativePicture();
				CDecorativePicture & DecorativePicture = vDecorativePicture[m_iGrabPicture - 1];
				CRect Rect = DecorativePicture.GetRect();
				CPoint PtDiff = m_Pt2 - m_Pt1;
				int ix = max(Rect.left + PtDiff.x,0);
				int iy = max(Rect.top + PtDiff.y,0);
				int nx = ix + Rect.Width();
				int ny = iy + Rect.Height();
				CRect PictureRect(ix,iy,nx,ny);
				DecorativePicture.SetRect(PictureRect);
				m_Pt1 = m_Pt2;
				pDoc->SetModifiedFlag();
				pDoc->SetFractalTitle();
			}
			else
			{
				std::vector<CDecorativeText> & vDecorativeText = pDoc->GetDecorativeText();
				CDecorativeText & DecorativeText = vDecorativeText[m_iGrabText - 1];
				CRect Rect = DecorativeText.GetRect();
				CPoint PtDiff = m_Pt2 - m_Pt1;
				int ix = max(Rect.left + PtDiff.x,0);
				int iy = max(Rect.top + PtDiff.y,0);
				int nx = ix + Rect.Width();
				int ny = iy + Rect.Height();
				CRect TextRect(ix,iy,nx,ny);
				DecorativeText.SetRect(TextRect);
				m_Pt1 = m_Pt2;
				pDoc->SetModifiedFlag();
				pDoc->SetFractalTitle();
			}
		}
		else if (m_bDragZoom)
			SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR1));
		else if (m_bDragPicture)
			SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR5));
		else if (m_bDragText)
			SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR2));
	}
	else if (PointOnBorder(Pt,iPicture,iText))
		SetWndCursor(IDC_SIZEALL,TRUE);
	else if (PointInPicture(Pt))
		SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR3));
	else if (PointInText(Pt))
		SetWndCursor(MAKEINTRESOURCE(IDC_CURSOR3));
	else
		SetWndCursor(IDC_ARROW,1);

	CScrollView::OnMouseMove(nFlags, point);
	RedrawWindow();

	int MC = pDoc->GetWidth();
	int MR = pDoc->GetHeight();
	double XMin = pDoc->GetXMin();
	double XMax = pDoc->GetXMax();
	double YMin = pDoc->GetYMin();
	double YMax = pDoc->GetYMax();
	double deltaP = (XMax - XMin) / (double)MC;
	double deltaQ = (YMax - YMin) / (double)MR;

	CString csCoord,csXMin,csYMin,csXMax,csYMax;
	if (!m_bLButtonDown)
	{
		XMin = XMin + (double)Pt.x * deltaP;
		YMin = YMin + (double)Pt.y * deltaQ;
		csXMin.Format(_T("%.15f"),XMin);csXMin.TrimRight(L'0');csXMin.TrimRight(L'.');
		csYMin.Format(_T("%.15f"),YMin);csYMin.TrimRight(L'0');csYMin.TrimRight(L'.');
		csCoord.Format(_T("(%s,%s)\n"),csXMin,csYMin);
	}
	else
	{
		CRect Rect(m_Pt1,m_Pt2);
		Rect.NormalizeRect();
		XMax = XMin + (double)Rect.right * deltaP;
		XMin = XMin + (double)Rect.left * deltaP;
		YMax = YMin + (double)Rect.bottom * deltaQ;
		YMin = YMin + (double)Rect.top * deltaQ;
		csXMin.Format(_T("%.15f"),XMin);csXMin.TrimRight(L'0');csXMin.TrimRight(L'.');
		csYMin.Format(_T("%.15f"),YMin);csYMin.TrimRight(L'0');csYMin.TrimRight(L'.');
		csXMax.Format(_T("%.15f"),XMax);csXMax.TrimRight(L'0');csXMax.TrimRight(L'.');
		csYMax.Format(_T("%.15f"),YMax);csYMax.TrimRight(L'0');csYMax.TrimRight(L'.');
		csCoord.Format(_T("(%s,%s) - (%s,%s)\n"),csXMin,csYMin,csXMax,csYMax);
	}

	CMainFrame * pMainFrame = (CMainFrame *)(AfxGetApp()->GetMainWnd());
	pMainFrame->SetCoord(csCoord);
}

// This event is tripped if zooming and the user leaves the document
void CFractalView::OnMouseLeave()
{
	CScrollView::OnMouseLeave();
	m_bLButtonDown = FALSE;
	RedrawWindow();
}

// Scale a point based on the zoom of the document
CPoint CFractalView::ScalePoint(CPoint Pt)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return Pt;

	// Get the zoom
	if (m_bZoomOut)
	{
		// Calculate the total size of this view
		CSize size,sizeSb;
		GetTrueClientSize(size,sizeSb);
		CRect Client;
		GetClientRect(&Client);

		// Get floating point versions of the coordinates
		double dPointX = (double)Pt.x;
		double dPointY = (double)Pt.y;
		double dWidth = (double)pDoc->GetWidth();
		double dHeight = (double)pDoc->GetHeight();
		double dSizeX = (double)Client.Width();
		double dSizeY = (double)Client.Height();

		// Adjust mouse coordinate to match the zoom out aspect ratio
		dPointX = dWidth / dSizeX * dPointX + 0.5;
		dPointY = dHeight / dSizeY * dPointY + 0.5;

		// Set the scaled point
		Pt.x = (LONG)dPointX;
		Pt.y = (LONG)dPointY;
	}
	return Pt;
}

// Zoom into a rectangular area
void CFractalView::Zoom(CPoint Pt1,CPoint Pt2)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect Rect(Pt1,Pt2);
	Rect.NormalizeRect();

	if (m_bDragPicture)
	{
		CAddPicture AddPicture;
		if (AddPicture.DoModal() == IDOK)
		{
			// Get the picture
			CDecorativePicture DecorativePicture = AddPicture.GetDecorativePicture();
			CImage Image;
			if (SUCCEEDED(Image.Load(DecorativePicture.GetPictureName())))
			{
				// Add the picture to the view
				DecorativePicture.SetRect(Rect);
				pDoc->AddPictureToDoc(DecorativePicture);

				// Update the view to render the text
				pDoc->UpdateAllViews(NULL);
			}
		}
	}
	else if (m_bDragText)
	{
		// Render decorative text into the fractal based on the coordinates of the drag rectangle
		CAddText AddText;
		if (AddText.DoModal() == IDOK)
		{
			// Get the font, color, and opacity for the text
			CString csFontName;
			BOOL bItalics,bNoBackColor;
			int iWeight;
			COLORREF TextRGB,BackRGB;
			UINT iPtSize,iOpacity;
			AddText.GetTextFont(csFontName,iWeight,bItalics,TextRGB,BackRGB,iPtSize,iOpacity,bNoBackColor);
			CString csText = AddText.GetText();
			iPtSize *= 10;

			// Add the text to the view
			CDecorativeText DecorativeText(Rect,csFontName,iWeight,bItalics,iPtSize,iOpacity,TextRGB,BackRGB,bNoBackColor,csText);
			pDoc->AddTextToDoc(DecorativeText);

			// Update the view to render the text
			pDoc->UpdateAllViews(NULL);
		}
	}
	else
	{
		int MC = pDoc->GetWidth();
		int MR = pDoc->GetHeight();
		double XMin = pDoc->GetXMin();
		double XMax = pDoc->GetXMax();
		double YMin = pDoc->GetYMin();
		double YMax = pDoc->GetYMax();
		double deltaP = (XMax - XMin) / (double)MC;
		double deltaQ = (YMax - YMin) / (double)MR;
		XMax = XMin + (double)Rect.right * deltaP;
		XMin = XMin + (double)Rect.left * deltaP;
		YMax = YMin + (double)Rect.bottom * deltaQ;
		YMin = YMin + (double)Rect.top * deltaQ;

		CString csZoomCoord,csXMin,csYMin,csXMax,csYMax;
		csXMin.Format(_T("%.15f"),XMin);csXMin.TrimRight(L'0');csXMin.TrimRight(L'.');
		csYMin.Format(_T("%.15f"),YMin);csYMin.TrimRight(L'0');csYMin.TrimRight(L'.');
		csXMax.Format(_T("%.15f"),XMax);csXMax.TrimRight(L'0');csXMax.TrimRight(L'.');
		csYMax.Format(_T("%.15f"),YMax);csYMax.TrimRight(L'0');csYMax.TrimRight(L'.');
		csZoomCoord.Format(_T("(%s,%s) - (%s,%s)\n"),csXMin,csYMin,csXMax,csYMax);

		// Make sure the Julia coordinate maps into the sphere
		int iFractalType = pDoc->GetFractalType();
		if (theApp.IsJuliaFractal(iFractalType))
		{
			double dP = pDoc->GetP();
			double dQ = pDoc->GetQ();
			if (!(dP >= XMin && dP <= XMax && dQ >= YMin && dQ <= YMax))
			{
				AfxMessageBox(_T("Can't zoom further, try increasing the resolution of the fractal"),MB_ICONINFORMATION);
				return;
			}
		}

		if (m_bZoomOut)
			OnZoomOut();
		if (m_bDragZoom)
		{
			// Zoom into the fractal based on the coordinates of the drag rectangle
			CZoomCoord ZoomCoord;
			ZoomCoord.SetZoomCoord(csZoomCoord);
			INT_PTR Ret = IDOK;
			if (!theApp.IsJuliaFractal(iFractalType))
				Ret = ZoomCoord.DoModal();
			if (Ret == IDOK)
			{
				// Flag which determines whether rendering happens now or during a zoom
				bool bRenderNow = false;

				// Get fractal dialog for this type
				CNewFractal & Fractal = theApp.GetFractalDialog(iFractalType);

				// Get the desired type of zoom
				if (!theApp.IsJuliaFractal(iFractalType) && ZoomCoord.GetChoice() == 2)
				{
					BOOL bFlyPixels = FALSE;
					UINT nFlyFrames = 0;
					if (ZoomCoord.FlyInto())
					{
						bFlyPixels = ZoomCoord.FlyPixels();
						if (!bFlyPixels)
							nFlyFrames = ZoomCoord.FlyFrames();
					}
					else
						nFlyFrames = 1;

					// Check the aspect ratio
					double dY = YMax - YMin;
					double dX = XMax - XMin;
					double dWAspect = fabs(dX / dY);
					double dYAspect = fabs(dY / dX);
					UINT iWidth = (UINT)((double)Fractal.GetHeight() * dWAspect);
					UINT iHeight = (UINT)((double)Fractal.GetWidth() * dYAspect);
					if (iWidth != Fractal.GetWidth() || iHeight != Fractal.GetHeight())
					{
						// Always make the adjustment bigger than the drag rectangle to show more with a slightly less zoom factor
						if (iWidth != Fractal.GetWidth() && iWidth < Fractal.GetWidth())
						{
							// Ask to adjust the width aspect ratio to match the fractal
							CString csAdjust;
							csAdjust.Format(L"Adjust the width from %d to %d to match the Fractals current aspect ratio?",Fractal.GetWidth(),iWidth);
							int iAdjust = AfxMessageBox(csAdjust,MB_YESNO);
							if (iAdjust == IDYES)
							{
								pDoc->SetWidth(iWidth);
								Fractal.SetWidth(iWidth);
							}
						}
						else
						{
							// Ask to adjust the height aspect ratio to match the fractal
							CString csAdjust;
							csAdjust.Format(L"Adjust the height from %d to %d to match the Fractals current aspect ratio?",Fractal.GetHeight(),iHeight);
							int iAdjust = AfxMessageBox(csAdjust,MB_YESNO);
							if (iAdjust == IDYES)
							{
								pDoc->SetHeight(iHeight);
								Fractal.SetHeight(iHeight);
							}
						}
					}
					
					if (nFlyFrames)
					{
						// Start the wait cursor
						BeginWaitCursor();

						// Set the open flag so not to clobber the color table
						pDoc->SetOpen(TRUE);

						// Get the current coordinates
						double dLastXMin = pDoc->GetXMin();
						double dLastXMax = pDoc->GetXMax();
						double dLastYMin = pDoc->GetYMin();
						double dLastYMax = pDoc->GetYMax();

						// Zoom the frames to the target coordinates
						double dXMinSteps = abs((dLastXMin - XMin) / (double)nFlyFrames);
						double dXMaxSteps = abs((dLastXMax - XMax) / (double)nFlyFrames);
						double dYMinSteps = abs((dLastYMin - YMin) / (double)nFlyFrames);
						double dYMaxSteps = abs((dLastYMax - YMax) / (double)nFlyFrames);

						// Set the initial zoom steps
						dLastXMin += dXMinSteps;
						dLastXMax -= dXMaxSteps;
						dLastYMin += dYMinSteps;
						dLastYMax -= dYMaxSteps;

						// Set the initial values for zooming during OnTimer
						m_dLastXMin = dLastXMin;
						m_dLastXMax = dLastXMax;
						m_dLastYMin = dLastYMin;
						m_dLastYMax = dLastYMax;
						m_dXMinSteps = dXMinSteps;
						m_dXMaxSteps = dXMaxSteps;
						m_dYMinSteps = dYMinSteps;
						m_dYMaxSteps = dYMaxSteps;
						m_XMin = XMin;
						m_XMax = XMax;
						m_YMin = YMin;
						m_YMax = YMax;
						m_nFlyFrames = nFlyFrames;

						// Start the zoom timer
						SetTimer(3,0,NULL);
					}
					else if (bFlyPixels)
					{
						// Set the destination rectangle
						m_Rect = Rect;
					}
					else
						bRenderNow = true;
				}
				else
					bRenderNow = true;

				if (bRenderNow)
				{
					// Set the current document properties
					Fractal.SetWidth(pDoc->GetWidth());
					Fractal.SetHeight(pDoc->GetHeight());
					Fractal.SetIterations(pDoc->GetIterations());
					Fractal.SetMaxCalc(pDoc->GetMaxCalc());
					Fractal.SetRed(pDoc->GetRed());
					Fractal.SetGreen(pDoc->GetGreen());
					Fractal.SetBlue(pDoc->GetBlue());
					Fractal.SetDropUnused(pDoc->GetDropUnused());
					Fractal.SetSmooth(pDoc->GetSmooth());
					Fractal.SetSortOrder(pDoc->GetSortOrder());
					Fractal.SetGradient(pDoc->GetGradient());
					Fractal.SetBase(pDoc->GetBase());
					Fractal.SetBaseColor(pDoc->GetBaseColor());
					Fractal.SetOrbitColor(pDoc->GetOrbitColor());
					Fractal.SetRGB1(pDoc->GetRGB1());
					Fractal.SetRGB2(pDoc->GetRGB2());
					Fractal.SetRGB3(pDoc->GetRGB3());
					Fractal.SetRGB4(pDoc->GetRGB4());
					Fractal.SetRGB5(pDoc->GetRGB5());
					Fractal.SetRGB6(pDoc->GetRGB6());
					Fractal.SetRGBBase(pDoc->GetRGBBase());
					Fractal.SetRR(pDoc->GetRR());
					Fractal.SetRG(pDoc->GetRG());
					Fractal.SetRB(pDoc->GetRB());

					// Set the zoomed coordinates
					Fractal.SetXMin(XMin);
					Fractal.SetXMax(XMax);
					Fractal.SetYMin(YMin);
					Fractal.SetYMax(YMax);

					// Request a new fractal
					theApp.SetZoomNew(iFractalType);
					AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_FILE_NEW);
				}
			}
		}
	}

	// Update the view
	RedrawWindow();
}

void CFractalView::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CScrollView::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

// Handle the context sensitive menu
void CFractalView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Don't allow K/B context menu
	if (point.x == -1 && point.y == -1)
		return;

	// Scale the point
	point = ScalePoint(point);

	// Load the context sensitive menu
	CMenu ContextMenu;
	int iFractalType = pDoc->GetFractalType();
	if (theApp.IsJuliaFractal(iFractalType))
	{
		if (!ContextMenu.LoadMenu(IDR_CSM_JULIA))
			return;
	}
	else
	{
		if (!ContextMenu.LoadMenu(IDR_CSM_VIEW))
			return;
	}

	// Get the context menu that will be displayed
	CMenu * pPopupMenu = ContextMenu.GetSubMenu(0);
	if (!pPopupMenu)
		return;

	// Check for intersecting text to edit or delete
	pPopupMenu->EnableMenuItem(4,MF_BYPOSITION | MF_DISABLED);
	pPopupMenu->EnableMenuItem(5,MF_BYPOSITION | MF_DISABLED);
	m_ClientPoint = point;
	ScreenToClient(&m_ClientPoint);
	m_ClientPoint += GetScrollPosition();

	// Test if a client coordinate intersects a picture or text block
	if (PointInPicture(m_ClientPoint))
	{
		// Enable the text menu options
		pPopupMenu->EnableMenuItem(5,MF_BYPOSITION | MF_ENABLED);
		CMenu * pPictureMenu = pPopupMenu->GetSubMenu(5);
		if (pPictureMenu)
		{
			pPictureMenu->EnableMenuItem(0,MF_BYPOSITION | MF_ENABLED);
			pPictureMenu->EnableMenuItem(1,MF_BYPOSITION | MF_ENABLED);
		}
	}
	else if (PointInText(m_ClientPoint))
	{
		// Enable the text menu options
		pPopupMenu->EnableMenuItem(4,MF_BYPOSITION | MF_ENABLED);
		CMenu * pTextMenu = pPopupMenu->GetSubMenu(4);
		if (pTextMenu)
		{
			pTextMenu->EnableMenuItem(0,MF_BYPOSITION | MF_ENABLED);
			pTextMenu->EnableMenuItem(1,MF_BYPOSITION | MF_ENABLED);
		}
	}

	// Display the menu and allow the view framework to handle the messages
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
}

// Animate the colors with a bleeding effect
void CFractalView::OnColorBleedUp()
{
	if (m_bColorBleedUp)
	{
		m_bColorBleedUp = FALSE;
		StopTimer(ID_COLORBLEED_UP);
	}
	else
	{
		if (m_bColorBleedDown)
			OnColorBleedDown();
		if (m_bColorBleedSubtleUp)
			OnColorBleedSubtleUp();
		if (m_bColorBleedSubtleDown)
			OnColorBleedSubtleDown();
		if (m_bColorBleedPaletteUp)
			OnColorBleedPaletteUp();
		if (m_bColorBleedPaletteDown)
			OnColorBleedPaletteDown();
		m_bColorBleedUp = TRUE;

		// Start the timer
		StartTimer(ID_COLORBLEED_UP);
	}
}

// Toggle the animate check
void CFractalView::OnUpdateColorBleedUp(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bColorBleedUp);
}

// Animate the colors with a bleeding effect
void CFractalView::OnColorBleedDown()
{
	if (m_bColorBleedDown)
	{
		m_bColorBleedDown = FALSE;
		StopTimer(ID_COLORBLEED_DOWN);
	}
	else
	{
		if (m_bColorBleedUp)
			OnColorBleedUp();
		if (m_bColorBleedSubtleUp)
			OnColorBleedSubtleUp();
		if (m_bColorBleedSubtleDown)
			OnColorBleedSubtleDown();
		if (m_bColorBleedPaletteUp)
			OnColorBleedPaletteUp();
		if (m_bColorBleedPaletteDown)
			OnColorBleedPaletteDown();
		m_bColorBleedDown = TRUE;

		// Start the timer
		StartTimer(ID_COLORBLEED_DOWN);
	}
}

// Toggle the animate check
void CFractalView::OnUpdateColorBleedDown(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bColorBleedDown);
}

// Animate the colors with a subtle bleeding effect
void CFractalView::OnColorBleedSubtleUp()
{
	if (m_bColorBleedSubtleUp)
	{
		m_bColorBleedSubtleUp = FALSE;
		StopTimer(ID_COLORBLEED_SUBTLEUP);
	}
	else
	{
		if (m_bColorBleedUp)
			OnColorBleedUp();
		if (m_bColorBleedDown)
			OnColorBleedDown();
		if (m_bColorBleedSubtleDown)
			OnColorBleedSubtleDown();
		if (m_bColorBleedPaletteUp)
			OnColorBleedPaletteUp();
		if (m_bColorBleedPaletteDown)
			OnColorBleedPaletteDown();
		m_bColorBleedSubtleUp = TRUE;

		// Start the timer
		StartTimer(ID_COLORBLEED_SUBTLEUP);
	}
}

// Toggle the animate check
void CFractalView::OnUpdateColorBleedSubtleUp(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bColorBleedSubtleUp);
}

// Animate the colors with a subtle bleeding effect
void CFractalView::OnColorBleedSubtleDown()
{
	if (m_bColorBleedSubtleDown)
	{
		m_bColorBleedSubtleDown = FALSE;
		StopTimer(ID_COLORBLEED_SUBTLEDOWN);
	}
	else
	{
		if (m_bColorBleedUp)
			OnColorBleedUp();
		if (m_bColorBleedDown)
			OnColorBleedDown();
		if (m_bColorBleedSubtleUp)
			OnColorBleedSubtleUp();
		if (m_bColorBleedPaletteUp)
			OnColorBleedPaletteUp();
		if (m_bColorBleedPaletteDown)
			OnColorBleedPaletteDown();
		m_bColorBleedSubtleDown = TRUE;

		// Start the timer
		StartTimer(ID_COLORBLEED_SUBTLEDOWN);
	}
}

// Toggle the animate check
void CFractalView::OnUpdateColorBleedSubtleDown(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bColorBleedSubtleDown);
}

// Animate the colors with a palette shifting effect
void CFractalView::OnColorBleedPaletteUp()
{
	if (m_bColorBleedPaletteUp)
	{
		m_bColorBleedPaletteUp = FALSE;
		StopTimer(ID_COLORBLEED_PALETTEUP);
	}
	else
	{
		if (m_bColorBleedUp)
			OnColorBleedUp();
		if (m_bColorBleedDown)
			OnColorBleedDown();
		if (m_bColorBleedSubtleUp)
			OnColorBleedSubtleUp();
		if (m_bColorBleedSubtleDown)
			OnColorBleedSubtleDown();
		if (m_bColorBleedPaletteDown)
			OnColorBleedPaletteDown();
		m_bColorBleedPaletteUp = TRUE;

		// Start the timer
		StartTimer(ID_COLORBLEED_PALETTEUP);
	}
}

// Toggle the animate check
void CFractalView::OnUpdateColorBleedPaletteUp(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bColorBleedPaletteUp);
}

// Animate the colors with a palette shifting effect
void CFractalView::OnColorBleedPaletteDown()
{
	if (m_bColorBleedPaletteDown)
	{
		m_bColorBleedPaletteDown = FALSE;
		StopTimer(ID_COLORBLEED_PALETTEDOWN);
	}
	else
	{
		if (m_bColorBleedUp)
			OnColorBleedUp();
		if (m_bColorBleedDown)
			OnColorBleedDown();
		if (m_bColorBleedSubtleUp)
			OnColorBleedSubtleUp();
		if (m_bColorBleedSubtleDown)
			OnColorBleedSubtleDown();
		if (m_bColorBleedPaletteUp)
			OnColorBleedPaletteUp();
		m_bColorBleedPaletteDown = TRUE;

		// Start the timer
		StartTimer(ID_COLORBLEED_PALETTEDOWN);
	}
}

// Toggle the animate check
void CFractalView::OnUpdateColorBleedPaletteDown(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bColorBleedPaletteDown);
}

// Zoom by the smallest increment
void CFractalView::OnSmallZoom()
{
	CWaitCursor Wait;
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_bDragZoom = TRUE;
	m_bDragText = FALSE;
	m_bDragPicture = FALSE;

	int MC = pDoc->GetWidth();
	int MR = pDoc->GetHeight();

	// Zoom in by 1 pixel
	CPoint Pt1(1,1);
	CPoint Pt2(MC - 1,MR - 1);
	CRect Rect(Pt1,Pt2);
	Rect.NormalizeRect();

	// Calculate the coordinate scalars
	double dLastXMin,XMin;
	double dLastXMax,XMax;
	double dLastYMin,YMin;
	double dLastYMax,YMax;

	dLastXMin = XMin = pDoc->GetXMin();
	dLastXMax = XMax = pDoc->GetXMax();
	dLastYMin = YMin = pDoc->GetYMin();
	dLastYMax = YMax = pDoc->GetYMax();
	double deltaP = (XMax - XMin) / (double)MC;
	double deltaQ = (YMax - YMin) / (double)MR;

	// Calculate the new bounding coordinates
	XMax = XMin + (double)Rect.right * deltaP;
	XMin = XMin + (double)Rect.left * deltaP;
	YMax = YMin + (double)Rect.bottom * deltaQ;
	YMin = YMin + (double)Rect.top * deltaQ;

	// Set the open flag so not to clobber the color table
	pDoc->SetOpen(TRUE);

	// Zoom the frames to the target coordinates
	double dXMinSteps = abs(dLastXMin - XMin);
	double dXMaxSteps = abs(dLastXMax - XMax);
	double dYMinSteps = abs(dLastYMin - YMin);
	double dYMaxSteps = abs(dLastYMax - YMax);

	// Set the initial zoom steps
	dLastXMin += dXMinSteps;
	dLastXMax -= dXMaxSteps;
	dLastYMin += dYMinSteps;
	dLastYMax -= dYMaxSteps;

	// Zoomed coordinates
	pDoc->SetXMin(dLastXMin);
	pDoc->SetXMax(dLastXMax);
	pDoc->SetYMin(dLastYMin);
	pDoc->SetYMax(dLastYMax);

	// Render the small zoomed fractal to the current view
	pDoc->Render();

	// Update the view
	RedrawWindow();
	InvalidateRect(NULL,TRUE);
}

void CFractalView::OnUpdateSmallZoom(CCmdUI *pCmdUI)
{
}

void CFractalView::OnContinuousSmallZoom()
{
	// Start/Stop the timer for continuous small zoom
	m_bContinuousSmallZoom = !m_bContinuousSmallZoom;
	if (m_bContinuousSmallZoom)
		SetTimer(4,0,NULL);
}

void CFractalView::OnUpdateContinuousSmallZoom(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bContinuousSmallZoom);
}

// Toggle the use of the drag rectangle as a zoom tool
void CFractalView::OnDragRectangleZoom()
{
	m_bDragZoom = TRUE;
	m_bDragText = FALSE;
	m_bDragPicture = FALSE;
}

void CFractalView::OnUpdateDragRectangleZoom(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bDragZoom);
}

// Toggle the use of the drag rectangle as a text tool
void CFractalView::OnDragRectangleText()
{
	m_bDragZoom = FALSE;
	m_bDragText = TRUE;
	m_bDragPicture = FALSE;
}

void CFractalView::OnUpdateDragRectangleText(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bDragText);
}

void CFractalView::OnDragRectanglePicture()
{
	m_bDragZoom = FALSE;
	m_bDragText = FALSE;
	m_bDragPicture = TRUE;
}

void CFractalView::OnUpdateDragRectanglePicture(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_bDragPicture);
}

// Start the timer
void CFractalView::StartTimer(UINT_PTR nIDEvent)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CAnimate Animate;
	if (Animate.DoModal() == IDOK)
	{
		UINT uiFPS = Animate.m_uiFPS;
		if (Animate.m_bRecord)
		{
			// Start recording
			m_AVI.Open(uiFPS);
		}

		// Create the color changer
		ASSERT(m_pChangeColor == NULL);
		m_pChangeColor = new CChangeColor(pDoc->GetFractal(),pDoc->GetBaseImage(),pDoc->GetFractalImage(),nIDEvent);

		// Check for image blending
		BOOL bUseImage = pDoc->GetUseImage();
		BOOL bUseImage2 = pDoc->GetUseImage2();
		BOOL bBlendBase = pDoc->GetBlendBase();
		BOOL bBlendFractal = pDoc->GetBlendFractal();
		BOOL bBlendBaseDIB = bUseImage & bBlendBase;
		BOOL bBlendFracDIB = bUseImage2 & bBlendFractal;
		if (bBlendBaseDIB || bBlendFracDIB)
		{
			// Multi-threaded blend with the images
			CDIBFrame & DisplayDIB = pDoc->GetFractal();
			CDIBFrame & BaseDIB = pDoc->GetBaseImage();
			CDIBFrame & FractalDIB = pDoc->GetFractalImage();
			int iAlpha = pDoc->GetAlpha();
			int iAlpha2 = pDoc->GetAlpha2();
			ASSERT(m_pBlendImage == NULL);
			m_pBlendImage = new	CChangeColor(DisplayDIB,BaseDIB,FractalDIB,ID_CONTEXT_BLEND_COLOR,0,bBlendBaseDIB,bBlendFracDIB,iAlpha,iAlpha2);
		}

		// Initialize the FPS clock
		m_fpsClock = clock();
		double dfps = (double)uiFPS;
		m_fpsDelta = CLOCKS_PER_SEC / dfps;

		// Set the timer
		SetTimer(nIDEvent,0,NULL);
	}
}

// Stop the timer
void CFractalView::StopTimer(UINT_PTR nIDEvent)
{
	// Kill the timer
	KillTimer(nIDEvent);

	// Set "Save as AVI" timer
	if (m_AVI.IsOpen())
		SetTimer(1,0,NULL);

	// Delete the color changer for a different event
	if (m_pChangeColor)
	{
		delete m_pChangeColor;
		m_pChangeColor = NULL;
	}

	// Delete the image blender for a different event
	if (m_pBlendImage)
	{
		delete m_pBlendImage;
		m_pBlendImage = NULL;
	}
}

// Cycle the colors
void CFractalView::OnTimer(UINT_PTR nIDEvent)
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (nIDEvent == 1)
	{
		// Kill the timer
		KillTimer(nIDEvent);

		// Stop recording
		CString csFileName = m_AVI.Close();
		CFileStatus FileStatus;
		if (CFile::GetStatus(csFileName,FileStatus))
		{
			CString csMsg;
			csMsg.Format(L"Do you wish to keep the recording?");
			if (AfxMessageBox(csMsg,MB_YESNO) == IDYES)
			{
				// Call the custom save dialog
				CString csSaveFileName = csFileName.Left(csFileName.GetLength() - 4);;
				csSaveFileName += _T(".avi");
				CFractalFileDialog FileSave(FALSE,_T("Save As AVI"),_T(".avi"),csSaveFileName,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
					_T("AVI Files (*.avi)|*.avi||"),TRUE,AfxGetApp()->GetMainWnd());
				if (FileSave.DoModal() == IDOK)
				{
					CWaitCursor wait;
					POSITION Pos = FileSave.GetStartPosition();
					MoveFile(csFileName,FileSave.GetNextPathName(Pos));
				}
				else
					CFile::Remove(csFileName);
			}
			else
				CFile::Remove(csFileName);
		}
		return;
	}
	else if (nIDEvent == 2)
	{
		// Swap the colors to create an ant selection illusion
		UpdateSelection();

		// Kill the timer (Drawing will start it again so we don't do that here)
		KillTimer(nIDEvent);
	}
	else if (nIDEvent == 3)
	{
		// Kill the timer (Possibly going to start it again if more zooming is necessary)
		KillTimer(nIDEvent);

		// Restore the wait cursor in case it has changed
		RestoreWaitCursor();

		// Get the fractal type and height
		int iFractalType = pDoc->GetFractalType();
		CNewFractal & Fractal = theApp.GetFractalDialog(iFractalType);
		int iHeight = Fractal.GetHeight();

		// Zoomed coordinates
		pDoc->SetXMin(m_dLastXMin);
		pDoc->SetXMax(m_dLastXMax);
		pDoc->SetYMin(m_dLastYMin);
		pDoc->SetYMax(m_dLastYMax);

		// Set the correct aspect ratio
		double dY = m_dLastYMax - m_dLastYMin;
		double dX = m_dLastXMax - m_dLastXMin;
		double dAspect = fabs(dX / dY);
		int iNewWidth = (int)((double)iHeight * dAspect);
		pDoc->SetWidth(iNewWidth);

		// Render the new fractal to the current view
		pDoc->RenderZoom(m_dLastXMin,m_dLastXMax,m_dLastYMin,m_dLastYMax,iNewWidth);

		// Update the view
		RedrawWindow();
		InvalidateRect(NULL,TRUE);

		// Set the next frames coordinates
		m_dLastXMin += m_dXMinSteps;
		m_dLastXMax -= m_dXMaxSteps;
		m_dLastYMin += m_dYMinSteps;
		m_dLastYMax -= m_dYMaxSteps;
		
		if (--m_nFlyFrames)
		{
			// Set the timer for the next frame
			SetTimer(nIDEvent,100,NULL);
		}
		else
		{
			// Zoomed coordinates
			pDoc->SetXMin(m_XMin);
			pDoc->SetXMax(m_XMax);
			pDoc->SetYMin(m_YMin);
			pDoc->SetYMax(m_YMax);

			// Set the zoomed coordinates to the dialog
			Fractal.SetXMin(m_XMin);
			Fractal.SetXMax(m_XMax);
			Fractal.SetYMin(m_YMin);
			Fractal.SetYMax(m_YMax);

			// Render the new fractal to the current view
			pDoc->Render();
			pDoc->SetModifiedFlag();
			pDoc->SetFractalTitle();

			// End the wait cursor
			EndWaitCursor();
		}
	}
	else if (nIDEvent == 4)
	{
		// Kill the timer (Possibly going to start it again if more zooming is necessary)
		KillTimer(nIDEvent);
		if (m_bContinuousSmallZoom)
		{
			// Zoom by the smallest increment
			CWaitCursor Wait;
			OnSmallZoom();

			// Start the zoom timer
			SetTimer(4,0,NULL);
		}
	}

	// Carry out the multi-threaded color effect
	if (m_pChangeColor)
	{
		double fpsFrame = (double)(clock() - m_fpsClock);
		if (fpsFrame > m_fpsDelta)
		{
			// Record the frame
			m_AVI.AddFrame(pDoc->GetFractal());

			// Update the frame
			m_pChangeColor->ChangeColor();

			// Check for image blending
			if (m_pBlendImage)
				m_pBlendImage->ChangeColor();

			// Update the FPS clock
			m_fpsClock = clock();
		}
	}

	// Redraw the fractal
	pDoc->UpdateAllViews(NULL);
}

// Smooth the image with a 9 point low pass filter
void CFractalView::OnSmooth()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Show the wait cursor
	CWaitCursor wait;

	// Get the width and height
	int MC = pDoc->GetWidth();
	int MR = pDoc->GetHeight();

	// Source
	CDIBFrame & DisplayDIB = pDoc->GetFractal();

	// Low Pass Filter DIB
	CDIBFrame LowPassFilter = DisplayDIB;

	// Smooth the fractal image
	for (int C = 1;C < MC - 1;++C)
	{
		for (int R = 1;R < MR - 1;++R)
		{
			// Color
			BYTE RR,GG,BB;
			int RA,GA,BA;
			RA = GA = BA = 0;

			// Matrix
			int CS = -1,CE = 1;
			int RS = -1,RE = 1;

			// Sum the colors in the 3x3 matrix around the point
			int nPoints = 0;
			for (int C1 = CS;C1 <= CE;C1++)
			{
				for (int R1 = RS;R1 <= RE;R1++)
				{
					DisplayDIB.GetPixel(C + C1,R + R1,RR,GG,BB);
					RA += RR;
					GA += GG;
					BA += BB;
					nPoints++;
				}
			}

			// Average the color for the point
			RA /= nPoints;
			GA /= nPoints;
			BA /= nPoints;

			// Set the new smoothed color
			LowPassFilter.SetPixel(C,R,RA,GA,BA);
		}
	}

	// Update the image
	DisplayDIB = LowPassFilter;

	// Redraw the fractal
	pDoc->UpdateAllViews(NULL);
}

// Draw the Julia Set for the point
void CFractalView::OnJulia()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get the point for the Julia Set
	CPoint Pt1 = m_Pt2;

	// Get the width and height
	int MC = pDoc->GetWidth();
	int MR = pDoc->GetHeight();

	// Set the Julia new fractal dialog
	int iFractalType = pDoc->GetFractalType();
	iFractalType = theApp.GetMandelJulia(iFractalType);
	CNewFractal & Fractal = theApp.GetFractalDialog(iFractalType);
	theApp.SetLastChoice(iFractalType);

	// Set the current document properties
	Fractal.SetFractalType(iFractalType);
	Fractal.SetWidth(MC);
	Fractal.SetHeight(MR);
	Fractal.SetIterations(pDoc->GetIterations());
	Fractal.SetMaxCalc(pDoc->GetMaxCalc());
	Fractal.SetRed(pDoc->GetRed());
	Fractal.SetGreen(pDoc->GetGreen());
	Fractal.SetBlue(pDoc->GetBlue());
	Fractal.SetDropUnused(pDoc->GetDropUnused());
	Fractal.SetSmooth(pDoc->GetSmooth());
	Fractal.SetSortOrder(pDoc->GetSortOrder());
	Fractal.SetGradient(pDoc->GetGradient());
	Fractal.SetBase(pDoc->GetBase());
	Fractal.SetBaseColor(pDoc->GetBaseColor());
	Fractal.SetOrbitColor(pDoc->GetOrbitColor());
	Fractal.SetRGB1(pDoc->GetRGB1());
	Fractal.SetRGB2(pDoc->GetRGB2());
	Fractal.SetRGB3(pDoc->GetRGB3());
	Fractal.SetRGB4(pDoc->GetRGB4());
	Fractal.SetRGB5(pDoc->GetRGB5());
	Fractal.SetRGB6(pDoc->GetRGB6());
	Fractal.SetRGBBase(pDoc->GetRGBBase());
	Fractal.SetRR(pDoc->GetRR());
	Fractal.SetRG(pDoc->GetRG());
	Fractal.SetRB(pDoc->GetRB());
	Fractal.SetTrapX(pDoc->GetTrapX());
	Fractal.SetTrapXValue(pDoc->GetTrapXValue());
	Fractal.SetTrapY(pDoc->GetTrapY());
	Fractal.SetTrapYValue(pDoc->GetTrapYValue());
	Fractal.SetGCR(pDoc->GetGCR());
	Fractal.SetGCRValue(pDoc->GetGCRValue());
	Fractal.SetGCG(pDoc->GetGCG());
	Fractal.SetGCGValue(pDoc->GetGCGValue());
	Fractal.SetGCB(pDoc->GetGCB());
	Fractal.SetGCBValue(pDoc->GetGCBValue());

	// Calculate the P and Q values
	double XMin = pDoc->GetXMin();
	double XMax = pDoc->GetXMax();
	double YMin = pDoc->GetYMin();
	double YMax = pDoc->GetYMax();
	double deltaP = (XMax - XMin) / (double)MC;
	double deltaQ = (YMax - YMin) / (double)MR;
	double dP = XMin + (double)Pt1.x * deltaP;
	double dQ = YMin + (double)Pt1.y * deltaQ;

	// Julia has only a coordinate that we expand around
	Fractal.SetP(dP);
	Fractal.SetQ(dQ);

	// Request a new julia fractal
	theApp.SetZoomNew(iFractalType);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_FILE_NEW);
}

// Draw the Julia Set for the point
void CFractalView::OnJulia2X()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Calculate the new rectangle
	double XMin = pDoc->GetXMin();
	double XMax = pDoc->GetXMax();
	double YMin = pDoc->GetYMin();
	double YMax = pDoc->GetYMax();

	// Get the focal point
	double dP = pDoc->GetP();
	double dQ = pDoc->GetQ();

	// Zoom 2x by shrinking the rectangle in
	double dXOff = (XMax - XMin) / 4.0;
	double dYOff = (YMax - YMin) / 4.0;
	double XMin1 = XMin + dXOff;
	double XMax1 = XMax - dXOff;
	double YMin1 = YMin + dYOff;
	double YMax1 = YMax - dYOff;

	// Clip the coordinates
	if (XMin1 < XMin)
		XMin1 = XMin;
	if (YMin1 < YMin)
		YMin1 = YMin;
	if (XMax1 > XMax)
		XMax1 = XMax;
	if (YMax1 > YMax)
		YMax1 = YMax;

	// Test for a good zoom
	if (dP >= XMin1 && dP <= XMax1 && dQ >= YMin1 && dQ <= YMax1)
	{
		int iFractalType = pDoc->GetFractalType();
		CNewFractal & Fractal = theApp.GetFractalDialog(iFractalType);

		// Set the coordinates
		Fractal.SetXMin(XMin1);
		Fractal.SetXMax(XMax1);
		Fractal.SetYMin(YMin1);
		Fractal.SetYMax(YMax1);

		// Request a new julia fractal
		theApp.SetZoomNew(iFractalType);
		AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_FILE_NEW);
	}
	else
		AfxMessageBox(_T("Can't zoom further, try increasing the resolution of the fractal"),MB_ICONINFORMATION);
}

// Edit the decorative text
void CFractalView::OnEditText()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Rediscover the selected text
	std::vector<CDecorativeText> & vDecorativeText = GetDocument()->GetDecorativeText();
	std::vector<CDecorativeText>::iterator itText;
	int iText = 0;
	CDecorativeText DecorativeText;
	for (itText = vDecorativeText.begin();itText != vDecorativeText.end();++itText,++iText)
	{
		CRect Rect = (*itText).GetRect();
		if (Rect.PtInRect(m_ClientPoint))
			DecorativeText = *itText;
	}

	// Render decorative text into the fractal based on the coordinates of the drag rectangle
	CAddText AddText(DecorativeText);
	if (AddText.DoModal() == IDOK)
	{
		// Get the font, color, and opacity for the text
		CString csFontName;
		BOOL bItalics,bNoBackColor;
		int iWeight;
		COLORREF TextRGB,BackRGB;
		UINT iPtSize,iOpacity;
		AddText.GetTextFont(csFontName,iWeight,bItalics,TextRGB,BackRGB,iPtSize,iOpacity,bNoBackColor);
		CString csText = AddText.GetText();
		iPtSize *= 10;

		// Replace the text
		CDecorativeText UpdatedDecorativeText(DecorativeText.GetRect(),csFontName,iWeight,bItalics,iPtSize,iOpacity,TextRGB,BackRGB,bNoBackColor,csText);
		pDoc->AddTextToDoc(UpdatedDecorativeText,TRUE);

		// Update the view to render the text
		pDoc->UpdateAllViews(NULL);
	}
}

// Delete the decorative text
void CFractalView::OnDeleteText()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Rediscover the selected text
	std::vector<CDecorativeText> & vDecorativeText = GetDocument()->GetDecorativeText();
	std::vector<CDecorativeText>::iterator itText;
	int iText = 0;
	CDecorativeText DecorativeText;
	for (itText = vDecorativeText.begin();itText != vDecorativeText.end();++itText,++iText)
	{
		CRect Rect = (*itText).GetRect();
		if (Rect.PtInRect(m_ClientPoint))
			DecorativeText = *itText;
	}

	// Delete the text
	pDoc->AddTextToDoc(DecorativeText,2);

	// Update the view to render the text
	pDoc->UpdateAllViews(NULL);
}

// Edit the decorative picture
void CFractalView::OnEditPicture()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Rediscover the selected picture
	std::vector<CDecorativePicture> & vDecorativePicture = GetDocument()->GetDecorativePicture();
	std::vector<CDecorativePicture>::iterator itPicture;
	CRect Rect;
	int iPicture = 0;
	CDecorativePicture DecorativePicture;
	for (itPicture = vDecorativePicture.begin();itPicture != vDecorativePicture.end();++itPicture,++iPicture)
	{
		Rect = (*itPicture).GetRect();
		if (Rect.PtInRect(m_ClientPoint))
			DecorativePicture = *itPicture;
	}

	// Render decorative text into the fractal based on the coordinates of the drag rectangle
	CAddPicture AddPicture(DecorativePicture);
	if (AddPicture.DoModal() == IDOK)
	{
		// Get the picture
		DecorativePicture = AddPicture.GetDecorativePicture();
		CImage Image;
		if (SUCCEEDED(Image.Load(DecorativePicture.GetPictureName())))
		{
			// Add the picture to the view
			DecorativePicture.SetRect(Rect);
			pDoc->AddPictureToDoc(DecorativePicture,TRUE);

			// Update the view to render the picture
			pDoc->UpdateAllViews(NULL);
		}
	}
}

// Delete the decorative picture
void CFractalView::OnDeletePicture()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Rediscover the selected text
	std::vector<CDecorativePicture> & vDecorativePicture = GetDocument()->GetDecorativePicture();
	std::vector<CDecorativePicture>::iterator itPicture;
	int iPicture = 0;
	CDecorativePicture DecorativePicture;
	for (itPicture = vDecorativePicture.begin();itPicture != vDecorativePicture.end();++itPicture,++iPicture)
	{
		CRect Rect = (*itPicture).GetRect();
		if (Rect.PtInRect(m_ClientPoint))
			DecorativePicture = *itPicture;
	}

	// Delete the text
	pDoc->AddPictureToDoc(DecorativePicture,2);

	// Update the view to render the text
	pDoc->UpdateAllViews(NULL);
}

// Edit the color at the coordinate
void CFractalView::OnEditColor()
{
	CFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get the DIB for updating
	CDIBFrame & DisplayDIB = pDoc->GetFractal();
	CDIBFrame & BaseDIB = pDoc->GetBaseImage();
	CDIBFrame & FractalDIB = pDoc->GetFractalImage();
	std::vector<std::vector<BYTE> > & vRGB = pDoc->GetRGB();

	// Get the current palette index for the color at the coordinate
	int iColor,iX,iY;
	iX = m_ClientPoint.x;
	iY = DisplayDIB.GetHeight() - m_ClientPoint.y - 1;
	DisplayDIB.GetPixelPaletteIndex(iX,iY,iColor);

	// Display the color dialog with the old color selected
	CColorDialog ClrDlg;
	ClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	ClrDlg.m_cc.rgbResult = RGB(vRGB[0][iColor],vRGB[1][iColor],vRGB[2][iColor]);
	if (ClrDlg.DoModal() == IDOK)
	{
		// Get the new color
		COLORREF RGB = ClrDlg.GetColor();
		BYTE R,G,B;
		R = GetRValue(RGB);
		G = GetGValue(RGB);
		B = GetBValue(RGB);

		// Update the color in the palette
		vRGB[0][iColor] = R;
		vRGB[1][iColor] = G;
		vRGB[2][iColor] = B;
		DisplayDIB.SetRGB(vRGB);

		// Multi-threaded recolor of one color
		CChangeColor ChangeColor(DisplayDIB,BaseDIB,FractalDIB,ID_CONTEXT_EDIT_COLOR,iColor);
		ChangeColor.ChangeColor();

		// Get fractal dialog for this type
		CNewFractal & Fractal = theApp.GetFractalDialog(pDoc->GetFractalType());
		Fractal.UpdateColor(vRGB);
		if (iColor == 0)
		{
			Fractal.SetBase(TRUE);
			Fractal.SetRGBBase(RGB);
		}

		// Update the view to render the text
		pDoc->UpdateAllViews(NULL);
	}
}

BOOL CFractalView::OnSetCursor(CWnd* pWnd,UINT nHitTest,UINT message)
{
	SetCursor(m_hCursor);
	return TRUE;
}
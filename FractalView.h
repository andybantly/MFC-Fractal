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

#include "ColorOp.h"
#include "AVI.h"

class CFractalView : public CScrollView
{
protected: // create from serialization only
	CFractalView();
	DECLARE_DYNCREATE(CFractalView)

// Attributes
public:
	CFractalDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CFractalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bZoomOut;
	CPoint m_Pt1,m_Pt2;
	CRect m_Rect;
	BOOL m_bLButtonDown;
	BOOL m_bZoomIn;
	HCURSOR m_hLastCursor,m_hCursor;
	CPoint m_ClientPoint;
	CChangeColor * m_pChangeColor;
	CChangeColor * m_pBlendImage;
	BOOL m_bColorBleedUp;
	BOOL m_bColorBleedDown;
	BOOL m_bColorBleedSubtleUp;
	BOOL m_bColorBleedSubtleDown;
	BOOL m_bColorBleedPaletteUp;
	BOOL m_bColorBleedPaletteDown;
	BOOL m_bDragZoom;
	BOOL m_bDragText;
	BOOL m_bDragPicture;
	BOOL m_bGrabText;
	BOOL m_bGrabPicture;
	BOOL m_bGrabBorder;
	int m_iGrabPicture,m_iGrabText;
	COLORREF m_crFGSelect,m_crBGSelect;
	clock_t m_ctClock;
	clock_t m_fpsClock;
	double m_fpsDelta;

	// The video recorder
	CAVI m_AVI;

	// Fly into frames
	double m_dLastXMin,m_dLastXMax,m_dLastYMin,m_dLastYMax;
	double m_dXMinSteps,m_dXMaxSteps,m_dYMinSteps,m_dYMaxSteps;
	double m_XMin,m_XMax,m_YMin,m_YMax;
	UINT m_nFlyFrames;
	BOOL m_bContinuousSmallZoom;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	CPoint ScalePoint(CPoint Pt);
	void Zoom(CPoint Pt1,CPoint Pt2);
	void StartTimer(UINT_PTR nIDEvent);
	void StopTimer(UINT_PTR nIDEvent);
	void DrawDecorativeText(CDC * pDC,CDecorativeText & DecorativeText);
	void DrawDecorativePicture(CDC * pDC,CDecorativePicture & DecorativePicture);
	HCURSOR SetWndCursor(LPTSTR Cursor,BOOL bPredefined = FALSE);
	int PointOnBorder(CPoint point,int & iPicture,int & iText);
	int PointInText(CPoint point);
	int PointInPicture(CPoint point);
	void UpdateSelection();

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomOut(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnContextMenu(CWnd * pWnd,CPoint point);
	afx_msg void OnColorBleedUp();
	afx_msg void OnUpdateColorBleedUp(CCmdUI *pCmdUI);
	afx_msg void OnColorBleedDown();
	afx_msg void OnUpdateColorBleedDown(CCmdUI *pCmdUI);
	afx_msg void OnColorBleedSubtleUp();
	afx_msg void OnUpdateColorBleedSubtleUp(CCmdUI *pCmdUI);
	afx_msg void OnColorBleedSubtleDown();
	afx_msg void OnUpdateColorBleedSubtleDown(CCmdUI *pCmdUI);
	afx_msg void OnColorBleedPaletteUp();
	afx_msg void OnUpdateColorBleedPaletteUp(CCmdUI *pCmdUI);
	afx_msg void OnColorBleedPaletteDown();
	afx_msg void OnUpdateColorBleedPaletteDown(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSmooth();
	afx_msg void OnJulia();
	afx_msg void OnJulia2X();
	afx_msg void OnDragRectangleZoom();
	afx_msg void OnUpdateDragRectangleZoom(CCmdUI *pCmdUI);
	afx_msg void OnDragRectangleText();
	afx_msg void OnUpdateDragRectangleText(CCmdUI *pCmdUI);
	afx_msg void OnDragRectanglePicture();
	afx_msg void OnUpdateDragRectanglePicture(CCmdUI *pCmdUI);
	afx_msg void OnEditPicture();
	afx_msg void OnDeletePicture();
	afx_msg void OnEditText();
	afx_msg void OnDeleteText();
	afx_msg void OnEditColor();
	afx_msg void OnSmallZoom();
	afx_msg void OnUpdateSmallZoom(CCmdUI *pCmdUI);
	afx_msg void OnContinuousSmallZoom();
	afx_msg void OnUpdateContinuousSmallZoom(CCmdUI *pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#ifndef _DEBUG  // debug version in FractalView.cpp
inline CFractalDoc* CFractalView::GetDocument() const
   { return reinterpret_cast<CFractalDoc*>(m_pDocument); }
#endif

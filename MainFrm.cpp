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
#include "MainFrm.h"
#include "afxext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFractalStatusBar
IMPLEMENT_DYNAMIC(CFractalStatusBar, CStatusBar)

AFX_STATUSPANE* CFractalStatusBar::_GetPanePtr(int nIndex) const
{
	ASSERT((nIndex >= 0 && nIndex < m_nCount) || m_nCount == 0);
	return ((AFX_STATUSPANE*)m_pData) + nIndex;
}

BOOL CFractalStatusBar::SetIndicators(const UINT * lpIDArray,const CString * lpcsStatusText, int nIDCount)
{
	ASSERT_VALID(this);
	ASSERT(nIDCount >= 1);  // must be at least one of them
	ASSERT(lpIDArray == NULL ||
		AfxIsValidAddress(lpIDArray, sizeof(UINT) * nIDCount, FALSE));
	ASSERT(::IsWindow(m_hWnd));

	// first allocate array for panes and copy initial data
	if (!AllocElements(nIDCount, sizeof(AFX_STATUSPANE)))
		return FALSE;
	ASSERT(nIDCount == m_nCount);

	// copy initial data from indicator array
	BOOL bResult = TRUE;
	if (lpIDArray != NULL)
	{
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
		CClientDC dcScreen(NULL);
		HGDIOBJ hOldFont = NULL;
		if (hFont != NULL)
			hOldFont = dcScreen.SelectObject(hFont);

		AFX_STATUSPANE* pSBP = _GetPanePtr(0);
		for (int i = 0; i < nIDCount; i++)
		{
			pSBP->nID = *lpIDArray++;
			pSBP->nFlags |= SBPF_UPDATE;
			if (pSBP->nID != 0)
			{
				pSBP->strText = lpcsStatusText[i];
				pSBP->cxText = dcScreen.GetTextExtent(pSBP->strText).cx;
				ASSERT(pSBP->cxText >= 0);
				if (!SetPaneText(i, pSBP->strText, FALSE))
				{
					bResult = FALSE;
					break;
				}
			}
			else
			{
				// no indicator (must access via index)
				// default to 1/4 the screen width (first pane is stretchy)
				pSBP->cxText = ::GetSystemMetrics(SM_CXSCREEN)/4;
				if (i == 0)
					pSBP->nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);
			}
			++pSBP;
		}
		if (hOldFont != NULL)
			dcScreen.SelectObject(hOldFont);
	}
	UpdateAllPanes(TRUE, TRUE);

	return bResult;
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(IDS_FRACTAL_CURSOR_POS,OnUpdateCursorPos)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	IDS_FRACTAL_CURSOR_POS,
};

static CString m_acsFractalStatusText [] = {_T(""),_T(""),_T("")};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this,WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,ID_FRACTAL_STATUS_BAR))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators,m_acsFractalStatusText,sizeof(indicators)/sizeof(UINT));

	return 0;
}

void CMainFrame::SetCoord(CString & csCoord)
{
	m_csCoord = csCoord;
}

void CMainFrame::OnUpdateCursorPos(CCmdUI * pCmdUI)
{
	m_acsFractalStatusText[0] = m_csCoord;
	m_wndStatusBar.SetIndicators(indicators,m_acsFractalStatusText,sizeof(indicators) / sizeof(UINT));
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// Blank out the coordinates since it is outside of a document
	m_csCoord = _T("");
	return CMDIFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

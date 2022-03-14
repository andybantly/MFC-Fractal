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
#pragma once

class CFractalStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CFractalStatusBar)
protected:
	AFX_STATUSPANE* _GetPanePtr(int nIndex) const;

public:
	BOOL SetIndicators(const UINT * lpIDArray,const CString * lpcsStatusText,int nIDCount);
};

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:
	void SetCoord(CString & csCoord);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CFractalStatusBar m_wndStatusBar;
	CString m_csCoord;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateCursorPos(CCmdUI * pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	// Used to blank out the coordinates
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#define SBPF_UPDATE 0x0001  // pending update of text
struct AFX_STATUSPANE
{
	UINT    nID;        // IDC of indicator: 0 => normal text area
	int     cxText;     // width of string area in pixels on both sides there is a 3 pixel gap and a one pixel border, making a pane 6 pixels wider
	UINT    nStyle;     // style flags (SBPS_*)
	UINT    nFlags;     // state flags (SBPF_*)
	CString strText;    // text in the pane
};

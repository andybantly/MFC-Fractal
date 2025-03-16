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

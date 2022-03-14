#pragma once


// CAnimate dialog

class CAnimate : public CDialog
{
	DECLARE_DYNAMIC(CAnimate)

public:
	CAnimate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimate();

// Dialog Data
	enum { IDD = IDD_ANIMATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_uiFPS;
	BOOL m_bRecord;
};

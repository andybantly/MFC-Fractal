#pragma once


// CHelpDialog dialog

class CHelpDialog : public CDialog
{
	DECLARE_DYNAMIC(CHelpDialog)

public:
	CHelpDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelpDialog();

// Dialog Data
	enum { IDD = IDD_HELPDIALOG };
	CStatic m_Static_Info;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

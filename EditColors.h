#pragma once
#include "afxcmn.h"
#include "resource.h"

// CEditColors dialog

class CEditColors : public CDialog
{
	DECLARE_DYNAMIC(CEditColors)

	CListCtrl m_Colors;
	CImageList m_ImageList;
	CDIBFrame m_Gradient;
	int m_nColors;
	std::vector<COLORREF> m_vecColors;
	BYTE m_RR,m_GG,m_BB;
	BOOL m_bR,m_bG,m_bB;
	std::vector<std::vector<BYTE> > m_RGB;
	bool m_bChange;

public:
	CEditColors(CWnd * pParent = NULL);   // standard constructor
	virtual ~CEditColors();
	void SetColors(int nColors);
	void SetRGB(COLORREF RGB1,COLORREF RGB2,BYTE RR,BYTE GG,BYTE BB,BOOL bR,BOOL bG,BOOL bB);
	void SetRGB(COLORREF RGB1,COLORREF RGB2,COLORREF RGB3,COLORREF RGB4,COLORREF RGB5,COLORREF RGB6,BYTE RR,BYTE GG,BYTE BB,BOOL bR,BOOL bG,BOOL bB);
	void UpdateColor(std::vector<std::vector<BYTE> > vecRGB);
	void BuildColorMap();
	std::vector<std::vector<BYTE> > GetColorMap();

// Dialog Data
	enum { IDD = IDD_EDIT_COLORS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnNMDblclkListColors(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	HBITMAP MakeColor(BYTE R,BYTE G,BYTE B,CString & csColor);
	void BuildImageList();
};

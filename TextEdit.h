#pragma once


// CTextEdit

class CTextEdit : public CEdit
{
	DECLARE_DYNAMIC(CTextEdit)

public:
	CTextEdit();
	virtual ~CTextEdit();
	void UpdateFont(CString csFontName,int iPtSize,int iWeight,BOOL bItalics);
	void SetTextColorRGB(COLORREF crTextColorRGB);
	void SetBackColorRGB(COLORREF crBackColorRGB);
	CString GetFontName();
	int GetPtSize();
	int GetWeight();
	BOOL GetItalics();

	COLORREF m_crTextColorRGB,m_crBackColorRGB;
	CBrush m_BackGround;
	CFont m_Font;
	int m_iPtSize;
	CString m_csFontName;
	BOOL m_bItalics;
	int m_iBold;

protected:
	DECLARE_MESSAGE_MAP()

	void TextEditFont();
	bool m_bSetFont;

public:
	afx_msg HBRUSH CtlColor(CDC * pDC,UINT nCtlColor);
};



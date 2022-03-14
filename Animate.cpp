// Animate.cpp : implementation file
//

#include "stdafx.h"
#include "Fractal.h"
#include "Animate.h"

// CAnimate dialog

IMPLEMENT_DYNAMIC(CAnimate, CDialog)

CAnimate::CAnimate(CWnd * pParent /*=NULL*/)	: 
	CDialog(CAnimate::IDD, pParent), m_uiFPS(15), m_bRecord(FALSE)
{
}

CAnimate::~CAnimate()
{
}

void CAnimate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FPS, m_uiFPS);
	DDV_MinMaxUInt(pDX, m_uiFPS, 1, 60);
	DDX_Check(pDX, IDC_CHECK_RECORD, m_bRecord);
}

BEGIN_MESSAGE_MAP(CAnimate, CDialog)
END_MESSAGE_MAP()

// CAnimate message handlers

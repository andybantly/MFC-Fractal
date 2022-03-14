// HelpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Fractal.h"
#include "HelpDialog.h"


// CHelpDialog dialog

IMPLEMENT_DYNAMIC(CHelpDialog, CDialog)

CHelpDialog::CHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDialog::IDD, pParent)
{
}

CHelpDialog::~CHelpDialog()
{
}

BOOL CHelpDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the helpful text
	m_Static_Info.SetWindowText(L"Base means the part of the system that solves the equation and fractal means the part that escapes to infinity. Larger widths and heights allow more precision which result in a better rendering. More iterations produce more precision and more colors reduce the banding effect. Scale the intensity of each color component using values from 0 to 255. Gradiate colors with the checkbox and choose the coloring based on the gradient between the colors. Click Random to generate a random scaling. Fix the base color to set the background. Blend images with the rendering by choosing the area (base or fractal) and set the opacity of the blend. Choose coordinates in the combox for preset fractals and choosing default resets the coordinates. X Scale, Y Scale, Bailout and Mode control the divergance test of the formula AX² +/- BY² < Bailout.  Mode controls if the equation is elliptical or hyperbolic. Values are 1-4.");

	return TRUE;
}

void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_Static_Info);
}


BEGIN_MESSAGE_MAP(CHelpDialog, CDialog)
END_MESSAGE_MAP()

// CHelpDialog message handlers
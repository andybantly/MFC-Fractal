// Copyright (C) 2012-Present Andrew S. Bantly
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
#include "stdafx.h"
#include <math.h>
#include "Fractal.h"
#include "FractalDoc.h"
#include "NewFractal.h"
#include "EditColors.h"
#include "atlenc.h"
#include "AC.h"
#include "Crypt.h"
#include "HelpDialog.h"

// CNewFractal dialog

IMPLEMENT_DYNAMIC(CNewFractal, CDialog)

CNewFractal::CNewFractal(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFractal::IDD, pParent)
{
	// Fractal Settings
	m_iFractalType = 0;
	m_iWidth = 1536;
	m_iHeight = 1024;
	m_iMode = 1;
	m_iIterations = 128;
	m_nMaxCalc = 1024;
	m_bRed = TRUE;
	m_bGreen = TRUE;
	m_bBlue = TRUE;
	m_bDropUnused = FALSE;
	m_bSmooth = FALSE;
	m_bSortOrder = FALSE;
	m_bGradient = FALSE;
	m_bBaseColor = FALSE;
	m_bOrbitColor = FALSE;
	m_RGB1 = RGB(0, 0, 0);
	m_RGB2 = RGB(51,51,51);
	m_RGB3 = RGB(102,102,102);
	m_RGB4 = RGB(153,153,153);
	m_RGB5 = RGB(204,204,204);
	m_RGB6 = RGB(255,255,255);
	m_RR = 0;
	m_RG = 0;
	m_RB = 0;
	m_dXMin = -2.0;
	m_dXMax = 1;
	m_dYMin = -1.0;
	m_dYMax = 1.0;
	m_iCurSel = 0;
	m_bBase = FALSE;
	m_RGBBase = RGB(0, 0, 0);
	m_bUseImage = FALSE;
	m_bUseImage2 = FALSE;
	m_dP = 0.238498;
	m_dQ = 0.512321;
	m_dXScale = 1;
	m_dYScale = 1;
	m_dBailRadius = 4;
	m_dStep = 0.0;
	m_iAlpha = 0;
	m_iAlpha2 = 0;
	m_bBlendBase = FALSE;
	m_bBlendFractal = FALSE;
	m_RGBStepColor = RGB(0,0,0);
	m_bTrapX = FALSE;
	m_bTrapY = FALSE;
	m_dTrapX = 0.01;
	m_dTrapY = 0.01;

	// Gamma Correct settings
	m_bGCR = TRUE;
	m_bGCG = TRUE;
	m_bGCB = TRUE;
	m_dGCR = 2.0;
	m_dGCG = 2.0;
	m_dGCB = 2.0;

	// Add the interesting Mandelbrot coordinates
	CFractalCoordinate Coordinate(_T("Default"),m_iWidth,m_iHeight,16,512,-2.0,1,-1.0,1.0,0,0);
	m_vCoordinates.push_back(Coordinate);
	Coordinate = CFractalCoordinate(_T("Location A"),m_iWidth,m_iHeight,16,512,-0.717997,-0.592801,0.395415,0.505444,0,0);
	m_vCoordinates.push_back(Coordinate);
	Coordinate = CFractalCoordinate(_T("Location B"),m_iWidth,m_iHeight,16,512,-0.707981,-0.627856,0.477937,0.367908,0,0);
	m_vCoordinates.push_back(Coordinate);
	Coordinate = CFractalCoordinate(_T("Location C"),m_iWidth,m_iHeight,16,512,-0.702973,-0.642879,0.374785,0.395415,0,0);
	m_vCoordinates.push_back(Coordinate);
	Coordinate = CFractalCoordinate(_T("Location D"),m_iWidth,m_iHeight,16,512,-0.691060,-0.690906,0.387103,0.387228,0,0);
	m_vCoordinates.push_back(Coordinate);
	Coordinate = CFractalCoordinate(_T("Location E"),m_iWidth,m_iHeight,16,512,-0.793113,-0.723005,0.037822,0.140974,0,0);
	m_vCoordinates.push_back(Coordinate);
	Coordinate = CFractalCoordinate(_T("Location F"),m_iWidth,m_iHeight,16,512,-0.745465,-0.745387,0.112896,0.113034,0,0);
	m_vCoordinates.push_back(Coordinate);
	Coordinate = CFractalCoordinate(_T("Location G"),m_iWidth,m_iHeight,16,512,-0.745464,-0.745388,0.112967,0.113030,0,0);
	m_vCoordinates.push_back(Coordinate);

	// Add the interesting Julia coordinates
	CFractalCoordinate Point(_T("Default"),m_iWidth,m_iHeight,16,512,-1.8,1.8,-1.2,1.2,0.238498,0.512321);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location A"),m_iWidth,m_iHeight,16,128,-1.8,1.8,-1.2,1.2,0.238498,0.519198);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location B"),m_iWidth,m_iHeight,16,96,-1.8,1.8,-1.2,1.2,-0.743036,0.113467);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location C"),m_iWidth,m_iHeight,16,64,-1.8,1.8,-1.2,1.2,-0.192175,0.656734);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location D"),m_iWidth,m_iHeight,16,32,-1.8,1.8,-1.2,1.2,0.108294,-0.670487);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location E"),m_iWidth,m_iHeight,16,64,-1.8,1.8,-1.2,1.2,-0.392488,-0.587966);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location F"),m_iWidth,m_iHeight,16,256,-1.8,1.8,-1.2,1.2,-0.392488,-0.587966);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location G"),m_iWidth,m_iHeight,16,32,-1.8,1.8,-1.2,1.2,0.138341,0.649857);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location H"),m_iWidth,m_iHeight,16,24,-1.8,1.8,-1.2,1.2,0.278560,-0.003483);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location I"),m_iWidth,m_iHeight,16,48,-1.8,1.8,-1.2,1.2,0.278560,-0.003483);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location J"),m_iWidth,m_iHeight,16,48,-1.8,1.8,-1.2,1.2,-1.258842,0.065330);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location K"),m_iWidth,m_iHeight,16,48,-1.8,1.8,-1.2,1.2,-1.028482,-0.264756);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location L"),m_iWidth,m_iHeight,16,64,-1.8,1.8,-1.2,1.2,0.268545,-0.003483);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location M"),m_iWidth,m_iHeight,16,24,-1.8,1.8,-1.2,1.2,0.268545,-0.003483);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location N"),m_iWidth,m_iHeight,16,256,-1.8,1.8,-1.2,1.2,0.318623,0.044699);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location O"),m_iWidth,m_iHeight,16,48,-1.8,1.8,-1.2,1.2,0.318623,0.044699);
	m_vPoints.push_back(Point);
	Point = CFractalCoordinate(_T("Location P"),m_iWidth,m_iHeight,16,64,-1.8,1.8,-1.2,1.2,0.383725,0.147851);
	m_vPoints.push_back(Point);

	// Add the interesting Mandelbrot Dragon coordinates
	CFractalCoordinate DragonCoordinate(_T("Default"),m_iWidth,m_iHeight,16,512,-2.2,4.2,-1.5,1.5,0,0);
	m_vDragonCoordinates.push_back(DragonCoordinate);

	// Add the interesting Mandelbrot Phoenix coordinates
	CFractalCoordinate PhoenixCoordinate(_T("Default"),m_iWidth,m_iHeight,16,512,-2.1,1.5,-2.0,2.0,0,0);
	m_vPhoenixCoordinates.push_back(PhoenixCoordinate);

	// Add the interesting Julia Dragon coordinates
	CFractalCoordinate DragonPoint(_T("Default"),m_iWidth,m_iHeight,16,256,-2.2,4.2,-1.6,1.6,1.646009,0.967049);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location A"),m_iWidth,m_iHeight,16,256,-2.2,4.2,-1.6,1.6,1.335524,0.769341);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location B"),m_iWidth,m_iHeight,16,64,-2.2,4.2,-1.6,1.6,1.646009,0.967049);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location C"),m_iWidth,m_iHeight,16,64,-2.2,4.2,-1.6,1.6,2.447261,-0.924069);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location D"),m_iWidth,m_iHeight,16,64,-2.2,4.2,-1.6,1.6,1.325508,0.786533);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location E"),m_iWidth,m_iHeight,16,64,-2.2,4.2,-1.6,1.6,1.415414,0.856803);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location F"),m_iWidth,m_iHeight,16,32,-2.2,4.2,-1.6,1.6,1.415414,0.856803);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location G"),m_iWidth,m_iHeight,16,128,-2.2,4.2,-1.6,1.6,1.255399,0.691977);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location H"),m_iWidth,m_iHeight,16,64,-2.2,4.2,-1.6,1.6,2.797809,-0.657593);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location I"),m_iWidth,m_iHeight,16,64,-2.2,4.2,-1.6,1.6,3.018153,-0.098854);
	m_vDragonPoints.push_back(DragonPoint);
	DragonPoint = CFractalCoordinate(_T("Location J"),m_iWidth,m_iHeight,16,64,-2.2,4.2,-1.6,1.6,2.998122,0.004298);
	m_vDragonPoints.push_back(DragonPoint);

	// Add the interesting Julia Phoenix coordinates
	CFractalCoordinate PhoenixPoint(_T("Default"),m_iWidth,m_iHeight,4,128,-1.5,1.5,-1.2,1.2,0.56667,-0.5);
	m_vPhoenixPoints.push_back(PhoenixPoint);
	PhoenixPoint = CFractalCoordinate(_T("Location A"),m_iWidth,m_iHeight,4,128,-1.5,1.5,-1.2,1.2,0.288732,0.510029);
	m_vPhoenixPoints.push_back(PhoenixPoint);
	PhoenixPoint = CFractalCoordinate(_T("Location B"),m_iWidth,m_iHeight,4,128,-1.5,1.5,-1.2,1.2,0.484194,-1.044413);
	m_vPhoenixPoints.push_back(PhoenixPoint);
	PhoenixPoint = CFractalCoordinate(_T("Location C"),m_iWidth,m_iHeight,4,128,-1.5,1.5,1.209169,1.2,0.356338,-1.209169);
	m_vPhoenixPoints.push_back(PhoenixPoint);
	PhoenixPoint = CFractalCoordinate(_T("Location D"),m_iWidth,m_iHeight,4,128,-1.5,1.5,-1.255014,1.2,-0.550704,-1.255014);
	m_vPhoenixPoints.push_back(PhoenixPoint);

	// Add the interesting Mandelbrot Phoenix coordinates
	CFractalCoordinate Phoenix2Coordinate(_T("Default"),m_iWidth,m_iHeight,16,512,-2.1,1.5,-2.0,2.0,0,0);
	m_vPhoenix2Coordinates.push_back(Phoenix2Coordinate);
}

CNewFractal::~CNewFractal()
{
}

void CNewFractal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_iWidth);
	DDV_MinMaxUInt(pDX, m_iWidth, 320, 28800);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_iHeight);
	DDV_MinMaxUInt(pDX, m_iHeight, 200, 16200);
	DDX_Text(pDX, IDC_EDIT_CALC, m_nMaxCalc);
	DDV_MinMaxInt(pDX, m_nMaxCalc, 1, 65536);
	DDX_Text(pDX, IDC_EDIT_MODE, m_iMode);
	DDV_MinMaxInt(pDX, m_iMode, 1, 4);
	DDX_Text(pDX, IDC_EDIT_ITER, m_iIterations);
	DDV_MinMaxInt(pDX, m_iIterations, 1, m_nMaxCalc);
	DDX_Check(pDX, IDC_CHECK_RED, m_bRed);
	DDX_Check(pDX, IDC_CHECK_GREEN, m_bGreen);
	DDX_Check(pDX, IDC_CHECK_BLUE, m_bBlue);
	DDX_Check(pDX, IDC_CHECK_DROPUNUSED, m_bDropUnused);
	DDX_Check(pDX, IDC_CHECK_SMOOTH, m_bSmooth);
	DDX_Radio(pDX, IDC_SORT_LESS, m_bSortOrder);
	DDX_Radio(pDX, IDC_RADIO_BASE_COLOR, m_bBaseColor);
	DDX_Radio(pDX, IDC_RADIO_ORBIT_COLOR, m_bOrbitColor);
	DDX_Control(pDX, IDC_SORT_LESS, m_BtnLT);
	DDX_Control(pDX, IDC_SORT_GREATER, m_BtnGT);
	DDX_Check(pDX, IDC_CHECK_GRADIENT, m_bGradient);
	DDX_Check(pDX, IDC_CHECK_BASE, m_bBase);
	DDX_Text(pDX, IDC_EDIT_RR, m_RR);
	DDV_MinMaxInt(pDX, m_RR, -255, 255);
	DDX_Text(pDX, IDC_EDIT_RG, m_RG);
	DDV_MinMaxInt(pDX, m_RG, -255, 255);
	DDX_Text(pDX, IDC_EDIT_RB, m_RB);
	DDV_MinMaxInt(pDX, m_RB, -255, 255);

	DDX_Text(pDX, IDC_EDIT_XMIN, m_dXMin);
	DDV_MinMaxDouble(pDX, m_dXMin, m_dDDVXMin, m_dDDVXMax);
	DDX_Text(pDX, IDC_EDIT_XMAX, m_dXMax);
	DDV_MinMaxDouble(pDX, m_dXMax, m_dDDVXMin, m_dDDVXMax);
	DDX_Text(pDX, IDC_EDIT_YMIN, m_dYMin);
	DDV_MinMaxDouble(pDX, m_dYMin, m_dDDVYMin, m_dDDVYMax);
	DDX_Text(pDX, IDC_EDIT_YMAX, m_dYMax);
	DDV_MinMaxDouble(pDX, m_dYMax, m_dDDVYMin, m_dDDVYMax);

	DDX_Text(pDX, IDC_EDIT_XSCALE, m_dXScale);
	DDV_MinMaxDouble(pDX, m_dXScale, -2, 2);
	DDX_Text(pDX, IDC_EDIT_YSCALE, m_dYScale);
	DDV_MinMaxDouble(pDX, m_dYScale, -2, 2);
	DDX_Text(pDX, IDC_EDIT_BAILOUT, m_dBailRadius);
	DDV_MinMaxDouble(pDX, m_dBailRadius, -8, 8);

	DDX_Control(pDX, IDC_COMBO_RANGE, m_Range);
	DDX_Control(pDX, IDC_EDIT_XMIN, m_XMin);
	DDX_Control(pDX, IDC_EDIT_YMIN, m_YMin);
	DDX_Control(pDX, IDC_STATIC_XMIN, m_Static_XMin);
	DDX_Control(pDX, IDC_STATIC_YMIN, m_Static_YMin);
	DDX_Control(pDX, IDC_EDIT_XMAX, m_XMax);
	DDX_Control(pDX, IDC_EDIT_YMAX, m_YMax);
	DDX_Control(pDX, IDC_STATIC_XMAX, m_Static_XMax);
	DDX_Control(pDX, IDC_STATIC_YMAX, m_Static_YMax);
	DDX_Control(pDX, IDC_EDIT_P, m_P);
	DDX_Control(pDX, IDC_EDIT_Q, m_Q);
	DDX_Control(pDX, IDC_STATIC_P, m_Static_P);
	DDX_Control(pDX, IDC_STATIC_Q, m_Static_Q);
	if (theApp.IsJuliaFractal(m_iFractalType))
	{
		DDX_Text(pDX, IDC_EDIT_P, m_dP);
		DDV_MinMaxDouble(pDX, m_dP, m_dXMin, m_dXMax);
		DDX_Text(pDX, IDC_EDIT_Q, m_dQ);
		DDV_MinMaxDouble(pDX, m_dQ, m_dYMin, m_dYMax);
	}
	DDX_Control(pDX, IDC_CHECK_BLENDBASE, m_BtnBlendBase);
	DDX_Check(pDX, IDC_CHECK_BLENDBASE, m_bBlendBase);
	DDX_Control(pDX, IDC_CHECK_BLENDFRACTAL, m_BtnBlendFractal);
	DDX_Check(pDX, IDC_CHECK_BLENDFRACTAL, m_bBlendFractal);
	DDX_Control(pDX, IDC_STATIC_STEP, m_Static_Step);
	DDX_Control(pDX, IDC_EDIT_STAIRSTEP, m_Step);
	DDX_Text(pDX, IDC_EDIT_STAIRSTEP, m_dStep);
	DDV_MinMaxDouble(pDX, m_dStep, 0.0, 10.0);
	DDX_Control(pDX, IDC_EDIT_OPACITY, m_Alpha);
	DDX_Text(pDX, IDC_EDIT_OPACITY, m_iAlpha);
	DDV_MinMaxInt(pDX, m_iAlpha,0,100);
	DDX_Control(pDX, IDC_STATIC_OPACITY, m_Static_Alpha);
	DDX_Control(pDX, IDC_EDIT_OPACITY2, m_Alpha2);
	DDX_Text(pDX, IDC_EDIT_OPACITY2, m_iAlpha2);
	DDV_MinMaxInt(pDX, m_iAlpha2,0,100);
	DDX_Control(pDX, IDC_STATIC_OPACITY2, m_Static_Alpha2);
	DDX_Control(pDX, IDC_STATIC_BASECOLOR, m_StaticBase);
	DDX_Control(pDX, IDC_STATIC_FRACTALCOLOR, m_StaticFractal);
	DDX_Control(pDX, IDC_RADIO_BASE_COLOR, m_RadioBase);
	DDX_Control(pDX, IDC_RADIO_BASE_AD_BASE, m_RadioBaseAD);
	DDX_Control(pDX, IDC_RADIO_ORBIT_COLOR, m_RadioFractal);
	DDX_Control(pDX, IDC_RADIO_BASE_AD_FRACTAL, m_RadioFractalAD);

	DDX_Check(pDX, IDC_CHECK_TRAPX, m_bTrapX);
	DDX_Check(pDX, IDC_CHECK_TRAPY, m_bTrapY);
	DDX_Control(pDX, IDC_EDIT_TRAPX, m_TrapX);
	DDX_Text(pDX, IDC_EDIT_TRAPX, m_dTrapX);
	DDV_MinMaxDouble(pDX, m_dTrapX, 0, 1);
	DDX_Control(pDX, IDC_EDIT_TRAPY, m_TrapY);
	DDX_Text(pDX, IDC_EDIT_TRAPY, m_dTrapY);
	DDV_MinMaxDouble(pDX, m_dTrapY, 0, 1);

	DDX_Check(pDX, IDC_CHECK_GCR, m_bGCR);
	DDX_Control(pDX, IDC_EDIT_GCR, m_GCR);
	DDX_Text(pDX, IDC_EDIT_GCR, m_dGCR);
	DDX_Check(pDX, IDC_CHECK_GCG, m_bGCG);
	DDX_Control(pDX, IDC_EDIT_GCG, m_GCG);
	DDX_Text(pDX, IDC_EDIT_GCG, m_dGCG);
	DDX_Check(pDX, IDC_CHECK_GCB, m_bGCB);
	DDX_Control(pDX, IDC_EDIT_GCB, m_GCB);
	DDX_Text(pDX, IDC_EDIT_GCB, m_dGCB);
}

BEGIN_MESSAGE_MAP(CNewFractal, CDialog)
	ON_CBN_CLOSEUP(IDC_COMBO_RANGE, &CNewFractal::OnComboRange)
	ON_CBN_SELCHANGE(IDC_COMBO_RANGE, &CNewFractal::OnSelChangeComboRange)
	ON_BN_CLICKED(IDC_CHECK_GRADIENT, &CNewFractal::OnCheckGradient)
	ON_BN_CLICKED(IDC_CHECK_BASE, &CNewFractal::OnCheckBase)
	ON_BN_CLICKED(IDC_BASE, &CNewFractal::OnBase)
	ON_BN_CLICKED(IDC_RANDOM, &CNewFractal::OnRandom)
	ON_BN_CLICKED(IDC_REVERSE, &CNewFractal::OnReverse)
	ON_BN_CLICKED(IDC_RESET, &CNewFractal::OnReset)
	ON_BN_CLICKED(IDC_CHECK_TRAPX, &CNewFractal::OnTrapX)
	ON_BN_CLICKED(IDC_CHECK_TRAPY, &CNewFractal::OnTrapY)
	ON_BN_CLICKED(IDC_GRADIENT1, &CNewFractal::OnRGB1)
	ON_BN_CLICKED(IDC_GRADIENT2, &CNewFractal::OnRGB2)
	ON_BN_CLICKED(IDC_GRADIENT3, &CNewFractal::OnRGB3)
	ON_BN_CLICKED(IDC_GRADIENT4, &CNewFractal::OnRGB4)
	ON_BN_CLICKED(IDC_GRADIENT5, &CNewFractal::OnRGB5)
	ON_BN_CLICKED(IDC_GRADIENT6, &CNewFractal::OnRGB6)
	ON_BN_CLICKED(IDC_CHECK_DROPUNUSED, &CNewFractal::OnDropUnused)
	ON_BN_CLICKED(IDC_CHECK_SMOOTH, &CNewFractal::OnSmooth)
	ON_BN_CLICKED(IDC_CHOOSE_IMAGE, &CNewFractal::OnChooseImage)
	ON_BN_CLICKED(IDC_CHOOSE_IMAGE2, &CNewFractal::OnChooseImage2)
	ON_BN_CLICKED(IDC_CHECK_RED, &CNewFractal::OnClickColor)
	ON_BN_CLICKED(IDC_CHECK_GREEN, &CNewFractal::OnClickColor)
	ON_BN_CLICKED(IDC_CHECK_BLUE, &CNewFractal::OnClickColor)
	ON_EN_CHANGE(IDC_EDIT_RR, &CNewFractal::OnChangeScaleColor)
	ON_EN_CHANGE(IDC_EDIT_RG, &CNewFractal::OnChangeScaleColor)
	ON_EN_CHANGE(IDC_EDIT_RB, &CNewFractal::OnChangeScaleColor)
	ON_BN_CLICKED(IDC_STEPCOLOR, &CNewFractal::OnStepColor)
	ON_BN_CLICKED(IDC_EDIT_COLORS, &CNewFractal::OnEditColors)
	ON_EN_KILLFOCUS(IDC_EDIT_ITER, &CNewFractal::OnKillFocusEditIter)
	ON_BN_CLICKED(IDC_DONATE, &CNewFractal::OnClickDonate)
	ON_BN_CLICKED(IDC_CHECK_GCR, &CNewFractal::OnClickGamma)
	ON_BN_CLICKED(IDC_CHECK_GCG, &CNewFractal::OnClickGamma)
	ON_BN_CLICKED(IDC_CHECK_GCB, &CNewFractal::OnClickGamma)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CNewFractal message handlers
BOOL CNewFractal::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bUseImage = !m_csImage.IsEmpty();
	m_bUseImage2 = !m_csImage2.IsEmpty();

	m_BtnChooseImage.CreateBtmp(this,IDB_PICTURE,IDC_CHOOSE_IMAGE,_T("Base"));
	m_BtnChooseImage2.CreateBtmp(this,IDB_PICTURE,IDC_CHOOSE_IMAGE2,_T("Fractal"));
	m_BtnBase.CreateRect(this,IDC_BASE,GetRGBBase(),_T("Base"));
	m_BtnBase.EnableWindow(m_bBase);
	m_BtnRnd.CreateRect(this,IDC_RANDOM,RGB(GetRR(),GetRG(),GetRB()),_T("Random"));
	m_BtnRGB1.CreateRect(this,IDC_GRADIENT1,GetRGB1(),_T("Color 1"));
	m_BtnRGB2.CreateRect(this,IDC_GRADIENT2,GetRGB2(),_T("Color 2"));
	m_BtnRGB3.CreateRect(this,IDC_GRADIENT3,GetRGB3(),_T("Color 3"));
	m_BtnRGB4.CreateRect(this,IDC_GRADIENT4,GetRGB4(),_T("Color 4"));
	m_BtnRGB5.CreateRect(this,IDC_GRADIENT5,GetRGB5(),_T("Color 5"));
	m_BtnRGB6.CreateRect(this,IDC_GRADIENT6,GetRGB6(),_T("Color 6"));
	m_BtnRGB1.EnableWindow();
	m_BtnRGB2.EnableWindow(m_bGradient);
	m_BtnRGB3.EnableWindow(m_bGradient);
	m_BtnRGB4.EnableWindow(m_bGradient);
	m_BtnRGB5.EnableWindow(m_bGradient);
	m_BtnRGB6.EnableWindow();
	COLORREF arrRGB[] = {GetRGB1(),GetRGB2(),GetRGB3(),GetRGB4(),GetRGB5(),GetRGB6()};
	m_Gradient.CreateGradient(this,IDC_GRADIENT_COLORS,arrRGB,6,GetRR(),GetRG(),GetRB(),m_bBase,GetRGBBase());
	m_Gradient.EnableWindow(m_bGradient);
	m_BtnLT.EnableWindow(m_bSmooth);
	m_BtnGT.EnableWindow(m_bSmooth);
	m_BtnBlendBase.EnableWindow(m_bUseImage);
	m_Static_Alpha.EnableWindow(m_bUseImage);
	m_Alpha.EnableWindow(m_bUseImage);
	m_BtnBlendFractal.EnableWindow(m_bUseImage2);
	m_Static_Alpha2.EnableWindow(m_bUseImage2);
	m_Alpha2.EnableWindow(m_bUseImage2);
	m_BtnStepColor.CreateRect(this,IDC_STEPCOLOR,GetRGBStepColor(),_T("Color"));
	m_BtnEditColors.CreateBtmp(this,IDB_COLORS,IDC_EDIT_COLORS,_T("Edit Colors"));
	m_TrapX.EnableWindow(m_bTrapX);
	m_TrapY.EnableWindow(m_bTrapY);

	if (m_iFractalType == 0)
	{
		for (int iCoordinate = 0;iCoordinate < (int)m_vCoordinates.size();++iCoordinate)
			m_Range.AddString(m_vCoordinates[iCoordinate].GetName());
	}
	else if (m_iFractalType == 1)
	{
		for (int iPoint = 0;iPoint < (int)m_vPoints.size();++iPoint)
			m_Range.AddString(m_vPoints[iPoint].GetName());
	}
	else if (m_iFractalType == 2)
	{
		for (int iCoordinate = 0;iCoordinate < (int)m_vDragonCoordinates.size();++iCoordinate)
			m_Range.AddString(m_vDragonCoordinates[iCoordinate].GetName());
	}
	else if (m_iFractalType == 3)
	{
		for (int iCoordinate = 0;iCoordinate < (int)m_vPhoenixCoordinates.size();++iCoordinate)
			m_Range.AddString(m_vPhoenixCoordinates[iCoordinate].GetName());
	}
	else if (m_iFractalType == 4)
	{
		for (int iCoordinate = 0;iCoordinate < (int)m_vDragonPoints.size();++iCoordinate)
			m_Range.AddString(m_vDragonPoints[iCoordinate].GetName());
	}
	else if (m_iFractalType == 5)
	{
		for (int iCoordinate = 0;iCoordinate < (int)m_vPhoenixPoints.size();++iCoordinate)
			m_Range.AddString(m_vPhoenixPoints[iCoordinate].GetName());
	}
	else if (m_iFractalType == 6)
	{
		for (int iCoordinate = 0;iCoordinate < (int)m_vPhoenix2Coordinates.size();++iCoordinate)
			m_Range.AddString(m_vPhoenix2Coordinates[iCoordinate].GetName());
	}
	else if (m_iFractalType == 7)
	{
		for (int iCoordinate = 0;iCoordinate < (int)m_vCoordinates.size();++iCoordinate)
			m_Range.AddString(m_vCoordinates[iCoordinate].GetName());

		m_Static_Step.EnableWindow(FALSE);
		m_Step.EnableWindow(FALSE);
		m_BtnStepColor.EnableWindow(FALSE);
		m_StaticBase.EnableWindow(FALSE);
		m_RadioBase.EnableWindow(FALSE);
		m_RadioBaseAD.EnableWindow(FALSE);
		m_StaticFractal.SetWindowText(L"Buddha");
		m_RadioFractal.SetWindowText(L"Plot Anti-Buddha");
		m_RadioFractalAD.SetWindowText(L"Plot Buddha");
	}
	m_Range.SetCurSel(m_iCurSel);

	// Julia Set specific
	BOOL bPQ = FALSE;
	if (theApp.IsJuliaFractal(m_iFractalType))
		bPQ = TRUE;
	m_Static_P.ShowWindow(bPQ ? SW_SHOW : SW_HIDE);
	m_Static_Q.ShowWindow(bPQ ? SW_SHOW : SW_HIDE);
	m_P.ShowWindow(bPQ ? SW_SHOW : SW_HIDE);
	m_Q.ShowWindow(bPQ ? SW_SHOW : SW_HIDE);

	m_XMin.EnableWindow(TRUE);
	m_YMin.EnableWindow(TRUE);
	m_Static_XMin.EnableWindow(TRUE);
	m_Static_YMin.EnableWindow(TRUE);
	m_XMax.EnableWindow(TRUE);
	m_YMax.EnableWindow(TRUE);
	m_Static_XMax.EnableWindow(TRUE);
	m_Static_YMax.EnableWindow(TRUE);

	m_GCR.EnableWindow(m_bGCR);
	m_GCG.EnableWindow(m_bGCG);
	m_GCB.EnableWindow(m_bGCB);

	if (m_iFractalType == 0)
		SetWindowText(_T("New Mandelbrot"));
	else if (m_iFractalType == 1)
		SetWindowText(_T("New Julia"));
	else if (m_iFractalType == 2)
		SetWindowText(_T("New Mandelbrot Dragon"));
	else if (m_iFractalType == 3)
		SetWindowText(_T("New Mandelbrot Phoenix"));
	else if (m_iFractalType == 4)
		SetWindowText(_T("New Julia Dragon"));
	else if (m_iFractalType == 5)
		SetWindowText(_T("New Julia Phoenix"));
	else if (m_iFractalType == 6)
		SetWindowText(_T("New Mandelbrot Phoenix Version 2"));
	else if (m_iFractalType == 7)
		SetWindowText(_T("New Buddhabrot"));

	// Create the Donate button
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_DONATE);
	BITMAP Bitmap;
	Bmp.GetBitmap(&Bitmap);
	m_BtnDonate.CreateBtmp(this,IDB_DONATE,IDC_DONATE);
	m_BtnDonate.SetWindowPos(NULL,0,0,Bitmap.bmWidth,Bitmap.bmHeight,SWP_NOMOVE | SWP_NOZORDER);

	// Make sure the color channels are reflected
	RedrawButtons();

	return TRUE;
}

void CNewFractal::OnOK()
{
	if (UpdateData())
	{
		// Build the final color map
		RedrawButtons();
		EndDialog(IDOK);
	}
}

std::vector<CFractalCoordinate> & CNewFractal::GetCoordinates()
{
	switch (m_iFractalType)
	{
	case 1:
		return m_vPoints;
	case 2:
		return m_vDragonCoordinates;
	case 3:
		return m_vPhoenixCoordinates;
	case 4:
		return m_vDragonPoints;
	case 5:
		return m_vPhoenixPoints;
	case 6:
		return m_vPhoenix2Coordinates;
	case 7:
		return m_vCoordinates;
	default:
		return m_vCoordinates;
	}
}

void CNewFractal::OnComboRange()
{
	int iSel = m_Range.GetCurSel();
	if (iSel == -1)
		return;
	m_iCurSel = iSel;

	// Get the coordinate
	std::vector<CFractalCoordinate> & vCoordinates = GetCoordinates();
	CFractalCoordinate & Coordinate = vCoordinates[m_iCurSel];
	UpdateData();
	m_iWidth = Coordinate.GetWidth();
	m_iHeight = Coordinate.GetHeight();
	m_iIterations = Coordinate.GetIterations();
	m_nMaxCalc = Coordinate.GetMaxCalc();
	m_dXMin = Coordinate.GetXMin();
	m_dXMax = Coordinate.GetXMax();
	m_dYMin = Coordinate.GetYMin();
	m_dYMax = Coordinate.GetYMax();
	m_dP = Coordinate.GetP();
	m_dQ = Coordinate.GetQ();

	// Adjust the aspect ratio
	double dY = m_dYMax - m_dYMin;
	double dX = m_dXMax - m_dXMin;
	double dAspect = fabs(dX / dY);
	m_iWidth = (int)((double)m_iHeight * dAspect);
	UpdateData(FALSE);
}

void CNewFractal::OnSelChangeComboRange()
{
	OnComboRange();
}

void CNewFractal::RedrawButtons()
{
	COLORREF RGBBase = MakeRGB(GetRGBBase());
	COLORREF RGBRnd = MakeRGB(RGB(GetRR(),GetRG(),GetRB()));
	COLORREF RGB1 = MakeRGB(GetRGB1());
	COLORREF RGB2 = MakeRGB(GetRGB2());
	COLORREF RGB3 = MakeRGB(GetRGB3());
	COLORREF RGB4 = MakeRGB(GetRGB4());
	COLORREF RGB5 = MakeRGB(GetRGB5());
	COLORREF RGB6 = MakeRGB(GetRGB6());
	COLORREF RGBStep = MakeRGB(GetRGBStepColor());
	BYTE RR = m_bRed ? GetRR() : 0;
	BYTE RG = m_bGreen ? GetRG() : 0;
	BYTE RB = m_bBlue ? GetRB() : 0;

	m_BtnRGB1.EnableWindow();
	m_BtnRGB2.EnableWindow(m_bGradient);
	m_BtnRGB3.EnableWindow(m_bGradient);
	m_BtnRGB4.EnableWindow(m_bGradient);
	m_BtnRGB5.EnableWindow(m_bGradient);
	m_BtnRGB6.EnableWindow();
	m_Gradient.EnableWindow(m_bGradient);
	m_BtnBase.EnableWindow(m_bBase);

	// Set the colors for editing
	SetColors();

	// Update the gradient with the colors
	std::vector<std::vector<BYTE> > vecColorMap = m_EditColors.GetColorMap();
	std::vector<COLORREF> vecRGB(m_iIterations);
	for (UINT iColor = 0;iColor < m_iIterations;++iColor)
		vecRGB[iColor] = RGB(vecColorMap[0][iColor + 1],vecColorMap[1][iColor + 1],vecColorMap[2][iColor + 1]);
	m_Gradient.UpdateColors(&vecRGB[0],m_iIterations,GetRR(),GetRG(),GetRB(),m_bBase,RGBBase);
}

// Set the colors for editing
void CNewFractal::SetColors()
{
	COLORREF RGB1 = MakeRGB(GetRGB1());
	COLORREF RGB2 = MakeRGB(GetRGB2());
	COLORREF RGB3 = MakeRGB(GetRGB3());
	COLORREF RGB4 = MakeRGB(GetRGB4());
	COLORREF RGB5 = MakeRGB(GetRGB5());
	COLORREF RGB6 = MakeRGB(GetRGB6());

	m_EditColors.SetColors(m_iIterations);
	if (m_bGradient)
		m_EditColors.SetRGB(RGB1,RGB2,RGB3,RGB4,RGB5,RGB6,GetRR(),GetRG(),GetRB(),m_bRed,m_bGreen,m_bBlue);
	else
		m_EditColors.SetRGB(RGB1,RGB6,GetRR(),GetRG(),GetRB(),m_bRed,m_bGreen,m_bBlue);
}

COLORREF CNewFractal::MakeRGB(COLORREF crRGB)
{
	BYTE RR = m_bRed ? GetRValue(crRGB) : 0;
	BYTE RG = m_bGreen ? GetGValue(crRGB) : 0;
	BYTE RB = m_bBlue ? GetBValue(crRGB) : 0;
	return RGB(RR,RG,RB);
}

void CNewFractal::ChangeColor(COLORREF & crRGB,CImageButton & BtnRGB)
{
	if (theApp.ChangeColor(crRGB,BtnRGB) == IDOK)
		RedrawButtons();
}

void CNewFractal::OnCheckGradient()
{
	UpdateData();
	RedrawButtons();
}

void CNewFractal::OnCheckBase()
{
	UpdateData();
	RedrawButtons();
}

void CNewFractal::OnBase()
{
	ChangeColor(m_RGBBase,m_BtnBase);
}

void CNewFractal::OnClickColor()
{
	UpdateData();
	RedrawButtons();
}

void CNewFractal::OnChangeScaleColor()
{
	UpdateData();
	RedrawButtons();
}

void CNewFractal::OnRandom()
{
	UpdateData();
	CRandom Random(0,255);
	if (!m_bGradient)
	{
		double dMin = 0;
		double dMax = 255;
		Random.SetRangeMin(dMin);
		Random.SetRangeMax(dMax);
		m_RR = (int)Random.NextRandomRange();
		m_RG = (int)Random.NextRandomRange();
		m_RB = (int)Random.NextRandomRange();
	}
	else
	{
		m_RGB1 = RGB(Random.NextRandomRange(),Random.NextRandomRange(),Random.NextRandomRange());
		m_RGB2 = RGB(Random.NextRandomRange(),Random.NextRandomRange(),Random.NextRandomRange());
		m_RGB3 = RGB(Random.NextRandomRange(),Random.NextRandomRange(),Random.NextRandomRange());
		m_RGB4 = RGB(Random.NextRandomRange(),Random.NextRandomRange(),Random.NextRandomRange());
		m_RGB5 = RGB(Random.NextRandomRange(),Random.NextRandomRange(),Random.NextRandomRange());
		m_RGB6 = RGB(Random.NextRandomRange(),Random.NextRandomRange(),Random.NextRandomRange());
	}
	UpdateData(FALSE);
	RedrawButtons();
}

void CNewFractal::OnReverse()
{
	UpdateData();
	COLORREF RGB1 = m_RGB1;
	COLORREF RGB2 = m_RGB2;
	COLORREF RGB3 = m_RGB3;
	COLORREF RGB4 = m_RGB4;
	COLORREF RGB5 = m_RGB5;
	COLORREF RGB6 = m_RGB6;

	bool bReverse = RGB1 == m_RGB1 ? true : false;
	m_RGB1 = bReverse ? RGB6 : RGB1;
	m_RGB2 = bReverse ? RGB5 : RGB2;
	m_RGB3 = bReverse ? RGB4 : RGB3;
	m_RGB4 = bReverse ? RGB3 : RGB4;
	m_RGB5 = bReverse ? RGB2 : RGB5;
	m_RGB6 = bReverse ? RGB1 : RGB6;

	UpdateData(FALSE);
	RedrawButtons();
}

void CNewFractal::OnEditColors()
{
	// OnOK reads the color map
	if (m_EditColors.DoModal() == IDOK)
		RedrawButtons();
}

// Update a color
void CNewFractal::UpdateColor(std::vector<std::vector<BYTE> > vecRGB)
{
	m_EditColors.UpdateColor(vecRGB);
}

void CNewFractal::OnKillFocusEditIter()
{
	UpdateData();
	RedrawButtons();
}

void CNewFractal::OnReset()
{
	UpdateData();
	m_RR = m_RG = m_RB = 0;
	m_bRed = m_bGreen = m_bBlue = TRUE;
	m_RGB1 = RGB(0,0,0);
	m_RGB2 = RGB(51,51,51);
	m_RGB3 = RGB(102,102,102);
	m_RGB4 = RGB(153,153,153);
	m_RGB5 = RGB(204,204,204);
	m_RGB6 = RGB(255,255,255);
	UpdateData(FALSE);
	RedrawButtons();
}

void CNewFractal::OnTrapX()
{
	UpdateData();
	m_TrapX.EnableWindow(m_bTrapX);
}

void CNewFractal::OnTrapY()
{
	UpdateData();
	m_TrapY.EnableWindow(m_bTrapY);
}

void CNewFractal::OnRGB1()
{
	ChangeColor(m_RGB1,m_BtnRGB1);
}

void CNewFractal::OnRGB2()
{
	ChangeColor(m_RGB2,m_BtnRGB2);
}

void CNewFractal::OnRGB3()
{
	ChangeColor(m_RGB3,m_BtnRGB3);
}

void CNewFractal::OnRGB4()
{
	ChangeColor(m_RGB4,m_BtnRGB4);
}

void CNewFractal::OnRGB5()
{
	ChangeColor(m_RGB5,m_BtnRGB5);
}

void CNewFractal::OnRGB6()
{
	ChangeColor(m_RGB6,m_BtnRGB6);
}

void CNewFractal::OnStepColor()
{
	ChangeColor(m_RGBStepColor,m_BtnStepColor);
}

void CNewFractal::OnDropUnused()
{
	UpdateData();
}

void CNewFractal::OnSmooth()
{
	UpdateData();
	m_BtnLT.EnableWindow(m_bSmooth);
	m_BtnGT.EnableWindow(m_bSmooth);
}

// Choose an image for blending
BOOL CNewFractal::ChooseImage(LPCTSTR lpszTitle,CString & csNewImage)
{
	BOOL bRet = FALSE;
	CString csImage;
	bRet = theApp.ChooseImage(lpszTitle,csImage);
	if (!bRet)
		return bRet;

	// Load the background image
	CImage Image;
	if (SUCCEEDED(Image.Load(csImage)))
	{
		int iWidth = Image.GetWidth();
		int iHeight = Image.GetHeight();
		if (iWidth != m_iWidth || iHeight != m_iHeight)
		{
			CString csMsg;
			csMsg.Format(L"Change resolution of %d x %d to match image of %d x %d?",m_iWidth,m_iHeight,iWidth,iHeight);
			if (AfxMessageBox(csMsg,MB_YESNO) == IDYES)
			{
				m_iWidth = iWidth;
				m_iHeight = iHeight;
				UpdateData(FALSE);
			}
		}
		csNewImage = csImage;
		bRet = TRUE;
	}
	return bRet;
}

// Choose a background image to render over
void CNewFractal::OnChooseImage()
{
	// Choose the image
	m_bUseImage = ChooseImage(_T("Choose Base Background Image"),m_csImage);

	// Update the controls
	m_BtnBlendBase.EnableWindow(m_bUseImage);
	m_Static_Alpha.EnableWindow(m_bUseImage);
	m_Alpha.EnableWindow(m_bUseImage);
}

// Choose a background image to render over
void CNewFractal::OnChooseImage2()
{
	// Choose the image
	m_bUseImage2 = ChooseImage(_T("Choose Fractal Background Image"),m_csImage2);

	// Update the controls
	m_BtnBlendFractal.EnableWindow(m_bUseImage2);
	m_Static_Alpha2.EnableWindow(m_bUseImage2);
	m_Alpha2.EnableWindow(m_bUseImage2);
}

void CNewFractal::OnClickGamma()
{
	UpdateData();
	m_GCR.EnableWindow(m_bGCR);
	m_GCG.EnableWindow(m_bGCG);
	m_GCB.EnableWindow(m_bGCB);
	RedrawButtons();
}

void CNewFractal::OnClickDonate()
{
	CStringA cs64("AAAAUkYAAAAAAwYAAAABCQAAAAELAAAAAg0AAAABFQAAAAEWAAAAARgAAAABGQAAAAEaAAAAAR4AAAABIQAAAAEiAAAAASgAAAACKQAAAAErAAAAAS0AAAABLgAAAAEyAAAAATgAAAABQwAAAAFHAAAAAUsAAAACUQAAAAFZAAAAAV0AAAABXgAAAAFoAAAAAWkAAAABcQAAAAFyAAAAAXgAAAACeQAAAAF6AAAAAn0AAAABfwAAAAGAAAAAAoQAAAABhgAAAAGLAAAAAYwAAAACjgAAAAGUAAAAAZkAAAABnAAAAAGdAAAAAaMAAAABpAAAAAGnAAAAAaoAAAACsAAAAAG0AAAAAboAAAABwAAAAAHBAAAAAcIAAAABwwAAAAHGAAAAAccAAAAByQAAAAHNAAAAAc4AAAAB2QAAAAHbAAAAAeEAAAAB4wAAAAHkAAAAAegAAAAC8gAAAAH7AAAAAf4AAAABFjmMqUMbHsKIY3Kr89C/IfYJxtd42LkmJSO82A08jOJ3Un7VYFVhutXGctQDBHC10Wc");
	int nLen64 = cs64.GetLength();
	char * p64 = cs64.GetBuffer();
	int nUnLen = Base64DecodeGetRequiredLength(nLen64);
	std::vector<char> vecUn64(nUnLen);
	Base64Decode(p64,nLen64,(BYTE *)&vecUn64[0],&nUnLen);
	std::vector<BYTE> vecUnLink(nUnLen);
	char * pUnLink = (char *)&vecUnLink[0];
	unsigned long nbUnLink = 0;
	CArithmeticEncoder AE;
	AE.DecodeBuffer(&vecUn64[0],nUnLen,&pUnLink,&nbUnLink,FALSE);
	vecUnLink.resize(nbUnLink);
	CStringA csUnLink;
	CCrypt Ring;
	Ring.Decrypt(vecUnLink,csUnLink);

	// Open the donation website
	ShellExecuteA(0,NULL,csUnLink,NULL,NULL,SW_SHOWDEFAULT);
}

BOOL CNewFractal::OnHelpInfo(HELPINFO* pHelpInfo)
{
	CHelpDialog HelpDialog;
	HelpDialog.DoModal();
	return CDialog::OnHelpInfo(pHelpInfo);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// CNewFractalType dialog

IMPLEMENT_DYNAMIC(CNewFractalType, CDialog)

CNewFractalType::CNewFractalType(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFractalType::IDD, pParent)
{
	m_iChoice = 0;
	m_csText = _T("Select the type of fractal to create.  This is either of Mandelbrot style or Julia set style.\n\nMandelbrot styles can jump to a Julia set by using a point on the plot.  To access this option, right-click over a point on the Mandelbrot plot and select Julia from the context sensitive menu");
}

CNewFractalType::~CNewFractalType()
{
}

void CNewFractalType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MANDELBROT, m_iChoice);
	DDX_Text(pDX, IDC_STATIC_FRACTAL_TYPE, m_csText);
}

BEGIN_MESSAGE_MAP(CNewFractalType, CDialog)
END_MESSAGE_MAP()

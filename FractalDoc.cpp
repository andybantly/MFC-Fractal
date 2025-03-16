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
#include <math.h>
#include "Fractal.h"
#include "FractalDoc.h"
#include "FractalBase.h"
#include "Mandelbrot.h"
#include "Julia.h"
#include "FractalView.h"
#include "MainFrm.h"
#include "AC.H"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFractalDoc

IMPLEMENT_DYNCREATE(CFractalDoc, CDocument)

BEGIN_MESSAGE_MAP(CFractalDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CFractalDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CFractalDoc::OnFileSaveAs)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFractalDoc, CDocument)
	DISP_PROPERTY_EX_ID(CFractalDoc, "XMin", 1, GetXMin, SetXMin, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "XMax", 2, GetXMax, SetXMax, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "YMin", 3, GetYMin, SetYMin, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "YMax", 4, GetYMax, SetYMax, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "P", 5, GetP, SetP, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Q", 6, GetQ, SetQ, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Width", 7, GetWidth, SetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Height", 8, GetHeight, SetHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Mode", 9, GetMode, SetMode, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Iterations", 10, GetIterations, SetIterations, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "MaxCalc", 11, GetMaxCalc, SetMaxCalc, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Red", 12, GetRed, SetRed, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Green", 13, GetGreen, SetGreen, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Blue", 14, GetBlue, SetBlue, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "DropUnused", 15, GetDropUnused, SetDropUnused, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Smooth", 16, GetSmooth, SetSmooth, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "SortOrder", 17, GetSortOrder, SetSortOrder, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Gradient", 18, GetGradient, SetGradient, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Base", 19, GetBase, SetBase, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "BaseColor", 20, GetBaseColor, SetBaseColor, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "OrbitColor", 21, GetOrbitColor, SetOrbitColor, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGB1", 22, GetRGB1, SetRGB1, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGB2", 23, GetRGB2, SetRGB2, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGB3", 24, GetRGB3, SetRGB3, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGB4", 25, GetRGB4, SetRGB4, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGB5", 26, GetRGB5, SetRGB5, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGB6", 27, GetRGB6, SetRGB6, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGBBase", 28, GetRGBBase, SetRGBBase, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RR", 29, GetRR, SetRR, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RG", 30, GetRG, SetRG, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RB", 31, GetRB, SetRB, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Step", 32, GetStep, SetStep, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "RGBStepColor", 33, GetRGBStepColor, SetRGBStepColor, VT_UI4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Image", 34, GetImage, SetImage, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFractalDoc, "UseImage", 35, GetUseImage, SetUseImage, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Image2", 36, GetImage2, SetImage2, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFractalDoc, "UseImage2", 37, GetUseImage2, SetUseImage2, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "FractalType", 38, GetFractalType, SetFractalType, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Alpha", 39, GetAlpha, SetAlpha, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Alpha2", 40, GetAlpha2, SetAlpha2, VT_I4)
	DISP_PROPERTY_EX_ID(CFractalDoc, "BlendBase", 41, GetBlendBase, SetBlendBase, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "BlendFractal", 42, GetBlendFractal, SetBlendFractal, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "FileName", 43, GetFileName, SetFileName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFractalDoc, "XScale", 44, GetXScale, SetXScale, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "YScale", 45, GetYScale, SetYScale, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "BailRadius", 46, GetBailRadius, SetBailRadius, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "TrapX", 47, GetTrapX, SetTrapX, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "TrapXValue", 48, GetTrapXValue, SetTrapXValue, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "TrapY", 49, GetTrapY, SetTrapY, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "TrapYValue", 50, GetTrapYValue, SetTrapYValue, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "GCR", 51, GetGCR, SetGCR, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "GCRValue", 52, GetGCRValue, SetGCRValue, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "GCG", 53, GetGCG, SetGCG, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "GCGValue", 54, GetGCGValue, SetGCGValue, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "GCB", 55, GetGCB, SetGCB, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFractalDoc, "GCBValue", 56, GetGCBValue, SetGCBValue, VT_R8)
	DISP_PROPERTY_EX_ID(CFractalDoc, "Picture", 57, GetPicture, SetPicture, VT_DISPATCH)
	DISP_FUNCTION_ID(CFractalDoc, "Render", 58, Render, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Note: we add support for IID_ITestAutoServer to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {76900ADC-BD0F-4e83-825D-96CF4DCC4D1C}
static const GUID IID_IFractalDoc = 
{ 0x76900adc, 0xbd0f, 0x4e83, { 0x82, 0x5d, 0x96, 0xcf, 0x4d, 0xcc, 0x4d, 0x1c } };

BEGIN_INTERFACE_MAP(CFractalDoc, CDocument)
	INTERFACE_PART(CFractalDoc, IID_IFractalDoc, Dispatch)
END_INTERFACE_MAP()

// CFractalDoc construction/destruction
CFractalDoc::CFractalDoc()
{
	EnableAutomation();
	AfxOleLockApp();

	m_bOpen = FALSE;
	m_iMode = 1;
	m_iIterations = 32;
	m_nMaxCalc = 512;
	m_bRed = TRUE;
	m_bGreen = TRUE;
	m_bBlue = TRUE;
	m_bDropUnused = FALSE;
	m_bSmooth = FALSE;
	m_bSortOrder = FALSE;
	m_bGradient = FALSE;
	m_bBase = FALSE;
	m_bBaseColor = TRUE;
	m_bOrbitColor = TRUE;
	m_RGB1 = RGB(0,0,0);
	m_RGB2 = RGB(255,255,255);
	m_RGBBase = RGB(0,0,0);
	m_RR = 0;
	m_RG = 0;
	m_RB = 0;
	m_dXMin = -2.0;
	m_dXMax = 1.0;
	m_dYMin = -1.0;
	m_dYMax = 1.0;

	m_NXMn = -20000000000000000;
	m_NXMx =  10000000000000000;
	m_NYMn = -10000000000000000;
	m_NYMx =  10000000000000000;

	m_iHeight = 480;
	m_iWidth = (int)((double)m_iHeight * fabs((m_dXMax - m_dXMin) / (m_dYMax - m_dYMin)));
	m_dP = 0.238498;
	m_dQ = 0.512321;
	m_dXScale = 1;
	m_dYScale = 1;
	m_dBailRadius = 4;
	m_dStep = 0.0;
	m_bUseImage = FALSE;
	m_bUseImage2 = FALSE;
	m_iFractalType = 0;
	m_bEmbedImage = FALSE;
	m_RGB.resize(3);
	m_iAlpha = 0;
	m_iAlpha2 = 0;
	m_bBlendBase = FALSE;
	m_bBlendFractal = FALSE;
	m_RGBStepColor = RGB(0,0,0);
	m_bTrapX = FALSE;
	m_dTrapX = 0.01;
	m_bTrapY = FALSE;
	m_dTrapY = 0.01;
	m_bGCR = TRUE;
	m_dGCR = 2.0;
	m_bGCG = TRUE;
	m_dGCG = 2.0;
	m_bGCB = TRUE;
	m_dGCB = 2.0;
}

CFractalDoc::~CFractalDoc()
{
	AfxOleUnlockApp();
}

// Open a document
BOOL CFractalDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	BOOL bRet = CDocument::OnOpenDocument(lpszPathName);
	m_bOpen = TRUE;

	// If the file format changes then modify the write code to write the new information and uncomment SetModifiedFlag so the file opens dirty
	// Load all the documents at once and then quit choosing to save each document
	// Add the code to read the new information and comment out SetModifiedFlag
	// SetModifiedFlag();

	return bRet;
}

// New document
BOOL CFractalDoc::OnNewDocument()
{
	// Test for being run as an automation server
	if (m_bEmbedded)
	{
		// Create a new document, with basic creation settings, since the automation model will be used to specify the fractal parameters
		if (!CDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	// Test for zooming into a new document
	int iFractalType = 0;
	BOOL bZoomNew = theApp.GetZoomNew(iFractalType);
	if (!bZoomNew)
	{
		// Normal file->new
		CNewFractalType NewFractalType;
		NewFractalType.SetChoice(iFractalType);
		if (NewFractalType.DoModal() == IDCANCEL)
			return FALSE;
		iFractalType = NewFractalType.GetChoice();
		theApp.SetLastChoice(iFractalType);
	}

	if (!CDocument::OnNewDocument())
		return FALSE;

	// Get fractal dialog for this type
	m_iFractalType = iFractalType;
	CNewFractal & Fractal = theApp.GetFractalDialog(m_iFractalType);

	// Display the new fractal dialog
	if (Fractal.DoModal() != IDOK)
		return FALSE;

	// Set the width and height
	SetWidth(Fractal.GetWidth());
	SetHeight(Fractal.GetHeight());

	// Check the aspect ratio
	double dXMin = Fractal.GetXMin();
	double dXMax = Fractal.GetXMax();
	double dYMin = Fractal.GetYMin();
	double dYMax = Fractal.GetYMax();
	double dY = dYMax - dYMin;
	double dX = dXMax - dXMin;
	double dWAspect = fabs(dX / dY);
	double dHAspect = fabs(dY / dX);
	UINT iWidth = (UINT)((double)GetHeight() * dWAspect);
	UINT iHeight = (UINT)((double)GetWidth() * dHAspect);

	if (iWidth != GetWidth() || iHeight != GetHeight())
	{
		if (iWidth != GetWidth() && iWidth < (UINT)GetWidth())
		{
			// Ask to adjust the width aspect ratio to match the fractal
			CString csAdjust;
			csAdjust.Format(L"Adjust the width from %d to %d to match the Fractals current aspect ratio?",Fractal.GetWidth(),iWidth);
			int iAdjust = AfxMessageBox(csAdjust,MB_YESNO);
			if (iAdjust == IDYES)
				SetWidth(iWidth);
		}
		else
		{
			// Ask to adjust the height aspect ratio to match the fractal
			CString csAdjust;
			csAdjust.Format(L"Adjust the height from %d to %d to match the Fractals current aspect ratio?",Fractal.GetHeight(),iHeight);
			int iAdjust = AfxMessageBox(csAdjust,MB_YESNO);
			if (iAdjust == IDYES)
				SetHeight(iHeight);
		}
	}

	// Set the new fractal parameters
	SetFractalType(iFractalType);
	SetMode(Fractal.GetMode());
	SetIterations(Fractal.GetIterations());
	SetMaxCalc(Fractal.GetMaxCalc());
	SetDropUnused(Fractal.GetDropUnused());
	SetSmooth(Fractal.GetSmooth());
	SetSortOrder(Fractal.GetSortOrder());
	SetGradient(Fractal.GetGradient());
	SetBase(Fractal.GetBase());
	SetBaseColor(Fractal.GetBaseColor());
	SetOrbitColor(Fractal.GetOrbitColor());
	SetRGBBase(Fractal.GetRGBBase());
	SetRGB1(Fractal.GetRGB1());
	SetRGB2(Fractal.GetRGB2());
	SetRGB3(Fractal.GetRGB3());
	SetRGB4(Fractal.GetRGB4());
	SetRGB5(Fractal.GetRGB5());
	SetRGB6(Fractal.GetRGB6());
	SetRR(Fractal.GetRR());
	SetRG(Fractal.GetRG());
	SetRB(Fractal.GetRB());
	SetRed(Fractal.GetRed());
	SetGreen(Fractal.GetGreen());
	SetBlue(Fractal.GetBlue());
	SetXMin(Fractal.GetXMin());
	SetXMax(Fractal.GetXMax());
	SetYMin(Fractal.GetYMin());
	SetYMax(Fractal.GetYMax());
	SetP(Fractal.GetP());
	SetQ(Fractal.GetQ());
	SetXScale(Fractal.GetXScale());
	SetYScale(Fractal.GetYScale());
	SetBailRadius(Fractal.GetBailRadius());
	SetStep(Fractal.GetStep());
	SetImage(CComBSTR(Fractal.GetImage()));
	SetUseImage(Fractal.GetUseImage());
	SetImage2(CComBSTR(Fractal.GetImage2()));
	SetUseImage2(Fractal.GetUseImage2());
	SetAlpha(Fractal.GetAlpha());
	SetAlpha2(Fractal.GetAlpha2());
	SetBlendBase(Fractal.GetBlendBase());
	SetBlendFractal(Fractal.GetBlendFractal());
	SetRGBStepColor(Fractal.GetRGBStepColor());
	SetRGB(Fractal.GetRGB());
	SetTrapX(Fractal.GetTrapX());
	SetTrapXValue(Fractal.GetTrapXValue());
	SetTrapY(Fractal.GetTrapY());
	SetTrapYValue(Fractal.GetTrapYValue());
	SetGCR(Fractal.GetGCR());
	SetGCRValue(Fractal.GetGCRValue());
	SetGCG(Fractal.GetGCG());
	SetGCGValue(Fractal.GetGCGValue());
	SetGCB(Fractal.GetGCB());
	SetGCBValue(Fractal.GetGCBValue());
	SetModifiedFlag();
	return TRUE;
}

void CFractalDoc::OnCloseDocument()
{
	CMainFrame * pMainFrame = (CMainFrame *)(AfxGetApp()->GetMainWnd());
	CString cs("");
	pMainFrame->SetCoord(cs);
	CDocument::OnCloseDocument();
}

// CFractalDoc serialization
void CFractalDoc::OnFileSave()
{
	CString csFileName = GetTitle();
	csFileName.Replace(L" *",L"");
	csFileName.Replace(L" :",L"");
	if (csFileName.Find(_T(".frc")) >= 0)
		csFileName = csFileName.Left(csFileName.GetLength() - 4);
	csFileName += _T(".frc");
	CFractalFileDialog FileSave(FALSE,_T("Save Fractal"),_T(".frc"),csFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATE | OFN_ENABLESIZING,
		_T("Fractal Files (*.frc)|*.frc||"),FALSE,AfxGetApp()->GetMainWnd());
	if (FileSave.DoModal() == IDOK)
	{
		CWaitCursor Wait;
		m_bEmbedImage = FileSave.GetEmbedded();
		POSITION Pos = FileSave.GetStartPosition();
		csFileName = FileSave.GetNextPathName(Pos);
		CFile File;
		File.Open(csFileName,CFile::modeCreate | CFile::modeWrite);
		CArchive ar(&File,CArchive::store);
		Serialize(ar);
		SetModifiedFlag(FALSE);
		SetTitle(csFileName);
	}
}

void CFractalDoc::OnFileSaveAs()
{
	CString csFileName = GetTitle();
	csFileName.Replace(L" *",L"");
	csFileName.Replace(L" :",L"");
	if (csFileName.Find(_T(".frc")) >= 0)
		csFileName = csFileName.Left(csFileName.GetLength() - 4);
	csFileName += _T(".png");
	CFractalFileDialog FileSave(FALSE,_T("Save Fractal As Image"),_T(".png"),csFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING,
		_T("Image Files (*.png)|*.png||"),TRUE,AfxGetApp()->GetMainWnd());
	if (FileSave.DoModal() == IDOK)
	{
		CWaitCursor Wait;
		POSITION Pos = FileSave.GetStartPosition();
		csFileName = FileSave.GetNextPathName(Pos);
		CImage Document;
		Document.Attach(m_DoubleBufferDIB);
		Document.Save(csFileName);
		Document.Detach();
	}
}

void CFractalDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_iWidth;
		ar << m_iHeight;
		ar << m_iMode;
		ar << m_iIterations;
		ar << m_nMaxCalc;
		ar << m_bRed;
		ar << m_bGreen;
		ar << m_bBlue;
		ar << m_bDropUnused;
		ar << m_bSmooth;
		ar << m_bSortOrder;
		ar << m_bGradient;
		ar << m_RGB1;
		ar << m_RGB2;
		ar << m_RGB3;
		ar << m_RGB4;
		ar << m_RGB5;
		ar << m_RGB6;
		ar << m_RR;
		ar << m_RG;
		ar << m_RB;
		int nRGB = (int)m_RGB[0].size();
		ar << nRGB;
		for (int iRGB = 0;iRGB < nRGB;++iRGB)
		{
			BYTE BR,BG,BB;
			BR = m_RGB[0][iRGB];
			BG = m_RGB[1][iRGB];
			BB = m_RGB[2][iRGB];
			ar << BR;
			ar << BG;
			ar << BB;
		}
		ar << m_dXMin;
		ar << m_dXMax;
		ar << m_dYMin;
		ar << m_dYMax;
		ar << m_dP;
		ar << m_dQ;
		ar << m_dXScale;
		ar << m_dYScale;
		ar << m_dBailRadius;
		ar << m_dStep;
		ar << m_bBase;
		ar << m_RGBBase;
		ar << m_csImage;
		ar << m_bUseImage;
		ar << m_csImage2;
		ar << m_bUseImage2;
		ar << m_iFractalType;
		ar << m_bBaseColor;
		ar << m_bOrbitColor;
		int nDecorativeText = (int)m_vDecorativeText.size();
		ar << nDecorativeText;
		for (int iDecorativeText = 0;iDecorativeText < nDecorativeText;++iDecorativeText)
		{
			CDecorativeText & DecorativeText = m_vDecorativeText[iDecorativeText];
			CRect DIBRect = DecorativeText.GetRect();
			ar << DIBRect;
			CString csFontName = DecorativeText.GetFontName();
			ar << csFontName;
			int iWeight = DecorativeText.GetWeight();
			ar << iWeight;
			BOOL bItalics = DecorativeText.GetItalics();
			ar << bItalics;
			UINT iPtSize = DecorativeText.GetPtSize();
			ar << iPtSize;
			UINT iOpacity = DecorativeText.GetOpacity();
			ar << iOpacity;
			COLORREF TextRGB = DecorativeText.GetTextRGB();
			ar << TextRGB;
			COLORREF BackRGB = DecorativeText.GetBackRGB();
			ar << BackRGB;
			CString csText = DecorativeText.GetText();
			ar << csText;
			BOOL bNoBackColor = DecorativeText.GetNoBackColor();
			ar << bNoBackColor;
		}

		int nDecorativePicture = (int)m_vDecorativePicture.size();
		ar << nDecorativePicture;
		for (int iDecorativePicture = 0;iDecorativePicture < nDecorativePicture;++iDecorativePicture)
		{
			CDecorativePicture & DecorativePicture = m_vDecorativePicture[iDecorativePicture];
			CRect DIBRect = DecorativePicture.GetRect();
			ar << DIBRect;
			UINT uiOpacity = DecorativePicture.GetOpacity();
			ar << uiOpacity;
			CString csPictureName = DecorativePicture.GetPictureName();
			ar << csPictureName;
			BOOL bTransparentColor = DecorativePicture.GetTransparentColorFlag();
			ar << bTransparentColor;
			COLORREF crTransparentColor = DecorativePicture.GetTransparentColor();
			ar << crTransparentColor;
			SaveImage(ar,csPictureName);
		}

		SaveImage(ar,m_csImage);
		SaveImage(ar,m_csImage2);

		ar << m_iAlpha;
		ar << m_iAlpha2;
		ar << m_bBlendBase;
		ar << m_bBlendFractal;
		ar << m_RGBStepColor;
		ar << m_bTrapX;
		ar << m_dTrapX;
		ar << m_bTrapY;
		ar << m_dTrapY;
		ar << m_bGCR;
		ar << m_dGCR;
		ar << m_bGCG;
		ar << m_dGCG;
		ar << m_bGCB;
		ar << m_dGCB;
	}
	else
	{
		ar >> m_iWidth;
		ar >> m_iHeight;
		ar >> m_iMode;
		ar >> m_iIterations;
		ar >> m_nMaxCalc;
		ar >> m_bRed;
		ar >> m_bGreen;
		ar >> m_bBlue;
		ar >> m_bDropUnused;
		ar >> m_bSmooth;
		ar >> m_bSortOrder;
		ar >> m_bGradient;
		ar >> m_RGB1;
		ar >> m_RGB2;
		ar >> m_RGB3;
		ar >> m_RGB4;
		ar >> m_RGB5;
		ar >> m_RGB6;
		ar >> m_RR;
		ar >> m_RG;
		ar >> m_RB;
		int nRGB;
		ar >> nRGB;
		m_RGB[0].resize(nRGB);
		m_RGB[1].resize(nRGB);
		m_RGB[2].resize(nRGB);
		for (int iRGB = 0;iRGB < nRGB;++iRGB)
		{
			BYTE BR,BG,BB;
			ar >> BR;
			ar >> BG;
			ar >> BB;
			m_RGB[0][iRGB] = BR;
			m_RGB[1][iRGB] = BG;
			m_RGB[2][iRGB] = BB;
		}
		ar >> m_dXMin;
		ar >> m_dXMax;
		ar >> m_dYMin;
		ar >> m_dYMax;
		ar >> m_dP;
		ar >> m_dQ;
		ar >> m_dXScale;
		ar >> m_dYScale;
		ar >> m_dBailRadius;
		ar >> m_dStep;
		ar >> m_bBase;
		ar >> m_RGBBase;
		ar >> m_csImage;
		ar >> m_bUseImage;
		ar >> m_csImage2;
		ar >> m_bUseImage2;
		ar >> m_iFractalType;
		ar >> m_bBaseColor;
		ar >> m_bOrbitColor;

		int nDecorativeText;
		ar >> nDecorativeText;
		for (int iDecorativeText = 0;iDecorativeText < nDecorativeText;++iDecorativeText)
		{
			CRect DIBRect;
			ar >> DIBRect;
			CString csFontName;
			ar >> csFontName;
			int iWeight;
			ar >> iWeight;
			BOOL bItalics;
			ar >> bItalics;
			UINT iPtSize;
			ar >> iPtSize;
			UINT iOpacity;
			ar >> iOpacity;
			COLORREF TextRGB;
			ar >> TextRGB;
			COLORREF BackRGB;
			ar >> BackRGB;
			CString csText;
			ar >> csText;
			BOOL bNoBackColor;
			ar >> bNoBackColor;
			CDecorativeText DecorativeText(DIBRect,csFontName,iWeight,bItalics,iPtSize,iOpacity,TextRGB,BackRGB,bNoBackColor,csText);
			AddTextToDoc(DecorativeText);
		}

		int nDecorativePicture;
		ar >> nDecorativePicture;
		for (int iDecorativePicture = 0;iDecorativePicture < nDecorativePicture;++iDecorativePicture)
		{
			CRect DIBRect;
			ar >> DIBRect;
			UINT uiOpacity;
			ar >> uiOpacity;
			CString csPictureName;
			ar >> csPictureName;
			BOOL bTransparentColor;
			ar >> bTransparentColor;
			COLORREF crTransparentColor;
			ar >> crTransparentColor;
			LoadImage(ar,csPictureName);
			CDecorativePicture DecorativePicture(DIBRect,csPictureName,uiOpacity,bTransparentColor,crTransparentColor);
			AddPictureToDoc(DecorativePicture);
		}

		LoadImage(ar,m_csImage);
		LoadImage(ar,m_csImage2);

		ar >> m_iAlpha;
		ar >> m_iAlpha2;
		ar >> m_bBlendBase;
		ar >> m_bBlendFractal;
		ar >> m_RGBStepColor;
		ar >> m_bTrapX;
		ar >> m_dTrapX;
		ar >> m_bTrapY;
		ar >> m_dTrapY;
		ar >> m_bGCR;
		ar >> m_dGCR;
		ar >> m_bGCG;
		ar >> m_dGCG;
		ar >> m_bGCB;
		ar >> m_dGCB;

		// Prime the renderer
		PrimeRender();
	}
}

// Save an image to the file
void CFractalDoc::SaveImage(CArchive & ar,CString & csPictureName)
{
	DWORD dwEncodedBytes = 0;
	if (m_bEmbedImage && csPictureName.GetLength())
	{
		// Persist the image
		CImage Image;
		if (SUCCEEDED(Image.Load(csPictureName)))
		{
			int iWidth = Image.GetWidth();
			int iHeight = Image.GetHeight();
			CDIBFrame DIB;
			DIB.Init(iWidth,iHeight);
			Image.BitBlt(DIB,0,0,SRCCOPY);
			CArithmeticEncoder AE;
			char * pEncodedBuffer = NULL;
			bool bEncode = AE.EncodeBuffer((LPSTR)DIB,DIB.GetSizeImage(),pEncodedBuffer,dwEncodedBytes);
			if (bEncode)
			{
				ar << dwEncodedBytes;
				ar << iWidth;
				ar << iHeight;
				ar.Write(pEncodedBuffer,dwEncodedBytes);
				delete [] pEncodedBuffer;
				pEncodedBuffer = NULL;
			}
			else
				ar << dwEncodedBytes;
		}
		else
			ar << dwEncodedBytes;
	}
	else
		ar << dwEncodedBytes;
}

// Load an image from the file
void CFractalDoc::LoadImage(CArchive & ar,CString & csPictureName)
{
	DWORD dwEncodedBytes;
	ar >> dwEncodedBytes;
	if (dwEncodedBytes)
	{
		int iWidth,iHeight;
		ar >> iWidth;
		ar >> iHeight;
		CDIBFrame DIB;
		DIB.Init(iWidth,iHeight);
		char * pDecodedBuffer = (LPSTR)DIB;
		DWORD dwDecodedBytes;
		char* pEncodedBuffer = new char[(size_t)(dwEncodedBytes)];
		ar.Read(pEncodedBuffer,dwEncodedBytes);
		CArithmeticEncoder AE;
		bool bDecode = AE.DecodeBuffer(pEncodedBuffer,dwEncodedBytes,&pDecodedBuffer,&dwDecodedBytes,FALSE);
		delete [] pEncodedBuffer;
		if (bDecode)
		{
			// Test for the existance of the file
			CFileStatus FileStatus;
			BOOL bFileStatus = CFile::GetStatus(csPictureName,FileStatus);
			if (bFileStatus)
			{
				// Test the image to be consistent with the width and height that was saved
				CImage Image;
				if (SUCCEEDED(Image.Load(csPictureName)))
				{
					if (Image.GetWidth() != iWidth || Image.GetHeight() != iHeight)
						bFileStatus = FALSE; // Recreate the file in the temp path
				}
			}
			if (!bFileStatus)
			{
				// Recreate the file in the temporary directory
				DWORD dwBufSize = GetTempPath(0,NULL);
				CString csTempPath;
				TCHAR * lpBuffer = csTempPath.GetBufferSetLength(dwBufSize);
				dwBufSize = GetTempPath(dwBufSize,lpBuffer);
				csTempPath.ReleaseBuffer();
				CString csFileName,csExt;
				TCHAR * lpFileName = csFileName.GetBufferSetLength(_MAX_FNAME);
				TCHAR * lpExt = csExt.GetBufferSetLength(_MAX_EXT);
				_tsplitpath(csPictureName,NULL,NULL,lpFileName,lpExt);
				csFileName.ReleaseBuffer();
				csExt.ReleaseBuffer();
				csTempPath += csFileName;
				csTempPath += csExt;
				CImage Image;
				Image.Attach(DIB);
				Image.Save(csTempPath);
				Image.Detach();
				csPictureName = csTempPath;
			}
		}
	}
}

// CFractalDoc diagnostics

#ifdef _DEBUG
void CFractalDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFractalDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CFractalDoc commands / automation
double CFractalDoc::GetXMin() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dXMin;
}

double CFractalDoc::GetXMax() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dXMax;
}

double CFractalDoc::GetYMin() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dYMin;
}

double CFractalDoc::GetYMax() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dYMax;
}

double CFractalDoc::GetP() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dP;
}

double CFractalDoc::GetQ() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dQ;
}

LONG CFractalDoc::GetWidth() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_iWidth;
}

LONG CFractalDoc::GetHeight()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_iHeight;
}

LONG CFractalDoc::GetMode() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_iMode;
}

LONG CFractalDoc::GetIterations() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_iIterations;
}

LONG CFractalDoc::GetMaxCalc() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_nMaxCalc;
}

VARIANT_BOOL CFractalDoc::GetRed() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bRed;
}

VARIANT_BOOL CFractalDoc::GetGreen() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bGreen;
}

VARIANT_BOOL CFractalDoc::GetBlue() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bBlue;
}

VARIANT_BOOL CFractalDoc::GetDropUnused() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bDropUnused;
}

VARIANT_BOOL CFractalDoc::GetSmooth() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bSmooth;
}

VARIANT_BOOL CFractalDoc::GetSortOrder() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bSortOrder;
}

VARIANT_BOOL CFractalDoc::GetGradient() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bGradient;
}

VARIANT_BOOL CFractalDoc::GetBase() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bBase;
}

VARIANT_BOOL CFractalDoc::GetBaseColor() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bBaseColor;
}

VARIANT_BOOL CFractalDoc::GetOrbitColor() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bOrbitColor;
}

COLORREF CFractalDoc::GetRGB1() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGB1;
}

COLORREF CFractalDoc::GetRGB2() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGB2;
}

COLORREF CFractalDoc::GetRGB3() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGB3;
}

COLORREF CFractalDoc::GetRGB4() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGB4;
}

COLORREF CFractalDoc::GetRGB5() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGB5;
}

COLORREF CFractalDoc::GetRGB6() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGB6;
}

COLORREF CFractalDoc::GetRGBBase() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGBBase;
}

LONG CFractalDoc::GetRR() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RR;
}

LONG CFractalDoc::GetRG() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RG;
}

LONG CFractalDoc::GetRB() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RB;
}

double CFractalDoc::GetXScale() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dXScale;
}

double CFractalDoc::GetYScale() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dYScale;
}

double CFractalDoc::GetBailRadius() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dBailRadius;
}

double CFractalDoc::GetStep() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dStep;
}

COLORREF CFractalDoc::GetRGBStepColor() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_RGBStepColor;
}

BSTR CFractalDoc::GetImage() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_csImage.AllocSysString();
}

VARIANT_BOOL CFractalDoc::GetUseImage() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bUseImage;
}

BSTR CFractalDoc::GetImage2() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_csImage2.AllocSysString();
}

VARIANT_BOOL CFractalDoc::GetUseImage2() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bUseImage2;
}

LONG CFractalDoc::GetFractalType() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_iFractalType;
}

LONG CFractalDoc::GetAlpha() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_iAlpha;
}

LONG CFractalDoc::GetAlpha2() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_iAlpha2;
}

VARIANT_BOOL CFractalDoc::GetBlendBase() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bBlendBase;
}

VARIANT_BOOL CFractalDoc::GetBlendFractal() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bBlendFractal;
}

BSTR CFractalDoc::GetFileName() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CString csPathFileName;
	if (m_csPathFileName.IsEmpty())
	{
		// Build the temporary file name
		CString csTempPath;
		TCHAR * pTempPath = csTempPath.GetBufferSetLength(MAX_PATH + 1);
		GetTempPath(MAX_PATH + 1,pTempPath);
		csTempPath.ReleaseBuffer();

		// Generate a temporary name
		CString csPathName;
		TCHAR * pTempFile = csPathName.GetBufferSetLength(MAX_PATH + 1);
		GetTempFileName(csTempPath,_T("FRC"),0,pTempFile);
		csPathName.ReleaseBuffer();
		csPathFileName = csPathName;
		csPathFileName.Replace(L".tmp",L".bmp");
		CFile::Rename(csPathName,csPathFileName);
	}
	else
		csPathFileName = m_csPathFileName;

	// Attach the DIB to the Image and save as a temporary file
	CImage ImageDIB;
	ImageDIB.Attach(GetFractal());
	if (FAILED(ImageDIB.Save(csPathFileName,Gdiplus::ImageFormatBMP)))
		csPathFileName.Empty();

	// Detach the DIB
	ImageDIB.Detach();

	// Set the file name that was created
	return csPathFileName.AllocSysString();
}

VARIANT_BOOL CFractalDoc::GetTrapX() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bTrapX;
}

double CFractalDoc::GetTrapXValue()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dTrapX;
}

VARIANT_BOOL CFractalDoc::GetTrapY() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bTrapY;
}

double CFractalDoc::GetTrapYValue()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dTrapY;
}

VARIANT_BOOL CFractalDoc::GetGCR() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bGCR;
}

double CFractalDoc::GetGCRValue()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dGCR;
}

VARIANT_BOOL CFractalDoc::GetGCG() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bGCG;
}

double CFractalDoc::GetGCGValue()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dGCG;
}

VARIANT_BOOL CFractalDoc::GetGCB() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_bGCB;
}

double CFractalDoc::GetGCBValue()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	return m_dGCB;
}

void CFractalDoc::SetXMin(double dXMin) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dXMin = dXMin;
}

void CFractalDoc::SetXMax(double dXMax) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dXMax = dXMax;
}

void CFractalDoc::SetYMin(double dYMin) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dYMin = dYMin;
}

void CFractalDoc::SetYMax(double dYMax) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dYMax = dYMax;
}

void CFractalDoc::SetP(double dP) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dP = dP;
}

void CFractalDoc::SetQ(double dQ) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dQ = dQ;
}

void CFractalDoc::SetWidth(LONG iWidth) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_iWidth = iWidth;
}

void CFractalDoc::SetHeight(LONG iHeight) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_iHeight = iHeight;
}

void CFractalDoc::SetMode(LONG iMode) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_iMode = iMode;
}

void CFractalDoc::SetIterations(LONG iIterations) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_iIterations = iIterations;
}

void CFractalDoc::SetMaxCalc(LONG nMaxCalc) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_nMaxCalc = nMaxCalc;
}

void CFractalDoc::SetRed(VARIANT_BOOL bRed) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bRed = bRed;
}

void CFractalDoc::SetGreen(VARIANT_BOOL bGreen) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bGreen = bGreen;
}

void CFractalDoc::SetBlue(VARIANT_BOOL bBlue) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bBlue = bBlue;
}

void CFractalDoc::SetDropUnused(VARIANT_BOOL bDropUnused) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bDropUnused = bDropUnused;
}

void CFractalDoc::SetSmooth(VARIANT_BOOL bSmooth) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bSmooth = bSmooth;
}

void CFractalDoc::SetSortOrder(VARIANT_BOOL bSortOrder) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bSortOrder = bSortOrder;
}

void CFractalDoc::SetGradient(VARIANT_BOOL bGradient) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bGradient = bGradient;
}

void CFractalDoc::SetBase(VARIANT_BOOL bBase) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bBase = bBase;
}

void CFractalDoc::SetBaseColor(VARIANT_BOOL bBaseColor) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bBaseColor = bBaseColor;
}

void CFractalDoc::SetOrbitColor(VARIANT_BOOL bOrbitColor) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bOrbitColor = bOrbitColor;
}

void CFractalDoc::SetRGB1(COLORREF RGB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGB1 = RGB;
}

void CFractalDoc::SetRGB2(COLORREF RGB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGB2 = RGB;
}

void CFractalDoc::SetRGB3(COLORREF RGB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGB3 = RGB;
}

void CFractalDoc::SetRGB4(COLORREF RGB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGB4 = RGB;
}

void CFractalDoc::SetRGB5(COLORREF RGB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGB5 = RGB;
}

void CFractalDoc::SetRGB6(COLORREF RGB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGB6 = RGB;
}

void CFractalDoc::SetRR(LONG RR) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RR = RR;
}

void CFractalDoc::SetRG(LONG RG) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RG = RG;
}

void CFractalDoc::SetRB(LONG RB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RB = RB;
}

void CFractalDoc::SetXScale(double dXScale) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dXScale = dXScale;
}

void CFractalDoc::SetYScale(double dYScale) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dYScale = dYScale;
}

void CFractalDoc::SetBailRadius(double dBailRadius) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dBailRadius = dBailRadius;
}

void CFractalDoc::SetStep(double dStep) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dStep = dStep;
}

void CFractalDoc::SetRGBStepColor(COLORREF RGBStepColor) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGBStepColor = RGBStepColor;
}

void CFractalDoc::SetRGBBase(COLORREF RGB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_RGBBase = RGB;
}

void CFractalDoc::SetImage(LPCTSTR Image) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_csImage = Image;
}

void CFractalDoc::SetUseImage(VARIANT_BOOL bUseImage) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bUseImage = bUseImage;
}

void CFractalDoc::SetImage2(LPCTSTR Image2) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_csImage2 = Image2;
}

void CFractalDoc::SetUseImage2(VARIANT_BOOL bUseImage2) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bUseImage2 = bUseImage2;
}

void CFractalDoc::SetFractalType(LONG iFractalType) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_iFractalType = iFractalType;
}

void CFractalDoc::SetAlpha(LONG iAlpha) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_iAlpha = iAlpha;
}

void CFractalDoc::SetAlpha2(LONG iAlpha2) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_iAlpha2 = iAlpha2;
}

void CFractalDoc::SetBlendBase(VARIANT_BOOL bBlendBase) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bBlendBase = bBlendBase;
}

void CFractalDoc::SetBlendFractal(VARIANT_BOOL bBlendFractal) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bBlendFractal = bBlendFractal;
}

void CFractalDoc::SetFileName(LPCTSTR FileName) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_csPathFileName = FileName;
}

void CFractalDoc::SetTrapX(VARIANT_BOOL bTrapX) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bTrapX = bTrapX;
}

void CFractalDoc::SetTrapXValue(double dTrapX)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dTrapX = dTrapX;
}

void CFractalDoc::SetTrapY(VARIANT_BOOL bTrapY) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bTrapY = bTrapY;
}

void CFractalDoc::SetTrapYValue(double dTrapY)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dTrapY = dTrapY;
}

void CFractalDoc::SetGCR(VARIANT_BOOL bGCR) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bGCR = bGCR;
}

void CFractalDoc::SetGCRValue(double dGCR)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dGCR = dGCR;
}

void CFractalDoc::SetGCG(VARIANT_BOOL bGCG) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bGCG = bGCG;
}

void CFractalDoc::SetGCGValue(double dGCG)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dGCG = dGCG;
}

void CFractalDoc::SetGCB(VARIANT_BOOL bGCB) 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_bGCB = bGCB;
}

void CFractalDoc::SetGCBValue(double dGCB)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_dGCB = dGCB;
}

// DON'T CALL THIS FROM AUTOMATION EVER
void CFractalDoc::RenderZoom(double dLastXMin,double dLastXMax,double dLastYMin,double dLastYMax,int iNewWidth)
{
	// Delete the current view frame
	GetFractal().DeleteFrame();

	// Render the new fractal
	RenderFractal(dLastXMin,dLastXMax,dLastYMin,dLastYMax,iNewWidth,m_RGB);

	// Clear the modified bit in case this is from an automation server
	SetModifiedFlag(FALSE);
}

// Render the fractal
void CFractalDoc::Render()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// Delete the current view frame
	GetFractal().DeleteFrame();

	if (theApp.IsAutomation())
	{
		// Prime the renderer
		PrimeRender();
	}

	// Render the new fractal
	RenderFractal();

	// Clear the modified bit in case this is from an automation server
	SetModifiedFlag(FALSE);
}

// Prime the renderer with the fractal properties
void CFractalDoc::PrimeRender()
{
	// Get fractal dialog for this type
	CNewFractal & Fractal = theApp.GetFractalDialog(m_iFractalType);
	theApp.SetLastChoice(m_iFractalType);

	// Set the loaded properties to the correct fractal dialog
	Fractal.SetWidth(m_iWidth);
	Fractal.SetHeight(m_iHeight);
	Fractal.SetMode(m_iMode);
	Fractal.SetIterations(m_iIterations);
	Fractal.SetMaxCalc(m_nMaxCalc);
	Fractal.SetRed(m_bRed);
	Fractal.SetGreen(m_bGreen);
	Fractal.SetBlue(m_bBlue);
	Fractal.SetDropUnused(m_bDropUnused);
	Fractal.SetSmooth(m_bSmooth);
	Fractal.SetSortOrder(m_bSortOrder);
	Fractal.SetGradient(m_bGradient);
	Fractal.SetBase(m_bBase);
	Fractal.SetBaseColor(m_bBaseColor);
	Fractal.SetOrbitColor(m_bOrbitColor);
	Fractal.SetRGB1(m_RGB1);
	Fractal.SetRGB2(m_RGB2);
	Fractal.SetRGB3(m_RGB3);
	Fractal.SetRGB4(m_RGB4);
	Fractal.SetRGB5(m_RGB5);
	Fractal.SetRGB6(m_RGB6);
	Fractal.SetRGBBase(m_RGBBase);
	Fractal.SetRR(m_RR);
	Fractal.SetRG(m_RG);
	Fractal.SetRB(m_RB);
	Fractal.SetXMin(m_dXMin);
	Fractal.SetXMax(m_dXMax);
	Fractal.SetYMin(m_dYMin);
	Fractal.SetYMax(m_dYMax);
	Fractal.SetP(m_dP);
	Fractal.SetQ(m_dQ);
	Fractal.SetXScale(m_dXScale);
	Fractal.SetYScale(m_dYScale);
	Fractal.SetBailRadius(m_dBailRadius);
	Fractal.SetStep(m_dStep);
	Fractal.SetImage(m_csImage);
	Fractal.SetUseImage(m_bUseImage);
	Fractal.SetImage2(m_csImage2);
	Fractal.SetUseImage2(m_bUseImage2);
	Fractal.SetAlpha(m_iAlpha);
	Fractal.SetAlpha2(m_iAlpha2);
	Fractal.SetBlendBase(m_bBlendBase);
	Fractal.SetBlendFractal(m_bBlendFractal);
	Fractal.SetRGBStepColor(m_RGBStepColor);
	Fractal.SetTrapX(m_bTrapX);
	Fractal.SetTrapXValue(m_dTrapX);
	Fractal.SetTrapY(m_bTrapY);
	Fractal.SetTrapYValue(m_dTrapY);
	Fractal.SetGCR(m_bGCR);
	Fractal.SetGCRValue(m_dGCR);
	Fractal.SetGCG(m_bGCG);
	Fractal.SetGCGValue(m_dGCG);
	Fractal.SetGCB(m_bGCB);
	Fractal.SetGCBValue(m_dGCB);
}

// Render the fractal image
void CFractalDoc::RenderFractal()
{
	// Get the parameters that are needed for generations of the fractal
	CDuration Duration(_T("Render Fractal"));
	CWaitCursor Wait;
	int iMode = GetMode();
	int iIterations = GetIterations();
	int nMaxCalc = GetMaxCalc();
	BOOL bDropUnused = GetDropUnused();
	BOOL bSmooth = GetSmooth();
	BOOL bRed = GetRed();
	BOOL bGreen = GetGreen();
	BOOL bBlue = GetBlue();
	BOOL bGradient = GetGradient();
	BOOL bBase = GetBase();
	BOOL bSortOrder = GetSortOrder();
	BOOL bBaseColor = GetBaseColor();
	BOOL bOrbitColor = GetOrbitColor();
	COLORREF RGB1 = GetRGB1();
	COLORREF RGB2 = GetRGB2();
	COLORREF RGB3 = GetRGB3();
	COLORREF RGB4 = GetRGB4();
	COLORREF RGB5 = GetRGB5();
	COLORREF RGB6 = GetRGB6();
	COLORREF RGBBase = GetRGBBase();
	COLORREF RGBStepColor = GetRGBStepColor();
	int MC = GetWidth();
	int MR = GetHeight();
	double XMin = GetXMin();
	double XMax = GetXMax();
	double YMin = GetYMin();
	double YMax = GetYMax();
	double dP = GetP();
	double dQ = GetQ();
	double dXScale = GetXScale();
	double dYScale = GetYScale();
	double dBailRadius = GetBailRadius();
	double dStep = GetStep();
	CComBSTR ccbImage;
	ccbImage.Attach(GetImage());
	CString csImage(ccbImage);
	BOOL bUseImage = GetUseImage();
	CComBSTR ccbImage2;
	ccbImage2.Attach(GetImage2());
	CString csImage2(ccbImage2);
	BOOL bUseImage2 = GetUseImage2();
	int iFractalType = GetFractalType();
	int iAlpha = GetAlpha();
	int iAlpha2 = GetAlpha2();
	BOOL bBlendBase = GetBlendBase();
	BOOL bBlendFractal = GetBlendFractal();
	int RR = GetRR();
	int RG = GetRG();
	int RB = GetRB();
	BOOL bTrapX = GetTrapX();
	double dTrapX = GetTrapXValue();
	BOOL bTrapY = GetTrapY();
	double dTrapY = GetTrapYValue();
	BOOL bRGC = GetGCR();
	BOOL bGGC = GetGCG();
	BOOL bBGC = GetGCB();
	double dRGC = bRGC ? GetGCRValue() : 1.0;
	double dGGC = bGGC ? GetGCGValue() : 1.0;
	double dBGC = bBGC ? GetGCBValue() : 1.0;
	
	// Apply gamma correction (cant' clobber the value)
	RGB1 = GRGB(RGB1,dRGC,dGGC,dBGC);
	RGB2 = GRGB(RGB2,dRGC,dGGC,dBGC);
	RGB3 = GRGB(RGB3,dRGC,dGGC,dBGC);
	RGB4 = GRGB(RGB4,dRGC,dGGC,dBGC);
	RGB5 = GRGB(RGB5,dRGC,dGGC,dBGC);
	RGB6 = GRGB(RGB6,dRGC,dGGC,dBGC);
	RGBBase = GRGB(RGBBase,dRGC,dGGC,dBGC);
	RGBStepColor = GRGB(RGBStepColor,dRGC,dGGC,dBGC);

	if (!m_DisplayDIB)
	{
		// Create the frame that is displayed
		m_DoubleBufferDIB.Init(MC,MR);

		// Create the image output of the document
		m_DisplayDIB.Init(MC,MR,true);
		m_DisplayDIB.Fill(RGB(bRed ? GetRValue(RGBStepColor) : 0,bGreen ? GetGValue(RGBStepColor) : 0,bBlue ? GetBValue(RGBStepColor) : 0));

		// Test for initializing the image
		if (csImage.GetLength())
		{
			CImage Image;
			if (SUCCEEDED(Image.Load(csImage)))
			{
				m_BaseDIB.Init(MC,MR);
				Image.StretchBlt(m_BaseDIB,0,0,MC,MR,SRCCOPY);
			}
		}

		// Test for initializing the image
		if (csImage2.GetLength())
		{
			CImage Image;
			if (SUCCEEDED(Image.Load(csImage2)))
			{
				m_FractalDIB.Init(MC,MR);
				Image.StretchBlt(m_FractalDIB,0,0,MC,MR,SRCCOPY);
			}
		}

		// Handle arrays
		HANDLE arrReadHandle[MAXTHREADS];

		// Get the number of threads to use for the work
		int nTotalThreads = m_CPU.GetNbThreads();

		// Dont ever create more threads than handles
		if (nTotalThreads > MAXTHREADS)
			nTotalThreads = MAXTHREADS;
		
		// Construct the signaling object and thread driver arrays
		CDriveMultiThreadedFractal ** ppDriveMultiThreadedFractal = new CDriveMultiThreadedFractal * [nTotalThreads];

		// Set the region to view
		double deltaP = (XMax - XMin) / (double)MC;
		double deltaQ = (YMax - YMin) / (double)MR;

		// Precalculate the colors
		int iColor = 0;

		// If the document was opened, or being zoom flown into, then use the saved color table
		if (!m_bOpen)
		{
			// Add in the scaling
			for (iColor = 1;iColor <= iIterations;++iColor)
			{
				if (bRed)
					m_RGB[0][iColor] += (BYTE)RR;
				else
					m_RGB[0][iColor] = 0;
				if (bGreen)
					m_RGB[1][iColor] += (BYTE)RG;
				else
					m_RGB[1][iColor] = 0;
				if (bBlue)
					m_RGB[2][iColor] += (BYTE)RB;
				else
					m_RGB[2][iColor] = 0;
			}

			// Set the base (in the set) color to be the last color unless specially colored
			m_RGB[0][0] = m_RGB[0][iIterations];
			m_RGB[1][0] = m_RGB[1][iIterations];
			m_RGB[2][0] = m_RGB[2][iIterations];

			// Gamma correction (base color already corrected)
			for (iColor = 0;iColor <= iIterations;++iColor)
			{
				COLORREF GammaRGB = GRGB(m_RGB[0][iColor],m_RGB[1][iColor],m_RGB[2][iColor],dRGC,dGGC,dBGC);
				m_RGB[0][iColor] = GetRValue(GammaRGB);
				m_RGB[1][iColor] = GetGValue(GammaRGB);
				m_RGB[2][iColor] = GetBValue(GammaRGB);
			}

			if (bBase)
			{
				m_RGB[0][0] = GetRValue(RGBBase);
				m_RGB[1][0] = GetGValue(RGBBase);
				m_RGB[2][0] = GetBValue(RGBBase);
			}
		}

		// Count the orbits per color
		std::vector<CIteration> Iterations;

		// Check for Buddha
		std::vector<LONG> vecEnergy;
		if (iFractalType == 7)
			vecEnergy.resize(MC * MR,0);

		// Build up the fractal parameters used during rendering
		CFractalParm FractalParm(iMode,iIterations,nMaxCalc,MC,MR,deltaP,deltaQ,XMin,YMin,XMax,YMax,dP,dQ,dXScale,dYScale,dBailRadius,dStep,m_RGB,bGradient,iFractalType,bBaseColor,bOrbitColor,&vecEnergy,bTrapX,dTrapX,bTrapY,dTrapY);

		// Set the colors to the main image for palette animation
		m_DisplayDIB.SetRGB(m_RGB);

		CFractalBase * pMultiThreadedFractal = NULL;
		for (int iHandle = 0;iHandle < nTotalThreads;++iHandle)
		{
			// Create the fractal object
			pMultiThreadedFractal = ChooseFractal(FractalParm);

			// Construct the Fractal thread driver
			CDriveMultiThreadedFractal * & pDriveMultiThreadedFractal = ppDriveMultiThreadedFractal[iHandle];
			pDriveMultiThreadedFractal = new CDriveMultiThreadedFractal(&arrReadHandle[iHandle],pMultiThreadedFractal);
		}

		// Process the number of segments that make up the Fractal
		int MRPT = MR / nTotalThreads + 1;

		// Process the number of threads of work
		for (int iThread = 0;iThread < nTotalThreads;iThread++)
		{
			// Set the range of cases to process
			int nBegRow = iThread * MRPT + 1;
			int nEndRow = min(nBegRow + MRPT - 1,MR);

			// Perform the work for this range
			CDriveMultiThreadedFractal * pDriveMultiThreadedFractal = ppDriveMultiThreadedFractal[iThread];
			pDriveMultiThreadedFractal->PostThreadMessage(WM_DOWORK,nBegRow,nEndRow);
		}

		// Wait for all the work to complete for this set of ranges, the driver signals these events
		WaitForMultipleObjects(nTotalThreads,&arrReadHandle[0],TRUE,INFINITE);

		// Apply the color now for these types whose MT step accumulated the color data
		if (iFractalType == 7)
		{
			int iX = 1,iY = 1,nEnergy = (int)vecEnergy.size();
			for (int iEnergy = 0;iEnergy < nEnergy;++iEnergy)
			{
				pMultiThreadedFractal->Apply((ULONG)vecEnergy[iEnergy],iX,iY);
				if (++iX > MC)
				{
					iX = 1;
					iY++;
				}
			}
		}

		// Clean up the threads of work
		for (int iThread = 0;iThread < nTotalThreads;iThread++)
		{
			// Get the thread
			CDriveMultiThreadedFractal * pDriveMultiThreadedFractal = ppDriveMultiThreadedFractal[iThread];

			// Get the iteration count for the thread
			CFractalCanvas * pFractalCanvas = (CFractalCanvas *)pDriveMultiThreadedFractal->GetFractal();
			if (pFractalCanvas)
			{
				// Join the results
				std::vector<CIteration> DestIter,ThreadIter = pFractalCanvas->GetIterations();
				if (Iterations.size())
				{
					for (int iIter = 0;iIter < (int)Iterations.size();++iIter)
						Iterations[iIter] = Iterations[iIter] + ThreadIter[iIter];
				}
				else
					Iterations = ThreadIter;
			}

			// End the thread
			pDriveMultiThreadedFractal->PostThreadMessage(WM_ENDTHREAD,0,0);
		}

		// Wait for all the drivers to process the end thread message
		WaitForMultipleObjects(nTotalThreads,&arrReadHandle[0],TRUE,INFINITE);

		// Close the event handle
		for (int iThread = 0;iThread < nTotalThreads;iThread++)
			CloseHandle(arrReadHandle[iThread]);

		// Delete the pointer array holding the thread drivers
		delete [] ppDriveMultiThreadedFractal;

		// Check for orbit sorting
		if (bSmooth || bDropUnused)
		{
			// Apply the sorting and drop unused effects
			SortDrop(Iterations);
		}

		// Check for image blending
		BOOL bBlendBaseDIB = bUseImage & bBlendBase;
		BOOL bBlendFracDIB = bUseImage2 & bBlendFractal;
		if (bBlendBaseDIB || bBlendFracDIB)
		{
			// Multi-threaded blend with the images
			CChangeColor ChangeColor(m_DisplayDIB,m_BaseDIB,m_FractalDIB,ID_CONTEXT_BLEND_COLOR,0,bBlendBaseDIB,bBlendFracDIB,iAlpha,iAlpha2);
			ChangeColor.ChangeColor();
		}
	}

	// Create the title for the document in the window title bar
	SetFractalTitle();
}

// Render the fractal image from zoom.  The documents open flag must be set due to MT access of RGB data
void CFractalDoc::RenderFractal(double dLastXMin,double dLastXMax,double dLastYMin,double dLastYMax,int iNewWidth,std::vector<std::vector<BYTE> > RGB)
{
	// Get the parameters that are needed for generations of the fractal
	CDuration Duration(_T("Render Fractal"));
	int iMode = GetMode();
	int iIterations = GetIterations();
	int nMaxCalc = GetMaxCalc();
	BOOL bDropUnused = GetDropUnused();
	BOOL bSmooth = GetSmooth();
	BOOL bRed = GetRed();
	BOOL bGreen = GetGreen();
	BOOL bBlue = GetBlue();
	BOOL bGradient = GetGradient();
	BOOL bBase = GetBase();
	BOOL bSortOrder = GetSortOrder();
	BOOL bBaseColor = GetBaseColor();
	BOOL bOrbitColor = GetOrbitColor();
	COLORREF RGB1 = GetRGB1();
	COLORREF RGB2 = GetRGB2();
	COLORREF RGB3 = GetRGB3();
	COLORREF RGB4 = GetRGB4();
	COLORREF RGB5 = GetRGB5();
	COLORREF RGB6 = GetRGB6();
	COLORREF RGBBase = GetRGBBase();
	COLORREF RGBStepColor = GetRGBStepColor();
	int MC = iNewWidth;
	int MR = GetHeight();
	double XMin = dLastXMin;
	double XMax = dLastXMax;
	double YMin = dLastYMin;
	double YMax = dLastYMax;
	double dP = GetP();
	double dQ = GetQ();
	double dXScale = GetXScale();
	double dYScale = GetYScale();
	double dBailRadius = GetBailRadius();
	double dStep = GetStep();
	CComBSTR ccbImage;
	ccbImage.Attach(GetImage());
	CString csImage(ccbImage);
	BOOL bUseImage = GetUseImage();
	CComBSTR ccbImage2;
	ccbImage2.Attach(GetImage2());
	CString csImage2(ccbImage2);
	BOOL bUseImage2 = GetUseImage2();
	int iFractalType = GetFractalType();
	int iAlpha = GetAlpha();
	int iAlpha2 = GetAlpha2();
	BOOL bBlendBase = GetBlendBase();
	BOOL bBlendFractal = GetBlendFractal();
	int RR = GetRR();
	int RG = GetRG();
	int RB = GetRB();
	BOOL bTrapX = GetTrapX();
	double dTrapX = GetTrapXValue();
	BOOL bTrapY = GetTrapY();
	double dTrapY = GetTrapYValue();
	BOOL bRGC = GetGCR();
	BOOL bGGC = GetGCG();
	BOOL bBGC = GetGCB();
	double dRGC = bRGC ? GetGCRValue() : 1.0;
	double dGGC = bGGC ? GetGCGValue() : 1.0;
	double dBGC = bBGC ? GetGCBValue() : 1.0;
	
	// Apply gamma correction (cant' clobber the value)
	RGB1 = GRGB(RGB1,dRGC,dGGC,dBGC);
	RGB2 = GRGB(RGB2,dRGC,dGGC,dBGC);
	RGB3 = GRGB(RGB3,dRGC,dGGC,dBGC);
	RGB4 = GRGB(RGB4,dRGC,dGGC,dBGC);
	RGB5 = GRGB(RGB5,dRGC,dGGC,dBGC);
	RGB6 = GRGB(RGB6,dRGC,dGGC,dBGC);
	RGBBase = GRGB(RGBBase,dRGC,dGGC,dBGC);
	RGBStepColor = GRGB(RGBStepColor,dRGC,dGGC,dBGC);

	if (!m_DisplayDIB)
	{
		// Create the frame that is displayed
		m_DoubleBufferDIB.Init(MC,MR);

		// Create the image output of the document
		m_DisplayDIB.Init(MC,MR,true);
		m_DisplayDIB.Fill(RGB(bRed ? GetRValue(RGBStepColor) : 0,bGreen ? GetGValue(RGBStepColor) : 0,bBlue ? GetBValue(RGBStepColor) : 0));

		// Test for initializing the image
		if (csImage.GetLength())
		{
			CImage Image;
			if (SUCCEEDED(Image.Load(csImage)))
			{
				m_BaseDIB.Init(MC,MR);
				Image.StretchBlt(m_BaseDIB,0,0,MC,MR,SRCCOPY);
			}
		}

		// Test for initializing the image
		if (csImage2.GetLength())
		{
			CImage Image;
			if (SUCCEEDED(Image.Load(csImage2)))
			{
				m_FractalDIB.Init(MC,MR);
				Image.StretchBlt(m_FractalDIB,0,0,MC,MR,SRCCOPY);
			}
		}

		// Handle arrays
		HANDLE arrReadHandle[MAXTHREADS];

		// Get the number of threads to use for the work
		int nTotalThreads = m_CPU.GetNbThreads();

		// Dont ever create more threads than handles
		if (nTotalThreads > MAXTHREADS)
			nTotalThreads = MAXTHREADS;
		
		// Construct the signaling object and thread driver arrays
		CDriveMultiThreadedFractal ** ppDriveMultiThreadedFractal = new CDriveMultiThreadedFractal * [nTotalThreads];

		// Set the region to view
		double deltaP = (XMax - XMin) / (double)MC;
		double deltaQ = (YMax - YMin) / (double)MR;

		// Precalculate the colors
		int iColor = 0;

		// If the document was opened, or being zoom flown into, then use the saved color table
		if (!m_bOpen)
		{
			// Add in the scaling
			for (iColor = 1;iColor <= iIterations;++iColor)
			{
				if (bRed)
					RGB[0][iColor] += (BYTE)RR;
				else
					RGB[0][iColor] = 0;
				if (bGreen)
					RGB[1][iColor] += (BYTE)RG;
				else
					RGB[1][iColor] = 0;
				if (bBlue)
					RGB[2][iColor] += (BYTE)RB;
				else
					RGB[2][iColor] = 0;
			}

			// Set the base (in the set) color to be the last color unless specially colored
			RGB[0][0] = RGB[0][iIterations];
			RGB[1][0] = RGB[1][iIterations];
			RGB[2][0] = RGB[2][iIterations];

			// Gamma correction (base color already corrected)
			for (iColor = 0;iColor <= iIterations;++iColor)
			{
				COLORREF GammaRGB = GRGB(RGB[0][iColor],RGB[1][iColor],RGB[2][iColor],dRGC,dGGC,dBGC);
				RGB[0][iColor] = GetRValue(GammaRGB);
				RGB[1][iColor] = GetGValue(GammaRGB);
				RGB[2][iColor] = GetBValue(GammaRGB);
			}

			if (bBase)
			{
				RGB[0][0] = GetRValue(RGBBase);
				RGB[1][0] = GetGValue(RGBBase);
				RGB[2][0] = GetBValue(RGBBase);
			}
		}

		// Count the orbits per color
		std::vector<CIteration> Iterations;

		// Check for Buddha
		std::vector<LONG> vecEnergy;
		if (iFractalType == 7)
			vecEnergy.resize(MC * MR,0);

		// Build up the fractal parameters used during rendering
		CFractalParm FractalParm(iMode,iIterations,nMaxCalc,MC,MR,deltaP,deltaQ,XMin,YMin,XMax,YMax,dP,dQ,dXScale,dYScale,dBailRadius,dStep,RGB,bGradient,iFractalType,bBaseColor,bOrbitColor,&vecEnergy,bTrapX,dTrapX,bTrapY,dTrapY);

		// Set the colors to the main image for palette animation
		m_DisplayDIB.SetRGB(RGB);

		CFractalBase * pMultiThreadedFractal = NULL;
		for (int iHandle = 0;iHandle < nTotalThreads;++iHandle)
		{
			// Create the fractal object
			pMultiThreadedFractal = ChooseFractal(FractalParm);

			// Construct the Fractal thread driver
			CDriveMultiThreadedFractal * & pDriveMultiThreadedFractal = ppDriveMultiThreadedFractal[iHandle];
			pDriveMultiThreadedFractal = new CDriveMultiThreadedFractal(&arrReadHandle[iHandle],pMultiThreadedFractal);
		}

		// Process the number of segments that make up the Fractal
		int MRPT = MR / nTotalThreads + 1;

		// Process the number of threads of work
		for (int iThread = 0;iThread < nTotalThreads;iThread++)
		{
			// Set the range of cases to process
			int nBegRow = iThread * MRPT + 1;
			int nEndRow = min(nBegRow + MRPT - 1,MR);

			// Perform the work for this range
			CDriveMultiThreadedFractal * pDriveMultiThreadedFractal = ppDriveMultiThreadedFractal[iThread];
			pDriveMultiThreadedFractal->PostThreadMessage(WM_DOWORK,nBegRow,nEndRow);
		}

		// Wait for all the work to complete for this set of ranges, the driver signals these events
		WaitForMultipleObjects(nTotalThreads,&arrReadHandle[0],TRUE,INFINITE);

		// Apply the color now for these types whose MT step accumulated the color data
		if (iFractalType == 7)
		{
			int iX = 1,iY = 1,nEnergy = (int)vecEnergy.size();
			for (int iEnergy = 0;iEnergy < nEnergy;++iEnergy)
			{
				pMultiThreadedFractal->Apply((ULONG)vecEnergy[iEnergy],iX,iY);
				if (++iX > MC)
				{
					iX = 1;
					iY++;
				}
			}
		}

		// Clean up the threads of work
		for (int iThread = 0;iThread < nTotalThreads;iThread++)
		{
			// Get the thread
			CDriveMultiThreadedFractal * pDriveMultiThreadedFractal = ppDriveMultiThreadedFractal[iThread];

			// Get the iteration count for the thread
			CFractalCanvas * pFractalCanvas = (CFractalCanvas *)pDriveMultiThreadedFractal->GetFractal();
			if (pFractalCanvas)
			{
				// Join the results
				std::vector<CIteration> DestIter,ThreadIter = pFractalCanvas->GetIterations();
				if (Iterations.size())
				{
					for (int iIter = 0;iIter < (int)Iterations.size();++iIter)
						Iterations[iIter] = Iterations[iIter] + ThreadIter[iIter];
				}
				else
					Iterations = ThreadIter;
			}

			// End the thread
			pDriveMultiThreadedFractal->PostThreadMessage(WM_ENDTHREAD,0,0);
		}

		// Wait for all the drivers to process the end thread message
		WaitForMultipleObjects(nTotalThreads,&arrReadHandle[0],TRUE,INFINITE);

		// Close the event handle
		for (int iThread = 0;iThread < nTotalThreads;iThread++)
			CloseHandle(arrReadHandle[iThread]);

		// Delete the pointer array holding the thread drivers
		delete [] ppDriveMultiThreadedFractal;

		// Check for orbit sorting
		if (bSmooth || bDropUnused)
		{
			// Apply the sorting and drop unused effects
			SortDrop(Iterations);
		}

		// Check for image blending
		BOOL bBlendBaseDIB = bUseImage & bBlendBase;
		BOOL bBlendFracDIB = bUseImage2 & bBlendFractal;
		if (bBlendBaseDIB || bBlendFracDIB)
		{
			// Multi-threaded blend with the images
			CChangeColor ChangeColor(m_DisplayDIB,m_BaseDIB,m_FractalDIB,ID_CONTEXT_BLEND_COLOR,0,bBlendBaseDIB,bBlendFracDIB,iAlpha,iAlpha2);
			ChangeColor.ChangeColor();
		}
	}

	// Create the title for the document in the window title bar
	SetFractalTitle();
}

// Get the rendered fractal as a picture dispatch object
IDispatch * CFractalDoc::GetPicture()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// OLE Stream file necessary for OleLoadPicture support
	IPictureDisp * pFractalImage = NULL;
	COleStreamFile ImageStream;

	// Prepare the Image for memory stream serialization
	if (ImageStream.CreateMemoryStream())
	{
		// GDI+ Image support
		CImage ImageDIB;

		// Attach the DIB to the Image
		ImageDIB.Attach(GetDoubleBuffer());

		// Serialize to the memory stream the DIB to the BMP format
		IStream * pImageStream = ImageStream.GetStream();
		if (pImageStream && SUCCEEDED(ImageDIB.Save(pImageStream,Gdiplus::ImageFormatBMP)))
		{
			// Rewind the stream for picture loading
			LARGE_INTEGER li;
			li.QuadPart  = 0;
			pImageStream->Seek(li,STREAM_SEEK_SET,NULL);                                 

			// Load the fractal into the return picture object
			CComPtr<IPictureDisp> Fractal;
			HRESULT hr = OleLoadPicture(pImageStream,li.LowPart,FALSE,IID_IPictureDisp,(void **)&Fractal);
			if (SUCCEEDED(hr))
				pFractalImage = Fractal.Detach();

			// Release the stream
			pImageStream->Release();
		}

		// Detach since we don't want to destroy the image
		ImageDIB.Detach();
	}
	return pFractalImage;
}

void CFractalDoc::SetPicture(IDispatch *)
{
}

// Create the fractal object
CFractalBase * CFractalDoc::ChooseFractal(CFractalParm & FractalParm)
{
	int iFractalType = GetFractalType();
	BOOL bBaseColor = GetBaseColor();
	BOOL bOrbitColor = GetOrbitColor();
	double dStep = GetStep();

	// Initialize the Fractal object
	CFractalBase * pMultiThreadedFractal = NULL;

	if (iFractalType == 7)
	{
		if (bOrbitColor == 0)
			pMultiThreadedFractal = new CRenderBuddhaBrotBaseFractal(FractalParm,m_DisplayDIB);
		else
			pMultiThreadedFractal = new CRenderBuddhaBrotOrbitFractal(FractalParm,m_DisplayDIB);
	}
	else if (dStep == 0.0)
	{
		if (bBaseColor == 0 && bOrbitColor == 0)
		{
			if (iFractalType == 0)
				pMultiThreadedFractal = new CRenderMandelbrotFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 1)
				pMultiThreadedFractal = new CRenderJuliaFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 2)
				pMultiThreadedFractal = new CRenderMandelbrotDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 3)
				pMultiThreadedFractal = new CRenderMandelbrotPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 4)
				pMultiThreadedFractal = new CRenderJuliaDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 5)
				pMultiThreadedFractal = new CRenderJuliaPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 6)
				pMultiThreadedFractal = new CRenderMandelbrotPhoenix2Fractal(FractalParm,m_DisplayDIB);
		}
		else // Specialization which uses more calculation so we split it from the main case for performance reasons
		{
			if (iFractalType == 0)
				pMultiThreadedFractal = new CRenderADMandelbrotFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 1)
				pMultiThreadedFractal = new CRenderADJuliaFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 2)
				pMultiThreadedFractal = new CRenderADMandelbrotDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 3)
				pMultiThreadedFractal = new CRenderADMandelbrotPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 4)
				pMultiThreadedFractal = new CRenderADJuliaDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 5)
				pMultiThreadedFractal = new CRenderADJuliaPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 6)
				pMultiThreadedFractal = new CRenderADMandelbrotPhoenix2Fractal(FractalParm,m_DisplayDIB);
		}
	}
	else // Stair Stepping
	{
		if (!bBaseColor && !bOrbitColor)
		{
			if (iFractalType == 0)
				pMultiThreadedFractal = new CRenderStepMandelbrotFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 1)
				pMultiThreadedFractal = new CRenderStepJuliaFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 2)
				pMultiThreadedFractal = new CRenderStepMandelbrotDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 3)
				pMultiThreadedFractal = new CRenderStepMandelbrotPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 4)
				pMultiThreadedFractal = new CRenderStepJuliaDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 5)
				pMultiThreadedFractal = new CRenderStepJuliaPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 6)
				pMultiThreadedFractal = new CRenderStepMandelbrotPhoenix2Fractal(FractalParm,m_DisplayDIB);
		}
		else // Specialization which uses more calculation so we split it from the main case for performance reasons
		{
			if (iFractalType == 0)
				pMultiThreadedFractal = new CRenderStepADMandelbrotFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 1)
				pMultiThreadedFractal = new CRenderStepADJuliaFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 2)
				pMultiThreadedFractal = new CRenderStepADMandelbrotDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 3)
				pMultiThreadedFractal = new CRenderStepADMandelbrotPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 4)
				pMultiThreadedFractal = new CRenderStepADJuliaDragonFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 5)
				pMultiThreadedFractal = new CRenderStepADJuliaPhoenixFractal(FractalParm,m_DisplayDIB);
			else if (iFractalType == 6)
				pMultiThreadedFractal = new CRenderStepADMandelbrotPhoenix2Fractal(FractalParm,m_DisplayDIB);
		}
	}
	return pMultiThreadedFractal;
}

bool CompareGT(const CIteration& LHS, const CIteration& RHS)
{
	return LHS > RHS;
}

bool CompareLT(const CIteration& LHS, const CIteration& RHS)
{
	return LHS < RHS;
}

// Apply the orbit sorting and drop unused colors effects based on the occurrence and counts of the orbits
void CFractalDoc::SortDrop(std::vector<CIteration> Iterations)
{
	BOOL bDropUnused = GetDropUnused();
	BOOL bSmooth = GetSmooth();
	BOOL bRed = GetRed();
	BOOL bGreen = GetGreen();
	BOOL bBlue = GetBlue();
	BOOL bGradient = GetGradient();
	BOOL bBase = GetBase();
	BOOL bSortOrder = GetSortOrder();
	COLORREF RGB1 = GetRGB1();
	COLORREF RGB2 = GetRGB2();
	COLORREF RGB3 = GetRGB3();
	COLORREF RGB4 = GetRGB4();
	COLORREF RGB5 = GetRGB5();
	COLORREF RGB6 = GetRGB6();
	COLORREF RGBBase = GetRGBBase();
	int MC = GetWidth();
	int MR = GetHeight();

	// Map the iteration count and its color to its index to preserve the index when sorting
	int nCounted = 0,nTotal = 0;
	std::vector<CIteration>::iterator it,itOld;
	for (it = Iterations.begin();it != Iterations.end();++it)
	{
		int iIndex = (int)std::distance(Iterations.begin(),it);
		(*it).SetIteration(iIndex);
		if (iIndex)
		{
			if ((int)*it)
				nCounted++;
			nTotal++;
		}
	}

	// Get the current iterations and color table
	std::vector<CIteration> OldIterations = Iterations;
	std::vector<std::vector<BYTE> > vRGB = m_RGB;

	// Sort the iterations
	if (bSmooth)
	{
		if (bSortOrder)
			std::sort(Iterations.begin() + 1, Iterations.end(), CompareGT);
		else
			std::sort(Iterations.begin() + 1, Iterations.end(), CompareLT);
	}

	// Remap of colors based on sorted index
	for (it = Iterations.begin(),itOld = OldIterations.begin();it != Iterations.end();++it,++itOld)
	{
		int iOldColor = (*itOld).GetIteration();
		int iNewColor = (*it).GetIteration();
		m_RGB[0][iNewColor] = vRGB[0][iOldColor];
		m_RGB[1][iNewColor] = vRGB[1][iOldColor];
		m_RGB[2][iNewColor] = vRGB[2][iOldColor];
	}

	// Recolor the DIB
	m_DisplayDIB.SetRGB(m_RGB);

	// Multi-threaded recolor of all colors
	CChangeColor ChangeColor(m_DisplayDIB,m_BaseDIB,m_FractalDIB,ID_CONTEXT_ALL_COLOR);
	ChangeColor.ChangeColor();

	// Get fractal dialog for this type
	int iFractalType = GetFractalType();
	CNewFractal & Fractal = theApp.GetFractalDialog(iFractalType);

	CString csMsg;
	CString csDialogTitle = theApp.GetFractalDialogTitle(iFractalType);
	if (bSmooth && bDropUnused)
		csMsg.Format(L"%d of %d colors were used.\n\nUpdate %s dialog with sorted colors and count?",nCounted,nTotal,csDialogTitle);
	else if (bSmooth && !bDropUnused)
		csMsg.Format(L"Update %s dialog with sorted colors?",csDialogTitle);
	else if (!bSmooth && bDropUnused)
		csMsg.Format(L"%d of %d colors were used.\n\nUpdate %s dialog with the count?",nCounted,nTotal,csDialogTitle);
	else if (!bSmooth && !bDropUnused)
		throw "You shouldn't be here";
	if (AfxMessageBox(csMsg,MB_YESNO) == IDYES)
	{
		// Update the settings
		if (bSmooth)
		{
			Fractal.UpdateColor(m_RGB);
			Fractal.SetSmooth(FALSE);
		}
		if (bDropUnused)
		{
			Fractal.SetDropUnused(FALSE);
			Fractal.SetIterations(nCounted);
		}
	}
}

// Set the title to the window
void CFractalDoc::SetFractalTitle()
{
	double XMin = GetXMin();
	double XMax = GetXMax();
	double YMin = GetYMin();
	double YMax = GetYMax();
	double dP = GetP();
	double dQ = GetQ();

	// Set the document title
	CString csTitle,csPathName = GetPathName();
	if (csPathName.IsEmpty())
	{
		CString csType,csXMin,csXMax,csYMin,csYMax,csP,csQ;
		csXMin.Format(_T("%.18lf"),XMin);csXMin.TrimRight(L'0');csXMin.TrimRight(L'.');
		csYMin.Format(_T("%.18lf"),YMin);csYMin.TrimRight(L'0');csYMin.TrimRight(L'.');
		csXMax.Format(_T("%.18lf"),XMax);csXMax.TrimRight(L'0');csXMax.TrimRight(L'.');
		csYMax.Format(_T("%.18lf"),YMax);csYMax.TrimRight(L'0');csYMax.TrimRight(L'.');
		csP.Format(_T("%.18lf"),dP);csP.TrimRight(L'0');csP.TrimRight(L'.');
		csQ.Format(_T("%.18lf"),dQ);csQ.TrimRight(L'0');csQ.TrimRight(L'.');
		if (m_iFractalType == 0)
			csType.Format(_T("%s"),_T("Mandelbrot"));
		else if (m_iFractalType == 1)
			csType.Format(_T("%s (%s,%s)"),_T("Julia"),csP,csQ);
		else if (m_iFractalType == 2)
			csType.Format(_T("%s"),_T("Mandelbrot Dragon"));
		else if (m_iFractalType == 3)
			csType.Format(_T("%s"),_T("Mandelbrot Phoenix"));
		else if (m_iFractalType == 4)
			csType.Format(_T("%s (%s,%s)"),_T("Julia Dragon"),csP,csQ);
		else if (m_iFractalType == 5)
			csType.Format(_T("%s (%s,%s)"),_T("Julia Phoenix"),csP,csQ);
		else if (m_iFractalType == 6)
			csType.Format(_T("%s"),_T("Mandelbrot Phoenix Version 2"));
		else if (m_iFractalType == 7)
			csType.Format(_T("%s"),_T("Buddhabrot"));
		if (IsModified())
			csTitle.Format(_T("%s : (%s,%s) - (%s,%s) *"),csType,csXMin,csYMin,csXMax,csYMax);
		else
			csTitle.Format(_T("%s : (%s,%s) - (%s,%s)"),csType,csXMin,csYMin,csXMax,csYMax);
	}
	else
	{
		CString csFileName,csExt;
		TCHAR * lpFileName = csFileName.GetBufferSetLength(_MAX_FNAME);
		TCHAR * lpExt = csExt.GetBufferSetLength(_MAX_EXT);
		_tsplitpath(csPathName,NULL,NULL,lpFileName,lpExt);
		csFileName.ReleaseBuffer();
		csExt.ReleaseBuffer();

		if (IsModified())
			csTitle.Format(_T("%s%s *"),csFileName,csExt);
		else
			csTitle.Format(_T("%s%s"),csFileName,csExt);
	}
	SetTitle(csTitle);
}

// Add/Replace/Delete decorative text to/from the document to be rendered during the drawing
// iReplaceText = 0 : Add Text
// iReplaceText = 1 : Replace Text
// iReplaceText = 2 : Delete Text
void CFractalDoc::AddTextToDoc(CDecorativeText & DecorativeText,int iReplaceText)
{
	if (iReplaceText == 0)
		m_vDecorativeText.push_back(DecorativeText);
	else
	{
		int iText = 0;
		std::vector<CDecorativeText>::iterator itText;
		for (itText = m_vDecorativeText.begin();itText != m_vDecorativeText.end();++itText,++iText)
		{
			if (DecorativeText == *itText)
			{
				if (iReplaceText == 1)
					m_vDecorativeText[iText] = DecorativeText;
				else
					m_vDecorativeText.erase(itText);
				break;
			}
		}
	}
	SetModifiedFlag();
	SetFractalTitle();
}

// Add/Replace/Delete decorative picture to/from the document to be rendered during the drawing
// iReplacePicture = 0 : Add Picture
// iReplacePicture = 1 : Replace Picture
// iReplacePicture = 2 : Delete Picture
void CFractalDoc::AddPictureToDoc(CDecorativePicture & DecorativePicture,int iReplacePicture)
{
	if (iReplacePicture == 0)
		m_vDecorativePicture.push_back(DecorativePicture);
	else
	{
		int iPicture = 0;
		std::vector<CDecorativePicture>::iterator itPicture;
		for (itPicture = m_vDecorativePicture.begin();itPicture != m_vDecorativePicture.end();++itPicture,++iPicture)
		{
			if (DecorativePicture == *itPicture)
			{
				if (iReplacePicture == 1)
					m_vDecorativePicture[iPicture] = DecorativePicture;
				else
					m_vDecorativePicture.erase(itPicture);
				break;
			}
		}
	}
	SetModifiedFlag();
	SetFractalTitle();
}
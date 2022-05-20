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
#include "afxwinappex.h"
#include "Fractal.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "FractalDoc.h"
#include "FractalView.h"

#include "FractalDocManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFractalApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CFractalApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CFractalApp construction

CFractalApp::CFractalApp()
{
	// Output the licensing disclaimer
	DebugMsg(_T("Fractal.exe Copyright (C) 2012-Present Andrew S. Bantly\n"));
	DebugMsg(_T("Andrew Bantly can be reached at jones.gravy@gmail.com\n"));
	DebugMsg(_T("Fractal comes with ABSOLUTELY NO WARRANTY\n"));
	DebugMsg(_T("This is free software, and you are welcome to redistribute it\n"));
	DebugMsg(_T("under of the GNU General Public License as published by\n"));
	DebugMsg(_T("the Free Software Foundation; version 2 of the License.\n"));
	DebugMsg(_T("\n"));
	DebugMsg(_T("\"We can forgive a man for making a useful thing as long as he does not admire it. The only excuse for making a useless thing is that one admires it intensely. All art is quite useless\" Oscar Wilde, The Picture of Dorian Gray\n"));
	srand((unsigned)time(NULL));
	m_bZoomNew = FALSE;
	m_iFractalType = 0;
	m_iLastChoice = 0;
	m_bAutomation = FALSE;
}

// The one and only CFractalApp object
CFractalApp theApp;

// UUID of the CoClass
// {5446301F-C657-4852-98EB-18FFBB64E136}
static const GUID clsid = 
{ 0x5446301f, 0xc657, 0x4852, { 0x98, 0xeb, 0x18, 0xff, 0xbb, 0x64, 0xe1, 0x36 } };

// UUID of the Library
// {19688AA8-277F-4932-8F1A-E9C491AC13A4}
static const GUID _tlid = 
{ 0x19688aa8, 0x277f, 0x4932, { 0x8f, 0x1a, 0xe9, 0xc4, 0x91, 0xac, 0x13, 0xa4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

// CFractalApp initialization
BOOL CFractalApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("AndrewBantly"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	m_pDocManager = new CFractalDocManager();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_FractalTYPE,
		RUNTIME_CLASS(CFractalDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFractalView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications
	COleTemplateServer::RegisterAll();
	// Note: MDI applications register all server objects without regard
	//  to the /Embedding or /Automation on the command line

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Don't show the main window
		m_bAutomation = TRUE;
		return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.  Unregister
	// typelibrary.  Other unregistration occurs in ProcessShellCommand().
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Don't show the main window
		return TRUE;
	}

	// Prevent an initial document from being created
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// Configure the Mandelbrot settings
	m_Mandelbrot.SetFractalType(0);
	m_Mandelbrot.SetWidth(1536);
	m_Mandelbrot.SetHeight(1024);
	m_Mandelbrot.SetXMin(-2.0);
	m_Mandelbrot.SetXMax(1.0);
	m_Mandelbrot.SetYMin(-1.0);
	m_Mandelbrot.SetYMax(1.0);
	m_Mandelbrot.SetDDVXMin(-2.2);
	m_Mandelbrot.SetDDVXMax(1.6);
	m_Mandelbrot.SetDDVYMin(-1.6);
	m_Mandelbrot.SetDDVYMax(1.6);

	// Configure Julia settings
	m_Julia.SetFractalType(1);
	m_Julia.SetWidth(1536);
	m_Julia.SetHeight(1024);
	m_Julia.SetXMin(-1.8);
	m_Julia.SetXMax(1.8);
	m_Julia.SetYMin(-1.2);
	m_Julia.SetYMax(1.2);
	m_Julia.SetDDVXMin(-1.8);
	m_Julia.SetDDVXMax(1.8);
	m_Julia.SetDDVYMin(-1.2);
	m_Julia.SetDDVYMax(1.2);

	// Configure Mandelbrot Dragon settings
	m_MandelbrotDragon.SetFractalType(2);
	m_MandelbrotDragon.SetWidth(2048);
	m_MandelbrotDragon.SetHeight(1024);
	m_MandelbrotDragon.SetXMin(-2.2);
	m_MandelbrotDragon.SetXMax(4.2);
	m_MandelbrotDragon.SetYMin(-1.6);
	m_MandelbrotDragon.SetYMax(1.6);
	m_MandelbrotDragon.SetDDVXMin(-2.2);
	m_MandelbrotDragon.SetDDVXMax(4.2);
	m_MandelbrotDragon.SetDDVYMin(-1.6);
	m_MandelbrotDragon.SetDDVYMax(1.6);

	// Configure Mandelbrot Phoenix settings
	m_MandelbrotPhoenix.SetFractalType(3);
	m_MandelbrotPhoenix.SetHeight(969);
	m_MandelbrotPhoenix.SetWidth(972);
	m_MandelbrotPhoenix.SetXMin(-2.2);
	m_MandelbrotPhoenix.SetXMax(1.6);
	m_MandelbrotPhoenix.SetYMin(-2.0);
	m_MandelbrotPhoenix.SetYMax(2.0);
	m_MandelbrotPhoenix.SetDDVXMin(-2.2);
	m_MandelbrotPhoenix.SetDDVXMax(1.6);
	m_MandelbrotPhoenix.SetDDVYMin(-2.0);
	m_MandelbrotPhoenix.SetDDVYMax(2.0);

	// Configure Julia Dragon settings
	m_JuliaDragon.SetFractalType(4);
	m_JuliaDragon.SetWidth(2048);
	m_JuliaDragon.SetHeight(1024);
	m_JuliaDragon.SetXMin(-2.2);
	m_JuliaDragon.SetXMax(4.2);
	m_JuliaDragon.SetYMin(-1.6);
	m_JuliaDragon.SetYMax(1.6);
	m_JuliaDragon.SetMaxCalc(64);
	m_JuliaDragon.SetP(1.4);
	m_JuliaDragon.SetQ(-0.8);
	m_JuliaDragon.SetDDVXMin(-2.2);
	m_JuliaDragon.SetDDVXMax(4.2);
	m_JuliaDragon.SetDDVYMin(-1.6);
	m_JuliaDragon.SetDDVYMax(1.6);

	// Configure Julia Phoenix settings
	m_JuliaPhoenix.SetFractalType(5);
	m_JuliaPhoenix.SetWidth(1536);
	m_JuliaPhoenix.SetHeight(853);
	m_JuliaPhoenix.SetXMin(-1.8);
	m_JuliaPhoenix.SetXMax(1.8);
	m_JuliaPhoenix.SetYMin(-1.2);
	m_JuliaPhoenix.SetYMax(1.2);
	m_JuliaPhoenix.SetMaxCalc(64);
	m_JuliaPhoenix.SetP(0.56667);
	m_JuliaPhoenix.SetQ(-0.5);
	m_JuliaPhoenix.SetDDVXMin(-1.8);
	m_JuliaPhoenix.SetDDVXMax(1.8);
	m_JuliaPhoenix.SetDDVYMin(-1.2);
	m_JuliaPhoenix.SetDDVYMax(1.2);

	// Configure Mandelbrot Phoenix V2 (Andy's mods) settings
	m_MandelbrotPhoenix2.SetFractalType(6);
	m_MandelbrotPhoenix2.SetHeight(969);
	m_MandelbrotPhoenix2.SetWidth(972);
	m_MandelbrotPhoenix2.SetXMin(-2.2);
	m_MandelbrotPhoenix2.SetXMax(1.6);
	m_MandelbrotPhoenix2.SetYMin(-2.0);
	m_MandelbrotPhoenix2.SetYMax(2.0);
	m_MandelbrotPhoenix2.SetDDVXMin(-2.2);
	m_MandelbrotPhoenix2.SetDDVXMax(1.6);
	m_MandelbrotPhoenix2.SetDDVYMin(-2.0);
	m_MandelbrotPhoenix2.SetDDVYMax(2.0);

	// Configure the Buddhabrot settings
	m_Buddhabrot.SetWidth(2048 * 4);
	m_Buddhabrot.SetHeight(1024 * 4);
	m_Buddhabrot.SetXMin(-2.2);
	m_Buddhabrot.SetXMax(4.2);
	m_Buddhabrot.SetYMin(-1.6);
	m_Buddhabrot.SetYMax(1.6);
	m_Buddhabrot.SetP(1.4);
	m_Buddhabrot.SetQ(-0.8);
	m_Buddhabrot.SetDDVXMin(-2.2);
	m_Buddhabrot.SetDDVXMax(4.2);
	m_Buddhabrot.SetDDVYMin(-1.6);
	m_Buddhabrot.SetDDVYMax(1.6);

	return TRUE;
}

// Helper functions for zooming into new document creation
void CFractalApp::SetZoomNew(int iFractalType)
{
	m_bZoomNew = TRUE;
	m_iFractalType = iFractalType;
}

BOOL CFractalApp::GetZoomNew(int & iFractalType)
{
	BOOL bZoomNew = m_bZoomNew;
	m_bZoomNew = FALSE;
	if (bZoomNew)
		iFractalType = m_iFractalType;
	else
		iFractalType = m_iLastChoice;
	return bZoomNew;
}

// Return the dialog used to configure the type of fractal
CNewFractal & CFractalApp::GetFractalDialog(int iFractalType)
{
	switch (iFractalType)
	{
	case 1:
		return m_Julia;
	case 2:
		return m_MandelbrotDragon;
	case 3:
		return m_MandelbrotPhoenix;
	case 4:
		return m_JuliaDragon;
	case 5:
		return m_JuliaPhoenix;
	case 6:
		return m_MandelbrotPhoenix2;
	case 7:
		return m_Buddhabrot;
	case 0:
	default:
		return m_Mandelbrot;
	}
}

CString CFractalApp::GetFractalDialogTitle(int iFractalType)
{
	switch (iFractalType)
	{
	case 1:
		return L"Julia";
	case 2:
		return L"Mandelbrot Dragon";
	case 3:
		return L"Mandelbrot Phoenix";
	case 4:
		return L"Julia Dragon";
	case 5:
		return L"Julia Phoenix";
	case 6:
		return L"Mandelbrot Phoenix2";
	case 7:
		return L"Buddhabrot";
	case 0:
	default:
		return L"Mandelbrot";
	}
}

// Return the dialog used to configure the type of fractal
BOOL CFractalApp::IsJuliaFractal(int iFractalType)
{
	switch (iFractalType)
	{
	case 1:
	case 4:
	case 5:
		return TRUE;
	default:
		return FALSE;
	}
}

// Return the julia fractal that matches the mandelbrot
int CFractalApp::GetMandelJulia(int iFractalType)
{
	switch (iFractalType)
	{
	case 2:
		// Mandelbrot Dragon to Julia Dragon
		return 4; 
	case 3:
		// Mandelbrot Phoenix to Julia Phoenix
		return 5;
	default:
		// Mandelbrot to Julia
		return 1;
	}
}

BOOL CFractalApp::IsAutomation()
{
	return m_bAutomation;
}

// Choose an image for blending
BOOL CFractalApp::ChooseImage(LPCTSTR lpszTitle,CString & csNewImage)
{
	// Get the image file filter
	BOOL bRet = FALSE;
	CString csFilter = _T("BMP (*.BMP;*.DIB;*.RLE)|*.BMP;*.DIB;*.RLE|JPEG (*.JPG;*.JPEG;*.JPE;*.JFIF)|*.JPG;*.JPEG;*.JPE;*.JFIF|GIF (*.GIF)|*.GIF|TIFF (*.TIF;*.TIFF)|*.TIF;*.TIFF|PNG (*.PNG)|*.PNG||");
	CString csFileFilter = _T("Common (*.bmp;*.jpg;*.gif;*.png)|*.bmp;*.jpg;*.gif;*.png|bmp (*.bmp;*.dib;*.rle)|*.bmp;*.dib;*.rle|jpeg (*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg;*.jpeg;*.jpe;*.jfif|gif (*.gif)|*.gif|tiff (*.tif;*.tiff)|*.tif;*.tiff|png (*.png)|*.png|emf (*.emf)|*.emf|wmf (*.wmf)|*.wmf||");
	CSimpleArray<GUID> aguidFileTypes;
	CImage Image;
	HRESULT hResult = Image.GetExporterFilterString(csFilter,aguidFileTypes);
	if (FAILED(hResult))
		return bRet;

	// Call the custom load dialog
	CImageFileDialog dlgFile(TRUE,lpszTitle,NULL,NULL,OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING,csFileFilter);
	INT_PTR nRet = dlgFile.DoModal();
	if (nRet == IDCANCEL)
		return bRet;

	// Get the files returned
	POSITION myPos = dlgFile.GetStartPosition();
	if (!myPos)
		return bRet;

	// Get the image name
	csNewImage = dlgFile.GetNextPathName(myPos);
	bRet = TRUE;

	return bRet;
}

INT_PTR CFractalApp::ChangeColor(COLORREF & crRGB,CImageButton & BtnRGB)
{
	CColorDialog ClrDlg;
	ClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	ClrDlg.m_cc.rgbResult = crRGB;
	if (ClrDlg.DoModal() == IDOK)
	{
		crRGB = ClrDlg.GetColor();
		BtnRGB.SetFillColor(crRGB);
		return IDOK;
	}
	return IDCANCEL;
}

void CFractalApp::CreatePointFont(CFont & Font, CDC * pDC, LONG lfWeight, BYTE lfItalic, BYTE lfCharSet, BYTE lfQuality, CString csFontName, int iPtSize)
{
	// Create the font for the text
	Font.DeleteObject();
	int iLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);
	LONG lfHeight = MulDiv(iPtSize, iLogPixelsY, 72);
	LOGFONT LogFont;
	memset(&LogFont, 0, sizeof(LOGFONT));
	LogFont.lfWeight = lfWeight;
	LogFont.lfItalic = lfItalic;
	LogFont.lfHeight = lfHeight;
	LogFont.lfCharSet = lfCharSet;
	LogFont.lfQuality = lfQuality;
	_tcsncpy(LogFont.lfFaceName, csFontName.GetBuffer(), LF_FACESIZE);
	const LOGFONT * lpLogFont = &LogFont;
	Font.CreatePointFontIndirect(lpLogFont);
}

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CFractalApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CFractalApp message handlers

BOOL CFractalApp::ExitInstance(void)
{
	return CWinAppEx::ExitInstance();
}

IMPLEMENT_DYNAMIC(CFractalFileDialog, CFileDialog)

CFractalFileDialog::CFractalFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszTitle, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, BOOL bSaveAs, CWnd* pParentWnd) :
	CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, sizeof(OPENFILENAME), FALSE), m_bSaveAs(bSaveAs)
{
	// Set the structure size
	m_ofn.lStructSize = sizeof(OPENFILENAME);

	// Set the dialog customization template
	if (!bSaveAs)
		m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_EMBED);

	// Set the title
	m_ofn.lpstrTitle = lpszTitle;

	// Embedded flag
	m_bEmbedded = TRUE;
}

void CFractalFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	if (!m_bSaveAs)
		DDX_Check(pDX, IDC_EMBEDDED, m_bEmbedded);
}

BEGIN_MESSAGE_MAP(CFractalFileDialog, CFileDialog)
	ON_BN_CLICKED(IDC_EMBEDDED, &CFractalFileDialog::OnEmbedded)
END_MESSAGE_MAP()

void CFractalFileDialog::OnEmbedded()
{
	UpdateData();
}

// Image File Dialog
IMPLEMENT_DYNAMIC(CImageFileDialog, CFileDialog)

CImageFileDialog::CImageFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszTitle, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
								   DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	// Initialize the file structure
	m_ofn.lStructSize = sizeof(OPENFILENAME);

	// Set the title
	m_csTitle = lpszTitle;
	m_ofn.lpstrTitle = m_csTitle;
}

void CImageFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageFileDialog, CFileDialog)
END_MESSAGE_MAP()
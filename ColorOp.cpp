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
#include "ColorOp.h"
#include "Number.h"

// Function operator
void CColorOp::operator() (int iRowBeg,int iRowEnd)
{
	CDIBFrame & DisplayDIB = m_DisplayDIB;
	int nWidth = DisplayDIB.GetWidth();

	switch (m_nIDEvent)
	{
	case ID_COLORBLEED_UP:
	{
		for (int iHeight = iRowBeg; iHeight < iRowEnd; ++iHeight)
		{
			for (int iWidth = 0; iWidth < nWidth; ++iWidth)
			{
				BYTE R, G, B;
				DisplayDIB.GetPixel(iWidth, iHeight, R, G, B);
				R++;G++;B++;
				DisplayDIB.SetPixel(iWidth, iHeight, R, G, B);
			}
		}
		break;
	}
	case ID_COLORBLEED_DOWN:
	{
		for (int iHeight = iRowBeg;iHeight < iRowEnd;++iHeight)
		{
			for (int iWidth = 0;iWidth < nWidth;++iWidth)
			{
				BYTE R,G,B;
				DisplayDIB.GetPixel(iWidth,iHeight,R,G,B);
				R--;G--;B--;
				DisplayDIB.SetPixel(iWidth,iHeight,R,G,B);
			}
		}
		break;
	}
	case ID_COLORBLEED_SUBTLEUP:
	{
		for (int iHeight = iRowBeg; iHeight < iRowEnd; ++iHeight)
		{
			for (int iWidth = 0; iWidth < nWidth; ++iWidth)
			{
				BYTE R, G, B;
				DisplayDIB.GetPixel(iWidth, iHeight, R, G, B);
				R++;
				if (R == 0)
				{
					G++;
					if (G == 0)
						B++;
				}
				DisplayDIB.SetPixel(iWidth, iHeight, R, G, B);
			}
		}
		break;
	}
	case ID_COLORBLEED_SUBTLEDOWN:
	{
		for (int iHeight = iRowBeg;iHeight < iRowEnd;++iHeight)
		{
			for (int iWidth = 0;iWidth < nWidth;++iWidth)
			{
				BYTE R,G,B;
				DisplayDIB.GetPixel(iWidth,iHeight,R,G,B);
				R--;
				if (R == 255)
				{
					G--;
					if (G == 255)
						B--;
				}
				DisplayDIB.SetPixel(iWidth,iHeight,R,G,B);
			}
		}
		break;
	}
	case ID_COLORBLEED_PALETTEUP:
	{
		std::vector<std::vector<BYTE> > & RGB = DisplayDIB.GetRGB();
		int nRGB = (int)RGB[0].size();
		for (int iHeight = iRowBeg; iHeight < iRowEnd; ++iHeight)
		{
			for (int iWidth = 0; iWidth < nWidth; ++iWidth)
			{
				int iColor;
				DisplayDIB.GetPixelPaletteIndex(iWidth, iHeight, iColor);
				if (iColor != 0)
				{
					iColor++;
					if (iColor >= nRGB)
						iColor = 1;
				}
				DisplayDIB.SetPixelAndPaletteIndex(iWidth, iHeight, RGB[0][iColor], RGB[1][iColor], RGB[2][iColor], iColor);
			}
		}
		break;
	}
	case ID_COLORBLEED_PALETTEDOWN:
	{
		std::vector<std::vector<BYTE> > & RGB = DisplayDIB.GetRGB();
		int nRGB = (int)RGB[0].size();
		for (int iHeight = iRowBeg;iHeight < iRowEnd;++iHeight)
		{
			for (int iWidth = 0;iWidth < nWidth;++iWidth)
			{
				int iColor;
				DisplayDIB.GetPixelPaletteIndex(iWidth,iHeight,iColor);
				if (iColor != 0)
				{
					iColor--;
					if (iColor < 1)
						iColor = nRGB - 1;
				}
				DisplayDIB.SetPixelAndPaletteIndex(iWidth, iHeight, RGB[0][iColor], RGB[1][iColor], RGB[2][iColor], iColor);
			}
		}
		break;
	}
	case ID_CONTEXT_EDIT_COLOR:
	{
		// Recolor the DIB with the new color
		std::vector<std::vector<BYTE> > & RGB = DisplayDIB.GetRGB();
		BYTE R = RGB[0][m_iColor];
		BYTE G = RGB[1][m_iColor];
		BYTE B = RGB[2][m_iColor];
		for (int iHeight = iRowBeg;iHeight < iRowEnd;++iHeight)
		{
			for (int iWidth = 0;iWidth < nWidth;++iWidth)
			{
				int iColor;
				DisplayDIB.GetPixelPaletteIndex(iWidth,iHeight,iColor);
				if (m_iColor == iColor)
					DisplayDIB.SetPixel(iWidth,iHeight,R,G,B);
			}
		}
		break;
	}
	case ID_CONTEXT_ALL_COLOR:
	{
		// Recolor the DIB
		std::vector<std::vector<BYTE> > & RGB = DisplayDIB.GetRGB();
		for (int iHeight = iRowBeg;iHeight < iRowEnd;++iHeight)
		{
			for (int iWidth = 0;iWidth < nWidth;++iWidth)
			{
				int iColor;
				DisplayDIB.GetPixelPaletteIndex(iWidth,iHeight,iColor);
				BYTE R = RGB[0][iColor];
				BYTE G = RGB[1][iColor];
				BYTE B = RGB[2][iColor];
				DisplayDIB.SetPixel(iWidth,iHeight,R,G,B);
			}
		}
		break;
	}
	case ID_CONTEXT_BLEND_COLOR:
	{
		CDIBFrame & BaseDIB = m_BaseDIB;
		CDIBFrame & FractalDIB = m_FractalDIB;
		if (m_bBlendBaseDIB || m_bBlendFracDIB)
		{
			// Set the base and fractal opacity values
			BYTE Alpha_Base = (BYTE)(m_iAlpha * 2.55);
			BYTE Alpha_Base2 = 255 - Alpha_Base;
			BYTE Alpha_Frac = (BYTE)(m_iAlpha2 * 2.55);
			BYTE Alpha_Frac2 = 255 - Alpha_Frac;
			BYTE BR,BG,BB;
			BYTE BRI,BGI,BBI;
			int iColor = 0;

			for (int iHeight = iRowBeg;iHeight < iRowEnd;++iHeight)
			{
				for (int iWidth = 0;iWidth < nWidth;++iWidth)
				{
					// Get the palette index of the coordinate (Color == 0 is base)
					DisplayDIB.GetPixelAndPaletteIndex(iWidth,iHeight,BR,BG,BB,iColor);

					if (iColor == 0)
					{
						if (m_bBlendBaseDIB)
						{
							// Get the image pixel for blending
							BaseDIB.GetPixel(iWidth,iHeight,BRI,BGI,BBI);

							// Update by blending the sources to the destination
							BB = (BB * Alpha_Base2 + BBI * Alpha_Base) >> 8;
							BG = (BG * Alpha_Base2 + BGI * Alpha_Base) >> 8;
							BR = (BR * Alpha_Base2 + BRI * Alpha_Base) >> 8;

							// Set the blended pixel
							DisplayDIB.SetPixel(iWidth,iHeight,BR,BG,BB);
						}
					}
					else
					{
						if (m_bBlendFracDIB)
						{
							// Get the image pixel for blending
							FractalDIB.GetPixel(iWidth,iHeight,BRI,BGI,BBI);

							// Update by blending the sources to the destination
							BB = (BB * Alpha_Frac2 + BBI * Alpha_Frac) >> 8;
							BG = (BG * Alpha_Frac2 + BGI * Alpha_Frac) >> 8;
							BR = (BR * Alpha_Frac2 + BRI * Alpha_Frac) >> 8;

							// Set the blended pixel
							DisplayDIB.SetPixel(iWidth,iHeight,BR,BG,BB);
						}
					}
				}
			}
		}
		break;
	}
	default:
		break;
	}
}

IMPLEMENT_DYNCREATE(CDriveMultiThreadedColor, CWinThread)

CDriveMultiThreadedColor::CDriveMultiThreadedColor(HANDLE * phHandle,CColorOp * pColorOp) : m_phHandle(phHandle), m_pColorOp(pColorOp), m_bPumpMessage(false)
{
	// Create the event signaling event
	m_hPump = CreateEvent(NULL,FALSE,FALSE,NULL);

	// Create the signaling event
	HANDLE & hHandle = *m_phHandle;
	hHandle = CreateEvent(NULL,FALSE,FALSE,NULL);

	// Create the thread
	CreateThread();
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);

	// Wait for the thread to initialize and signal its message pump has started
	WaitForSingleObject(m_hPump,INFINITE);

	// Close the initialization event
	CloseHandle(m_hPump);
	m_hPump = NULL;
}

CDriveMultiThreadedColor::~CDriveMultiThreadedColor()
{
	// Signal the end of the thread
	HANDLE & hHandle = *m_phHandle;
	SetEvent(hHandle);
}

BOOL CDriveMultiThreadedColor::InitInstance()
{
	// Initialize the large number library
	NumberTranscriber& NumberX = NumberTranscriber::getInstance();

	return TRUE;
}

// Use the message pump as a marker to indicate the thread can accept posted messages
BOOL CDriveMultiThreadedColor::PumpMessage()
{
	// Test for initialization
	if (!m_bPumpMessage)
	{
		// The message pump is active
		m_bPumpMessage = true;

		// Signal completion of this stage
		SetEvent(m_hPump);
	}

	// Allow the base class to handle the message pump
	return CWinThread::PumpMessage();
}

int CDriveMultiThreadedColor::ExitInstance()
{
	return CWinThread::ExitInstance();
}

// These functions are called by PostThreadMessage
BEGIN_MESSAGE_MAP(CDriveMultiThreadedColor, CWinThread)
	ON_THREAD_MESSAGE(WM_DOWORK,&CDriveMultiThreadedColor::OnDoWork)
	ON_THREAD_MESSAGE(WM_ENDTHREAD,&CDriveMultiThreadedColor::OnEndThread)
END_MESSAGE_MAP()

void CDriveMultiThreadedColor::OnDoWork(WPARAM wParam,LPARAM lParam)
{
	// Do the work
	int RowBeg = (int)wParam;
	int RowEnd = (int)lParam;
	try
	{
		// Carry out the work for this range
		m_pColorOp->operator()(RowBeg,RowEnd);
	}
	catch (...)
	{
	}

	// Signal completion
	HANDLE & hHandle = *m_phHandle;
	SetEvent(hHandle);
}

void CDriveMultiThreadedColor::OnEndThread(WPARAM wParam,LPARAM lParam)
{
	// Delete the work object
	if (m_pColorOp)
		delete m_pColorOp;

	// End the thread
	PostQuitMessage(0);
}

// Wrapper for thread creation and work
CChangeColor::CChangeColor(CDIBFrame & DisplayDIB,CDIBFrame & BaseDIB,CDIBFrame & FractalDIB,UINT_PTR nIDEvent,int iColor,BOOL bBlendBaseDIB,BOOL bBlendFracDIB,int iAlpha,int iAlpha2) : m_DisplayDIB(DisplayDIB), m_BaseDIB(BaseDIB), m_FractalDIB(FractalDIB), m_nIDEvent(nIDEvent), m_bBlendBaseDIB(bBlendBaseDIB),m_bBlendFracDIB(bBlendFracDIB),m_iAlpha(iAlpha),m_iAlpha2(iAlpha2)
{
	// Get the number of threads to use for the work
	m_nTotalThreads = m_CPU.GetNbThreads();

	// Dont ever create more threads than handles
	if (m_nTotalThreads > MAXTHREADS)
		m_nTotalThreads = MAXTHREADS;

	// Construct the signaling object and thread driver arrays
	m_ppDriveMultiThreadedColor = new CDriveMultiThreadedColor * [m_nTotalThreads];

	for (int iThread = 0;iThread < m_nTotalThreads;++iThread)
	{
		// Construct the color changing object
		CColorOp * pColorOp = new CColorOp(DisplayDIB,BaseDIB,FractalDIB,nIDEvent,iColor,bBlendBaseDIB,bBlendFracDIB,iAlpha,iAlpha2);

		// Construct the thread driver
		CDriveMultiThreadedColor * & pDriveMultiThreadedColor = m_ppDriveMultiThreadedColor[iThread];
		pDriveMultiThreadedColor = new CDriveMultiThreadedColor(&m_arrHandle[iThread],pColorOp);
	}
}

CChangeColor::~CChangeColor()
{
	// Clean up the threads of work
	for (int iThread = 0;iThread < m_nTotalThreads;iThread++)
	{
		// Get the thread
		CDriveMultiThreadedColor * pDriveMultiThreadedColor = m_ppDriveMultiThreadedColor[iThread];

		// End the thread
		pDriveMultiThreadedColor->PostThreadMessage(WM_ENDTHREAD,0,0);
	}

	// Wait for all the drivers to process the end thread message
	WaitForMultipleObjects(m_nTotalThreads,&m_arrHandle[0],TRUE,INFINITE);

	// Close the event handle
	for (int iThread = 0;iThread < m_nTotalThreads;iThread++)
		CloseHandle(m_arrHandle[iThread]);

	// Delete the pointer array holding the thread drivers
	delete [] m_ppDriveMultiThreadedColor;
}

// Cause the color to change based on the event
void CChangeColor::ChangeColor()
{
	// Get the number of rows
	int MR = m_DisplayDIB.GetHeight();

	// Process the number of segments that make up the Fractal
	int MRPT = MR / m_nTotalThreads + 1;

	// Process the number of threads of work
	for (int iThread = 0;iThread < m_nTotalThreads;iThread++)
	{
		// Set the range of cases to process
		int nBegRow = iThread * MRPT;
		int nEndRow = min(nBegRow + MRPT,MR);

		// Perform the work for this range
		CDriveMultiThreadedColor * pDriveMultiThreadedColor = m_ppDriveMultiThreadedColor[iThread];
		pDriveMultiThreadedColor->PostThreadMessage(WM_DOWORK,nBegRow,nEndRow);
	}

	// Wait for all the work to complete for this set of ranges
	WaitForMultipleObjects(m_nTotalThreads,&m_arrHandle[0],TRUE,INFINITE);
}
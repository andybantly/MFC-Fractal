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
#include "FractalDoc.h"
#include "FractalBase.h"
#include "intrin.h"
#include <algorithm>
#include <map>
#include "Number.h"

CFractalBase::CFractalBase(CFractalParm FractalParm) : m_FractalParm(FractalParm)
{
}

/////////////////////////////////////////////////////////////////////////////

CFractalCanvas::CFractalCanvas(CFractalParm FractalParm,CDIBFrame & DisplayDIB) : 
	CFractalBase(FractalParm), m_DisplayDIB(DisplayDIB)
{
	// Get the colors
	m_RGB = FractalParm.GetRGB();

	// Get the color count and maximum number of escape orbit calculations
	m_nMaxIterations = m_FractalParm.GetIterations();
	m_nMaxCalc = m_FractalParm.GetMaxCalc();

	// Check for modulo division on color scaling (iterations <> color palette)
	m_bModulo = m_nMaxIterations != m_nMaxCalc;

	// Prepare the collection that counts the orbits
	m_Iterations.resize(m_nMaxIterations + 1);
}

// Return the count of orbits per color
std::vector<CIteration> CFractalCanvas::GetIterations()
{
	return m_Iterations;
}

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDriveMultiThreadedFractal, CWinThread)

CDriveMultiThreadedFractal::CDriveMultiThreadedFractal(HANDLE * phHandle,CFractalBase * pFractal) : m_phHandle(phHandle), m_pFractal(pFractal), m_bPumpMessage(false)
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

CDriveMultiThreadedFractal::~CDriveMultiThreadedFractal()
{
	// Signal the end of the thread
	HANDLE & hHandle = *m_phHandle;
	SetEvent(hHandle);
}

BOOL CDriveMultiThreadedFractal::InitInstance()
{
	// Initialize the large number library
	NumberTranscriber& NumberX = NumberTranscriber::getInstance();

	return TRUE;
}

// Use the message pump as a marker to indicate the thread can accept posted messages
BOOL CDriveMultiThreadedFractal::PumpMessage()
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

int CDriveMultiThreadedFractal::ExitInstance()
{
	return CWinThread::ExitInstance();
}

// These functions are called by PostThreadMessage
BEGIN_MESSAGE_MAP(CDriveMultiThreadedFractal, CWinThread)
	ON_THREAD_MESSAGE(WM_DOWORK,&CDriveMultiThreadedFractal::OnDoWork)
	ON_THREAD_MESSAGE(WM_ENDTHREAD,&CDriveMultiThreadedFractal::OnEndThread)
END_MESSAGE_MAP()

void CDriveMultiThreadedFractal::OnDoWork(WPARAM wParam,LPARAM lParam)
{
	// Do the work
	int RowBeg = (int)wParam;
	int RowEnd = (int)lParam;
	try
	{
		// Carry out the work for this range
		m_pFractal->operator()(RowBeg,RowEnd);
	}
	catch (...)
	{
	}

	// Signal completion
	HANDLE & hHandle = *m_phHandle;
	SetEvent(hHandle);
}

void CDriveMultiThreadedFractal::OnEndThread(WPARAM wParam,LPARAM lParam)
{
	// Delete the work object
	if (m_pFractal)
		delete m_pFractal;

	// End the thread
	PostQuitMessage(0);
}
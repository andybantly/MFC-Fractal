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
   - Under no circumstances shall[Your Name or Organization] be liable for any damages, including incidental, special, or consequential damages arising out of or in connection with the use of this software.

4. Violation of Terms :
   - Any unauthorized use, modification, or distribution of this software will result in immediate revocation of any implied permissions and may lead to legal action.

5. Jurisdiction :
   - This license shall be governed by the laws of The United States of America.
*/

#include "stdafx.h"
#include "AVI.h"

void DebugAviMsg(HRESULT hr);

CAVI::CAVI() : 
	m_bInit(FALSE), m_bOpen(FALSE), m_bStream(FALSE),
	m_pAVIFile(NULL), m_dwFPS(30), m_dwFrame(0),
	m_pAVIStream(NULL), m_pAVICompressedStream(NULL)
{
	// Initialize AVI
	Init();
}

CAVI::~CAVI()
{
	// Close any open files
	Close();

	// Cleanup AVI
	if (m_bInit)
		Term();
}

// Init AVI
void CAVI::Init()
{
	WORD wVer = HIWORD(VideoForWindowsVersion());
	if (wVer >= 0x010a)
	{
		AVIFileInit();
		m_bInit = TRUE;
	}
}

// Cleanup AVI
void CAVI::Term()
{
	// Exit AVI
	if (m_bInit)
	{
		AVIFileExit();
		m_bInit = FALSE;
	}
}

// Open the filename of the video
BOOL CAVI::Open(DWORD dwFPS)
{
	// Test for successful initialization (should never fail)
	if (!m_bInit)
		return FALSE;

	// If already opened then fail
	if (m_bOpen)
		return FALSE;

	// Set the FPS
	m_dwFPS = dwFPS;

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
	CFileStatus FileStatus;
	if (CFile::GetStatus(csPathName,FileStatus))
		CFile::Remove(csPathName);
	csPathName.Replace(_T(".tmp"),_T(".avi"));

	// Set the file name
	m_csFileName = csPathName;

	// Delete an old copy of the file
	if (CFile::GetStatus(m_csFileName,FileStatus))
		CFile::Remove(m_csFileName);

	// Open the file for writing
	HRESULT hr = AVIFileOpen(&m_pAVIFile,m_csFileName,OF_WRITE|OF_CREATE,NULL);
	if (hr == AVIERR_OK)
		m_bOpen = TRUE;
	else
		DebugAviMsg(hr);

	// Return the status
	return m_bOpen;
}

// Close the AVI file
CString CAVI::Close()
{
	// Release the streams
	ReleaseStream();

	// Release the file
	if (m_bOpen)
	{
		AVIFileRelease(m_pAVIFile);
		m_pAVIFile = NULL;
		m_bOpen = FALSE;
	}

	// Return the file name
	return m_csFileName;
}

// Create the AVI file stream for the file
BOOL CAVI::CreateStream(CDIBFrame & DIBFrame)
{
	// Initialize the stream for the uncompressed DIB
	AVISTREAMINFO StreamInfo;
	ZeroMemory(&StreamInfo,sizeof(StreamInfo));
    StreamInfo.fccType = streamtypeVIDEO;
    StreamInfo.fccHandler = 0; 
    StreamInfo.dwScale = m_dwFPS;
    StreamInfo.dwRate = m_dwFPS * m_dwFPS;
    StreamInfo.dwSuggestedBufferSize = DIBFrame.GetSizeImage();
    SetRect(&StreamInfo.rcFrame,0,0,DIBFrame.GetWidth(),DIBFrame.GetHeight());

	// Create the stream
    HRESULT hr = AVIFileCreateStream(m_pAVIFile,&m_pAVIStream,&StreamInfo);
    if (hr == AVIERR_OK)
	{
		// Set the compression options
		AVICOMPRESSOPTIONS CompressOptions;
		ZeroMemory(&CompressOptions,sizeof(CompressOptions));
		CompressOptions.fccHandler = mmioFOURCC('D','I','B',' ');

		// Create the compression
		hr = AVIMakeCompressedStream(&m_pAVICompressedStream,m_pAVIStream,&CompressOptions,NULL);
		if (hr == AVIERR_OK)
		{
			BITMAPINFOHEADER * lpBMIH = (BITMAPINFOHEADER *)DIBFrame;
			hr = AVIStreamSetFormat(m_pAVICompressedStream,0,lpBMIH,lpBMIH->biSize + lpBMIH->biClrUsed * sizeof(RGBQUAD));
			if (hr == AVIERR_OK)
			{
				m_dwFrame = 0;
				m_bStream = TRUE;
			}
			else
				DebugAviMsg(hr);
		}
		else
			DebugAviMsg(hr);
	}
	else
		DebugAviMsg(hr);

	// Return the status
	return m_bStream;
}

// Release the DIB streams both uncompressed and compressed
void CAVI::ReleaseStream()
{
	// Release the stream
	if (m_pAVIStream)
	{
		AVIStreamRelease(m_pAVIStream);
		m_pAVIStream = NULL;
	}
	
	// Release the uncompressed stream
	if (m_pAVICompressedStream)
	{
		AVIStreamRelease(m_pAVICompressedStream);
		m_pAVICompressedStream = NULL;
	}

	// Reset the indicators
	m_bStream = FALSE;
	m_dwFrame = 0;
}

// Add a frame to the AVI
BOOL CAVI::AddFrame(CDIBFrame & DIBFrame)
{
	// Test for a file
	if (!m_bOpen)
		return FALSE;

	// Test for a stream
	if (!m_bStream)
	{
		// Create the stream for the first time
		if (!CreateStream(DIBFrame))
			return FALSE;
	}

	// Add the frame
	HRESULT hr = AVIStreamWrite(m_pAVICompressedStream,m_dwFrame,1,(LPSTR)DIBFrame,DIBFrame.GetSizeImage(),AVIIF_KEYFRAME,NULL,NULL);
	if (hr == AVIERR_OK)
	{
		m_dwFrame++;
		return TRUE;
	}
	else
		DebugAviMsg(hr);

	return FALSE;
}

void DebugAviMsg(HRESULT hr)
{
	switch (hr)
	{
	case S_OK: DebugMsg(_T("%s\n"),_T("Success")); break;
	case AVIERR_BADFORMAT: DebugMsg(_T("%s\n"),_T("AVIERR_BADFORMAT: corrupt file or unrecognized format")); break;
	case AVIERR_MEMORY: DebugMsg(_T("%s\n"),_T("AVIERR_MEMORY: insufficient memory")); break;
	case AVIERR_FILEREAD: DebugMsg(_T("%s\n"),_T("AVIERR_FILEREAD: disk error while reading file")); break;
	case AVIERR_FILEOPEN: DebugMsg(_T("%s\n"),_T("AVIERR_FILEOPEN: disk error while opening file")); break;
	case REGDB_E_CLASSNOTREG: DebugMsg(_T("%s\n"),_T("REGDB_E_CLASSNOTREG: file type not recognised")); break;
	case AVIERR_READONLY: DebugMsg(_T("%s\n"),_T("AVIERR_READONLY: file is read-only")); break;
	case AVIERR_NOCOMPRESSOR: DebugMsg(_T("%s\n"),_T("AVIERR_NOCOMPRESSOR: a suitable compressor could not be found")); break;
	case AVIERR_UNSUPPORTED: DebugMsg(_T("%s\n"),_T("AVIERR_UNSUPPORTED: compression is not supported for this type of data")); break;
	case AVIERR_INTERNAL: DebugMsg(_T("%s\n"),_T("AVIERR_INTERNAL: internal error")); break;
	case AVIERR_BADFLAGS: DebugMsg(_T("%s\n"),_T("AVIERR_BADFLAGS")); break;
	case AVIERR_BADPARAM: DebugMsg(_T("%s\n"),_T("AVIERR_BADPARAM")); break;
	case AVIERR_BADSIZE: DebugMsg(_T("%s\n"),_T("AVIERR_BADSIZE")); break;
	case AVIERR_BADHANDLE: DebugMsg(_T("%s\n"),_T("AVIERR_BADHANDLE")); break;
	case AVIERR_FILEWRITE: DebugMsg(_T("%s\n"),_T("AVIERR_FILEWRITE: disk error while writing file")); break;
	case AVIERR_COMPRESSOR: DebugMsg(_T("%s\n"),_T("AVIERR_COMPRESSOR")); break;
	case AVIERR_NODATA: DebugMsg(_T("%s\n"),_T("AVIERR_READONLY")); break;
	case AVIERR_BUFFERTOOSMALL: DebugMsg(_T("%s\n"),_T("AVIERR_BUFFERTOOSMALL")); break;
	case AVIERR_CANTCOMPRESS: DebugMsg(_T("%s\n"),_T("AVIERR_CANTCOMPRESS")); break;
	case AVIERR_USERABORT: DebugMsg(_T("%s\n"),_T("AVIERR_USERABORT")); break;
	case AVIERR_ERROR: DebugMsg(_T("%s\n"),_T("AVIERR_ERROR")); break;
	default: DebugMsg(_T("%s\n"),_T("Unknown"));
	}
}
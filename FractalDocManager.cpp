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
#include "FractalDocManager.h"

//extern void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,CDocTemplate* pTemplate, CString* pstrDefaultExt);
void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ENSURE_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
		!strFilterExt.IsEmpty() &&
		pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
		!strFilterName.IsEmpty())
	{
		if (pstrDefaultExt != NULL)
			pstrDefaultExt->Empty();

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please

		int iStart = 0;
		do
		{
			CString strExtension = strFilterExt.Tokenize(_T(";"), iStart);

			if (iStart != -1)
			{
				// a file based document template - add to filter list

				// If you hit the following ASSERT, your document template 
				// string is formatted incorrectly.  The section of your 
				// document template string that specifies the allowable file
				// extensions should be formatted as follows:
				//    .<ext1>;.<ext2>;.<ext3>
				// Extensions may contain wildcards (e.g. '?', '*'), but must
				// begin with a '.' and be separated from one another by a ';'.
				// Example:
				//    .jpg;.jpeg
				ASSERT(strExtension[0] == '.');
				if ((pstrDefaultExt != NULL) && pstrDefaultExt->IsEmpty())
				{
					// set the default extension
					*pstrDefaultExt = strExtension.Mid(1);  // skip the '.'
					ofn.lpstrDefExt = const_cast<LPTSTR>((LPCTSTR)(*pstrDefaultExt));
					ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
				}

				filter += (TCHAR)'*';
				filter += strExtension;
				filter += (TCHAR)';';  // Always append a ';'.  The last ';' will get replaced with a '\0' later.
			}
		} while (iStart != -1);

		filter.SetAt(filter.GetLength() - 1, '\0');;  // Replace the last ';' with a '\0'
		ofn.nMaxCustFilter++;
	}
}

std::vector<CString> SplitStringToVector(LPCTSTR sz, LPCTSTR szSeparators)
{
	CString cs(sz);
	cs += szSeparators[0];
	std::vector<CString> vecOut;

	while (!cs.IsEmpty())
	{
		int nSepPos = cs.FindOneOf(szSeparators);
		vecOut.push_back(cs.Left(nSepPos));
		cs = cs.Mid(nSepPos + 1);
	}
	return vecOut;
}

CFractalDocManager::CFractalDocManager()
{
}

CFractalDocManager::~CFractalDocManager()
{
}

BOOL CFractalDocManager::DoPromptFileName(CString & csFileNames, UINT nIDSTitle, DWORD dwFlags, BOOL bOpenFileDialog, CDocTemplate * pTemplate)
{
	CFileDialog dlgFile(bOpenFileDialog, NULL, NULL, dwFlags, NULL, NULL, 0);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= dwFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// append all docs filter
		CString strFilters;
		POSITION pos = m_templateList.GetHeadPosition();
		while (pos != NULL)
		{
			CDocTemplate * pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			CString strFilterExt;
			pTemplate->GetDocString(strFilterExt,CDocTemplate::filterExt);
			if (!strFilterExt.IsEmpty())
				strFilters += _T('*') + strFilterExt + _T(';');
		}
		strFilters.Delete(strFilters.GetLength() - 1,1);

		CString allDocsFilter = _T("*.frc");
		strFilter += allDocsFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += strFilters;
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		// do for all doc template
		pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate * pTemplate = (CDocTemplate *)m_templateList.GetNext(pos);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = csFileNames.GetBufferSetLength((_MAX_PATH + 1) * 16384);
	dlgFile.m_ofn.nMaxFile = _MAX_PATH * 16384;
	dlgFile.m_ofn.nFilterIndex = 0;

	INT_PTR nResult = dlgFile.DoModal();
	csFileNames.ReleaseBuffer();

	return nResult == IDOK;
}

void CFractalDocManager::OnFileOpen()
{
	// Prompt
	CString csFileNames;
	if (!DoPromptFileName(csFileNames, AFX_IDS_OPENFILE, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, TRUE, NULL))
		return; // open cancelled

	LPWSTR wszName = csFileNames.GetBuffer();
	for (int i = 0;i < csFileNames.GetAllocLength();i++)
	{
		if ((_T('\0') == wszName[i]) && (_T('\0') != wszName[i+1]))
			wszName[i] = _T('*');
	}
	csFileNames.ReleaseBuffer();

	// Open all the documents
	std::vector<CString> vecFiles = SplitStringToVector(csFileNames,_T("*"));
	CString strPath = vecFiles[0];
	if (vecFiles.size() == 1)
		AfxGetApp()->OpenDocumentFile(strPath);
	else
	{
		for (std::vector<CString>::const_iterator itFile = vecFiles.begin() + 1;itFile != vecFiles.end();++itFile)
			AfxGetApp()->OpenDocumentFile(strPath + _T('\\') + *itFile);
	}
}

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
#include "Crypt.h"

using namespace std;

CCrypt::CCrypt(BYTE * pPassword,DWORD dwLen) : m_hCryptProv(NULL), m_hHash(NULL), m_hKey(NULL), m_hSessionKey(NULL)
{
	// Get handle to the default provider. 
	if (!CryptAcquireContext(&m_hCryptProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,0))
	{
		if (!CryptAcquireContext(&m_hCryptProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,CRYPT_NEWKEYSET))
		{
			if (!CryptAcquireContext(&m_hCryptProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,CRYPT_DELETEKEYSET))
				return;
			if (!CryptAcquireContext(&m_hCryptProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,CRYPT_NEWKEYSET))
				return;
		}
	}
	HashData(pPassword,dwLen);
}

CCrypt::~CCrypt()
{
	// Destroy the MD5 Hash
	if (m_hHash)
	{
		CryptDestroyHash(m_hHash); 
		m_hHash = NULL;
	}

	// Destroy session key. 
	if (m_hSessionKey)
	{
		CryptDestroyKey(m_hSessionKey); 
		m_hSessionKey = NULL;
	}

	// Destroy the cryptographic session
	if (m_hKey) 
	{
		CryptDestroyKey(m_hKey);
		m_hKey = NULL;
	}

	// Release the cryptographic provider
	if (m_hCryptProv)
	{
		CryptReleaseContext(m_hCryptProv,0);
		m_hCryptProv = NULL;
	}
}

// Encrypt the data
bool CCrypt::Encrypt(CStringA & csData,vector<BYTE> & vecData)
{
	bool bRet = false;
	DWORD dwOut = csData.GetLength();
	if (dwOut == 0)
		return true;
	vecData.resize(dwOut);
Encrypt:
	memcpy(&vecData[0],csData.GetBuffer(),dwOut);
	if (!CryptEncrypt(m_hSessionKey,0,TRUE,0,&vecData[0],&dwOut,dwOut))
	{
		if (GetLastError() == ERROR_MORE_DATA)
		{
			vecData.resize(dwOut);
			goto Encrypt;
		}
	}
	else
		bRet = true;
	return bRet;
}

// Decrypt the data
bool CCrypt::Decrypt(vector<BYTE> & vecData,CStringA & csData)
{
	bool bRet = false;
	vector<BYTE> vecDecrypt;
	DWORD dwLength = (DWORD)vecData.size();
	if (dwLength == 0)
		return true;
	vecDecrypt.resize(dwLength);
	memcpy(&vecDecrypt[0],&vecData[0],dwLength);
	if (CryptDecrypt(m_hSessionKey,0,TRUE,0,&vecDecrypt[0],&dwLength))
	{
		char * pBuffer = csData.GetBufferSetLength(dwLength);
		memcpy(pBuffer,&vecDecrypt[0],dwLength);
		bRet = true;
	}
	return bRet;
}

// Create the hash and then the session key for encrypting/decrypting
bool CCrypt::HashData(BYTE * pPassword,DWORD dwLen)
{
	if (!m_hHash)
	{
		// Test for a good initialization
		if (!m_hCryptProv)
			return false;

		// Create the MD5 cryptographic hash
		if (!CryptCreateHash(m_hCryptProv,CALG_MD5,0,0,&m_hHash))
			return false;
	}

	// Add the password to the MD5 hash if it is being used
	if (pPassword)
	{
		bool bHash = CryptHashData(m_hHash,pPassword,dwLen,0) ? true : false;
		if (!bHash)
			return false;
	}
	return CreateSessionKey();
}

// Create the actual session key using the algorithm
bool CCrypt::CreateSessionKey()
{
	// Derive a session key from the hash object. 
	if (!CryptDeriveKey(m_hCryptProv,CALG_RC4,m_hHash,0x00800000,&m_hSessionKey))
		return false;
	CryptDestroyHash(m_hHash); 
	m_hHash = NULL;
	return true;
}

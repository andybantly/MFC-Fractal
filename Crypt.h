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

#pragma once
#include <wincrypt.h>
#include <vector>

class CCrypt
{
public:
	CCrypt(BYTE * pPassword = NULL,DWORD dwLen = 0);
	~CCrypt();

	// Encrypt the data
	bool Encrypt(CStringA & csData,std::vector<BYTE> & vecData);

	// Decrypt the data
	bool Decrypt(std::vector<BYTE> & vecData,CStringA & csData);

private:

	// Create the hash and then the session key for encrypting/decrypting
	bool HashData(BYTE * pPassword,DWORD dwLen);

	// Create the actual session key using the algorithm
	bool CreateSessionKey();

private:
	bool m_bError;
	HCRYPTPROV m_hCryptProv;
	HCRYPTHASH m_hHash;
	HCRYPTKEY m_hSessionKey;
	HCRYPTKEY m_hKey;
};

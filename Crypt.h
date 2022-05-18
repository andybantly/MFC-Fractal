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

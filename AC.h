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

#pragma once

// CArithmeticEncoding (high level implementation)
#define BITMASK 0x80
#define MSB 0x8000
#define NSB 0x4000
#define USB 0x3FFF

class CArithmeticEncoder
{
public:
	CArithmeticEncoder();
	bool EncodeBuffer(char * pBufferIn,unsigned long ulnSrcCount,char * & pBufferOut,unsigned long & ulnDestCount);
	bool DecodeBuffer(char * pBufferIn,unsigned long ulnSrcCount,char ** ppBufferOut,unsigned long * pulnDestCount,BOOL bAlloc = TRUE);

	CArithmeticEncoder(char * pBufferIn,unsigned long ulnSrcCount);
	void SetBuffer(char * pBufferIn,unsigned long ulnSrcCount);
	void GetBuffer(char * & pBufferOut,unsigned long & ulnDestCount);
	bool EncodeBuffer();
	bool DecodeBuffer();

	~CArithmeticEncoder();

protected:
	void InitModel();
	void ScaleCounts();
	unsigned int RangeCounts();
	void BuildMap();
	void OutputBit(unsigned short int usiBit,unsigned char & ucOutSymbol,unsigned char & ucBitMask,unsigned long & ulDestCount,char * pBuffer);
	void OutputUnderflowBits(unsigned short int usiBit,unsigned long & ulUnderflowBits,unsigned char & ucOutSymbol,unsigned char & ucBitMask,unsigned long & ulDestCount,char * pBuffer);
	void FlushBitMask(unsigned char & ucBitMask,unsigned char & ucOutSymbol,unsigned long & ulDestCount,char * pBuffer);

protected:
	char * m_pBufferIn;
	unsigned long m_ulnSrcCount;
	char * m_pBufferOut;
	unsigned long m_ulnDestCount;
	unsigned long m_ulnLastBuffer;

private:
	unsigned long m_ac[256];
	unsigned long m_ac2[256];
	unsigned short int m_ar[257];
	unsigned int m_aMap[16384];
};
// Copyright (C) 2012-Present Andrew S. Bantly

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
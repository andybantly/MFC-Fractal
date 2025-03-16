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
#include <intrin.h>

// CPU id helper structure
struct CPU
{
	CPU() : m_r1(0), m_r2(0), m_nProc(0), m_r3(0), m_r4(0), m_r5(0) {Init();}
	CPU(int nProc) : m_r1(0), m_r2(0), m_nProc(nProc), m_r3(0), m_r4(0), m_r5(0) {Init();}
	~CPU() {};
	CPU(const CPU & rhs) {*this = rhs;}
	CPU & operator = (const CPU & rhs)
	{
		if (this != &rhs)
		{
			m_r1 = rhs.m_r1;
			m_r2 = rhs.m_r2;
			m_nProc = rhs.m_nProc;
			m_r3 = rhs.m_r3;
			m_r4 = rhs.m_r4;
			m_r5 = rhs.m_r5;
		}
		return *this;
	}
	void Init() {__cpuid((int*)this,1);}
	int GetNbProcs() {return m_nProc;}
	int GetNbThreads() {return min(GetNbProcs() * 2,MAXTHREADS);}

	unsigned m_r1 : 32;
	unsigned m_r2 : 16;
	unsigned m_nProc : 8; // Number of logical processors
	unsigned m_r3 : 32;
	unsigned m_r4 : 32;
	unsigned m_r5 : 4;
	unsigned m_HT : 1; // multi-threading / hyperthreading bit
	unsigned m_r6 : 3;
};


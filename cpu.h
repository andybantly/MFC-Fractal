// Copyright (C) 2012-2016 Andrew S. Bantly
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


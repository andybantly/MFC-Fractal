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
#include "Number.h"
#include "Constants.h"
#include <sstream>
#include <map>

#pragma warning(disable:6385)

std::unique_ptr<NumberTranscriber> NumberTranscriber::instance = nullptr;
std::mutex NumberTranscriber::mutex;
std::map<std::string, std::string, NumberTranscriber::cilt> NumberTranscriber::mapWordTo99;
std::map<std::string, std::string, NumberTranscriber::cilt> NumberTranscriber::mapWordTo100;

NumberTranscriber& NumberTranscriber::getInstance()
{
	if (instance == nullptr)
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (instance == nullptr)
			instance.reset(new NumberTranscriber());
	}
	return *instance;
}

void NumberTranscriber::init()
{
	for (int iOne = 0; iOne < g_nOnes; ++iOne)
		mapWordTo99[g_ones[iOne]] = g_nones[iOne];

	for (int iTen = 2; iTen < g_nTens; ++iTen)
		mapWordTo99[g_tens[iTen]] = g_ntens[iTen];

	for (int iTen = 2, iTens; iTen < g_nTens - 1; ++iTen)
	{
		iTens = iTen * 10;
		std::string strTen = g_tens[iTen] + "-";
		for (int iOne = 1; iOne < 10; ++iOne)
			mapWordTo99[std::string(strTen + g_ones[iOne])] = std::string(std::to_string(iTens + iOne));
	}

	for (int iHun = 0, nZero = 3; iHun < g_nHuns; iHun++, nZero += 3)
		mapWordTo100[g_huns[iHun]] = ("1" + std::string(nZero, g_cZero));
}

std::string NumberTranscriber::ToPhrase(const std::string& number)
{
	std::string strResult;
	if (number.empty())
		return strResult;

	int iResult = 0;
	bool bNegative = false;
	int digs, nd, nh;

	std::string strLhs;
	size_t stP1 = number.find(g_cDecSep);
	if (stP1 == std::string::npos)
		strLhs = number;
	else
		strLhs = number.substr(0, stP1);
	if (strLhs.empty())
		strLhs = g_cZero;

	strResult.clear();
	if (*(strLhs.begin()) == '-')
	{
		bNegative = true;
		strLhs = strLhs.substr(1);
	}

start:
	digs = (int)strLhs.length() - 1;
	nh = digs / 3;
	if (nh <= g_nHuns)
	{
		nd = digs % 3 + 1;
		std::string strLhs2 = strLhs.substr(0, nd);
		int iLhs2, digs2, div, l;
		try
		{
			iLhs2 = stoi(strLhs2);
		}
		catch (std::invalid_argument)
		{
			iResult = -1;
		}

		if (iResult == 0)
		{
			bool b10 = false;
		start2:
			digs2 = 0;
			if (iLhs2 > 0)
				digs2 = (int)(floor(log10((double)iLhs2)));
			div = 1;
			for (l = 0; l < digs2; l++)
				div *= 10;

			if (iLhs2 < 20)
			{
				if (!strResult.empty())
					strResult += (b10 ? "-" : " ");
				strResult += g_ones[iLhs2];
				b10 = false;
			}
			else if (iLhs2 < 100)
			{
				if (!strResult.empty())
					strResult += " ";
				strResult += g_tens[iLhs2 / div];
				b10 = true;
				if ((iLhs2 % div) != 0)
				{
					iLhs2 = iLhs2 % div;
					goto start2;
				}
			}
			else if (iLhs2 < 1000)
			{
				if (!strResult.empty())
					strResult += " ";
				strResult += g_ones[iLhs2 / div] + " " + g_tens[g_nTens - 1];
				if ((iLhs2 % div) != 0)
				{
					iLhs2 = iLhs2 % div;
					goto start2;
				}
			}

			if (strLhs.length() > 3)
			{
				if (!strResult.empty())
					strResult += " ";
				strResult += g_huns[nh - 1];
			}

			strLhs = strLhs.substr(nd);
			strLhs.erase(0, strLhs.find_first_not_of(g_cZero));
			if (strLhs.length())
				goto start;
		}
	}
	else
		iResult = -2;

	if (bNegative && iResult == 0)
		strResult = "Negative " + strResult;

	if (iResult == 0 && stP1 != std::string::npos)
	{
		strResult += " Point";
		for (std::string::const_iterator cit = number.begin() + stP1 + 1; iResult == 0 && cit != number.end(); ++cit)
		{
			try
			{
				strResult += " " + g_ones[*cit - g_cZero];
			}
			catch (std::invalid_argument)
			{
				iResult = -1;
				break;
			}
		}
	}

	return strResult;
}

std::string NumberTranscriber::ToNumber(const std::string& phrase)
{
	std::string strResult;
	if (phrase.empty())
		return strResult;

	int iResult = 0;
	bool bNegative = false;
	bool bPoint = false;

	// Build the token list
	std::vector<std::string> vstrTokens;

	std::string strToken;
	size_t istart = 0, ipos;
	do
	{
		ipos = phrase.find(' ', istart);
		if (ipos == std::string::npos)
			vstrTokens.push_back(phrase.substr(istart));
		else
		{
			strToken = phrase.substr(istart, ipos - istart);
			vstrTokens.push_back(strToken);
			istart = ipos + 1;
		}
	} while (ipos != std::string::npos);

	// Build the number groups
	bool bFound = false;
	std::vector<std::string> vstrNumbers;
	std::vector<std::vector<std::string> > vvstrNumbers;
	std::map<std::string, std::string>::iterator mit;
	std::vector<std::string>::iterator it;
	for (it = vstrTokens.begin(); !bPoint && it != vstrTokens.end(); ++it)
	{
		bFound = false;
		mit = mapWordTo99.find(*it);
		if (mit != mapWordTo99.end())
		{
			vstrNumbers.push_back(mit->second);
			bFound = true;
		}
		else
		{
			mit = mapWordTo100.find(*it);
			if (mit != mapWordTo100.end())
			{
				vstrNumbers.push_back(mit->second);
				vvstrNumbers.push_back(vstrNumbers);
				vstrNumbers.clear();
				bFound = true;
			}
		}

		if (!bFound)
		{
			if (TextEqual(*it, "Negative"))
				bNegative = true;
			else if (TextEqual(*it, "Point"))
				bPoint = true;
			if (!bNegative && !bPoint)
			{
				iResult = -1;
				break;
			}
		}
	}

	if ((bFound || bPoint) && !vstrNumbers.empty())
		vvstrNumbers.push_back(vstrNumbers);

	if (iResult == -1)
		return strResult;

	std::string strNumber;
	for (std::vector<std::vector<std::string> >::iterator vvit = vvstrNumbers.begin(); vvit != vvstrNumbers.end(); ++vvit)
	{
		std::string strGroupNumber = *((*vvit).begin());
		for (std::vector<std::string>::iterator vit = (*vvit).begin() + 1; vit != (*vvit).end(); ++vit)
		{
			strNumber = *vit;
			if (strNumber.length() > 2 &&
				*(strNumber.end() - 1) == g_cZero)
				strGroupNumber.append(strNumber.begin() + 1, strNumber.end());
			else
				strGroupNumber.replace(strGroupNumber.end() - strNumber.length(), strGroupNumber.end(), strNumber);
		}

		if (!strResult.empty())
			strResult.replace(strResult.end() - strGroupNumber.length(), strResult.end(), strGroupNumber);
		else
			strResult = strGroupNumber;
	}

	if (bNegative)
		strResult = "-" + strResult;

	if (bPoint)
	{
		strResult += ".";
		for (; iResult == 0 && it != vstrTokens.end(); ++it)
		{
			mit = mapWordTo99.find(*it);
			if (mit != mapWordTo99.end())
				strResult += mit->second;
			else
				iResult = -1;
		}

	}
	return strResult;
}

bool NumberTranscriber::TextEqual(const std::string& s1, const std::string& s2)
{
	bool bEqual = true;
	if (s1.length() != s1.length())
		bEqual = false;
	else
	{
		for (size_t stIdx = 0; bEqual && stIdx < s1.length(); ++stIdx)
		{
			if (tolower(*(s1.begin() + stIdx)) != tolower(*(s2.begin() + stIdx)))
				bEqual = false;
		}
	}
	return bEqual;
}

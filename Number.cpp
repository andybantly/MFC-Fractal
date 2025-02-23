#include "stdafx.h"
#include "Number.h"
#include "Constants.h"
#include <sstream>
#include <map>
#pragma warning(disable:6385)

static std::map<std::string, std::string, CILT> g_mapWordTo99;
static std::map<std::string, std::string, CILT> g_mapWordTo100;
static const std::string g_one("1");

std::string Number::ToPhrase() const
{
	static bool bInit = false;
	if (!bInit)
	{
		bInit = true;
		CNumber::Init();
	}

	CNumber N(ToDisplay());
	return N.GetPhrase();
}

CNumber::CNumber() : m_bNegative(false)
{
	m_strNumber.clear();
	m_strPhrase.clear();
}

CNumber::CNumber(const std::string& strInput, bool bNum)
{
	SetNumber(bNum ? strInput : Contract(strInput));
}

void CNumber::SetNumber(const std::string& strInput)
{
	bool bZero = true;
	if (!strInput.empty())
	{
		m_bNegative = false;
		bool bDigit = false;
		bool bDec = false;
		std::deque<char> dqInput;
		std::string::const_iterator cit;
		for (cit = strInput.begin(); cit != strInput.end(); ++cit)
		{
			if (*cit == ' ')
				continue;
			break;
		}
		for (; cit != strInput.end(); ++cit)
		{
			if (!m_bNegative && *cit == '-')
			{
				if (bDigit || bDec || m_bNegative)
					throw(std::exception("Invalid Number"));
				dqInput.push_front('-');
				m_bNegative = true;
				continue;
			}

			if (*cit == g_cDecSep)
			{
				if (bDec)
					throw(std::exception("Invalid Number"));
				bDec = true;
				if (dqInput.size() == 0)
					dqInput.push_back(g_cZero);
				dqInput.push_back(g_cDecSep);
				continue;
			}

			if (isdigit(*cit))
			{
				if (*cit == g_cZero)
				{
					if (dqInput.size() == 0)
						continue;
					if (m_bNegative && dqInput.size() == 1)
						continue;
				}

				if (bZero && *cit != g_cZero)
					bZero = false;
				dqInput.push_back(*cit);
				bDigit = true;
				continue;
			}
			
			if (!isdigit(*cit))
			{
				std::string strResult = Contract(strInput);
				if (strResult.length() > 0)
					SetNumber(strResult); // Sometimes recursion is good
				return;
			}
		}

		while (bDec && dqInput.size() > 0 && *(dqInput.end() - 1) == g_cZero)
			dqInput.pop_back();
		if (dqInput.size() > 0 && *(dqInput.end() - 1) == g_cDecSep)
			dqInput.pop_back();
		if (dqInput.size() > 0)
			m_strNumber = std::string(dqInput.begin(), dqInput.end());
		else
			m_strNumber = "0";

		Convert();
	}
	else
	{
		m_strNumber.clear();
		m_strPhrase.clear();
		m_bNegative = false;
	}
}

std::string CNumber::WB()
{
	return g_huns[g_nHuns - 1];
}

std::string CNumber::Expand(const std::string& strInput)
{
	std::string strResult;
	if (strInput.empty())
		return strResult;

	int iResult = 0;
	m_bNegative = false;
	int digs, nd, nh;

	std::string strLhs;
	size_t stP1 = strInput.find(g_cDecSep);
	if (stP1 == std::string::npos)
		strLhs = strInput;
	else
		strLhs = strInput.substr(0, stP1);
	if (strLhs.empty())
		strLhs = g_cZero;

	strResult.clear();
	if (*(strLhs.begin()) == '-')
	{
		m_bNegative = true;
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

	if (m_bNegative && iResult == 0)
		strResult = "Negative " + strResult;

	if (iResult == 0 && stP1 != std::string::npos)
	{
		strResult += " Point";
		for (std::string::const_iterator cit = strInput.begin() + stP1 + 1; iResult == 0 && cit != strInput.end(); ++cit)
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

std::string CNumber::Contract(const std::string& strInput) // Description
{
	std::string strResult;
	if (strInput.empty())
		return strResult;
	
	int iResult = 0;
	m_bNegative = false;
	bool bPoint = false;

	// Build the token list
	std::vector<std::string> vstrTokens;
	Split(strInput, vstrTokens);

	// Build the number groups
	bool bFound = false;
	std::vector<std::string> vstrNumbers;
	std::vector<std::vector<std::string> > vvstrNumbers;
	std::map<std::string, std::string>::iterator mit;
	std::vector<std::string>::iterator it;
	for (it = vstrTokens.begin(); !bPoint && it != vstrTokens.end(); ++it)
	{
		bFound = false;
		mit = g_mapWordTo99.find(*it);
		if (mit != g_mapWordTo99.end())
		{
			vstrNumbers.push_back(mit->second);
			bFound = true;
		}
		else
		{
			mit = g_mapWordTo100.find(*it);
			if (mit != g_mapWordTo100.end())
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
				m_bNegative = true;
			else if (TextEqual(*it, "Point"))
				bPoint = true;
			if (!m_bNegative && !bPoint)
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

	if (m_bNegative)
		strResult = "-" + strResult;

	if (bPoint)
	{
		strResult += ".";
		for (; iResult == 0 && it != vstrTokens.end(); ++it)
		{
			mit = g_mapWordTo99.find(*it);
			if (mit != g_mapWordTo99.end())
				strResult += mit->second;
			else
				iResult = -1;
		}

	}
	return strResult;
}

void CNumber::Convert()
{
	m_strPhrase = Expand(m_strNumber);
	if (m_strPhrase.empty())
		throw(std::exception("Invalid Number"));
}

void CNumber::Split(const std::string& strInput, std::vector<std::string>& vstrTokens, const char cFind)
{
	vstrTokens.clear();
	if (strInput.empty())
		return;

	std::string strToken;
	size_t istart = 0, ipos;
	do
	{
		ipos = strInput.find(cFind, istart);
		if (ipos == std::string::npos)
			vstrTokens.push_back(strInput.substr(istart));
		else
		{
			strToken = strInput.substr(istart, ipos - istart);
			vstrTokens.push_back(strToken);
			istart = ipos + 1;
		}
	} while (ipos != std::string::npos);
}

const std::string& CNumber::GetNumber() const
{
	return m_strNumber;
}

const std::string& CNumber::GetPhrase() const
{
	return m_strPhrase;
}

bool CNumber::TextEqual(const std::string& strLHS, const std::string& strRHS)
{
	bool bEqual = true;
	if (strLHS.length() != strRHS.length())
		bEqual = false;
	else
	{
		for (size_t stIdx = 0; bEqual && stIdx < strLHS.length(); ++stIdx)
		{
			if (tolower(*(strLHS.begin() + stIdx)) !=
				tolower(*(strRHS.begin() + stIdx)))
				bEqual = false;
		}
	}
	return bEqual;
}

void CNumber::Init()
{
	std::string strTen, strWord, strNum, strHun;
	for (int iOne = 0; iOne < g_nOnes; ++iOne)
		g_mapWordTo99[g_ones[iOne]] = g_nones[iOne];

	for (int iTen = 2; iTen < g_nTens; ++iTen)
		g_mapWordTo99[g_tens[iTen]] = g_ntens[iTen];

	for (int iTen = 2, iTens; iTen < g_nTens - 1; ++iTen)
	{
		iTens = iTen * 10;
		strTen = g_tens[iTen] + "-";
		for (int iOne = 1; iOne < 10; ++iOne)
		{
			strWord = strTen + g_ones[iOne];
			strNum = std::to_string(iTens + iOne);
			g_mapWordTo99[strWord] = strNum;
		}
	}

	for (int iHun = 0, nZero = 3; iHun < g_nHuns; iHun++, nZero += 3)
	{
		strHun = g_one + std::string(nZero, g_cZero);
		g_mapWordTo100[g_huns[iHun]] = strHun;
	}
}

std::ostream& operator<<(std::ostream& out, const CNumber& Number)
{
	out << Number.m_strNumber;
	return out;
}

// error C2338 : static_assert failed : 'Test writer must define specialization of ToString<const Q& q> 
// for your class class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> > 
// __cdecl Microsoft::VisualStudio::CppUnitTestFramework::ToString<class CNumber>(const class CNumber &).
std::wstring CNumber::ToString(const CNumber& rhs)
{
	std::wstringstream wstrStream;
	wstrStream << rhs.m_strNumber.c_str();
	return wstrStream.str();
}
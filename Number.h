#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <ctime>
#include <thread>
#include <functional>

class CNumber
{
public:
    CNumber();
    CNumber(const std::string& strInput, bool bNum = true);

    ~CNumber() {};

public:
    void SetNumber(const std::string& strInput);

    std::string Contract(const std::string& strInput);
    std::string Expand(const std::string& strInput);

    static std::string WB();
    const std::string& GetNumber() const;
    const std::string& GetPhrase() const;

    static void Init();
    static bool TextEqual(const std::string& strLHS, const std::string& strRHS);
    friend std::ostream& operator<<(std::ostream& out, const CNumber& Number);

    // error C2338 : static_assert failed : 'Test writer must define specialization of ToString<const Q& q> 
    // for your class class std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> > 
    // __cdecl Microsoft::VisualStudio::CppUnitTestFramework::ToString<class CNumber>(const class CNumber &).
    static std::wstring ToString(const CNumber& Number);

protected:
    void Convert();
    void Split(const std::string& strInput, std::vector<std::string>& vstrTokens, const char cFind = ' ');

    bool m_bNegative;

    std::string m_strNumber;
    std::string m_strPhrase;
};

static uint8_t g_pow[8] = { 1,2,4,8,16,32,64,128 };

class Number
{
protected:
    class CByte
    {
    public:
        CByte()
        {
            U = 0;
            OF = 0;
        }

        CByte(uint8_t byte)
        {
            U = byte;
            OF = 0;
        };

        CByte(const CByte& rhs)
        {
            *this = rhs;
        }

        CByte& operator = (const CByte& rhs)
        {
            if (this != &rhs)
            {
                U = rhs.U;
                OF = rhs.OF;
            }
            return *this;
        }

        CByte operator + (const CByte& rhs) const // Full-Adder
        {
            CByte Out;
            Out.OF = OF; // Kerry-In

            for (uint8_t ui = 1, uj = 0; ui != 0; ui <<= 1, ++uj)
            {
                Out.U |= (Out.OF ^ (((U & ui) >> uj) ^ ((rhs.U & ui) >> uj))) << uj;                                                // SUM:   Kerry-in XOR (A XOR B)
                Out.OF = (((U & ui) >> uj) & Out.OF) | (((U & ui) >> uj) & ((rhs.U & ui) >> uj)) | (((rhs.U & ui) >> uj) & Out.OF); // CARRY: Kerry-out AB OR BC OR ACin
            }

            return Out;
        }
        
        CByte operator - (const CByte& rhs) const // Full-Subtractor
        {
            CByte Out;
            Out.OF = OF; // Borrow-In

            for (uint8_t ui = 1, uj = 0; ui != 0; ui <<= 1, ++uj)
            {
                Out.U |= (Out.OF ^ (((U & ui) >> uj) ^ ((rhs.U & ui) >> uj))) << uj;                                                  // DIFFERENCE: (A XOR B) XOR Borrow-in
                Out.OF = (~((U & ui) >> uj) & Out.OF) | (~((U & ui) >> uj) & ((rhs.U & ui) >> uj)) | (((rhs.U & ui) >> uj) & Out.OF); // BORROW: A'Borrow-in OR A'B OR AB (' = 2s complement)
            }

            return Out;
        }

        uint8_t U;
        uint8_t OF;
    };

    // Helper to convert to the internal format
    void Convert(const int32_t iNumber)
    {
        static CByte _0(0), _255(255);

        m_bNeg = iNumber < 0;
        m_Bytes.resize(4, m_bNeg ? _255 : _0);

        m_Bytes[0] = (uint32_t)(iNumber) & 0xFF;
        m_Bytes[1] = ((uint32_t)(iNumber) >> 8) & 0xFF;
        m_Bytes[2] = ((uint32_t)(iNumber) >> 16) & 0xFF;
        m_Bytes[3] = (uint32_t)(iNumber) >> 24;

        m_bNAN = false;
    }

public:
    void Shl() // Shift Left (double)
    {
        size_t iByte = m_Bytes.size() - 1;
        for (; iByte != 0; --iByte)
        {
            m_Bytes[iByte].U <<= 1;
            m_Bytes[iByte].U |= m_Bytes[iByte - 1].U & 128 ? 1 : 0;
        }
        m_Bytes[iByte].U <<= 1;
        m_bNeg = m_Bytes[m_Bytes.size() - 1].U && 128;
    }

    void Shr() // Shift Right (halve)
    {
        size_t iByte = 0;
        for (; iByte != m_Bytes.size() - 1; ++iByte)
        {
            m_Bytes[iByte].U >>= 1;
            if (m_Bytes[iByte + 1].U & 1)
                m_Bytes[iByte].U |= 128;
        }

        m_Bytes[iByte].U >>= 1;
        if (m_bNeg)
            m_Bytes[iByte].U |= 128;
    }

public:
    Number() : m_bNeg(false), m_bNAN(true) {};

    Number(const char* pstrNumber)
    {
        ToBinary(pstrNumber);
    }

    Number(const std::string& strNumber)
    {
        ToBinary(strNumber);
    }

    Number(const int32_t iNumber)
    {
        Convert(iNumber);
    }

    Number(CByte ch, size_t size)
    {
        static CByte _0(0), _255(255);

        m_bNeg = (ch.U & 128) >> 7 ? true : false;
        m_Bytes.resize(size, m_bNeg ? _255 : _0);
        m_Bytes[0] = ch;
        m_bNAN = false;
    }

    Number(const CNumber rhs)
    {
        ToBinary(rhs.GetNumber());
    }

    ~Number()
    {
    }

    Number& operator = (const Number& rhs)
    {
        if (this != &rhs)
        {
            m_Bytes = rhs.m_Bytes;
            m_bNeg = rhs.m_bNeg;
            m_bNAN = rhs.m_bNAN;
        }
        return *this;
    }

    Number& operator = (const int32_t iNumber)
    {
        Convert(iNumber);
        return *this;
    }

    Number& operator = (const std::string& strNumber)
    {
        ToBinary(strNumber);
        return *this;
    }

    Number& operator = (const char* pstrNumber)
    {
        ToBinary(pstrNumber);
        return *this;
    }

    bool operator == (const Number& rhs) const
    {
        if (this == &rhs) // I AM ALWAYS EQUAL TOO MYSELF!
            return true;

        if (m_bNAN || rhs.m_bNAN)
            return false;

        if (m_bNeg != rhs.m_bNeg)
            return false;

        if (m_Bytes.size() != rhs.m_Bytes.size())
            return false;

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static CByte Zero(0), Neg1(255);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            CByte lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            CByte rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.U != rb.U)
                return false;
        }

        return true;
    }

    bool operator != (const Number& rhs) const
    {
        return !(operator == (rhs));
    }

    bool operator < (const Number& rhs) const
    {
        if (this == &rhs)
            return false; // I CANT BE LESS THAN MYSELF!

        if (m_bNAN || rhs.m_bNAN)
            return false;

        if (m_bNeg != rhs.m_bNeg)
            return m_bNeg;

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static CByte Zero(0), Neg1(255);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            CByte lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            CByte rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.U != rb.U)
                return m_bNeg ? lb.U < rb.U : lb.U < rb.U;
        }

        return false;
    }

    bool operator <= (const Number& rhs) const
    {
        return (operator < (rhs) || operator == (rhs));
    }

    bool operator > (const Number& rhs) const
    {
        return !(operator <= (rhs));
    }

    bool operator >= (const Number& rhs) const
    {
        return !(operator < (rhs));
    }

    Number Add(const Number& rhs, size_t st = 0) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMin = l == r ? l : (l < r ? l : r);
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static CByte Zero(0), Neg1(255);
        Number out(Zero, stMax);
        uint8_t of = 0;

        CByte lb, rb;
        for (; st < stMin; ++st)
        {
            lb = m_Bytes[st];
            rb = rhs.m_Bytes[st];
            CByte& ob = out.m_Bytes[st];
            lb.OF = of;
            ob = lb + rb;
            of = ob.OF;
            ob.OF = 0;
        }

        if (st < stMax)
        {
            for (; st < stMax; ++st)
            {
                lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
                rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
                CByte& ob = out.m_Bytes[st];
                lb.OF = of;
                ob = lb + rb;
                of = ob.OF;
                ob.OF = 0;
            }
        }

        out.m_bNeg = (out.m_Bytes[out.GetSize() - 1].U & 128) >> 7 ? true : false;

        return out;
    }

    Number Sub(const Number& rhs, size_t st = 0) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMin = l == r ? l : (l < r ? l : r);
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static CByte Zero(0), Neg1(255);
        Number out(Zero, stMax);
        uint8_t of = 0;

        CByte lb, rb;
        for (; st < stMin; ++st)
        {
            lb = m_Bytes[st];
            rb = rhs.m_Bytes[st];
            CByte& ob = out.m_Bytes[st];
            lb.OF = of;
            ob = lb - rb;
            of = ob.OF;
            ob.OF = 0;
        }

        if (st < stMax)
        {
            for (; st < stMax; ++st)
            {
                lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
                rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
                CByte& ob = out.m_Bytes[st];
                lb.OF = of;
                ob = lb - rb;
                of = ob.OF;
                ob.OF = 0;
            }
        }

        out.m_bNeg = (out.m_Bytes[out.GetSize() - 1].U & 128) >> 7 ? true : false;

        return out;
    }

    Number operator + (const Number& rhs) const
    {
        return Add(rhs);
    }

    Number operator - (const Number& rhs) const
    {
        return Sub(rhs);
    }

    Number operator * (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        size_t stMB = m_Bytes.size() > rhs.m_Bytes.size() ? m_Bytes.size() : rhs.m_Bytes.size();
        Number out(CByte(0), stMB);
        Number lhs = *this;
        lhs.SetSize(stMB);

        for (size_t iByte = 0, nBytes = rhs.m_Bytes.size(); iByte < nBytes; ++iByte)
        {
            for (uint8_t ui = 1; ui != 0; ui <<= 1)
            {
                if (ui & rhs.m_Bytes[iByte].U)
                    out += lhs;
                lhs.Shl();
            }
        }

        return out;
    }

    Number operator / (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        const static Number _0(CByte(0), 1);
        Number quot;
        if (rhs == _0)
            return quot;

        size_t stMB = m_Bytes.size() > rhs.m_Bytes.size() ? m_Bytes.size() : rhs.m_Bytes.size();
        Number rem = *this;
        Number rhsin = rhs;
        rem.SetSize(stMB);
        rhsin.SetSize(stMB);

        if (m_bNeg != rhs.m_bNeg)
        {
            rhsin = rhsin.TwosComplement();
            rhsin.m_bNeg = m_bNeg;
        }

        Number dbl = rhsin;
        Number pow(m_bNeg == rhs.m_bNeg ? CByte(1) : CByte(-1), stMB);
        size_t stn = 1;

        if (!m_bNeg)
        {
            while (dbl < rem)
            {
                dbl.Shl();
                if (dbl.m_bNeg)
                    return quot;
                pow.Shl();
                ++stn;
            }

            quot = _0;
            for (size_t ndbl = stn; ndbl > 0; --ndbl)
            {
                if (dbl > rem)
                {
                    dbl.Shr();
                    pow.Shr();
                    continue;
                }

                quot += pow;
                rem -= dbl;

                dbl.Shr();
                pow.Shr();
            }
        }
        else
        {
            while (dbl > rem)
            {
                dbl.Shl();
                if (!dbl.m_bNeg)
                    return quot;
                pow.Shl();
                ++stn;
            }

            quot = _0;
            for (size_t ndbl = stn; ndbl > 0; --ndbl)
            {
                if (dbl < rem)
                {
                    dbl.Shr();
                    pow.Shr();
                    continue;
                }

                quot += pow;
                rem -= dbl;

                dbl.Shr();
                pow.Shr();
            }
        }

        return quot;
    }

    Number operator % (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");
        const static Number _0(CByte(0), 1);
        Number rem;
        if (rhs == _0)
            return rem;

        size_t stMB = m_Bytes.size() > rhs.m_Bytes.size() ? m_Bytes.size() : rhs.m_Bytes.size();
        rem = *this;
        Number rhsin = rhs;
        rem.SetSize(stMB);
        rhsin.SetSize(stMB);

        if (m_bNeg != rhs.m_bNeg)
        {
            rhsin = rhsin.TwosComplement();
            rhsin.m_bNeg = m_bNeg;
        }

        Number dbl = rhsin;
        size_t stn = 1;

        if (!m_bNeg)
        {
            while (dbl < rem)
            {
                dbl.Shl();
                if (dbl.m_bNeg)
                    return Number();
                ++stn;
            }

            for (size_t ndbl = stn; ndbl > 0; --ndbl)
            {
                if (dbl > rem)
                {
                    dbl.Shr();
                    continue;
                }
                rem -= dbl;
                dbl.Shr();
            }
        }
        else
        {
            while (dbl > rem)
            {
                dbl.Shl();
                if (!dbl.m_bNeg)
                    return Number();
                ++stn;
            }

            for (size_t ndbl = stn; ndbl > 0; --ndbl)
            {
                if (dbl < rem)
                {
                    dbl.Shr();
                    continue;
                }
                rem -= dbl;
                dbl.Shr();
            }
        }

        return rem;
    }

    // pre/post increment/decrement
    /*
    The prefix increment/decrement operator (++/--) adds/subs one to its operand and this incremented value is the result of the expression.
    The postfix increment/decrement operator (++/--)(int) adds/subs one to its operand and the previous value is the result of the expression
    */

    Number operator ++ ()
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw("Invalid number");

        *this += _1;
        return *this;
    }

    Number operator -- ()
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw("Invalid number");

        *this -= 1;
        return *this;
    }

    Number operator ++ (int)
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw("Invalid number");

        Number rhs = *this;
        *this += _1;
        return rhs;
    }

    Number operator -- (int)
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw("Invalid number");

        Number rhs = *this;
        *this += _1;
        return rhs;
    }

    Number operator , (const Number& rhs) const
    {
        return rhs;
    }

    Number operator += (const Number& rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    Number operator -= (const Number& rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    Number operator *= (const Number& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    Number operator /= (const Number& rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    Number operator %= (const Number& rhs)
    {
        *this = *this % rhs;
        return *this;
    }

    void SetSize(size_t size)
    {
        if (size != m_Bytes.size())
            m_Bytes.resize(size, m_bNeg ? 255 : 0);
    }

    size_t GetSize() const
    {
        return m_Bytes.size();
    }

    // Conversion functions
    Number TwosComplement() const
    {
        if (m_bNAN)
            throw("Invalid number");

        size_t size = m_Bytes.size();
        Number Out(CByte(0), size), One(CByte(1), size);

        uint8_t iByte = 0;
        do
        {
            Out.m_Bytes[iByte].U = ~m_Bytes[iByte].U;
            iByte++;
        } while (iByte != size);

        size_t sz = Out.m_Bytes.size();
        Out = Out + One;
        Out.m_Bytes.resize(sz);
        Out.m_bNeg = m_bNeg;

        return Out;
    }

    std::string ToDisplay() const
    {
        if (m_bNAN)
            return "NAN";

        if (m_bNeg)
        {
            Number Disp = TwosComplement();
            Disp.m_bNeg = false;
            return "-" + Disp.ToDisplay();
        }

        size_t size = m_Bytes.size();
        std::string strResult = "0";

        const uint8_t cZero = '0', cOne = '1', cDec = '.';
        std::string strNum = "1";

        uint8_t iByte = 0;
        uint8_t iBit = 0;

        do
        {
            uint8_t iProd;
            std::deque<char> mout;
            bool bCarry = false;
            for (std::string::const_reverse_iterator crit2 = strNum.rbegin(); crit2 != strNum.rend(); )
            {
                uint8_t iMP = *crit2++ - cZero;
                iProd = 2 * iMP;
                if (bCarry)
                {
                    iProd++;
                    bCarry = false;
                }
                if (iProd >= 10)
                {
                    iProd -= 10;
                    bCarry = true;
                }
                mout.push_front(cZero + iProd);
            }

            if (bCarry)
                mout.push_front(cOne);

            if (m_Bytes[iByte].U & g_pow[iBit++]) // Evaluates to False=0 or True=one of 1,2,4,8,16,32,66,128
            {
                const std::string& strS1 = strNum;
                const std::string& strS2 = strResult;

                uint8_t iSum;
                std::deque<char> Sum;
                bool bCarry = false;

                std::string::const_reverse_iterator S1_crend = strS1.rend();
                std::string::const_reverse_iterator S2_crend = strS2.rend();

                for (std::string::const_reverse_iterator S1_crit = strS1.rbegin(), S2_crit = strS2.rbegin();
                    S1_crit != S1_crend || S2_crit != S2_crend;)
                {
                    uint8_t S1 = S1_crit != S1_crend ? *S1_crit++ : cZero;
                    uint8_t S2 = S2_crit != S2_crend ? *S2_crit++ : cZero;

                    if (S1 == cDec || S2 == cDec)
                    {
                        Sum.push_front(cDec);
                        continue;
                    }

                    uint8_t N1 = S1 - cZero;
                    uint8_t N2 = S2 - cZero;

                    iSum = N1 + N2;

                    if (bCarry)
                    {
                        iSum++;
                        bCarry = false;
                    }

                    if (iSum >= 10)
                    {
                        iSum -= 10;
                        bCarry = true;
                    }

                    Sum.push_front(cZero + iSum);
                }

                if (bCarry)
                    Sum.push_front(cOne);

                if (*Sum.begin() == cDec)
                    Sum.push_front(cZero);

                strResult = std::string(Sum.begin(), Sum.end());
            }
            strNum = std::string(mout.begin(), mout.end());

            if (iBit == 8)
            {
                iByte++;
                iBit = 0;
            }
        } while (iByte != size);

        return strResult;
    }

    std::string ToBinary() const
    {
        size_t nBin = m_Bytes.size() * size_t(8);
        std::string strBin(nBin, '0');

        for (size_t iByte = 0, nBytes = m_Bytes.size(); iByte < nBytes; ++iByte, nBin -= 8)
        {
            for (size_t iBit = 0; iBit < 8; ++iBit)
            {
                if (g_pow[iBit] & m_Bytes[iByte].U)
                    strBin[nBin - iBit - 1] = '1';
            }
        }

        return strBin;
    }

    std::string ToPhrase() const;

    friend std::ostream& operator << (std::ostream& out, Number& rhs)
    {
        out << rhs.ToDisplay();
        return out;
    }

protected:

    void ToBinary(const std::string strNumber)
    {
        if (strNumber.empty())
            throw("Invalid number");
        
        if (strNumber[0] == '-')
        {
            if (strNumber.length() < 2)
                throw("Invalid number");
            m_bNeg = true;
        }
        else
            m_bNeg = false;

        std::string strInput = strNumber.substr(m_bNeg ? 1 : 0);
        if (strInput.empty())
            throw("Invalid number");

        m_bNAN = false;

        std::string strOut;
        uint8_t idnm = 0;
        std::deque<char> binary;

        uint8_t bVal = 0;
        uint8_t bPos = 0;
        uint8_t bSiz = 0;
        std::vector<uint8_t> vbytes;

        std::string::const_iterator cit = strInput.begin();
        for (;;)
        {
            // Compute the denominator of the division
            idnm = idnm * 10 + *cit - '0';
            if (idnm < 2 && cit + 1 != strInput.end())
            {
                // Carry a 0
                if (!strOut.empty())
                    strOut += '0';

                // The denominator has to be greater than 2 now
                idnm = idnm * 10 + (*(cit + 1) - '0');

                // Move to the next character
                cit += 2;
            }
            else
            {
                // Check for completion the conversion
                if (strInput.length() == 1 && idnm < 2)
                {
                    /////////////////////////////////////
                    // Binary stream 0s and 1s

                    binary.push_front('0' + idnm);

                    /////////////////////////////////////
                    // Byte stream 0-255

                    bSiz++;
                    if (idnm)
                        bVal += g_pow[bPos];
                    bPos++;
                    if (bPos > 7)
                    {
                        vbytes.push_back(bVal);
                        bVal = 0;
                        bPos = 0;
                    }

                    /////////////////////////////////////

                    break;
                }

                // Move to the next character
                cit++;
            }

            // Append the digit to the output that becomes the new input from integer division by 2
            strOut += '0' + idnm / 2;
            idnm = idnm % 2;

            // Has the input been processed
            if (cit == strInput.end())
            {
                // Add the remainder of 0 or 1 to the binary string
                binary.push_front('0' + idnm);

                /////////////////////////////////////

                bSiz++;
                if (idnm)
                    bVal += g_pow[bPos];
                bPos++;
                if (bPos > 7)
                {
                    vbytes.push_back(bVal);
                    bVal = 0;
                    bPos = 0;
                }

                /////////////////////////////////////

                // Reset and restart (but not the incremental bytes, they carry over)
                strInput = strOut;
                strOut.clear();
                idnm = 0;
                cit = strInput.begin();
            }
        }
        if (bVal)
            vbytes.push_back(bVal);

        size_t size = uint8_t(vbytes.size());
        if (size)
        {
            m_Bytes.resize(size);
            for (size_t iByte = 0; iByte < size; ++iByte)
                m_Bytes[iByte].U = vbytes[iByte];
        }
        else
        {
            m_Bytes.resize(1);
            m_Bytes[0].U = 0;
        }

        if (m_bNeg)
            *this = TwosComplement();

        SetSize(max(GetSize(), size_t(4)));
    }
    
    protected:
        std::vector<CByte> m_Bytes;
        bool m_bNeg;
        bool m_bNAN;
};

struct CILT
{
    struct Compare
    {
        bool operator() (const unsigned char& c1, const unsigned char& c2) const
        {
            return tolower(c1) < tolower(c2);
        }
    };

    bool operator() (const std::string& strLhs, const std::string& strRhs) const
    {
        return std::lexicographical_compare
        (
            strLhs.begin(), strLhs.end(),
            strRhs.begin(), strRhs.end(),
            Compare()
        );
    }
};
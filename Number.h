#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <ctime>
#include <thread>
#include <functional>

static uint8_t g_pow[8] = { 1,2,4,8,16,32,64,128 };

class Number
{
protected:
    class CByte
    {
    public:
        CByte()
        {
            m_b.U = 0;
            m_x.U = 0;
        }

        CByte(int32_t byte)
        {
            m_b.U = byte;
            m_x.U = 0;
        };

        CByte(const CByte& rhs)
        {
            *this = rhs;
        }

        CByte& operator = (const CByte& rhs)
        {
            if (this != &rhs)
            {
                m_b = rhs.m_b;
                m_x = rhs.m_x;
            }
            return *this;
        }

        CByte operator + (const CByte& rhs) const // Full-Adder
        {
            CByte Out;
            Out.setOF(getOF()); // Is there a better way to handle Carry?

            Out.m_b.B.B1 = Out.m_x.X.X0 ^ (m_b.B.B1 ^ rhs.m_b.B.B1);  // SUM: Carry-in XOR (A XOR B)
            Out.m_x.X.X1 = (m_b.B.B1 & rhs.m_b.B.B1) | (rhs.m_b.B.B1 & Out.m_x.X.X0) | (m_b.B.B1 & Out.m_x.X.X0); // CARRY: Carry-out AB OR BC OR ACin

            Out.m_b.B.B2 = Out.m_x.X.X1 ^ (m_b.B.B2 ^ rhs.m_b.B.B2);
            Out.m_x.X.X2 = (m_b.B.B2 & rhs.m_b.B.B2) | (rhs.m_b.B.B2 & Out.m_x.X.X1) | (m_b.B.B2 & Out.m_x.X.X1);

            Out.m_b.B.B3 = Out.m_x.X.X2 ^ (m_b.B.B3 ^ rhs.m_b.B.B3);
            Out.m_x.X.X3 = (m_b.B.B3 & rhs.m_b.B.B3) | (rhs.m_b.B.B3 & Out.m_x.X.X2) | (m_b.B.B3 & Out.m_x.X.X2);

            Out.m_b.B.B4 = Out.m_x.X.X3 ^ (m_b.B.B4 ^ rhs.m_b.B.B4);
            Out.m_x.X.X4 = (m_b.B.B4 & rhs.m_b.B.B4) | (rhs.m_b.B.B4 & Out.m_x.X.X3) | (m_b.B.B4 & Out.m_x.X.X3);

            Out.m_b.B.B5 = Out.m_x.X.X4 ^ (m_b.B.B5 ^ rhs.m_b.B.B5);
            Out.m_x.X.X5 = (m_b.B.B5 & rhs.m_b.B.B5) | (rhs.m_b.B.B5 & Out.m_x.X.X4) | (m_b.B.B5 & Out.m_x.X.X4);

            Out.m_b.B.B6 = Out.m_x.X.X5 ^ (m_b.B.B6 ^ rhs.m_b.B.B6);
            Out.m_x.X.X6 = (m_b.B.B6 & rhs.m_b.B.B6) | (rhs.m_b.B.B6 & Out.m_x.X.X5) | (m_b.B.B6 & Out.m_x.X.X5);

            Out.m_b.B.B7 = Out.m_x.X.X6 ^ (m_b.B.B7 ^ rhs.m_b.B.B7);
            Out.m_x.X.X7 = (m_b.B.B7 & rhs.m_b.B.B7) | (rhs.m_b.B.B7 & Out.m_x.X.X6) | (m_b.B.B7 & Out.m_x.X.X6);

            Out.m_b.B.B8 = Out.m_x.X.X7 ^ (m_b.B.B8 ^ rhs.m_b.B.B8);
            Out.m_x.X.X0 = (m_b.B.B8 & rhs.m_b.B.B8) | (rhs.m_b.B.B8 & Out.m_x.X.X7) | (m_b.B.B8 & Out.m_x.X.X7);

            return Out;
        }

        CByte operator - (const CByte& rhs) const // Full-Subtractor
        {
            CByte Out;
            Out.setOF(getOF()); // Is there a better way to handle borrowing?

            Out.m_b.B.B1 = (m_b.B.B1 ^ rhs.m_b.B.B1) ^ Out.m_x.X.X0; // DIFFERENCE: (A XOR B) XOR Borrow-in
            Out.m_x.X.X1 = (~m_b.B.B1 & Out.m_x.X.X0) | (~m_b.B.B1 & rhs.m_b.B.B1) | (rhs.m_b.B.B1 & Out.m_x.X.X0); // BORROW: A'Borrow-in OR A'B OR AB (' = 2's complement)

            Out.m_b.B.B2 = (m_b.B.B2 ^ rhs.m_b.B.B2) ^ Out.m_x.X.X1;
            Out.m_x.X.X2 = (~m_b.B.B2 & Out.m_x.X.X1) | (~m_b.B.B2 & rhs.m_b.B.B2) | (rhs.m_b.B.B2 & Out.m_x.X.X1);

            Out.m_b.B.B3 = (m_b.B.B3 ^ rhs.m_b.B.B3) ^ Out.m_x.X.X2;
            Out.m_x.X.X3 = (~m_b.B.B3 & Out.m_x.X.X2) | (~m_b.B.B3 & rhs.m_b.B.B3) | (rhs.m_b.B.B3 & Out.m_x.X.X2);

            Out.m_b.B.B4 = (m_b.B.B4 ^ rhs.m_b.B.B4) ^ Out.m_x.X.X3;
            Out.m_x.X.X4 = (~m_b.B.B4 & Out.m_x.X.X3) | (~m_b.B.B4 & rhs.m_b.B.B4) | (rhs.m_b.B.B4 & Out.m_x.X.X3);

            Out.m_b.B.B5 = (m_b.B.B5 ^ rhs.m_b.B.B5) ^ Out.m_x.X.X4;
            Out.m_x.X.X5 = (~m_b.B.B5 & Out.m_x.X.X4) | (~m_b.B.B5 & rhs.m_b.B.B5) | (rhs.m_b.B.B5 & Out.m_x.X.X4);

            Out.m_b.B.B6 = (m_b.B.B6 ^ rhs.m_b.B.B6) ^ Out.m_x.X.X5;
            Out.m_x.X.X6 = (~m_b.B.B6 & Out.m_x.X.X5) | (~m_b.B.B6 & rhs.m_b.B.B6) | (rhs.m_b.B.B6 & Out.m_x.X.X5);

            Out.m_b.B.B7 = (m_b.B.B7 ^ rhs.m_b.B.B7) ^ Out.m_x.X.X6;
            Out.m_x.X.X7 = (~m_b.B.B7 & Out.m_x.X.X6) | (~m_b.B.B7 & rhs.m_b.B.B7) | (rhs.m_b.B.B7 & Out.m_x.X.X6);

            Out.m_b.B.B8 = (m_b.B.B8 ^ rhs.m_b.B.B8) ^ Out.m_x.X.X7;
            Out.m_x.X.X0 = (~m_b.B.B8 & Out.m_x.X.X7) | (~m_b.B.B8 & rhs.m_b.B.B8) | (rhs.m_b.B.B8 & Out.m_x.X.X7);

            return Out;
        }

        unsigned getOF() const
        {
            return m_x.X.X0; // X0 used to indicate a carry/borrow
        }

        void setOF(const unsigned rhs)
        {
            m_x.X.X0 = rhs;
        }

        struct _Bits
        {
            unsigned B1 : 1;
            unsigned B2 : 1;
            unsigned B3 : 1;
            unsigned B4 : 1;
            unsigned B5 : 1;
            unsigned B6 : 1;
            unsigned B7 : 1;
            unsigned B8 : 1;
        };

        struct _Xtra
        {
            unsigned X0 : 1;
            unsigned X1 : 1;
            unsigned X2 : 1;
            unsigned X3 : 1;
            unsigned X4 : 1;
            unsigned X5 : 1;
            unsigned X6 : 1;
            unsigned X7 : 1;
        };

        union BITS
        {
            struct _Bits B;
            uint8_t U;
        };

        union XTRA
        {
            struct _Xtra X;
            uint8_t U;
        };

        BITS m_b;
        XTRA m_x;
    };

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
        ToBinary(std::to_string(iNumber));
    }

    Number(CByte ch, size_t size)
    {
        m_bNeg = ch.m_b.B.B8;
        m_Bytes.resize(size, m_bNeg ? CByte(255) : CByte(0));
        m_Bytes[0] = ch;
        m_bNAN = false;
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
        ToBinary(std::to_string(iNumber));
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
        CByte Zero(0), Neg1(255);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            CByte lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            CByte rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.m_b.U != rb.m_b.U)
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
        CByte Zero(0), Neg1(255);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            CByte lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            CByte rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.m_b.U != rb.m_b.U)
                return m_bNeg ? lb.m_b.U < rb.m_b.U : lb.m_b.U < rb.m_b.U;
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

    Number& operator ++ () // ++Number
    {
        if (m_bNAN)
            throw("Invalid number");

        *this = *this + Number(CByte(1), m_Bytes.size());
        return *this;
    }

    Number& operator -- ()
    {
        if (m_bNAN)
            throw("Invalid number");

        *this = *this - Number(CByte(1), m_Bytes.size());
        return *this;
    }

    Number& operator ++ (int)
    {
        if (m_bNAN)
            throw("Invalid number");

        *this = *this + Number(CByte(1), m_Bytes.size());
        return *this;
    }

    Number& operator -- (int)
    {
        if (m_bNAN)
            throw("Invalid number");

        *this = *this - Number(CByte(1), m_Bytes.size());
        return *this;
    }

    Number operator + (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMax = l == r ? l : (l < r ? r : l);
        CByte Zero(0), Neg1(255);
        Number out(Zero, stMax);
        unsigned of = 0;
        for (size_t st = 0; st < stMax; ++st)
        {
            CByte lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            CByte rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            CByte& ob = out.m_Bytes[st];
            if (of)
                lb.setOF(of);
            ob = lb + rb;
            of = ob.getOF();
            if (of)
                ob.setOF(0);
        }

        out.m_bNeg = out.m_Bytes[out.GetSize() - 1].m_b.B.B8;

        return out;
    }

    Number operator - (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMax = l == r ? l : (l < r ? r : l);
        CByte Zero(0), Neg1(255);
        Number out(Zero, stMax);
        unsigned of = 0;
        for (size_t st = 0; st < stMax; ++st)
        {
            CByte lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            CByte rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            CByte& ob = out.m_Bytes[st];
            if (of)
                lb.setOF(of);
            ob = lb - rb;
            of = ob.getOF();
            if (of)
                ob.setOF(0);
        }

        out.m_bNeg = out.m_Bytes[out.GetSize() - 1].m_b.B.B8;

        return out;
    }

    Number operator * (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        const Number& lhs = *this;

        Number out(0, 4), one(1, 4), zero(0, 4);

        Number loop = zero;
        if (rhs.m_bNeg)
        {
            // - and -/+
            while (loop > rhs)
            {
                out = out - lhs;
                loop = loop - one;
            }
        }
        else
        {
            // + and -/+
            while (loop < rhs)
            {
                out = out + lhs;
                loop = loop + one;
            }
        }

        return out;
    }

    Number operator / (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        const Number& lhs = *this;

        Number tmp(0, 4), one(1, 4), zero(0, 4);

        Number loop;
        if (rhs == zero)
            return loop;
        loop = zero;
        if (lhs == zero)
            return zero;

        if (m_bNeg)
        {
            if (rhs.m_bNeg)
            {
                while (tmp > lhs)
                {
                    tmp = tmp + rhs;
                    loop = loop + one;
                }

                if (tmp != lhs)
                    loop = loop - one;
            }
            else
            {
                while (tmp > lhs)
                {
                    tmp = tmp - rhs;
                    loop = loop - one;
                }

                if (tmp != lhs)
                    loop = loop + one;
            }
        }
        else
        {
            if (rhs.m_bNeg)
            {
                while (tmp < lhs)
                {
                    tmp = tmp - rhs;
                    loop = loop - one;
                }

                if (tmp != lhs)
                    loop = loop + one;
            }
            else
            {
                while (tmp < lhs)
                {
                    tmp = tmp + rhs;
                    loop = loop + one;
                }

                if (tmp != lhs)
                    loop = loop - one;
            }
        }

        return loop;
    }

    Number operator % (const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw("Invalid number");

        const Number& lhs = *this;

        Number tmp(0, 4), one(1, 4), zero(0, 4);

        Number rem;
        if (rhs == zero)
            return rem;
        rem = zero;

        if (m_bNeg)
        {
            if (rhs.m_bNeg)
            {
                while (tmp > lhs)
                    tmp = tmp + rhs;

                if (tmp != lhs)
                {
                    tmp = tmp - rhs;
                    rem = lhs - tmp;
                }
            }
            else
            {
                while (tmp > lhs)
                    tmp = tmp - rhs;

                if (tmp != lhs)
                {
                    tmp = tmp + rhs;
                    rem = lhs - tmp;
                }
            }
        }
        else
        {
            if (rhs.m_bNeg)
            {
                while (tmp < lhs)
                    tmp = tmp - rhs;

                if (tmp != lhs)
                {
                    tmp = tmp + rhs;
                    rem = lhs - tmp;
                }
            }
            else
            {
                while (tmp < lhs)
                    tmp = tmp + rhs;

                if (tmp != lhs)
                {
                    tmp = tmp - rhs;
                    rem = lhs - tmp;
                }
            }
        }

        return rem;
    }

    void SetSize(size_t uiSize)
    {
        m_Bytes.resize(uiSize, m_bNeg ? 255 : 0);
    }

    size_t GetSize()
    {
        return m_Bytes.size();
    }

    // Conversion functions
    Number TwosComplement()
    {
        if (m_bNAN)
            throw("Invalid number");

        size_t size = m_Bytes.size();
        Number Out(CByte(0), size), One(CByte(1), size);

        uint8_t iByte = 0;
        do
        {
            Out.m_Bytes[iByte].m_b.U = ~m_Bytes[iByte].m_b.U;
            iByte++;
        } while (iByte != size);

        size_t sz = Out.m_Bytes.size();
        Out = Out + One;
        Out.m_Bytes.resize(sz);
        Out.m_bNeg = m_bNeg;

        return Out;
    }

    std::string ToDisplay()
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

            if (m_Bytes[iByte].m_b.U & g_pow[iBit++]) // Evaluates to False=0 or True=one of 1,2,4,8,16,32,66,128
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
                m_Bytes[iByte].m_b.U = vbytes[iByte];
        }
        else
        {
            m_Bytes.resize(1);
            m_Bytes[0].m_b.U = 0;
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

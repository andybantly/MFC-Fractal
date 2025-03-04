#pragma once
#include "Constants.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <ctime>
#include <thread>
#include <functional>
#include <mutex>

///////////////////////////////////////////////////////////////////////////////////////////////////

/*
* CONSTANT/TYPEDEF MEANINGS
* SHFT - Number of bits to left shift. BITWIDTH - 1 in value.
* SHFM - Number of bits for multiplication. (3=8 bits, 4=16 bits, etc)
* AND  - Number of bits for masking in shifting
* NTH  - Maximum value of the internal type
* UNUM - The mapping to the actual type used (change one place, changes many)
*/
#define BITWIDTH        32

#define SHFT            31
#define SHFM            5
#define AND             0x80000000
#define NTH             0xFFFFFFFF

typedef uint32_t UNUM;  // The internal type is a 'unsigned number'


// Global singleton for number transcribing
class NumberTranscriber
{
    struct cilt  // case insensitive less-than for mixed case map text
    {
        struct compare { bool operator() (const unsigned char& cl, const unsigned char& cr) const { return tolower(cl) < tolower(cr); } };
        bool operator() (const std::string& sl, const std::string& sr) const { return std::lexicographical_compare(sl.begin(), sl.end(), sr.begin(), sr.end(), compare()); }
    };

    NumberTranscriber() { init(); }
    NumberTranscriber(const NumberTranscriber&) = delete;
    NumberTranscriber& operator=(const NumberTranscriber&) = delete;

    static std::unique_ptr<NumberTranscriber> instance;
    static std::mutex mutex;
    static std::map<std::string, std::string, NumberTranscriber::cilt> mapWordTo99;
    static std::map<std::string, std::string, NumberTranscriber::cilt> mapWordTo100;
    static void init();

public:
    static NumberTranscriber& getInstance();
    static std::string ToPhrase(const std::string& number);
    static std::string ToNumber(const std::string& phrase);
    static bool TextEqual(const std::string& s1, const std::string& s2);
};

class Number
{
protected:
    struct DATA
    {
        alignas(8) UNUM U;
        UNUM OF;

        DATA(UNUM u = 0) : U(u), OF(0) {};

        const DATA Add(const DATA& data, const UNUM of) const // Full-Adder
        {
            DATA Out;
            Out.OF = of; // Kerry-In
            for (UNUM ui = 1, uj = 0; ui != 0; ui <<= 1, ++uj)
            {
                Out.U |= (Out.OF ^ (((U & ui) >> uj) ^ ((data.U & ui) >> uj))) << uj;                                                  // SUM:   Kerry-in XOR (A XOR B)
                Out.OF = (((U & ui) >> uj) & Out.OF) | (((U & ui) >> uj) & ((data.U & ui) >> uj)) | (((data.U & ui) >> uj) & Out.OF);  // CARRY: Kerry-out AB OR BC OR ACin
            }
            return Out;
        }

        const DATA Sub(const DATA& data, const UNUM of) const // Full-Subtractor
        {
            DATA Out;
            Out.OF = of; // Borrow-In
            for (UNUM ui = 1, uj = 0; ui != 0; ui <<= 1, ++uj)
            {
                Out.U |= (Out.OF ^ (((U & ui) >> uj) ^ ((data.U & ui) >> uj))) << uj;                                                   // DIFFERENCE: (A XOR B) XOR Borrow-in
                Out.OF = (~((U & ui) >> uj) & Out.OF) | (~((U & ui) >> uj) & ((data.U & ui) >> uj)) | (((data.U & ui) >> uj) & Out.OF); // BORROW: A'Borrow-in OR A'B OR AB (' = 2s complement)
            }
            return Out;
        }

        const bool ispow2() const { return (U > 0) && (U & (U - 1)) == 0; }
    };

public:

    Number() : m_bNeg(false), m_bNAN(true) {};

    Number(const char* pstrNumber) { ToBinary(pstrNumber); }

    Number(const std::string& strNumber) { ToBinary(strNumber); }

    Number(const int32_t iNumber) { Convert(iNumber); }

    Number(DATA ch, size_t size)
    {
        const static DATA _0(0), _255(NTH);

        m_bNeg = (ch.U & AND) >> SHFT ? true : false;
        m_Bytes.resize(size, m_bNeg ? _255 : _0);
        m_Bytes[0] = ch;
        m_bNAN = false;
    }

    Number(const Number& rhs) { *this = rhs; }

    ~Number() {}

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

protected:

    void ToBinary(const std::string& strNumberIn)
    {
        if (strNumberIn.empty())
            throw std::exception();

        std::string strNumber = NumberTranscriber::getInstance().ToNumber(strNumberIn);
        if (strNumber.empty())
            strNumber = strNumberIn;

        m_bNeg = false;
        m_bNAN = false;

        bool bNeg = false;
        if (strNumber[0] == '-')
        {
            if (strNumber.length() < 2)
                throw std::exception();
            bNeg = true;
        }

        std::string strInput = strNumber.substr(bNeg ? 1 : 0);
        if (strInput.empty())
            throw std::exception();

        std::string strOut;
        UNUM idnm = 0;
        UNUM val = 0;
        UNUM pow = 1;
        std::vector<UNUM> vbytes;

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
                    // Byte stream 0-255

                    if (idnm)
                        val += pow;
                    pow <<= 1;
                    if (!pow)
                    {
                        vbytes.push_back(val);
                        val = 0;
                        pow = 1;
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
                /////////////////////////////////////
                // Byte stream 0-255

                if (idnm)
                    val += pow;
                pow <<= 1;
                if (!pow)
                {
                    vbytes.push_back(val);
                    val = 0;
                    pow = 1;
                }

                /////////////////////////////////////

                // Reset and restart (but not the incremental bytes, they carry over)
                strInput = strOut;
                strOut.clear();
                idnm = 0;
                cit = strInput.begin();
            }
        }

        if (val)
            vbytes.push_back(val);

        size_t size = UNUM(vbytes.size());
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

        if (bNeg)
            *this = TwosComplement();

        SetSize(GetSize() + 1);
    }

    // Helper to convert to the internal format
#if BITWIDTH == 32
    void Convert(const int32_t iNumber)
    {
        m_bNAN = false;

        m_Bytes.resize(2);
        m_Bytes[0] = (uint32_t)(iNumber); // 32

        m_bNeg = iNumber < 0;
        m_Bytes[1] = m_bNeg ? NTH : 0;
    }
#elif BITWIDTH == 16
    void Convert(const int32_t iNumber)
    {
        m_bNAN = false;

        m_Bytes.resize(3);
        m_Bytes[0] = ((uint32_t)(iNumber)) & 0x0000FFFF; // 16
        m_Bytes[1] = ((uint32_t)(iNumber) >> 0x10); // 32

        m_bNeg = iNumber < 0;
        m_Bytes[2] = m_bNeg ? NTH : 0;
    }
#elif BITWIDTH == 8
    void Convert(const int32_t iNumber)
    {
        m_bNAN = false;

        m_Bytes.resize(5);
        m_Bytes[0] = (uint32_t)(iNumber) & 0xFF;
        m_Bytes[1] = ((uint32_t)(iNumber) >> 0x08) & 0xFF;
        m_Bytes[2] = ((uint32_t)(iNumber) >> 0x10) & 0xFF;
        m_Bytes[3] = (uint32_t)(iNumber) >> 0x18;

        m_Bytes[4] = (m_bNeg = iNumber < 0) ? NTH : 0;
    }
#endif

public:
    Number Add(const Number& rhs, size_t st = 0) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw std::exception();

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMin = l == r ? l : (l < r ? l : r);
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static DATA Zero(0), Neg1(NTH);
        Number out(Zero, stMax);
        UNUM of = 0;

        for (; st < stMin; ++st)
        {
            if (of == 0 && m_Bytes[st].U == 0 && rhs.m_Bytes[st].U == 0) continue;
            of = (out.m_Bytes[st] = m_Bytes[st].Add(rhs.m_Bytes[st], of), out.m_Bytes[st].OF);
        }

        for (DATA lb, rb; st < stMax; ++st)
        {
            lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (of == 0 && lb.U == 0 && rb.U == 0) continue;
            of = (out.m_Bytes[st] = lb.Add(rb, of), out.m_Bytes[st].OF);
        }

        out.m_bNeg = (out.m_Bytes[out.GetSize() - 1].U & AND) >> SHFT ? true : false; // Shift nbits - 1  to match size of data

        return out;
    }

    Number Sub(const Number& rhs, size_t st = 0) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw std::exception();

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMin = l == r ? l : (l < r ? l : r);
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static DATA Zero(0), Neg1(NTH);
        Number out(Zero, stMax);
        UNUM of = 0;

        for (; st < stMin; ++st)
        {
            if (of == 0 && m_Bytes[st].U == 0 && rhs.m_Bytes[st].U == 0) continue;
            of = (out.m_Bytes[st] = m_Bytes[st].Sub(rhs.m_Bytes[st], of), out.m_Bytes[st].OF);
        }

        for (DATA lb, rb; st < stMax; ++st)
        {
            lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (of == 0 && lb.U == 0 && rb.U == 0) continue;
            of = (out.m_Bytes[st] = lb.Sub(rb, of), out.m_Bytes[st].OF);
        }

        out.m_bNeg = (out.m_Bytes[out.GetSize() - 1].U & AND) >> SHFT ? true : false;

        return out;
    }

    Number Mul(const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw std::exception();

        bool bND = m_Bytes.size() >= rhs.m_Bytes.size();

        size_t stMB = bND ? m_Bytes.size() : rhs.m_Bytes.size();
        Number prod(DATA(0), stMB);
        Number mulp = *this;
        Number mulc = rhs;

        if (bND) // N =>= D
        {
            for (size_t iByte = 0, nBytes = mulc.m_Bytes.size(); iByte < nBytes; ++iByte)
            {
                for (UNUM ui = 1; ui != 0; ui <<= 1)
                {
                    if (ui & mulc.m_Bytes[iByte].U)
                        prod += mulp;
                    mulp.Shl();
                }
            }
        }
        else // D > N
        {
            for (size_t iByte = 0, nBytes = mulp.m_Bytes.size(); iByte < nBytes; ++iByte)
            {
                for (UNUM ui = 1; ui != 0; ui <<= 1)
                {
                    if (ui & mulp.m_Bytes[iByte].U)
                        prod += mulc;
                    mulc.Shl();
                }
            }
        }

        return prod;
    }

    Number Div(const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw std::exception();

        const static Number _0(DATA(0), 1);
        Number quot;
        if (rhs == _0)
            return quot;

        size_t stMB = m_Bytes.size() > rhs.m_Bytes.size() ? m_Bytes.size() : rhs.m_Bytes.size();

        Number rem = *this;
        rem.SetSize(stMB);

        Number rhsin = rhs;
        if (m_bNeg != rhs.m_bNeg)
            rhsin = rhsin.TwosComplement();

        Number dbl = rhsin;
        dbl.SetSize(stMB);

        Number pow(m_bNeg == rhs.m_bNeg ? DATA(1) : DATA(-1), stMB);
        size_t stn = 0;

        size_t dlh = dbl.MSb();
        if (dlh != size_t(-1))
        {
            size_t rlh = rem.MSb();
            if (rlh != size_t(-1))
            {
                if (rlh > dlh)
                {
                    size_t nb = rlh - dlh + 1;
                    dbl.Shl(-1, nb);
                    pow.Shl(-1, nb);
                    stn = nb;
                }
            }
        }

        if (!m_bNeg)
        {
            while (dbl < rem)
            {
                dbl.Shl();
                pow.Shl(stn++);
            }

            quot = _0;
            for (size_t ndbl = stn + 1; ndbl > 0; --ndbl)
            {
                if (dbl > rem)
                {
                    dbl.Shr();
                    pow.Shr(stn--);
                    continue;
                }

                quot = quot.Add(pow, stn >> SHFM);
                rem -= dbl;

                dbl.Shr();
                pow.Shr(stn--);
            }
        }
        else
        {
            while (dbl > rem)
            {
                dbl.Shl();
                pow.Shl(stn++);
            }

            quot = _0;
            for (size_t ndbl = stn + 1; ndbl > 0; --ndbl)
            {
                if (dbl < rem)
                {
                    dbl.Shr();
                    pow.Shr(stn--);
                    continue;
                }

                quot = quot.Add(pow, stn >> SHFM);
                rem -= dbl;

                dbl.Shr();
                pow.Shr(stn--);
            }
        }

        return quot;
    }

    Number Mod(const Number& rhs) const
    {
        if (m_bNAN || rhs.m_bNAN)
            throw std::exception();

        const static Number _0(DATA(0), 1);
        Number quot;
        if (rhs == _0)
            return quot;

        size_t stMB = m_Bytes.size() > rhs.m_Bytes.size() ? m_Bytes.size() : rhs.m_Bytes.size();

        Number rem = *this;
        rem.SetSize(stMB);

        Number rhsin = rhs;
        if (m_bNeg != rhs.m_bNeg)
            rhsin = rhsin.TwosComplement();

        Number dbl = rhsin;
        dbl.SetSize(stMB);

        Number pow(m_bNeg == rhs.m_bNeg ? DATA(1) : DATA(-1), stMB);
        size_t stn = 0;

        size_t dlh = dbl.MSb();
        if (dlh != size_t(-1))
        {
            size_t rlh = rem.MSb();
            if (rlh != size_t(-1))
            {
                if (rlh > dlh)
                {
                    size_t nb = rlh - dlh + 1;
                    dbl.Shl(-1, nb);
                    pow.Shl(-1, nb);
                    stn = nb;
                }
            }
        }

        if (!m_bNeg)
        {
            while (dbl < rem)
            {
                dbl.Shl();
                pow.Shl(stn++);
            }

            quot = _0;
            for (size_t ndbl = stn + 1; ndbl > 0; --ndbl)
            {
                if (dbl > rem)
                {
                    dbl.Shr();
                    pow.Shr(stn--);
                    continue;
                }

                quot = quot.Add(pow, stn >> SHFM);
                rem -= dbl;

                dbl.Shr();
                pow.Shr(stn--);
            }
        }
        else
        {
            while (dbl > rem)
            {
                dbl.Shl();
                pow.Shl(stn++);
            }

            quot = _0;
            for (size_t ndbl = stn + 1; ndbl > 0; --ndbl)
            {
                if (dbl < rem)
                {
                    dbl.Shr();
                    pow.Shr(stn--);
                    continue;
                }

                quot = quot.Add(pow, stn >> SHFM);
                rem -= dbl;

                dbl.Shr();
                pow.Shr(stn--);
            }
        }

        return rem;
    }

    bool Equals(const Number& rhs) const
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
        const static DATA Zero(0), Neg1(NTH);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            DATA lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            DATA rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.U != rb.U)
                return false;
        }

        return true;
    }

    bool LessThan(const Number& rhs) const
    {
        if (this == &rhs)
            return false; // I CANT BE LESS THAN MYSELF!

        if (m_bNAN || rhs.m_bNAN)
            return false;

        if (m_bNeg != rhs.m_bNeg)
            return m_bNeg;

        size_t l = m_Bytes.size(), r = rhs.m_Bytes.size();
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static DATA Zero(0), Neg1(NTH);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            DATA lb = st < l ? m_Bytes[st] : (m_bNeg ? Neg1 : Zero);
            DATA rb = st < r ? rhs.m_Bytes[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.U != rb.U)
                return m_bNeg ? lb.U < rb.U : lb.U < rb.U;
        }

        return false;
    }

    Number TwosComplement() const
    {
        if (m_bNAN)
            throw std::exception();

        size_t size = m_Bytes.size();
        Number Out(DATA(0), size);
        const static Number _1(DATA(1), 1);

        UNUM iByte = 0;
        do
        {
            Out.m_Bytes[iByte].U = ~m_Bytes[iByte].U;
            iByte++;
        } while (iByte != size);

        Out = Out + _1;
        Out.m_bNeg = !m_bNeg;

        return Out;
    }

    // Binary to text based base10 conversion 
    std::string ToDisplay() const
    {
        const static std::string strNAN = "NAN";
        if (m_bNAN)
            return strNAN;

        if (m_bNeg)
        {
            Number TC = TwosComplement();
            return "-" + TC.ToDisplay();
        }

        std::string Num1(1, '1');
        std::string Num2(1, '0');
        std::string Disp(1, '0');

        UNUM iByte = 0;
        UNUM pow = 1;
        do
        {
            UNUM iProd;
            UNUM iCarry;

            if (m_Bytes[iByte].U & pow) // Evaluates to False=0 or True=one of 1,2,4,8,16,32,64,128
            {
                Disp.clear();
                iCarry = 0;

                std::string::const_iterator D1_cend = Num1.end();
                std::string::const_iterator D2_cend = Num2.end();

                for (std::string::const_iterator D1_crit = Num1.begin(), D2_crit = Num2.begin(); D1_crit != D1_cend || D2_crit != D2_cend;)
                {
                    UNUM N1 = (D1_crit != D1_cend ? *D1_crit++ : g_cZero) - g_cZero;
                    UNUM N2 = (D2_crit != D2_cend ? *D2_crit++ : g_cZero) - g_cZero;

                    UNUM iSum = N1 + N2 + iCarry; // adding with carry
                    iCarry = iSum >= 10;
                    if (iCarry)
                        iSum -= 10;

                    Disp.push_back(g_cZero + iSum);
                }

                if (iCarry)
                    Disp.push_back(g_cOne);

                Num2 = Disp;
            }

            iCarry = 0;
            std::string Prod;
            for (std::string::const_iterator dit2 = Num1.begin(); dit2 != Num1.end(); )
            {
                UNUM N1 = *dit2++ - g_cZero;

                iProd = 2 * N1 + iCarry; // doubling with carry
                iCarry = iProd >= 10;
                if (iCarry)
                    iProd -= 10;

                Prod.push_back(g_cZero + iProd); // double value
            }

            if (iCarry)
                Prod.push_back(g_cOne);

            Num1 = Prod;

            if (!(pow <<= 1)) // When doubling overflows to 0
            {
                iByte++;
                pow = 1;
            }
        } while (iByte != m_Bytes.size());

        return std::string(Disp.rbegin(), Disp.rend());
    }

    std::string ToBinary() const
    {
        size_t nbit = m_Bytes.size() * size_t(BITWIDTH) - 1;
        std::string sbin(nbit + 1, '0');
        UNUM iByte = 0;
        UNUM pow = 1;
        do
        {
            if (m_Bytes[iByte].U & pow)
                sbin[nbit] = '1';
            if (!(pow <<= 1))
            {
                iByte++;
                pow = 1;
            }
        } while (--nbit, iByte != m_Bytes.size());
        return sbin;
    }

    std::string ToPhrase() const
    {
        return NumberTranscriber::getInstance().ToPhrase(ToDisplay());
    }

    friend std::ostream& operator << (std::ostream& out, Number& rhs)
    {
        out << rhs.ToDisplay();
        return out;
    }

public:
    void operator = (const int32_t iNumber)
    {
        Convert(iNumber);
    }

    void operator = (const std::string& strNumber)
    {
        ToBinary(strNumber);
    }

    void operator = (const char* pstrNumber)
    {
        ToBinary(pstrNumber);
    }

    bool operator == (const Number& rhs) const
    {
        return Equals(rhs);
    }

    bool operator != (const Number& rhs) const
    {
        return !(operator == (rhs));
    }

    bool operator < (const Number& rhs) const
    {
        return LessThan(rhs);
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

    Number& operator << (const size_t nbits)
    {
        Shl(size_t(-1), nbits);
        return *this;
    }

    Number& operator >> (const size_t nbits)
    {
        Shr(size_t(-1), nbits);
        return *this;
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
        return Mul(rhs);
    }

    Number operator / (const Number& rhs) const
    {
        return Div(rhs);
    }

    Number operator % (const Number& rhs) const
    {
        return Mod(rhs);
    }

    // pre/post increment/decrement
    /*
    The prefix increment/decrement operator (++/--) adds/subs one to its operand and this incremented value is the result of the expression.
    The postfix increment/decrement operator (++/--)(int) adds/subs one to its operand and the previous value is the result of the expression
    */

    Number& operator ++ ()
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw std::exception();

        *this = this->Add(_1);
        return *this;
    }

    Number& operator -- ()
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw std::exception();

        *this = this->Sub(_1);
        return *this;
    }

    const Number operator ++ (int) // By standard, returns the value before arithmetic
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw std::exception();

        Number prev = *this;

        *this = this->Add(_1);

        return prev;
    }

    const Number operator -- (int)  // By standard, returns the value before arithmetic
    {
        const static Number _1(1, 1);

        if (m_bNAN)
            throw std::exception();

        Number prev = *this;

        *this = this->Sub(_1);

        return prev;
    }

    //
    // Comma Operators
    //
    const Number& operator , (const Number& rhs) const
    {
        return rhs;
    }

    Number& operator , (Number& rhs)
    {
        return rhs;
    }

    //
    // Arithmetic operators
    //
    Number& operator += (const Number& rhs)
    {
        *this = this->Add(rhs);
        return *this;
    }

    Number& operator -= (const Number& rhs)
    {
        *this = this->Sub(rhs);
        return *this;
    }

    Number& operator *= (const Number& rhs)
    {
        *this = this->Mul(rhs);
        return *this;
    }

    Number& operator /= (const Number& rhs)
    {
        *this = this->Div(rhs);
        return *this;
    }

    Number& operator %= (const Number& rhs)
    {
        *this = this->Mod(rhs);
        return *this;
    }

    //
    // Helpers
    //
    void SetSize(size_t size)
    {
        if (size != m_Bytes.size())
            m_Bytes.resize(size, m_bNeg ? NTH : 0);
    }

    size_t GetSize() const
    {
        return m_Bytes.size();
    }

protected:

    // Find the low and high bits
    size_t MSb() const
    {
        size_t lhp(size_t(-1));
        size_t lhn(size_t(-1));

        size_t bit = m_Bytes.size() * BITWIDTH - 1;
        size_t iByte = m_Bytes.size() - 1;
        UNUM pow = AND;
        do
        {
            if (!m_bNeg && (m_Bytes[iByte].U & pow))
            {
                if (lhp == size_t(-1))
                {
                    lhp = bit;
                    break;
                }
            }
            else if (m_bNeg && (~m_Bytes[iByte].U & pow))
            {
                if (lhn == size_t(-1))
                {
                    lhn = bit + 1;
                    break;
                }
            }

            if (!(pow >>= 1))
            {
                iByte--;
                pow = AND;
            }
        } while (--bit, iByte != -1);

        return !m_bNeg ? lhp : lhn;
    }

    // Left Bit Shift by n bits <- double value n times
    void Shl(size_t stbit = size_t(-1), size_t nbits = 1)
    {
        size_t stn = 0;
        size_t iByte = m_Bytes.size() - 1;
        if (stbit != size_t(-1))
        {
            iByte = (stbit >> SHFM) + 1;
            if (iByte >= m_Bytes.size())
                iByte = m_Bytes.size() - 1;
            if (iByte)
                stn = iByte - 1;
        }

        if (nbits >= BITWIDTH)
        {
            size_t tmp = iByte;
            size_t sb = nbits / BITWIDTH;
            if (sb)
            {
                for (; iByte != -1; --iByte)
                {
                    if (iByte >= sb)
                    {
                        m_Bytes[iByte].U = m_Bytes[iByte - sb].U;
                        m_Bytes[iByte - sb].U = 0;
                    }
                }
                nbits = nbits % BITWIDTH;
            }
            iByte = tmp;
        }

        for (; iByte != stn; --iByte)
        {
            m_Bytes[iByte].U <<= nbits;
            m_Bytes[iByte].U |= (m_Bytes[iByte - 1].U >> (BITWIDTH - nbits));
        }
        m_Bytes[iByte].U <<= nbits;

        m_bNeg = (m_Bytes[m_Bytes.size() - 1].U & AND) >> SHFT ? true : false;
    }

    // Right Bit Shift by n bits -> halve value n times
    void Shr(size_t stbit = size_t(-1), size_t nbits = 1)
    {
        size_t stn = m_Bytes.size() - 1;
        size_t iByte = 0;
        if (stbit != size_t(-1))
        {
            stn = stbit >> SHFM;
            if (stn >= m_Bytes.size())
                stn = m_Bytes.size() - 1;
            iByte = stn;
            if (iByte)
                --iByte;
        }

        if (nbits >= BITWIDTH)
        {
            size_t tmp = iByte;
            size_t sb = nbits / BITWIDTH;
            if (sb)
            {
                for (; iByte != m_Bytes.size() - 1; ++iByte)
                {
                    if (iByte + sb < m_Bytes.size())
                    {
                        m_Bytes[iByte].U = m_Bytes[iByte + sb].U;
                        m_Bytes[iByte + sb].U = 0;
                    }
                }
                nbits = nbits % BITWIDTH;
            }
            iByte = tmp;
        }

        for (; iByte != stn; ++iByte)
        {
            m_Bytes[iByte].U >>= nbits;
            m_Bytes[iByte].U |= (m_Bytes[iByte + 1].U << (BITWIDTH - nbits));
        }

        m_Bytes[iByte].U >>= nbits;
        if (m_bNeg)
            m_Bytes[iByte].U |= AND;
    }

protected:
    std::vector<DATA> m_Bytes;
    bool m_bNeg;
    bool m_bNAN;
};

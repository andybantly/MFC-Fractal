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
   - Under no circumstances shall Andrew Scott Bantly be liable for any damages, including incidental, special, or consequential damages arising out of or in connection with the use of this software.

4. Violation of Terms :
   - Any unauthorized use, modification, or distribution of this software will result in immediate revocation of any implied permissions and may lead to legal action.

5. Jurisdiction :
   - This license shall be governed by the laws of The United States of America.
*/

#pragma once
#include "Constants.h"
#include <map>
#include <vector>
#include <string>
#include <mutex>

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__x86_64__) || defined(__ppc64__) || defined(_WIN64)
#define IS_64BIT 1
#else
#define IS_32BIT 1
#endif

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

/*
* CONSTANT/TYPEDEF MEANINGS/DEFINE
* SHFT - Number of bits to left shift. BITWIDTH - 1 in value.
* SHFM - Number of bits for multiplication. (3=8 bits, 4=16 bits, 5=32, 6=64, etc)
* AND  - Number of bits for masking in shifting
* NTH  - Maximum value of the internal type
* UNUM - The mapping to the actual type used
* BNUM - The mapping to the type used for bits in 1 UNUM
* IS_64BIT - 64 bit build
* IS_32BIT - 32 bit build
*/

typedef uint8_t BNUM;

#ifdef IS_64BIT

#define BITWIDTH        64

#if     BITWIDTH == 64
#define SHFT            63
#define SHFM            6
#define AND             0x8000000000000000
#define NTH             0xFFFFFFFFFFFFFFFF

// The internal type
typedef uint64_t UNUM;
typedef int64_t SNUM;

const static UNUM _pow[BITWIDTH] = { 0x1,               0x2,               0x4,               0x8,               0x10,               0x20,               0x40,               0x80,    //  8 bits
                                                 0x100,             0x200,             0x400,             0x800,             0x1000,             0x2000,             0x4000,             0x8000,    // 16 bits
                                               0x10000,           0x20000,           0x40000,           0x80000,           0x100000,           0x200000,           0x400000,           0x800000,    // 24 bits
                                             0x1000000,         0x2000000,         0x4000000,         0x8000000,         0x10000000,         0x20000000,         0x40000000,         0x80000000,    // 32 bits
                                           0x100000000,       0x200000000,       0x400000000,       0x800000000,       0x1000000000,       0x2000000000,       0x4000000000,       0x8000000000,    // 40 bits
                                         0x10000000000,     0x20000000000,     0x40000000000,     0x80000000000,     0x100000000000,     0x200000000000,     0x400000000000,     0x800000000000,    // 48 bits
                                       0x1000000000000,   0x2000000000000,   0x4000000000000,   0x8000000000000,   0x10000000000000,   0x20000000000000,   0x40000000000000,   0x80000000000000,    // 56 bits
                                     0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000 };  // 64 bits
#endif
#if     BITWIDTH == 32
#define SHFT            31
#define SHFM            5
#define AND             0x80000000
#define NTH             0xFFFFFFFF

// The internal type
typedef uint32_t UNUM;
typedef int32_t SNUM;

const static UNUM _pow[BITWIDTH] = { 0x1,               0x2,               0x4,               0x8,               0x10,               0x20,               0x40,               0x80,   //  8 bits
                                                 0x100,             0x200,             0x400,             0x800,             0x1000,             0x2000,             0x4000,             0x8000,   // 16 bits
                                               0x10000,           0x20000,           0x40000,           0x80000,           0x100000,           0x200000,           0x400000,           0x800000,   // 24 bits
                                             0x1000000,         0x2000000,         0x4000000,         0x8000000,         0x10000000,         0x20000000,         0x40000000,         0x80000000 }; // 32 bits
#endif
#if     BITWIDTH == 16
#define SHFT            15
#define SHFM            4
#define AND             0x8000
#define NTH             0xFFFF

// The internal type
typedef uint16_t UNUM;
typedef int16_t SNUM;

const static UNUM _pow[BITWIDTH] = { 0x1,               0x2,               0x4,               0x8,               0x10,               0x20,               0x40,               0x80,   //  8 bits
                                                 0x100,             0x200,             0x400,             0x800,             0x1000,             0x2000,             0x4000,             0x8000 }; // 16 bits
#endif
#endif

#ifdef IS_32BIT

#define BITWIDTH        32

#if     BITWIDTH == 32
#define SHFT            31
#define SHFM            5
#define AND             0x80000000
#define NTH             0xFFFFFFFF

// The internal type
typedef uint32_t UNUM;
typedef int32_t SNUM;

const static UNUM _pow[BITWIDTH] = { 0x1,               0x2,               0x4,               0x8,               0x10,               0x20,               0x40,               0x80,   //  8 bits
                                                 0x100,             0x200,             0x400,             0x800,             0x1000,             0x2000,             0x4000,             0x8000,   // 16 bits
                                               0x10000,           0x20000,           0x40000,           0x80000,           0x100000,           0x200000,           0x400000,           0x800000,   // 24 bits
                                             0x1000000,         0x2000000,         0x4000000,         0x8000000,         0x10000000,         0x20000000,         0x40000000,         0x80000000 }; // 32 bits
#endif
#if     BITWIDTH == 16
#define SHFT            15
#define SHFM            4
#define AND             0x8000
#define NTH             0xFFFF

// The internal type
typedef uint16_t UNUM;
typedef int16_t SNUM;

const static UNUM _pow[BITWIDTH] = { 0x1,               0x2,               0x4,               0x8,               0x10,               0x20,               0x40,               0x80,   //  8 bits
                                                 0x100,             0x200,             0x400,             0x800,             0x1000,             0x2000,             0x4000,             0x8000 }; // 16 bits
#endif
#endif

class Number
{
private:
    struct DATA
    {
#ifdef IS_32BIT
        alignas(8)
#endif
#ifdef IS_64BIT
            alignas(64)
#endif
            UNUM U;
        BNUM F;

        DATA(UNUM u = 0, BNUM f = 0) : U(u), F(f) {};

        inline const DATA Add(const DATA& data, const BNUM c) const // Adder with carry
        {
            UNUM u = U + data.U + c;
            return DATA(u, (BNUM)(u < U));
        }

        inline const DATA Sub(const DATA& data, const BNUM b) const // Subtractor with borrow
        {
            return DATA(U - data.U - b, (BNUM)(U < data.U));
        }

#ifdef DEBUG
        /*
        * For simulation purposes
        * FullAdd, an implementation of a full-adder circuit, is functionally equivalent to Add and a text book implementation
        * FullSub, an implementation of a full-subtractor circuit, is functionally equivalent to Sub and a text book implementation
        */
        inline const DATA FullAdd(const DATA& data, const BNUM f) const // Full-Adder
        {
            DATA Out;
            Out.F = f; // Kerry-In
            for (BNUM uj = 0; uj < BITWIDTH; ++uj)
            {
                UNUM ui = _pow[uj];
                Out.U |= (Out.F ^ (((U & ui) >> uj) ^ ((data.U & ui) >> uj))) << uj;                                                                         // SUM:   Kerry-in XOR (A XOR B)
                Out.F = (BNUM)(((U & ui) >> uj) & Out.F) | (BNUM)(((U & ui) >> uj) & (BNUM)((data.U & ui) >> uj)) | (BNUM)(((data.U & ui) >> uj) & Out.F);  // CARRY: Kerry-out AB OR BC OR ACin
            }
            return Out;
        }

        inline const DATA FullSub(const DATA& data, const BNUM f) const // Full-Subtractor
        {
            DATA Out;
            Out.F = f; // Borrow-In
            for (BNUM uj = 0; uj < BITWIDTH; ++uj)
            {
                UNUM ui = _pow[uj];
                Out.U |= (Out.F ^ (((U & ui) >> uj) ^ ((data.U & ui) >> uj))) << uj;                                                                          // DIFFERENCE: (A XOR B) XOR Borrow-in
                Out.F = (BNUM)(~((U & ui) >> uj) & Out.F) | (BNUM)(~((U & ui) >> uj) & (BNUM)((data.U & ui) >> uj)) | (BNUM)(((data.U & ui) >> uj) & Out.F); // BORROW: A'Borrow-in OR A'B OR AB (' = 2s complement)
            }
            return Out;
        }
#endif // DEBUG

        const bool IsPow2() const { return (U > 0) && (U & (U - 1)) == 0; }
    };

public:

    Number() : m_bNeg(false), m_bNan(true), m_bOvf(false) {};

    Number(const char* pnum) { ToBinary(pnum); }

    Number(const std::string& number) { ToBinary(number); }

    // Special helper constructor for twos complement addition
    Number(const int8_t u, size_t st) { Convert(u); SetSize(st); m_bOvf = true; }

    Number(const int32_t u) { Convert(u); }

    Number(const int64_t u) { Convert(u); }

    Number(const Number& rhs) { *this = rhs; }

    ~Number() {}

    Number& operator = (const Number& rhs)
    {
        if (this != &rhs)
        {
            m_Data = rhs.m_Data;
            m_bNeg = rhs.m_bNeg;
            m_bNan = rhs.m_bNan;
            m_bOvf = rhs.m_bOvf;
        }
        return *this;
    }

protected:

    // Helper to convert to the internal format
#if BITWIDTH == 64
    void Convert(const int64_t u)
    {
        m_bNan = false;
        m_bOvf = false;

        m_Data.resize(1);

        m_Data[0] = (UNUM)(u);

        m_bNeg = u < 0;
    }
#elif BITWIDTH == 32
    void Convert(const int32_t u)
    {
        m_bNan = false;
        m_bOvf = false;

        m_Data.resize(1);
        m_Data[0] = (UNUM)(u);

        m_bNeg = u < 0;
    }

    void Convert(const int64_t u)
    {
        m_bNan = false;
        m_bOvf = false;

        m_Data.resize(2);

        m_Data[0] = ((UNUM)(u)) & 0x00000000FFFFFFFF;
        m_Data[1] = ((UNUM)((u) >> 32));

        m_bNeg = u < 0;
    }
#elif BITWIDTH == 16
    void Convert(const int64_t u)
    {
        m_bNan = false;
        m_bOvf = false;
        m_Data.resize(4);

        m_Data[0] = (u) & 0xFFFF;
        m_Data[1] = (u >> 16) & 0xFFFF;
        m_Data[2] = (u >> 32) & 0xFFFF;
        m_Data[3] = (u >> 48) & 0xFFFF;

        m_bNeg = u < 0;
    }

    void Convert(const int32_t u)
    {
        m_bNan = false;
        m_bOvf = false;

        m_Data.resize(2);
        m_Data[0] = ((uint32_t)(u)) & 0x0000FFFF; // 16
        m_Data[1] = ((uint32_t)(u) >> 0x10); // 32

        m_bNeg = u < 0;
    }
#endif

    void ToBinary(const std::string& strNumberIn)
    {
        if (strNumberIn.empty())
            throw std::exception();

        m_Data.clear();

        std::string strNumber = NumberTranscriber::getInstance().ToNumber(strNumberIn);
        if (strNumber.empty())
            strNumber = strNumberIn;

        bool bNeg = false;
        std::string::iterator it = strNumber.begin();
        if (*it == '-')
        {
            bNeg = true;
            if (++it == strNumber.end())
                throw std::exception();
        }

        std::string strOut;
        UNUM idnm = 0;
        UNUM val = 0;
        UNUM pow = 1;

        for (;;)
        {
            // compute the denominator
            idnm = idnm * 10 + *it - '0';
            if (idnm < 2 && it + 1 != strNumber.end())
            {
                // Carry a 0
                if (!strOut.empty())
                    strOut.push_back('0');

                // The denominator has to be greater than 2 now
                idnm = idnm * 10 + (*(it + 1) - '0');
                it += 2;
            }
            else
            {
                // done?
                if (idnm < 2 && strNumber.length() == 1)
                {
                    if (idnm)
                        val += pow;
                    pow <<= 1;
                    if (!pow)
                    {
                        m_Data.push_back(val);
                        val = 0;
                        pow = 1;
                    }
                    break;
                }
                it++;
            }

            // append to the new string after division
            strOut.push_back('0' + (unsigned)(idnm >> 1));
            idnm %= 2;

            // done?
            if (it == strNumber.end())
            {
                if (idnm)
                    val += pow;
                pow <<= 1;
                if (!pow)
                {
                    m_Data.push_back(val);
                    val = 0;
                    pow = 1;
                }

                // start processing over
                strNumber = strOut;
                strOut.clear();
                idnm = 0;
                it = strNumber.begin();
            }
        }

        if (val)
            m_Data.push_back(val);

        if (!m_Data.size())
        {
            m_Data.resize(1);
            m_Data[0].U = 0;
        }

        m_bNeg = false;
        m_bNan = false;
        m_bOvf = false;

        if (!bNeg && (m_Data[m_Data.size() - 1].U & AND) >> SHFT ? true : false)
            SetSize(m_Data.size() + 1);

        if (bNeg)
            *this = TwosComplement();
    }

    const size_t IsPow2() const
    {
        size_t stcnt = 0;
        size_t stpos = 0;

        for (size_t st = 0; st < m_Data.size() && stcnt < 2; ++st)
        {
            if (m_Data[st].IsPow2())
            {
                stpos = st;
                stcnt++;
            }
        }

        size_t stbpos = 0;
        if (stcnt == 1)
        {
            for (BNUM uj = 0; uj < BITWIDTH; ++uj)
            {
                UNUM ui = _pow[uj];
                if (m_Data[stpos].U & ui)
                {
                    stbpos = BITWIDTH * stpos + uj;
                    break;
                }
            }

            return stbpos;
        }

        return -1;
    }

public:
    Number Add(const Number& rhs, size_t st = 0) const
    {
        if (m_bNan || rhs.m_bNan)
            throw std::exception();

        size_t l = m_Data.size(), r = rhs.m_Data.size();
        size_t stMin = l == r ? l : (l < r ? l : r);
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static DATA Zero(0), Neg1(NTH);
        Number out(0); out.SetSize(stMax);
        BNUM c = 0;

        for (; st < stMin; ++st)
        {
            if (c == 0 && m_Data[st].U == 0 && rhs.m_Data[st].U == 0) continue;
            c = (out.m_Data[st] = m_Data[st].Add(rhs.m_Data[st], c), out.m_Data[st].F);
        }

        for (DATA lb, rb; st < stMax; ++st) // This loop occurs for mixed Number sizes
        {
            lb = st < l ? m_Data[st] : (m_bNeg ? Neg1 : Zero);
            rb = st < r ? rhs.m_Data[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (c == 0 && lb.U == 0 && rb.U == 0) continue;
            c = (out.m_Data[st] = lb.Add(rb, c), out.m_Data[st].F);
        }

        out.m_bNeg = (out.m_Data[out.GetSize() - 1].U & AND) >> SHFT;

        if (!rhs.IsOverFlow())
        {
            out.m_bOvf = ((m_bNeg == rhs.m_bNeg) && (m_bNeg != out.m_bNeg));
            if (out.m_bOvf)
            {
                out.m_Data.push_back(0);
                out.m_bNeg = false;
            }
        }

        return out;
    }

    Number Sub(const Number& rhs, size_t st = 0) const
    {
        if (m_bNan || rhs.m_bNan)
            throw std::exception();

        size_t l = m_Data.size(), r = rhs.m_Data.size();
        size_t stMin = l == r ? l : (l < r ? l : r);
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static DATA Zero(0), Neg1(NTH);
        Number out(0); out.SetSize(stMax);
        BNUM b = 0;

        for (; st < stMin; ++st)
        {
            if (b == 0 && m_Data[st].U == 0 && rhs.m_Data[st].U == 0) continue;
            b = (out.m_Data[st] = m_Data[st].Sub(rhs.m_Data[st], b), out.m_Data[st].F);
        }

        for (DATA lb, rb; st < stMax; ++st) // This loop occurs for mixed Number sizes
        {
            lb = st < l ? m_Data[st] : (m_bNeg ? Neg1 : Zero);
            rb = st < r ? rhs.m_Data[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (b == 0 && lb.U == 0 && rb.U == 0) continue;
            b = (out.m_Data[st] = lb.Sub(rb, b), out.m_Data[st].F);
        }

        out.m_bNeg = (out.m_Data[out.GetSize() - 1].U & AND) >> SHFT;
        if (!rhs.IsOverFlow())
        {
            out.m_bOvf = ((m_bNeg != rhs.m_bNeg) && (m_bNeg != out.m_bNeg));
            if (out.m_bOvf)
            {
                out.m_Data.push_back(m_bNeg ? NTH : 0);
                out.m_bNeg = m_bNeg;
            }
        }

        return out;
    }

    Number Mul(const Number& rhs) const
    {
        if (m_bNan || rhs.m_bNan)
            throw std::exception();

        Number prod(0);
        if (*this == prod || rhs == prod)
            return prod;

        Number This = m_bNeg ? this->TwosComplement() : *this;
        Number That = rhs.m_bNeg ? rhs.TwosComplement() : rhs;

        size_t l1 = This.Count1();
        size_t r1 = That.Count1();

        if (l1 == 1 || r1 == 1)
        {
            size_t stnb = l1 == 1 ? This.IsPow2() : That.IsPow2();
            prod = l1 == 1 ? That : This;
            prod.Shl(-1, stnb);

            if (m_bNeg != rhs.m_bNeg)
                prod = prod.TwosComplement();

            return prod;
        }

        bool bND = l1 > r1;

        size_t stMB = bND ? This.m_Data.size() : That.m_Data.size();
        prod.SetSize(stMB);
        Number mulc = That;

        if (bND) // N1s > D1s
        {
            Number mulp = This;
            for (size_t data = 0, ndata = mulc.m_Data.size(); data < ndata; ++data)
            {
                for (UNUM ui = 1; ui != 0; ui <<= 1)
                {
                    if (ui & mulc.m_Data[data].U)
                        prod += mulp;
                    mulp.Shl();
                }
            }
        }
        else // D1s >= N1s
        {
            for (size_t data = 0, ndata = This.m_Data.size(); data < ndata; ++data)
            {
                for (UNUM ui = 1; ui != 0; ui <<= 1)
                {
                    if (ui & This.m_Data[data].U)
                        prod += mulc;
                    mulc.Shl();
                }
            }
        }

        if (m_bNeg != rhs.m_bNeg)
            prod = prod.TwosComplement();

        return prod;
    }

    Number Div(const Number& rhs) const
    {
        if (m_bNan || rhs.m_bNan)
            throw std::exception();

        Number quot(0);
        if (rhs == quot)
            return Number();
        if (*this == quot)
            return quot;

        size_t stMB = m_Data.size() > rhs.m_Data.size() ? m_Data.size() : rhs.m_Data.size();

        Number rem = *this;
        rem.SetSize(stMB);

        Number dbl = rhs;
        if (m_bNeg != rhs.m_bNeg)
            dbl = dbl.TwosComplement();

        size_t stbpos = dbl.IsPow2();
        if (stbpos != -1)
        {
            if (stbpos)
                rem >>= stbpos;
            if (rem == quot)
                return rem;
            if (m_bNeg != rhs.m_bNeg)
                rem = rem.TwosComplement();
            return rem;
        }
        dbl.SetSize(stMB);

        Number pow(m_bNeg == rhs.m_bNeg ? 1 : -1); pow.SetSize(stMB);
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

        while (!m_bNeg ? dbl < rem : dbl > rem)
        {
            dbl.Shl();
            pow.Shl(stn++);
        }

        for (size_t ndbl = stn + 1; ndbl > 0; --ndbl)
        {
            if (!m_bNeg ? dbl > rem : dbl < rem)
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

        return quot;
    }

    Number Mod(const Number& rhs) const
    {
        if (m_bNan || rhs.m_bNan)
            throw std::exception();

        const static Number _0(0);
        Number rem;
        if (rhs == _0)
            return rem;

        size_t stMB = m_Data.size() > rhs.m_Data.size() ? m_Data.size() : rhs.m_Data.size();

        rem = *this;
        rem.SetSize(stMB);

        Number rhsin = rhs;
        if (m_bNeg != rhs.m_bNeg)
            rhsin = rhsin.TwosComplement();

        Number dbl = rhsin;
        dbl.SetSize(stMB);

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
                    stn = nb;
                }
            }
        }

        while (!m_bNeg ? dbl < rem : dbl > rem) { dbl.Shl(); ++stn; }

        for (size_t ndbl = stn + 1; ndbl > 0; --ndbl)
        {
            if (!m_bNeg ? dbl > rem : dbl < rem)
            {
                dbl.Shr();
                continue;
            }
            rem -= dbl;
            dbl.Shr();
        }

        return rem;
    }

    Number Sqrt() const
    {
        if (m_bNan)
            throw std::exception();

        if (m_bNeg)
            return TwosComplement().Sqrt();

        const static Number _0(0);
        const static Number _1(1);
        const static Number _2(2);

        Number low = _0, mid, high = *this, sqrt = _0;
        Number msq;
        while (low <= high)
        {
            mid = low + ((high - low) >> 1);
            msq = mid * mid;
            if (msq == *this)
                return mid;
            if (msq < *this)
            {
                low = mid + _1;
                sqrt = mid;
            }
            else
                high = mid - _1;
        }
        return sqrt;
    }

    Number Factorial() const
    {
        if (m_bNan)
            throw std::exception();

        Number fact;
        if (m_bNeg)
        {
            fact = TwosComplement().Factorial();
            return fact.TwosComplement();
        }

        const static Number _1(1);
        const static Number _0(0);

        fact = *this;
        if (fact == _1 || fact == _0)
            return fact;

        for (Number mul = fact - _1; mul != _1; --mul)
            fact *= mul;

        return fact;
    }

    bool Equals(const Number& rhs) const
    {
        if (this == &rhs) // I AM ALWAYS EQUAL TOO MYSELF!
            return true;

        if (m_bNan || rhs.m_bNan)
            return false;

        if (m_bNeg != rhs.m_bNeg)
            return false;

        size_t l = m_Data.size(), r = rhs.m_Data.size();
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static DATA Zero(0), Neg1(NTH);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            DATA lb = st < l ? m_Data[st] : (m_bNeg ? Neg1 : Zero);
            DATA rb = st < r ? rhs.m_Data[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.U != rb.U)
                return false;
        }

        return true;
    }

    bool LessThan(const Number& rhs) const
    {
        if (this == &rhs)
            return false; // I CANT BE LESS THAN MYSELF!

        if (m_bNan || rhs.m_bNan)
            return false;

        if (m_bNeg != rhs.m_bNeg)
            return m_bNeg;

        size_t l = m_Data.size(), r = rhs.m_Data.size();
        size_t stMax = l == r ? l : (l < r ? r : l);
        const static DATA Zero(0), Neg1(NTH);
        for (size_t st = stMax - 1; st != size_t(-1); --st)
        {
            DATA lb = st < l ? m_Data[st] : (m_bNeg ? Neg1 : Zero);
            DATA rb = st < r ? rhs.m_Data[st] : (rhs.m_bNeg ? Neg1 : Zero);
            if (lb.U != rb.U)
                return m_bNeg ? lb.U < rb.U : lb.U < rb.U;
        }

        return false;
    }

    Number TwosComplement() const
    {
        if (m_bNan)
            throw std::exception();

        size_t size = m_Data.size();
        Number Out(0); Out.SetSize(size);
        const static Number _1(1, 1);

        Out = BitNot();
        Out += _1;
        Out.m_bNeg = !m_bNeg;

        return Out;
    }

    // Binary to text based base10 conversion 
    std::string ToDisplay() const
    {
        const static std::string strNAN = "NAN";
        if (m_bNan)
            return strNAN;

        if (m_bNeg)
        {
            Number TC = TwosComplement();
            return "-" + TC.ToDisplay();
        }

        std::string Num1(1, '1');
        std::string Num2(1, '0');
        std::string Disp(1, '0');

        UNUM data = 0;
        UNUM pow = 1;
        do
        {
            UNUM iProd;
            UNUM iCarry;
            if (m_Data[data].U & pow) // Evaluates to False=0 or True=one of 1,2,4,8,16,32,64,128
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

                    Disp.push_back(g_cZero + (unsigned)iSum);
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

                Prod.push_back(g_cZero + (unsigned)iProd); // double value
            }

            if (iCarry)
                Prod.push_back(g_cOne);

            Num1 = Prod;

            if (!(pow <<= 1)) // When doubling overflows to 0
            {
                data++;
                pow = 1;
            }
        } while (data != m_Data.size());

        return std::string(Disp.rbegin(), Disp.rend());
    }

    size_t Count1() const
    {
        size_t stcnt = 0;
        for (size_t st = 0; st < m_Data.size(); ++st)
        {
            for (BNUM uj = 0; uj < BITWIDTH; ++uj)
            {
                UNUM ui = _pow[uj];
                if (m_Data[st].U & ui)
                    ++stcnt;
            }
        }
        return stcnt;
    }

    Number BitNot() const
    {
        size_t size = m_Data.size();
        Number Out(0); Out.SetSize(size);

        for (UNUM data = 0; data != size; ++data) { Out.m_Data[data].U = ~m_Data[data].U; }

        return Out;
    }

    std::string ToBinary() const
    {
        size_t nbit = m_Data.size() * size_t(BITWIDTH) - 1;
        std::string sbin(nbit + 1, '0');
        UNUM data = 0;
        UNUM pow = 1;
        do
        {
            if (m_Data[data].U & pow)
                sbin[nbit] = '1';
            if (!(pow <<= 1))
            {
                data++;
                pow = 1;
            }
        } while (--nbit, data != m_Data.size());
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

protected:

    // Find the MSB
    size_t MSb() const
    {
        size_t lhp(size_t(-1));
        size_t lhn(size_t(-1));

        size_t bit = m_Data.size() * BITWIDTH - 1;
        size_t data = m_Data.size() - 1;
        UNUM pow = AND;
        do
        {
            if (!m_bNeg && (m_Data[data].U & pow))
            {
                if (lhp == size_t(-1))
                {
                    lhp = bit;
                    break;
                }
            }
            else if (m_bNeg && (~m_Data[data].U & pow))
            {
                if (lhn == size_t(-1))
                {
                    lhn = bit + 1;
                    break;
                }
            }

            if (!(pow >>= 1))
            {
                --data;
                pow = AND;
            }
        } while (--bit, data != -1);

        return !m_bNeg ? lhp : lhn;
    }

    // Find the LSB
    size_t LSb() const
    {
        size_t lhp(-1);
        size_t lhn(-1);

        size_t bit = 0;
        size_t data = 0;
        UNUM pow = 1;
        do
        {
            if (!m_bNeg && (m_Data[data].U & pow))
            {
                if (lhp == size_t(-1))
                {
                    lhp = bit;
                    break;
                }
            }
            else if (m_bNeg && (~m_Data[data].U & pow))
            {
                if (lhn == size_t(-1))
                {
                    lhn = bit + 1;
                    break;
                }
            }

            if (!(pow <<= 1))
            {
                ++data;
                pow = 1;
            }
        } while (++bit, data < m_Data.size());

        return !m_bNeg ? lhp : lhn;
    }

    // Left Bit Shift by n bits <- double value n times
    void Shl(size_t stbit = size_t(-1), size_t nbits = 1)
    {
        const static UNUM Zero(0), Neg1(NTH);

        size_t stn = 0;
        size_t data = m_Data.size() - 1;
        if (stbit != size_t(-1))
        {
            data = (stbit >> SHFM) + 1;
            if (data >= m_Data.size())
                data = m_Data.size() - 1;
            if (data)
                stn = data - 1;
        }

        if (nbits >= BITWIDTH)
        {
            size_t tmp = data;
            size_t sb = nbits / BITWIDTH;
            if (sb >= m_Data.size())
            {
                SetSize(m_Data.size() + sb);
                return Shl(stbit, nbits);
            }

            for (; data != -1; --data)
            {
                if (data >= sb)
                {
                    m_Data[data].U = m_Data[data - sb].U;
                    m_Data[data - sb].U = 0;
                }
            }
            nbits = nbits % BITWIDTH;
            data = tmp;
        }

        if (nbits)
        {
            for (; data != stn; --data)
            {
                m_Data[data].U <<= nbits;
                m_Data[data].U |= (m_Data[data - 1].U >> (BITWIDTH - nbits));
            }
            m_Data[data].U <<= nbits;
        }

        bool bNeg = (m_Data[m_Data.size() - 1].U & AND) >> SHFT ? true : false;
        if (m_bNeg != bNeg)
        {
            // shift caused sign change
            if (!m_bNeg)
                SetSize(m_Data.size() + 1);
        }
    }

    // Right Bit Shift by n bits -> halve value n times
    void Shr(size_t stbit = size_t(-1), size_t nbits = 1)
    {
        const static UNUM Zero(0), Neg1(NTH);

        size_t stn = m_Data.size() - 1;
        size_t data = 0;
        if (stbit != size_t(-1))
        {
            stn = stbit >> SHFM;
            if (stn >= m_Data.size())
                stn = m_Data.size() - 1;
            data = stn;
            if (data)
                --data;
        }

        if (nbits >= BITWIDTH)
        {
            size_t tmp = data;
            size_t sb = nbits / BITWIDTH;
            if (sb)
            {
                for (; data != m_Data.size() - 1; ++data)
                {
                    if (data + sb < m_Data.size())
                    {
                        m_Data[data].U = m_Data[data + sb].U;
                        m_Data[data + sb].U = 0;
                    }
                    else
                        m_Data[data].U = (m_bNeg ? Neg1 : Zero);
                }
                nbits = nbits % BITWIDTH;
            }
            data = tmp;
        }

        for (; data != stn; ++data)
        {
            m_Data[data].U >>= nbits;
            m_Data[data].U |= (m_Data[data + 1].U << (BITWIDTH - nbits));
        }

        m_Data[data].U >>= nbits;
        if (m_bNeg)
            m_Data[data].U |= AND;
    }

public:
    void operator = (const int64_t u)
    {
        Convert(u);
    }

    void operator = (const int32_t u)
    {
        Convert(u);
    }

    void operator = (const std::string& number)
    {
        ToBinary(number);
    }

    void operator = (const char* pnumber)
    {
        ToBinary(pnumber);
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

    Number operator << (const size_t nbits) const
    {
        Number out = *this;
        out <<= nbits;
        return out;
    }

    Number operator >> (const size_t nbits) const
    {
        Number out = *this;
        out >>= nbits;
        return out;
    }

    Number operator ~ () const
    {
        return BitNot();
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
        const static Number _1(1);

        if (m_bNan)
            throw std::exception();

        *this = this->Add(_1);
        return *this;
    }

    Number& operator -- ()
    {
        const static Number _1(1);

        if (m_bNan)
            throw std::exception();

        *this = this->Sub(_1);
        return *this;
    }

    const Number operator ++ (int) // By standard, returns the value before arithmetic
    {
        const static Number _1(1);

        if (m_bNan)
            throw std::exception();

        Number prev = *this;

        *this = this->Add(_1);

        return prev;
    }

    const Number operator -- (int)  // By standard, returns the value before arithmetic
    {
        const static Number _1(1);

        if (m_bNan)
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

    Number& operator >>= (const size_t nbits)
    {
        Shr(size_t(-1), nbits);
        return *this;
    }

    Number& operator <<= (const size_t nbits)
    {
        Shl(size_t(-1), nbits);
        return *this;
    }

    //
    // Helpers
    //
    void SetSize(size_t size)
    {
        if (size != m_Data.size())
            m_Data.resize(size, m_bNeg ? NTH : 0);
    }

    size_t GetSize() const
    {
        return m_Data.size();
    }

public: // helpers
    const bool IsOverFlow() const { return m_bOvf; }
    const bool IsNegative() const { return m_bNeg; }
    const bool IsNan() const { return m_bNan; }

protected:
    std::vector<DATA> m_Data;
    bool m_bNeg;
    bool m_bNan;
    bool m_bOvf;
};

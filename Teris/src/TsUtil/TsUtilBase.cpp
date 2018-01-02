#include "TsUtilBase.h"
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#include <algorithm>
using namespace std;

#pragma warning(disable:4554)
#pragma warning(disable:4552)
#pragma warning(disable:4806)


CRandom::CRandom()
:   rseed(1),
    mti(CMATH_N + 1)
{}

unsigned int CRandom::Random(unsigned int n)
{
    unsigned long y;
    static unsigned long mag01[2] = {0x0, CMATH_MATRIX_A};

    if (n == 0)
    {
        return 0;
    }

    if (mti >= CMATH_N)
    {
        int kk;

        if (mti == CMATH_N + 1)
        {
            SetRandomSeed(4357);
        }

        for (kk = 0; kk < CMATH_N - CMATH_M; kk++)
        {
            y = (mt[kk]&CMATH_UPPER_MASK)|(mt[kk+1]&CMATH_LOWER_MASK);
            mt[kk] = mt[kk+CMATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        for (; kk < CMATH_N - 1; kk++)
        {
            y = (mt[kk]&CMATH_UPPER_MASK)|(mt[kk+1]&CMATH_LOWER_MASK);
            mt[kk] = mt[kk+(CMATH_M - CMATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        y = (mt[CMATH_N - 1]&CMATH_UPPER_MASK)|(mt[0]&CMATH_LOWER_MASK);
        mt[CMATH_N - 1] = mt[CMATH_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti = 0;
    }

    y = mt[mti++];
    y ^= CMATH_TEMPERING_SHIFT_U(y);
    y ^= CMATH_TEMPERING_SHIFT_S(y) & CMATH_TEMPERING_MASK_B;
    y ^= CMATH_TEMPERING_SHIFT_T(y) & CMATH_TEMPERING_MASK_C;
    y ^= CMATH_TEMPERING_SHIFT_L(y);

    return (y % n);
}

void CRandom::SetRandomSeed(unsigned int n)
{
    mt[0] = n & 0xffffffff;
    for (mti = 1; mti < CMATH_N; mti++)
    {
        mt[mti] = (69069 * mt[mti - 1]) & 0xffffffff;
    }
    rseed = n;
}

unsigned int CRandom::GetRandomSeed()
{
    return rseed;
}

void CRandom::Randomize()
{
    SetRandomSeed(time(NULL));
}

// from 0 to N (exclude N)
CPrimeSearch::CPrimeSearch(unsigned int elements)
{
	assert(elements > 0);

	m_arBuffer.reserve(elements);
	for(int i=0; i<elements; ++i)
	{
		m_arBuffer.push_back(i);
	}

	Restart();
}

//from nStart to nEnd (include nStart and nEnd)
CPrimeSearch::CPrimeSearch(unsigned int nStart, unsigned int nEnd)
{
	int nCount = nEnd - nStart + 1;
	assert(nCount > 0);
	
	m_arBuffer.reserve(nCount);
	for(int i=nStart; i<=nEnd; ++i)
	{
		m_arBuffer.push_back(i);
	}

	Restart();
}

int CPrimeSearch::GetNext(bool restart/* =false */)
{
	int nRet(-1);

	if(restart && m_nCurrentIndex != -1)
	{
		Restart();
	}
	
	int nIndex = m_nCurrentIndex + 1;
	if(nIndex < GetCount())
	{
		m_nCurrentIndex = nIndex;
		nRet = m_arBuffer[m_nCurrentIndex];
	}

	return nRet;
}

bool CPrimeSearch::Done()
{
	return	m_nCurrentIndex >= GetCount();
}

void CPrimeSearch::Restart()
{
	const int VEC_MAX_COUNT = 1024; // if the count is too big, the performance is poor
	assert(GetCount() > 0 && GetCount() < VEC_MAX_COUNT);

	random_shuffle(m_arBuffer.begin(), m_arBuffer.end());

	m_nCurrentIndex = -1;
}
int CPrimeSearch::GetCount()
{
	return m_arBuffer.size();
}
#include "MTRandom.h"

//========================================================================
// CRandom.cpp -  a useful random number generator class
//
// "Mersenne Twister pseudorandom number generator" 
// Original Code written by Takuji Nishimura and Makoto Matsumoto
//
//========================================================================

#include <ctime>

MTRandom::MTRandom(void) :
	rseed(1), 
	mti(m_N + 1)
{
}

float MTRandom::RandomF( float f )
{
	uint32_t rnd = Random(static_cast<uint32_t>(f));
	return static_cast<float>(rnd);
}

// Returns a number from 0 to n (excluding n)
uint32_t MTRandom::Random( uint32_t n )
{
	unsigned long y;
	static unsigned long mag01[2]= { 0x0, m_matrixA };

	if (n == 0)
		return(0);

	// mag01[x] = x * MATRIX_A  for x=0,1

	// generate N words at one time
	if (mti >= m_N) 
	{ 
		int kk;

		if (mti == m_N + 1)			// if SetRandomSeed() has not been called,
			SetRandomSeed(4357);	// a default initial seed is used

		for (kk=0; kk < m_N - m_M; kk++) 
		{
			y = (mt[kk] & m_upperMask) | (mt[kk+1] & m_lowerMask);
			mt[kk] = mt[kk+m_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}

		for (;kk < m_N - 1; kk++) 
		{
			y = (mt[kk] & m_upperMask) | (mt[kk+1] & m_lowerMask);
			mt[kk] = mt[kk + (m_M - m_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}

		y = (mt[m_N - 1] & m_upperMask) | (mt[0] & m_lowerMask);
		mt[m_N - 1] = mt[m_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		mti = 0;
	}

	y = mt[mti++];
	y ^= TemperingShiftU(y);
	y ^= TemperingShiftS(y) & m_temperingMaskB;
	y ^= TemperingShiftT(y) & m_temperingMaskC;
	y ^= TemperingShiftL(y);

	return (y % n);
}


// Returns a random float between 0.0f and 1.0f
float MTRandom::Random( )
{
	const auto r = static_cast<float>(Random(INT32_MAX));
	const auto divisor = static_cast<float>(INT32_MAX);
	return (r / divisor) + 0.5f;
}


void MTRandom::SetRandomSeed(unsigned int n)
{
	// setting initial seeds to mt[N] using
	// the generator Line 25 of Table 1 in
	// [KNUTH 1981, The Art of Computer Programming
	//   Vol. 2 (2nd Ed.), pp102]
	mt[0]= n & 0xffffffff;
	for (mti = 1; mti < m_N; mti++)
		mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;

	rseed = n;
}

unsigned int MTRandom::GetRandomSeed(void) const
{
	return(rseed);
}

void MTRandom::Randomize(void)
{
	SetRandomSeed(static_cast<unsigned int>(time(nullptr)));
}

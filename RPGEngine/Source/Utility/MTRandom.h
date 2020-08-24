#pragma once
#include <cstdint>

class MTRandom
{
public:
	MTRandom(void);

	[[nodiscard]] float RandomF(float f);
	[[nodiscard]] uint32_t Random(uint32_t n);
	[[nodiscard]] float Random();
	[[nodiscard]] unsigned int GetRandomSeed() const;
	void SetRandomSeed(unsigned int n);
	void Randomize();

private:
	// Period parameters
	static const int m_N = 624;
	static const int m_M = 397;
	static const unsigned int m_matrixA = 0x9908b0df;	 // Constant vector A
	static const unsigned int m_upperMask = 0x80000000; // Most significant w-r bits
	static const unsigned int m_lowerMask = 0x7fffffff; // Least significant r bits

	// Tempering parameters
	static const unsigned int m_temperingMaskB = 0x9d2c5680;
	static const unsigned int m_temperingMaskC = 0xefc60000;

	static unsigned long TemperingShiftU(unsigned long y) { return (y >> 11); }
	static unsigned long TemperingShiftS(unsigned long y) { return (y << 7); }
	static unsigned long TemperingShiftT(unsigned long y) { return (y << 15); }
	static unsigned long TemperingShiftL(unsigned long y) { return (y >> 18); }

	unsigned int rseed;
	unsigned long mt[m_N]{};			// the array for the state vector
	int mti;						// mti == N+1 means mt[N] is not initialized
};

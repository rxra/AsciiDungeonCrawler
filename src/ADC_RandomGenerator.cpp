
#include "ADC_RandomGenerator.h"
#include <random>

using namespace std;

namespace ADC
{

	// it is ok because we have no thread 
	// but there is to have one random generator per thread.
	mt19937 g_TheGameRNG;
	uniform_int_distribution<uint32_t> g_Dist4(0, 3);

	void RandomGenerator::Initialize()
	{
		random_device rd;
		g_TheGameRNG.seed(static_cast<unsigned long>(rd()));
	}

	World::Direction RandomGenerator::RandomDirection()
	{
		return static_cast<World::Direction>(g_Dist4(g_TheGameRNG));
	}

	World::Position RandomGenerator::RandomPosition(size_t width, size_t height)
	{
		uniform_int_distribution<size_t> g_DistW(0, width - 1);
		uniform_int_distribution<size_t> g_DistH(0, height - 1);

		return { g_DistW(g_TheGameRNG), g_DistH(g_TheGameRNG) };
	}
}
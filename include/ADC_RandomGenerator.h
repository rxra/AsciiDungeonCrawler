#if !defined(ADC_RANDOMGENERATOR_H)
#define ADC_RANDOMGENERATOR_H

#include "ADC_World.h"

namespace ADC
{

	class RandomGenerator
	{
	public:

		static void Initialize();

		static World::Direction RandomDirection();

		static World::Position RandomPosition(size_t width, size_t height);

	};
}

#endif // ADC_RANDOMGENERATOR_H

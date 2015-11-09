#if !defined(ADC_CANBEUPDATED_H)
#define ADC_CANBEUPDATED_H

#include <string>
#include <set>
#include <vector>
#include <memory>

namespace ADC
{

	class CanBeUpdated
	{
	public:

		CanBeUpdated(bool updated)
			: _hasBeenUpdated(updated)
		{

		}

		bool hasBeenUpdated(bool clear) const
		{
			bool value = _hasBeenUpdated;
			if (clear && _hasBeenUpdated) {
				_hasBeenUpdated = false;
			}
			return value;
		}

	protected:

		mutable bool _hasBeenUpdated;

	};

}

#endif // ADC_CANBEUPDATED_H
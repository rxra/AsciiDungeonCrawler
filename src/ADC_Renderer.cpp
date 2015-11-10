
#include "ADC_Renderer.h"
#include "ADC_RendererConsole.h"

using namespace std;

namespace ADC
{

	std::shared_ptr<Renderer> Renderer::CreateRendererConsole(const shared_ptr<World>& world, size_t width, size_t height)
	{
		auto renderer = std::make_shared<RendererConsole>(world);
		if (!renderer->initialize(width, height))
		{
			return shared_ptr<Renderer>();
		}

		return renderer;
	}

}
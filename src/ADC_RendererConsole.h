#if !defined(ADC_RENDERERCONSOLE_H)
#define ADC_RENDERERCONSOLE_H

#include "ADC_World.h"
#include "ADC_Renderer.h"

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <windows.h>

namespace ADC
{

	class RendererConsole : public Renderer
	{
	public:

		RendererConsole(const std::shared_ptr<World>& world);

		virtual ~RendererConsole();

		bool initialize(size_t width = 0, size_t height = 0);

		bool updateInput();
		void render();
		void updateGUI(const Player& player);
		void playerDead();

	private:

		void _updateFromWorld();

		const std::shared_ptr<World>& _world;
		size_t _width;
		size_t _height;
		HANDLE _outputHandle;
		HANDLE _inputHandle;
		std::vector<CHAR_INFO> _map;
		std::vector<INPUT_RECORD> _events;

		////////////////////////////////////////////////////////////////////////////////
		// private constructor/operator not impletemend to be sure we cannot call them.
		RendererConsole(const RendererConsole&) = delete;
		RendererConsole(const RendererConsole&&) = delete;
		RendererConsole& operator=(const RendererConsole&) = delete;
		RendererConsole& operator=(const RendererConsole&&) = delete;
		////////////////////////////////////////////////////////////////////////////////

	};

}

#endif // ADC_RENDERERCONSOLE_H
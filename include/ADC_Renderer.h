#if !defined(ADC_RENDERER_H)
#define ADC_RENDERER_H

#include <memory>
#include <functional>
#include <vector>

namespace ADC
{

	// forward declaration just to avoid including ADC_World.h
	class World;
	// forward declaration just to avoid including ADC_Player.h
	class Player;

	class Renderer
	{
	public:

		typedef std::function<void()> OnKeyListener;

		static std::shared_ptr<Renderer> CreateRendererConsole(const std::shared_ptr<World>& world, size_t width = 0, size_t height = 0);

		Renderer() {}

		virtual ~Renderer() {}

		virtual bool updateInput() = 0;

		virtual void render(bool checkWorld) = 0;

		virtual void updateReference() = 0;

		virtual void updateGUI(const Player& player) = 0;

		virtual void playerDead() = 0;

		virtual void playerWin() = 0;

		void addOnQuitListener(OnKeyListener onQuit);
		void addOnRestartListener(OnKeyListener onRestart);
		void addOnMoveUpListener(OnKeyListener onUp);
		void addOnMoveDownListener(OnKeyListener onDown);
		void addOnMoveLeftListener(OnKeyListener onLeft);
		void addOnMoveRightListener(OnKeyListener onRight);

	protected:

		std::vector<OnKeyListener> _onQuitListeners;
		std::vector<OnKeyListener> _onRestartListeners;
		std::vector<OnKeyListener> _onMoveUpListener;
		std::vector<OnKeyListener> _onMoveDownListener;
		std::vector<OnKeyListener> _onMoveLeftListener;
		std::vector<OnKeyListener> _onMoveRightListener;

	private:

		////////////////////////////////////////////////////////////////////////////////
		// private constructor/operator not impletemend to be sure we cannot call them.
		Renderer(const Renderer&) = delete;
		Renderer(const Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(const Renderer&&) = delete;
		////////////////////////////////////////////////////////////////////////////////

	};

}


inline void ADC::Renderer::addOnQuitListener(OnKeyListener onQuit)
{
	_onQuitListeners.push_back(onQuit);
}

inline void ADC::Renderer::addOnRestartListener(OnKeyListener onRestart)
{
	_onRestartListeners.push_back(onRestart);
}

inline void ADC::Renderer::addOnMoveUpListener(OnKeyListener onUp)
{
	_onMoveUpListener.push_back(onUp);
}

inline void ADC::Renderer::addOnMoveDownListener(OnKeyListener onDown)
{
	_onMoveDownListener.push_back(onDown);
}

inline void ADC::Renderer::addOnMoveLeftListener(OnKeyListener onLeft)
{
	_onMoveLeftListener.push_back(onLeft);
}

inline void ADC::Renderer::addOnMoveRightListener(OnKeyListener onRight)
{
	_onMoveRightListener.push_back(onRight);
}

#endif // ADC_RENDERER_H
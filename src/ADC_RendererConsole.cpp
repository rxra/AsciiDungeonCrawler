
#include "ADC_RendererConsole.h"
#include "ADC_Player.h"
#include <sstream>
#include <iomanip>

using namespace std;

namespace ADC
{

	void SetFontProperties(int fontX, int fontY, wchar_t *fontName);

	RendererConsole::RendererConsole(const shared_ptr<World>& world)
		: _referenceX(0), _referenceY(0), _world(world), _width(0), _height(0), _outputHandle(NULL), _inputHandle(NULL)
	{
	}

	RendererConsole::~RendererConsole()
	{
	}

	bool RendererConsole::initialize(size_t width, size_t height)
	{
		// initiliaze renderer width/height
		_width = (width == 0) ? _world->getWidth() : width;
		_height = (height == 0) ? _world->getHeight() : height;

		if (_world->getWidth() < _width)
		{
			_width = _world->getWidth();
		}

		if (_world->getHeight() < _height)
		{
			_height = _world->getHeight();
		}

		// add a line for the GUI
		_height++;

		// Used to center the window on the user's screen
		int x = GetSystemMetrics(SM_CXSCREEN) / 2;
		int y = GetSystemMetrics(SM_CYSCREEN) / 2;

		/* Window size coordinates, be sure to start index at zero! */
		SMALL_RECT windowSize = { 0, 0, _width - 1, _height - 1 };

		/* A COORD struct for specificying the console's screen buffer dimensions */
		COORD bufferSize = { _width, _height };

		CONSOLE_CURSOR_INFO cursorInfo = { 1, 0 };

		/* initialize handles */
		_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		_inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		HWND WINDOW_HANDLE = GetConsoleWindow();

		/* Set the console's title */
		SetConsoleTitle("title");

		/* Set cursor info */
		SetConsoleCursorInfo(_outputHandle, &cursorInfo);

		/* Set the console font settings and palette */
		SetFontProperties(8, 8, L"");
		//SetConsolePalette(palette, fontx, fonty, L"");

		/* Set the window size */
		SetConsoleWindowInfo(_outputHandle, TRUE, &windowSize);

		/* Set the screen's buffer size */
		SetConsoleScreenBufferSize(_outputHandle, bufferSize);

		/* Set the window size */
		SetConsoleWindowInfo(_outputHandle, TRUE, &windowSize);

		/* Set the window position */
		//SetWindowPos(WINDOW_HANDLE, HWND_TOP, (x - ((_width / 2) * fontx)) - 5, 0/*(y - ((_HEIGHT / 2) * fonty)) - 5*/, 0, 0, SWP_NOSIZE);
		//SetWindowPos(WINDOW_HANDLE, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);

		_map.resize(_width *  _height);

		return true;
	}

	bool RendererConsole::updateInput()
	{
		DWORD numEvents = 0;
		GetNumberOfConsoleInputEvents(_inputHandle, &numEvents);

		if (numEvents == 0)
		{
			return false;
		}

		_events.clear();
		_events.resize(numEvents);

		DWORD numEventsRead = 0;
		ReadConsoleInput(_inputHandle, &_events[0], numEvents, &numEventsRead);

		for (DWORD i = 0; i < numEventsRead; i++)
		{
			switch (_events[i].EventType)
			{
			case KEY_EVENT:
				switch (_events[i].Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_ESCAPE:
					if (!_onQuitListeners.empty())
					{
						for (auto listener : _onQuitListeners)
						{
							listener();
						}
						return true;
					}
					break;

				case VK_RETURN:
					if (!_onRestartListeners.empty())
					{
						for (auto listener : _onRestartListeners)
						{
							listener();
						}
						return true;
					}
					break;

				case VK_LEFT:
					if (!_onMoveLeftListener.empty())
					{
						for (auto listener : _onMoveLeftListener)
						{
							listener();
						}
						return true;
					}
					break;

				case VK_RIGHT:
					if (!_onMoveRightListener.empty())
					{
						for (auto listener : _onMoveRightListener)
						{
							listener();
						}
						return true;
					}
					break;

				case VK_DOWN:
					if (!_onMoveDownListener.empty())
					{
						for (auto listener : _onMoveDownListener)
						{
							listener();
						}
						return true;
					}
					break;

				case VK_UP:
					if (!_onMoveUpListener.empty())
					{
						for (auto listener : _onMoveUpListener)
						{
							listener();
						}
						return true;
					}
					break;
				}
			}
		}

		return false;
	}

	void RendererConsole::render(bool checkWorld)
	{
		if (checkWorld && _world->hasBeenUpdated(true)) {
			_updateFromWorld();
		}

		COORD characterBufferSize = { _width, _height };
		COORD characterPosition = { 0, 0 };
		SMALL_RECT consoleWriteArea = { 0, 0, _width - 1, _height - 1 };

		WriteConsoleOutputA(_outputHandle, &_map[0], characterBufferSize, characterPosition, &consoleWriteArea);
	}

	void RendererConsole::updateGUI(const Player& player)
	{
		stringstream stream;
		stream << "coins:" << std::setfill('0') << std::setw(2) << player.getCoins();
		stream << " - pickAxes:" << std::setfill('0') << std::setw(2) << player.getPickAxes();
		stream << " - swords:" << std::setfill('0') << std::setw(2) << player.getSwords();
		stream << " - keys:" << std::setfill('0') << std::setw(2) << player.getKeyBronze() << '-' << std::setfill('0') << std::setw(2) << player.getKeyIron() << '-' << std::setfill('0') << std::setw(2) << player.getKeyAll();
		stream << "- lives:" << std::setfill('0') << std::setw(2) << player.getLives();

		size_t x = 0;
		for (auto c : stream.str())
		{
			_map[x].Char.AsciiChar = c;
			_map[x++].Attributes =
				BACKGROUND_GREEN |
				BACKGROUND_INTENSITY;
		}

		for (; x < _width; x++)
		{
			_map[x].Char.AsciiChar = ' ';
			_map[x].Attributes =
				BACKGROUND_GREEN |
				BACKGROUND_INTENSITY;
		}
	}

	void RendererConsole::playerDead()
	{
		for (size_t x = 0; x < _width; x++)
		{
			for (size_t y = 1; y < _height; y++)
			{
				_map[x + y * _width].Char.AsciiChar = ' ';
				_map[x + y * _width].Attributes =
					BACKGROUND_RED |
					BACKGROUND_INTENSITY;
			}
		}

	}

	void RendererConsole::playerWin()
	{
		for (size_t x = 0; x < _width; x++)
		{
			for (size_t y = 1; y < _height; y++)
			{
				_map[x + y * _width].Char.AsciiChar = ' ';
				_map[x + y * _width].Attributes =
					BACKGROUND_BLUE |
					BACKGROUND_INTENSITY;
			}
		}

	}

	void RendererConsole::updateReference()
	{
		int rx = _world->getPlayer()->getPosition().x - (static_cast<int>(_width / 2.f));
		int ry = _world->getPlayer()->getPosition().y - (static_cast<int>((_height-1) / 2.f));

		if (rx < 0)
		{
			_referenceX = 0;
		}
		else if (static_cast<size_t>(rx) > (_world->getWidth() - _width))
		{
			_referenceX = _world->getWidth() - _width;
		}
		else
		{
			_referenceX = rx;
		}

		if (ry < 0)
		{
			_referenceY = 0;
		}
		else if (static_cast<size_t>(ry) > (_world->getHeight() - (_height-1)))
		{
			_referenceY = _world->getHeight() - (_height-1);
		}
		else
		{
			_referenceY = ry;
		}
	}

	void RendererConsole::_updateFromWorld()
	{
		for (size_t x = 0; x < _width; x++)
		{
			for (size_t y = 1; y < _height; y++)
			{
				_map[x + y * _width].Char.AsciiChar = static_cast<char>(_world->getCell(x + _referenceX, y -1 + _referenceY));
				_map[x + y * _width].Attributes =
					BACKGROUND_BLUE |
					BACKGROUND_GREEN |
					BACKGROUND_RED |
					BACKGROUND_INTENSITY;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	// Helper function to access console properties
	//
	//////////////////////////////////////////////////////////////////////////////

	typedef BOOL(WINAPI *PGetCurrentConsoleFontEx)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
	typedef BOOL(WINAPI *PSetCurrentConsoleFontEx)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);

#define WM_SETCONSOLEINFO (WM_USER+201) 
#pragma pack(push, 1) 
	typedef struct _CONSOLE_INFO2
	{
		ULONG Length;
		COORD ScreenBufferSize;
		COORD WindowSize;
		ULONG WindowPosX;
		ULONG WindowPosY;
		COORD FontSize;
		ULONG FontFamily;
		ULONG FontWeight;
		WCHAR FaceName[32];
		ULONG CursorSize;
		ULONG FullScreen;
		ULONG QuickEdit;
		ULONG AutoPosition;
		ULONG InsertMode;
		USHORT ScreenColors;
		USHORT PopupColors;
		ULONG HistoryNoDup;
		ULONG HistoryBufferSize;
		ULONG NumberOfHistoryBuffers;
		COLORREF ColorTable[16];
		ULONG CodePage;
		HWND Hwnd;
		WCHAR ConsoleTitle[0x100];
	} CONSOLE_INFO2;
#pragma pack(pop) 

	// GetConsoleSizeInfo (XP only)
	// Fills up some info about the console font in the CONSOLE_INFO struct.
	static void GetConsoleSizeInfo(CONSOLE_INFO2 *pci, HANDLE hOutput)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(hOutput, &csbi);
		pci->ScreenBufferSize = csbi.dwSize;
		pci->WindowSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		pci->WindowSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		pci->WindowPosX = csbi.srWindow.Left;
		pci->WindowPosY = csbi.srWindow.Top;
	}

	// SetConsoleInfo (XP only)
	// Ends up sending a message to windows to reset the console info.
	BOOL SetConsoleInfo(HWND hwndConsole, CONSOLE_INFO2 *pci)
	{
		DWORD dwConsoleOwnerPid;
		HANDLE hProcess;
		HANDLE hSection, hDupSection;
		PVOID ptrView = 0;
		HANDLE hThread;

		/* Open the process which "owns" the console */
		GetWindowThreadProcessId(hwndConsole, &dwConsoleOwnerPid);
		hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwConsoleOwnerPid);


		/* Create a SECTION object backed by page-file, then map a view of  */
		/* this section into the owner process so we can write the contents  */
		/* of the CONSOLE_INFO buffer into it  */
		hSection = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, pci->Length, 0);


		/* Copy our console structure into the section-object */
		ptrView = MapViewOfFile(hSection, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, pci->Length);
		memcpy(ptrView, pci, pci->Length);
		UnmapViewOfFile(ptrView);


		/* Map the memory into owner process  */
		DuplicateHandle(GetCurrentProcess(), hSection, hProcess, &hDupSection, 0, FALSE, DUPLICATE_SAME_ACCESS);

		/* Send console window the "update" message WM_SETCONSOLEINFO */
		SendMessage(hwndConsole, WM_SETCONSOLEINFO, (WPARAM)hDupSection, 0);

		/*clean up */
		hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)CloseHandle, hDupSection, 0, 0);
		CloseHandle(hThread);
		CloseHandle(hSection);
		CloseHandle(hProcess);
		return TRUE;
	}

	// Sets the console font properties.
	void SetFontProperties(int fontX, int fontY, wchar_t *fontName)
	{
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		/* Retrieving pointers for Windows Vista/7 Functions */
		PGetCurrentConsoleFontEx pGetCurrentConsoleFontEx = (PGetCurrentConsoleFontEx)
			GetProcAddress(GetModuleHandle("kernel32.dll"), "GetCurrentConsoleFontEx");
		PSetCurrentConsoleFontEx pSetCurrentConsoleFontEx = (PSetCurrentConsoleFontEx)
			GetProcAddress(GetModuleHandle("kernel32.dll"), "SetCurrentConsoleFontEx");

		/* Check for pointers: if they exist, we have Vista/7 and we can use them */
		if (pGetCurrentConsoleFontEx && pSetCurrentConsoleFontEx)
		{
			CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };

			/* Tell the font info how big it is, to avoid memory corruption */
			cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
			pGetCurrentConsoleFontEx(hOutput, TRUE, &cfi);

			/* Set the font type to name indicated by wide string literal */
			/* Set to 0 to keep current settings */
			lstrcpyW(cfi.FaceName, fontName);
			cfi.dwFontSize.X = fontX;
			cfi.dwFontSize.Y = fontY;
			cfi.FontFamily = 0; /* Set to 0x30 for Terminal font */
			cfi.FontWeight = 0;

			/* Set the console font info */
			pSetCurrentConsoleFontEx(hOutput, TRUE, &cfi);
		}
		else  /* We don't have access to the vista functions */
		{
			CONSOLE_INFO2 ci = { sizeof(ci) };
			HWND hwndConsole = GetConsoleWindow();

			GetConsoleSizeInfo(&ci, hOutput);

			/* Set to 0 to keep current settings */
			ci.FontSize.X = fontX;
			ci.FontSize.Y = fontY;
			ci.FontFamily = 0; /* Set to 0x30 for Terminal font */
			ci.FontWeight = 0;

			lstrcpyW(ci.FaceName, fontName);
			ci.CursorSize = 100;
			ci.FullScreen = FALSE;
			ci.QuickEdit = FALSE;
			ci.AutoPosition = 0x10000;
			ci.InsertMode = TRUE;
			ci.ScreenColors = MAKEWORD(0x7, 0x0);
			ci.PopupColors = MAKEWORD(0x5, 0xf);
			ci.HistoryNoDup = TRUE;
			ci.HistoryBufferSize = 50;
			ci.NumberOfHistoryBuffers = 4;

			ci.CodePage = 0;
			ci.Hwnd = hwndConsole;
			lstrcpyW(ci.ConsoleTitle, L"");

			SetConsoleInfo(hwndConsole, &ci);
		}
	}

} // namespace ADC

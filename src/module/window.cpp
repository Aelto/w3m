#include <std_include.hpp>
#include "window.hpp"
#include "scheduler.hpp"
#include "loader/component_loader.hpp"

namespace window
{
	namespace
	{
		BOOL __stdcall enum_windows_proc(const HWND window, const LPARAM param)
		{
			DWORD process = 0;
			GetWindowThreadProcessId(window, &process);

			if (process == GetCurrentProcessId())
			{
				char class_name[500] = {0};
				GetClassNameA(window, class_name, sizeof(class_name));

				if (class_name == "W2ViewportClass"s)
				{
					*reinterpret_cast<HWND*>(param) = window;
				}
			}

			return TRUE;
		}

		class component final : public component_interface
		{
		public:
			void post_load() override
			{
				scheduler::frame([]()
				{
					if (const auto game_window = get_game_window())
					{
						SetWindowTextA(game_window, "Witcher 3: Online");
						return scheduler::cond_end;
					}

					return scheduler::cond_continue;
				});
			}
		};
	}

	HWND get_game_window()
	{
		static HWND window = nullptr;
		if (!window || !IsWindow(window))
		{
			EnumWindows(enum_windows_proc, LPARAM(&window));
		}

		return window;
	}
}

REGISTER_COMPONENT(window::component)

#pragma once

#include <wx/wx.h>
#include "App/EditorApp.h"

namespace gbe {
	class Editor {
	private:

	public:
		Editor();
		void Run(int& argc, char* argv[]);
	};
}
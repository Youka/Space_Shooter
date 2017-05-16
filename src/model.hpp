#pragma once

namespace Model{
	struct Dim2{unsigned x, y;};

	struct GameState{
		// Metainformation
		struct {bool alive : 4, changed : 4;} status;
		enum class Window{MENU, GAME} window;
		// Windows
		enum class Menu{START, EXIT} menu;
		struct{} game;
	};
}
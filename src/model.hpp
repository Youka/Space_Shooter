#pragma once

#include <vector>

namespace Model{
	template<typename T>
	struct Dim2{T x, y;};
	
	using Dim2u = Dim2<unsigned>;

	struct GameState{
		// Metainformation
		struct {bool alive : 4, changed : 4;} status;
		enum class Window{MENU, GAME} window;
		// Windows
		enum class Menu{START, EXIT} menu;
		struct Game{
			enum class Status{RUN, WON, LOST} status;
			Dim2u player;
			std::vector<Dim2u> enemies, player_bullets, enemy_bullets;
		} game;
	};
}
#pragma once

#include <forward_list>

namespace Model{
	template<typename T>
	struct Dim2{T x, y;};
	
	using Dim2u = Dim2<unsigned>;
	using Dim2i = Dim2<int>;

	struct GameState{
		// Metainformation
		struct {bool alive : 4, changed : 4;} status;
		enum class Window{MENU, GAME} window;
		// Windows
		enum class Menu{START, EXIT} menu;
		struct Game{
			enum class Status{RUN, WON, LOST} status;
			unsigned time_ms;
			Dim2i player;
			std::forward_list<Dim2i> enemies, player_bullets, enemy_bullets;
		} game;
	};
}
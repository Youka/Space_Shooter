#include "controller.hpp"

#include <vector>
#include <random>

namespace Controller{
	void event(Model::GameState& state, const View::TUI& tui) noexcept{
		// Extract current key code
		const int key_code = tui.getKey();
		// Context-dependend actions
		switch(state.window){
			case Model::GameState::Window::MENU:
				if(key_code == 27 /* <ESC> */)
					state.status.alive = false;
				else
					switch(state.menu){
						case Model::GameState::Menu::START:
							switch(key_code){
								case KEY_DOWN:
								case KEY_UP:
									state.menu = Model::GameState::Menu::EXIT;
									state.status.changed = true;
									break;
								case ' ':
									state.window = Model::GameState::Window::GAME;
									state.status.changed = true;
									break;
							}
							break;
						case Model::GameState::Menu::EXIT:
							switch(key_code){
								case KEY_DOWN:
								case KEY_UP:
									state.menu = Model::GameState::Menu::START;
									state.status.changed = true;
									break;
								case ' ':
									state.status.alive = false;
									break;
							}
							break;
					}
				break;
			case Model::GameState::Window::GAME:
				// Bullet movement
				
				// TODO
				
				// Enemy movement
				
				// TODO
				
				// Enemies & entities generation
				
				// TODO
				
				// Control actions
				switch(key_code){
					case 27: /* <ESC> */
						state.window = Model::GameState::Window::MENU;
						state.status.changed = true;
						break;
					case KEY_DOWN:
						if(state.game.player.y < tui.getMaxSize().y - 2){
							state.game.player.y++;
							state.status.changed = true;
						}
						break;
					case KEY_UP:
						if(state.game.player.y > 1){
							state.game.player.y--;
							state.status.changed = true;
						}
						break;
					case KEY_LEFT:
						if(state.game.player.x > 1){
							state.game.player.x--;
							state.status.changed = true;
						}
						break;
					case KEY_RIGHT:
						if(state.game.player.x < tui.getMaxSize().x - 2){
							state.game.player.x++;
							state.status.changed = true;
						}
						break;
					case ' ':
						state.game.bullets.push_back({state.game.player.x + 1, state.game.player.y});
						state.status.changed = true;
						break;
				}
				break;
		}
	}

	void draw(Model::GameState& state, const View::TUI& tui) noexcept{
		// Any change to render?
		if(state.status.changed){
			// Clear screen first
			tui.clear();
			// Draw menu background (once)
			static std::vector<unsigned short> stars_pos;
			const auto sz = tui.getMaxSize();
			if(stars_pos.size() != sz.x){
				stars_pos.resize(sz.x);
				std::random_device rd;
				std::uniform_int_distribution<int> rdist(0, sz.y-1);
				for(unsigned short x = 0; x < sz.x; x++)
					stars_pos[x] = rdist(rd);
			}
			for(unsigned short x = 0; x < stars_pos.size(); x++){
				tui.move(x, stars_pos[x]);
				tui.addChar('.');
			}
			// Context-dependend actions
			switch(state.window){
				case Model::GameState::Window::MENU:
				{
					// Draw menu points
					static const std::string game_title("S  P  A  C  E     S  H  O  O  T  E  R"),
										start_title("Start"),
										exit_title("Exit"),
										creator_title("by Youka");
					tui.move((sz.x - 1 - game_title.length()) >> 1, ((sz.y - 1) >> 1) - 3);
					tui.addString(game_title, A_BOLD);
					tui.move((sz.x - 1 - start_title.length()) >> 1, (sz.y - 1) >> 1);
					tui.addString(start_title, state.menu == Model::GameState::Menu::START ? A_BOLD : 0);
					tui.move((sz.x - 1 - exit_title.length()) >> 1, ((sz.y - 1) >> 1) + 2);
					tui.addString(exit_title, state.menu == Model::GameState::Menu::EXIT ? A_BOLD : 0);
					tui.move(sz.x - creator_title.length(), sz.y-1);
					tui.addString(creator_title, COLOR_RED, COLOR_BLACK, A_BOLD);
					break;
				}
				case Model::GameState::Window::GAME:
				{
					// Draw border
					const auto sz = tui.getMaxSize();
					const std::string hline = std::string(sz.x, '#');
					tui.move(0, 0);
					tui.addString(hline);
					tui.move(0, sz.y - 1);
					tui.addString(hline);
					for(unsigned short y = 0; y < sz.y; y++){
						tui.move(0, y);
						tui.addChar('#');
						tui.move(sz.x - 1, y);
						tui.addChar('#');
					}
					// Draw player ship
					tui.move(state.game.player.x, state.game.player.y);
					tui.addChar('}', A_BOLD);
					// Draw enemies
					for(const Model::Dim2 enemy : state.game.enemies){
						tui.move(enemy.x, enemy.y);
						tui.addChar('<', A_BOLD);
					}
					// Draw bullets
					for(const Model::Dim2 bullet : state.game.bullets){
						tui.move(bullet.x, bullet.y);
						tui.addChar('-', A_BOLD);
					}
					break;
				}
			}
			// Draw changes to screen
			tui.draw();
			// Changes rendered, no further
			state.status.changed = false;
		}
	}
}
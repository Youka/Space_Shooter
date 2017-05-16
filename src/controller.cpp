#include "controller.hpp"
#include "view.hpp"
#include "conf.h"

#include <iostream>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <random>

// Current time shortcut
#define NOW std::chrono::system_clock::now()

namespace Controller{
	static void event(Model::GameState& state, const View::TUI& tui){
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
				switch(key_code){
					case 27: /* <ESC> */
						state.window = Model::GameState::Window::MENU;
						state.status.changed = true;
						break;
					case KEY_DOWN:
						if(state.game.player_y < static_cast<short>(tui.getMaxSize().y - 2)){
							state.game.player_y++;
							state.status.changed = true;
						}
						break;
					case KEY_UP:
						if(state.game.player_y > 1){
							state.game.player_y--;
							state.status.changed = true;
						}
						break;
					case KEY_LEFT:
						if(state.game.player_x > 1){
							state.game.player_x--;
							state.status.changed = true;
						}
						break;
					case KEY_RIGHT:
						if(state.game.player_x < static_cast<short>(tui.getMaxSize().x - 2)){
							state.game.player_x++;
							state.status.changed = true;
						}
						break;
					case ' ':
						
						// TODO
						
						break;
				}
				break;
		}
	}
	static void draw(Model::GameState& state, const View::TUI& tui){
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
					tui.move(state.game.player_x, state.game.player_y);
					tui.addChar('}', A_BOLD);
				
					// TODO
					
					break;
				}
			}
			// Draw changes to screen
			tui.draw();
			// Changes rendered, no further
			state.status.changed = false;
		}
	}
	static void life(Model::GameState& state, const View::TUI& tui){
		// Circle status
		auto event_last_time = NOW,
			draw_last_time = event_last_time;
		// Circling for updates
		do{
			// Time to process an event?
			if(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - event_last_time).count() >= EVENT_DELAY_MS){
				event(state, tui);
				event_last_time = NOW;
			}
			// Time to draw something?
			if(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - draw_last_time).count() >= DRAW_DELAY_MS){
				draw(state, tui);
				draw_last_time = NOW;
			}
			// Skip idle time / set scheduler for better performance
			usleep(std::max(static_cast<typename std::chrono::milliseconds::rep>(0), std::min(
					EVENT_DELAY_MS - std::chrono::duration_cast<std::chrono::milliseconds>(NOW - event_last_time).count(),
					DRAW_DELAY_MS - std::chrono::duration_cast<std::chrono::milliseconds>(NOW - draw_last_time).count()
				)) * 1000 /* MS to US */);
		}while(state.status.alive);
	}

	void run(const std::set<std::string>& params){
		// Show command line help
		if(params.find("-help") != params.cend()){
			std::cout <<
			"Options:\n"
			"-help\t\tShows this information\n"
			"-invert\t\tInverts screen colors"
			<< std::endl;
			return;
		}
		
		// Create TUI
		const View::TUI tui = View::TUI::create();
		// Create game state
		Model::GameState state = {
			{true, true},
			Model::GameState::Window::MENU,
			Model::GameState::Menu::START,
			{2, static_cast<short>((tui.getMaxSize().y - 1) >> 1)}
		};

		// Change TUI colors
		if(params.find("-invert") != params.cend())
			tui.setBK(COLOR_BLACK, COLOR_WHITE);

		// Start application life circle
		life(state, tui);
	}
}
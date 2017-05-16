#include "controller.hpp"
#include "view.hpp"
#include "conf.h"

#include <iostream>
#include <chrono>
#include <unistd.h>

// Current time shortcut
#define NOW std::chrono::system_clock::now()

namespace Controller{
	static void event(Model::GameState& state, const int key_code){
		// Global close
		if(key_code == 27 /* 27 = <ESC> */)
			state.status.alive = false;
		// Context-dependend actions
		switch(state.window){
			case Model::GameState::Window::MENU:
				break;
			case Model::GameState::Window::GAME:
				break;
		}
	}
	static void draw(Model::GameState& state, const View::TUI& tui){
		// Any change to render?
		if(state.status.changed){
			// Context-dependend actions
			switch(state.window){
				case Model::GameState::Window::MENU:
					break;
				case Model::GameState::Window::GAME:
					break;
			}
			// Changes rendered, no further
			state.status.changed = false;
		}
		/*
		// Get current screen size/maximal cursor positions
		Model::Dim2 dim = tui.getMaxSize();
		
		// Clear old screen
		tui.clear();

		// Output screen size
		tui.moveCursor(5, 2);
		tui.addString(std::to_string(dim.width));
		tui.moveCursor(5, 3);
		tui.addString(std::to_string(dim.height));

		// Output runtime
		static int i = 0;
		tui.moveCursor(5+i, 4);
		tui.addString(std::to_string(i++));
		
		// Draw outputs
		tui.draw();
		*/
	}
	static void life(Model::GameState& state, const View::TUI& tui){
		// Circle status
		auto event_last_time = NOW,
			draw_last_time = event_last_time;
		// Circling for updates
		do{
			// Time to process an event?
			if(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - event_last_time).count() >= EVENT_DELAY_MS){
				event(state, tui.getKey());
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
			{}
		};
		
		// Change TUI colors
		if(params.find("-invert") != params.cend())
			tui.setBK(COLOR_BLACK, COLOR_WHITE);

		// Start application life circle
		life(state, tui);
	}
}
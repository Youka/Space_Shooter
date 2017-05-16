#include "controller.hpp"
#include "conf.h"

#include <iostream>
#include <chrono>
#include <unistd.h>

// Current time shortcut
#define NOW std::chrono::system_clock::now()

namespace Controller{
	static void life(Model::GameState& state, const View::TUI& tui){
		// Circle status
		auto event_last_time = NOW,
			draw_last_time = event_last_time;
		// Circling for updates
		do{
			// Time to process an event?
			if(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - event_last_time).count() >= EVENT_DELAY_MS){
				Controller::event(state, tui);
				event_last_time = NOW;
			}
			// Time to draw something?
			if(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - draw_last_time).count() >= DRAW_DELAY_MS){
				Controller::draw(state, tui);
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
			{
				true, true
			},
			Model::GameState::Window::MENU,
			Model::GameState::Menu::START,
			{
				{2, (tui.getMaxSize().y - 1) >> 1},
				{}, {}
			}
		};

		// Change TUI colors
		if(params.find("-invert") != params.cend())
			tui.setBK(COLOR_BLACK, COLOR_WHITE);

		// Start application life circle
		life(state, tui);
	}
}
#include "controller.hpp"
#include "conf.h"

#include <iostream>
#include <chrono>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(ms) usleep(ms * 1000)
#endif

// Current time shortcut
#define NOW std::chrono::system_clock::now()

namespace Controller{
	static void life(Model::GameState& state, View::TUI& tui){
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
			const auto rest_delay_ms = std::max(static_cast<typename std::chrono::milliseconds::rep>(0), std::min(
					EVENT_DELAY_MS - std::chrono::duration_cast<std::chrono::milliseconds>(NOW - event_last_time).count(),
					DRAW_DELAY_MS - std::chrono::duration_cast<std::chrono::milliseconds>(NOW - draw_last_time).count()
				));
			Sleep(rest_delay_ms);
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
		View::TUI tui = View::TUI::create();
		// Create game state
		Model::GameState state = {
			{
				true, true
			},
			Model::GameState::Window::MENU,
			Model::GameState::Menu::START,
			{
				Model::GameState::Game::Status::RUN,
				0, 0,
				{2, static_cast<int>((tui.getMaxSize().y - 1) >> 1)}
			}
		};

		// Change TUI colors
		if(params.find("-invert") != params.cend())
			tui.setBK(COLOR_BLACK, COLOR_WHITE);

		// Start application life circle
		life(state, tui);
	}
}
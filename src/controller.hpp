#pragma once

#include <set>
#include <string>

#include "view.hpp"

namespace Controller{
	// Run controller sequence
	void run(const std::set<std::string>& params);
	// Event handlers (used by 'run')
	void event(Model::GameState& state, const View::TUI& tui) noexcept;
	void draw(Model::GameState& state, View::TUI& tui) noexcept;
}
#include "view.hpp"

#include <exception>

namespace View{
	TUI::TUI() : win(initscr(), [](const WINDOW* stdwin) -> void{if(stdwin) endwin();}){
		// Check terminal setup
		if(!this->win)
			throw std::runtime_error("Couldn't setup TUI!");
		// Set terminal properties
		if(noecho() /* Don't display user input*/ ||
		   raw() /* Take special keys too/disable line buffering */ ||
		   keypad(stdscr, true) /* Enable special keys (F1-12, arrows, etc.) */ ||
		   curs_set(0) /* Hide cursor */ > 2)
			throw std::domain_error("Couldn't set TUI properties properly!");
		timeout(0); /* Disable blocking on input requests */
	}

	TUI::TUI(TUI&& other) noexcept : win(std::move(other.win)){}
	TUI& TUI::operator=(TUI&& other) noexcept{
		this->win = std::move(other.win);
		return *this;
	}

	TUI TUI::create(void){
		if(stdscr)
			throw std::domain_error("Just one TUI instance is allowed to exist!");
		return TUI();
	}

	Model::Dim2 TUI::getMaxSize(void) const noexcept{
		unsigned char width, height;
		getmaxyx(stdscr, height, width);
		return {width, height};
	}
	Model::Dim2 TUI::getPos(void) const noexcept{
		unsigned char x, y;
		getyx(stdscr, y, x);
		return {x, y};
	}
	int TUI::getKey(void) const noexcept{
		return getch();
	}
	void TUI::setBK(const short fcolor, const short bcolor) const{
		if(!has_colors())
			throw std::domain_error("TUI doesn't support color change!");
		start_color();
		init_pair(1, fcolor, bcolor);
		bkgd(COLOR_PAIR(1));
	}
	void TUI::move(const int x, const int y) const noexcept{
		::move(y, x);
	}

	void TUI::addString(const std::string& str, const int attr) const noexcept{
		attron(attr);
		addstr(str.c_str());
		attroff(attr);
	}
	void TUI::addString(const std::string& str) const noexcept{
		addstr(str.c_str());
	}
	void TUI::clear(void) const noexcept{
		erase();
	}
	void TUI::draw(void) const noexcept{
		refresh();
	}
}
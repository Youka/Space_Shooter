#pragma once

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#include <memory>

#include "model.hpp"

namespace View{
	// Terminal UI
	class TUI{
		private:
			// NCurses terminal window storage
			std::unique_ptr<WINDOW,void(*)(const WINDOW*)> win;
			
			// Construction
			TUI();

		public:
			// Rule-of-5
			TUI(const TUI&) = delete;
			TUI(TUI&&) noexcept;
			TUI& operator=(const TUI&) = delete;
			TUI& operator=(TUI&&) noexcept;
		
			// Create instance
			static TUI create(void);
			
			// TUI operations
			Model::Dim2 getMaxSize(void) const noexcept;
			Model::Dim2 getPos(void) const noexcept;
			int getKey(void) const noexcept;
			void setBK(const short, const short) const;
			void move(const int, const int) const noexcept;
			void addString(const std::string&, const short) const noexcept;
			void addString(const std::string&) const noexcept;
			void clear(void) const noexcept;
			void draw(void) const noexcept;
	};
}
#include "controller.hpp"
#include "view.hpp"

#include <iostream>
#include <unistd.h>

namespace Controller{
    // Lifecircle
    static bool update(const View::TUI& tui){
        // Get current screen size
		Model::Dim2 dim = tui.getMaxSize();

        // Output screen size
		tui.moveCursor(5, 2);
		tui.addString(std::to_string(dim.width));
		tui.moveCursor(5, 3);
		tui.addString(std::to_string(dim.height));

        // Output runtime
        static int i = 0;
        tui.moveCursor(5, 4);
		tui.addString(std::to_string(i++));
		
		// Draw outputs
        tui.draw();

        // TODO: read input on beginning

        // Check for ESC keypress for termination
        if(tui.getKey() == 27 /* ASCII: <ESC> */)
            return false;
        // Repeat after delay
        usleep(100000);	// 100000 microseconds = 100 milliseconds
        return true;
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
		
		// Change TUI colors
		if(params.find("-invert") != params.cend())
			tui.setBK(COLOR_BLACK, COLOR_WHITE);


        // TODO: define FPS by params

        // Repeat update callback till termination return
        while(update(tui));
    }
}
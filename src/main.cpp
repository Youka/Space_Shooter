#include "controller.hpp"

#include <exception>
#include <iostream>
#include <typeinfo>

// Program entry
int main(const int argc, const char** argv) noexcept{
    // Continue in safe/verbose context
    try{
        // Run controller with packed command line arguments
        Controller::run(std::set<std::string>(argv, argv+argc));
    }catch(const std::exception& e){
        // Problem occured! Print it and finish program with status code of error
        std::cerr << typeid(e).name() << ": " << e.what() << std::endl;
        return 1;
    }
    // Finish program with status code of success
    return 0;
}


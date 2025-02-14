#include "../include/Util.hpp"
#include <type_traits>
#include <filesystem>
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <print>
using std::string;
using std::print;

namespace fs = std::filesystem;

namespace lagrange::util{
    void Log(string msg, WarningLevel level, fs::path const &filename){
        switch(level){
            case WarningLevel::LOW:
                msg =       "\33[33m[LOW]    " + msg; break; // YELLOW FG
            case WarningLevel::MEDIUM:
                msg =       "\33[31m[MEDIUM] " + msg; break; // RED FG
            case WarningLevel::HIGH:
                msg = "\33[1;30;101m[HIGH]   " + msg; break; // RED BG BLACK FG BOLD
        }
        msg += "\033[0m";

        std::ostream *output = &std::cerr;

        std::ofstream file;
        bool exists = fs::exists(filename) && (file = std::ofstream(filename), file.is_open());

        if(exists)
            output = &file;

        print(*output, "{}\n", msg);

        if(exists)
            file.close();
    }
}

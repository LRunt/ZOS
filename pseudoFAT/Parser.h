//
// Created by Lukas Runt on 11.11.2022.
//

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>
#include <map>
#include "Commands.h"


class Parser final{
    public:
        Parser() = default;

        int loadCommand(const std::string &command);

        std::vector<std::string> parseCommand(const std::string& command);

    private:
        std::vector<std::string> mCommand;
        Commands mCmd;
};



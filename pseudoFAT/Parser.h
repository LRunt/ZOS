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


class Parser final{
    public:
        Parser() = default;

        std::vector<std::string> parseCommand(const std::string& command);

        int getTypeOfCommand(const std::string& command);

    private:
        std::vector<std::string> mCommand;
};



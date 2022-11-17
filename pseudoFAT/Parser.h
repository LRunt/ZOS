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
        explicit Parser(char* filename){
            mCmd = new Commands(filename);
        };

        int loadCommand(const std::string &command);

        static std::vector<std::string> parseCommand(const std::string& command);

    private:
        std::vector<std::string> mCommand;
        Commands* mCmd;
};



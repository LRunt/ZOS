//
// Created by Lenovo on 11.11.2022.
//

#include "Parser.h"

std::map<std::string, int> commandMap = {
        {"quit", 0},
        {"cp", 1},
        {"mv", 2},
        {"rm", 3},
        {"mkdir", 4},
        {"rmdir", 5},
        {"ls", 6},
        {"cat", 7},
        {"cd", 8},
        {"pwd", 9},
        {"info", 10},
        {"incp", 11},
        {"outcp", 12},
        {"load", 13},
        {"format", 14}
};

int getCommandType(const std::string& command){
    auto it = commandMap.find(command);
    if(it != commandMap.end()){
        return it->second;
    }else{
        return -1;
    }
}




std::vector<std::string> Parser::parseCommand(const std::string &command) {

    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, ' ')) {
        if (!token.empty())
            tokens.push_back(token);
    }

    return tokens;
}

int Parser::loadCommand(const std::string &command) {
    std::vector<std::string> myVector = parseCommand(command);
    switch(getCommandType(command)){
        case 0:
            return 0;
        case 1:
            std::cout << "CP" << std::endl;
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
        case 6:
            return 6;
        case 7:
            return 7;
        case 8:
            return 8;
        case 9:
            return 9;
        case 10:
            return 10;
        case 11:
            return 11;
        case 12:
            return 12;
        case 13:
            return 13;
        case 14:
            return 14;
        default:
            std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            return -1;
    }
}

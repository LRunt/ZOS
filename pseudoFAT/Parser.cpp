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
    if(!command.length()){
        return -1;
    }
    std::vector<std::string> myVector = parseCommand(command);
    switch(getCommandType(myVector[0])){
        case 0:
            return 0;
        case 1:
            std::cout << "CP" << std::endl;
            mCmd->cp(myVector);
            return 1;
        case 2:
            std::cout << "MV" << std::endl;
            mCmd->mv(myVector);
            return 2;
        case 3:
            std::cout << "RM" << std::endl;
            mCmd->rm(myVector);
            return 3;
        case 4:
            mCmd->mkdir(myVector);
            std::cout << "MKDIR" << std::endl;
            mCmd->mkdir(myVector);
            return 4;
        case 5:
            std::cout << "RMDIR" << std::endl;
            mCmd->rmdir(myVector);
            return 5;
        case 6:
            std::cout << "LS" << std::endl;
            mCmd->ls(myVector);
            return 6;
        case 7:
            std::cout << "CAT" << std::endl;
            mCmd->cat(myVector);
            return 7;
        case 8:
            std::cout << "CD" << std::endl;
            mCmd->cd(myVector);
            return 8;
        case 9:
            std::cout << "PWD" << std::endl;
            mCmd->pwd(myVector);
            return 9;
        case 10:
            std::cout << "INFO" << std::endl;
            mCmd->info(myVector);
            return 10;
        case 11:
            std::cout << "INCP" << std::endl;
            mCmd->incp(myVector);
            return 11;
        case 12:
            std::cout << "OUTCP" << std::endl;
            mCmd->outcp(myVector);
            return 12;
        case 13:
            std::cout << "LOAD" << std::endl;
            mCmd->load(myVector);
            return 13;
        case 14:
            if(mCmd->format(myVector)){
                std::cout << "OK" << std::endl;
            }else{
                std::cout << "CANNOT CREATE FILE" << std::endl;
            }
            return 14;
        default:
            std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            return -1;
    }
}

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
    int commandReturnValue;
    std::vector<std::string> myVector = parseCommand(command);
    switch(getCommandType(myVector[0])){
        case 0: //quit
            return 0;
        case 1: //cp
            commandReturnValue = mCmd->cp(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            }else{
                std::cout << "PATH NOT FOUND" << std::endl;
            }
            return 1;
        case 2: //mv
            mCmd->mv(myVector);
            return 2;
        case 3: //rm
            mCmd->rm(myVector);
            return 3;
        case 4: //mkdir
            commandReturnValue = mCmd->mkdir(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cout << "EXIST" << std::endl;
            }else if(commandReturnValue == 3){
                std::cout << "PATH NOT FOUND" << std::endl;
            }else{
                std::cout << "NOT ENOUGH SPACE" << std::endl;
            }
            return 4;
        case 5: //rmdir
            mCmd->rmdir(myVector);
            return 5;
        case 6: //ls
            commandReturnValue = mCmd->ls(myVector);
            if(commandReturnValue){
                std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            }
            return 6;
        case 7: //cat
            mCmd->cat(myVector);
            return 7;
        case 8: //cd
            commandReturnValue = mCmd->cd(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            }else{
                std::cout << "PATH NOT FOUND" << std::endl;
            }
            return 8;
        case 9: //pwd
            std::cout << mCmd->pwd(myVector) << std::endl;
            return 9;
        case 10: //info
            mCmd->info(myVector);
            return 10;
        case 11: //incp
            commandReturnValue = mCmd->incp(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cout << "FILE NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cout << "PATH NOT FOUND" << std::endl;
            }
            return 11;
        case 12: //outcp
            commandReturnValue = mCmd->outcp(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cout << "FILE NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cout << "PATH NOT FOUND" << std::endl;
            }
            return 12;
        case 13: //load
            commandReturnValue = load(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cout << command << ": COMMAND NOT FOUND" << std::endl;
            }else {
                std::cout << "FILE NOT FOUND" << std::endl;
            }
            return 13;
        case 14: //format
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

/**
 * Method load commands form a file
 * @param vectorOfCommands vector of command line
 * @return 0 - if everything is okay, 1 - if command have incorrect number of parameters, 2 - if file not exists
 */
int Parser::load(std::vector<std::string> vectorOfCommands) {
    if(vectorOfCommands.size() != 2){
        return 1;
    }
    std::ifstream input(vectorOfCommands[1], std::ios::in);
    std::string line;

    if(!input){
        return 2;
    }else{
        while(std::getline(input, line)){
            loadCommand(line);
        }
    }

    input.close();
    return 0;
}
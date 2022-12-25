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
        {"format", 14},
        {"check", 15},
        {"bug", 16}
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
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "FILE NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cerr << "PATH NOT FOUND" << std::endl;
            }else if(commandReturnValue == 4){
                std::cerr << "NOT ENOUGH SPACE" << std::endl;
            }else if(commandReturnValue == 5){
                std::cerr << "FILE WITH SAME NAME EXIST IN DIRECTORY" << std::endl;
            }else{
                std::cerr << "NOT ENOUGH SPACE IN DIRECTORY" << std::endl;
            }
            return 1;
        case 2: //mv
            commandReturnValue = mCmd->mv(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "FILE NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cerr << "PATH NOT FOUND" << std::endl;
            }else if(commandReturnValue == 4){
                std::cerr << "FILE WITH SAME NAME EXIST IN DIRECTORY" << std::endl;
            }else{
                std::cerr << "NOT ENOUGH SPACE IN DIRECTORY" << std::endl;
            }
            return 2;
        case 3: //rm
            commandReturnValue = mCmd->rm(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else{
                std::cerr << "FILE NOT FOUND" << std::endl;
            }
            return 3;
        case 4: //mkdir
            commandReturnValue = mCmd->mkdir(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "PATH NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cerr << "EXIST" << std::endl;
            }else{
                std::cerr << "NOT ENOUGH SPACE" << std::endl;
            }
            return 4;
        case 5: //rmdir
            commandReturnValue = mCmd->rmdir(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "FILE NOT FOUND" << std::endl;
            }else if (commandReturnValue == 3){
                std::cerr << "NOT EMPTY" << std::endl;
            }else{
                std::cerr << "CANNOT DELETE DIRECTORY WHERE YOU ACTUALLY ARE" << std::endl;
            }
            return 5;
        case 6: //ls
            commandReturnValue = mCmd->ls(myVector);
            if(commandReturnValue){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }
            return 6;
        case 7: //cat
            commandReturnValue = mCmd->cat(myVector);
            if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if (commandReturnValue == 2){
                std::cerr << "PATH NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cerr << "DATA ERROR" << std::endl;
            }
            return 7;
        case 8: //cd
            commandReturnValue = mCmd->cd(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else{
                std::cerr << "PATH NOT FOUND" << std::endl;
            }
            return 8;
        case 9: //pwd
            std::cout << mCmd->pwd(myVector) << std::endl;
            return 9;
        case 10: //info
            commandReturnValue = mCmd->info(myVector);
            if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "FILE NOT FOUND" << std::endl;
            }
            return 10;
        case 11: //incp
            commandReturnValue = mCmd->incp(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "FILE NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cerr << "PATH NOT FOUND" << std::endl;
            }else if(commandReturnValue == 4){
                std::cerr << "NOT ENOUGH SPACE" << std::endl;
            }else if(commandReturnValue == 5){
                std::cerr << "FILE WITH SAME NAME EXIST IN DIRECTORY" << std::endl;
            }else{
                std::cerr << "NOT ENOUGH SPACE IN DIRECTORY" << std::endl;
            }
            return 11;
        case 12: //outcp
            commandReturnValue = mCmd->outcp(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "FILE NOT FOUND" << std::endl;
            }else if(commandReturnValue == 3){
                std::cerr << "PATH NOT FOUND" << std::endl;
            }
            return 12;
        case 13: //load
            commandReturnValue = load(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else {
                std::cerr << "FILE NOT FOUND" << std::endl;
            }
            return 13;
        case 14: //format
            if(mCmd->format(myVector)){
                std::cout << "OK" << std::endl;
            }else{
                std::cerr << "CANNOT CREATE FILE" << std::endl;
            }
            return 14;
        case 15: //check
            commandReturnValue = mCmd->check(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else if(commandReturnValue == 1){
                std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
            }else if(commandReturnValue == 2){
                std::cerr << "FILE NOT FOUND" << std::endl;
            }else{
                std::cerr << "BUG FAILED" << std::endl;
            }
            return 15;
        case 16: //bug
            commandReturnValue = mCmd->bug(myVector);
            if(commandReturnValue == 0){
                std::cout << "OK" << std::endl;
            }else{
                std::cout << "ERROR" << std::endl;
            }
            return 16;
        default:
            std::cerr << command << ": COMMAND NOT FOUND" << std::endl;
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
            std::cout << line << std::endl;
            loadCommand(line);
        }
    }

    input.close();
    return 0;
}
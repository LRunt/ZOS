//
// Created by Lenovo on 12.11.2022.
//

#include "Commands.h"

std::map<std::string, int> units = {
        {"B", 1},
        {"kB", 1024},
        {"MB", 1024*1024}
};

int getUnitSize(const std::string& command){
    auto it = units.find(command);
    if(it != units.end()){
        return it->second;
    }else{
        return 0;
    }
}

std::vector<std::string> getSize(const std::string& input) {
    std::vector<std::string> sizeAndUnit;

    int i = 0;
    while(std::isdigit(input[i])){
        i++;
    }

    sizeAndUnit.push_back(input.substr(0, i));
    sizeAndUnit.push_back(input.substr(i, input.size()));

    return sizeAndUnit;
}

bool Commands::format(std::vector<std::string> myVectorOfCommands) {
    if(myVectorOfCommands.size() != 2){
        return false;
    }
    std::vector<std::string> parsed = getSize(myVectorOfCommands[1]);

    int unit = getUnitSize(parsed[1]);
    if(!unit || parsed.size() == 1){
        return false;
    }

    std::ofstream fileSystem(mFileSystemName, std::ios::out | std::ios::binary);

    const char* str = "";

    mFileSize = stoi(parsed[0])*unit;

    for(int i = 0; i < mFileSize; i++){
        fileSystem.write(str, 1);
    }
    return true;
}

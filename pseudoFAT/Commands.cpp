//
// Created by Lenovo on 12.11.2022.
//

#include "Commands.h"

bool Commands::format(std::vector<std::string> myVectorOfCommands) {
    std::ofstream fileSystem("myFileSystem.bin", std::ios::out | std::ios::binary);

    std::string binbuf = *myVectorOfCommands.begin();
    const char* str = "";

    for(int i = 0; i < 800*1024; i++){
        fileSystem.write(str, 1);
    }
    std::cout << "OK" <<std::endl;
    return false;
}

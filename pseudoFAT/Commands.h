//
// Created by Lenovo on 12.11.2022.
//

#ifndef PSEUDOFAT_COMMANDS_H
#define PSEUDOFAT_COMMANDS_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

class Commands {

    public:
        explicit Commands(char* fileSystemName) :  mFileSystemName{fileSystemName}{}

        bool format(std::vector<std::string> myVectorOfCommands);


    private:
        char* mFileSystemName;
        int mClusterSize = 1024;
        int mFileSize;
        int mTableCellSize;
        int mNumberOfClusters;
};


#endif //PSEUDOFAT_COMMANDS_H

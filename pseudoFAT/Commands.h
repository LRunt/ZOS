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
#include <cmath>
#include <map>

class Commands {

    public:
        explicit Commands(char* fileSystemName) :  mFileSystemName{fileSystemName}{}

        bool format(std::vector<std::string> myVectorOfCommands);

        int cp(std::vector<std::string> vectorOfCommands);

        int mv(std::vector<std::string> vectorOfCommands);

        bool rm(std::vector<std::string> vectorOfCommands);

        int mkdir(std::vector<std::string> vectorOfCommands);

        int rmdir(std::vector<std::string> vectorOfCommands);

        int ls(std::vector<std::string> vectorOfCommands);

        bool cat(std::vector<std::string> vectorOfCommands);

        bool cd(std::vector<std::string> vectorOfCommands);

        std::string pwd(std::vector<std::string> vectorOfCommands);

        bool info(std::vector<std::string> vectorOfCommands);

        int incp(std::vector<std::string> vectorOfCommands);

        bool outcp(std::vector<std::string> vectorOfCommands);

        bool load(std::vector<std::string> vectorOfCommands);

    private:
        char* mFileSystemName;
        int mClusterSize = 1024;
        int mFileSize;
        int mTableCellSize;
        int mNumberOfClusters;
        int mStartClusterOfData;
        //defines where i am
        int mActualCluster;
};


#endif //PSEUDOFAT_COMMANDS_H

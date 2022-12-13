//
// Created by Lenovo on 12.11.2022.
//

#ifndef PSEUDOFAT_COMMANDS_H
#define PSEUDOFAT_COMMANDS_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <filesystem>

class Commands {

    public:
        explicit Commands(char* fileSystemName) :  mFileSystemName{fileSystemName}{
        }

        bool format(std::vector<std::string> myVectorOfCommands);

        int cp(std::vector<std::string> vectorOfCommands);

        int mv(std::vector<std::string> vectorOfCommands);

        bool rm(std::vector<std::string> vectorOfCommands);

        int mkdir(std::vector<std::string> vectorOfCommands);

        int rmdir(std::vector<std::string> vectorOfCommands);

        int ls(std::vector<std::string> vectorOfCommands);

        bool cat(std::vector<std::string> vectorOfCommands);

        int cd(std::vector<std::string> vectorOfCommands);

        std::string pwd(std::vector<std::string> vectorOfCommands);

        bool info(std::vector<std::string> vectorOfCommands);

        int incp(std::vector<std::string> vectorOfCommands);

        int outcp(std::vector<std::string> vectorOfCommands);

        void saveFileSystemParameters();

    private:
        char* mFileSystemName;
        int mClusterSize = 1024;
        int mFileSize;
        int mTableCellSize;
        int mNumberOfClusters;
        int mStartClusterOfData = 0;
        int mLengthOfFile;
        //defines where i am
        int mActualCluster = -1;

        int getNumberFromFat(int cluster);

        bool rewriteTableCell(int cluster, int tableNumber);

        void writeFileToTheCluster(int cluster, std::string fileName, bool isDirectory, int size, int directoryCluster);

        int getFreeCluster();

        int getNumberOfFreeClusters();

        bool fileExist(const std::string& fileName);

        int getDirectoryCluster(const std::string& fileName, int cluster);

        int getParentCluster(int cluster);

        std::string getDirectoryName(int cluster);

        int absolutePathClusterNumber(const std::vector<std::string>& vectorOfFiles);

        void printAllFiles(int cluster);

    void hehe();
};


#endif //PSEUDOFAT_COMMANDS_H

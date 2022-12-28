/**
 * Class execute the commands of the file system
 * Created by Lukas Runt on 12.11.2022.
 */



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

        int rm(std::vector<std::string> vectorOfCommands);

        int mkdir(std::vector<std::string> vectorOfCommands);

        int rmdir(std::vector<std::string> vectorOfCommands);

        int ls(std::vector<std::string> vectorOfCommands);

        int cat(std::vector<std::string> vectorOfCommands);

        int cd(std::vector<std::string> vectorOfCommands);

        std::string pwd(const std::vector<std::string>& vectorOfCommands);

        int info(std::vector<std::string> vectorOfCommands);

        int incp(std::vector<std::string> vectorOfCommands);

        int outcp(std::vector<std::string> vectorOfCommands);

        int check(std::vector<std::string> vectorOfCommands);

        int bug(std::vector<std::string> vectorOfCommands);

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

        bool saveFileSystemParameters();

        int getNumberFromFat(int cluster);

        bool rewriteTableCell(int cluster, int tableNumber);

        int writeFileToTheCluster(int cluster, const std::string& fileName, bool isDirectory, int size, int directoryCluster);

        int getFreeCluster();

        int getNumberOfFreeClusters();

        bool fileExist(const std::string& fileName, int cluster);

        int getDirectoryCluster(const std::string& fileName, int cluster);

        int getParentCluster(int cluster);

        std::string getDirectoryName(int cluster);

        int relativePathClusterNumber(const std::vector<std::string>& vectorOfFiles, int type);

        int absolutePathClusterNumber(const std::vector<std::string>& vectorOfFiles, int type);

        void printAllFiles(int cluster);

        int getCluster(const std::string &fileName, int cluster);

        int getFileCluster(const std::string& fileName, int cluster);

        int getFileSize(const std::string &fileName, int cluster);

        int getFileSizeAbsolute(const std::vector<std::string> &vectorOfFiles);

        bool isDirectoryEmpty(int cluster);

        void clearCluster(int cluster);

        void deleteFileFromDirectory(int cluster, const std::string& fileName);

        bool rewriteFileCluster(int cluster, const std::string& fileName, int newFileCluster);
};


#endif //PSEUDOFAT_COMMANDS_H

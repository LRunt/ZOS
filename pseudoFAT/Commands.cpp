//
// Created by Lenovo on 12.11.2022.
//

#include "Commands.h"

const int LAST_BLOCK = -1;
const int FREE_BLOCK = -2;

/** Map of units where key is unit and the size is the value*/
std::map<std::string, int> units = {
        {"B", 1},
        {"kB", 1024},
        {"MB", 1024*1024}
};

/**
 * Method returns size of the units
 * @param unit unit which will be replaced by number, the size of the unit
 * @return size of the unit
 */
int getUnitSize(const std::string& unit){
    auto it = units.find(unit);
    if(it != units.end()){
        return it->second;
    }else{
        return 0;
    }
}

/**
 * Method to get size of the fileSystem
 * @param input size of the file system - format <number><units>, allowed units are B, kB, MB
 * @return size of the fileSystem
 */
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

/**
 * Command format <sizeOfFileSystem> - creating the new file system, with defined size
 * @param myVectorOfCommands
 * @return true-fileSystem was created, false-creating of file system failed
 */
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

    mFileSize = stoi(parsed[0])*unit;

    std::string size = std::to_string(mFileSize);
    std::string clusterSize = std::to_string(mClusterSize);

    size = size + '\0';
    clusterSize = clusterSize + '\0';

    mNumberOfClusters = mFileSize/mClusterSize;

    mTableCellSize = std::to_string(mNumberOfClusters).size() + 1;

    fileSystem.write(&size[0], size.size());
    fileSystem.write(&clusterSize[0], clusterSize.size());

    int tableSize = mTableCellSize * mNumberOfClusters;
    double numberOfClustersForTable = tableSize / (double)mClusterSize;
    //Counting start of the data section
    mStartClusterOfData = 1 + std::ceil(numberOfClustersForTable);
    mActualCluster = mStartClusterOfData;

    fileSystem.write(&std::to_string(mStartClusterOfData)[0], std::to_string(mStartClusterOfData).size());

    const char* str = "";

    //BOOT record
    for(int i = 0; i < mClusterSize - size.size() - clusterSize.size() - std::to_string(mStartClusterOfData).size(); i++){
        fileSystem.write(str, 1);
    }

    //FAT table
    fileSystem.write(&std::to_string(LAST_BLOCK)[0], mTableCellSize);
    //Table
    int tableCluster = 1;
    while(tableCluster < numberOfClustersForTable){
        fileSystem.write(&std::to_string(tableCluster + 1)[0], std::to_string(tableCluster + 1).size());
        for(int i = std::to_string(tableCluster + 1).size(); i < mTableCellSize; i++){
            fileSystem.write(str, 1);
        }
        tableCluster++;
    }

    fileSystem.write(&std::to_string(LAST_BLOCK)[0], std::to_string(LAST_BLOCK).size());
    for(int j = std::to_string(LAST_BLOCK).size(); j < mTableCellSize; j++){
        fileSystem.write(str, 1);
    }

    //data
    for(int i = 0; i < mNumberOfClusters - 2; i++){
        fileSystem.write(&std::to_string(FREE_BLOCK)[0], std::to_string(FREE_BLOCK).size());
        for(int j = std::to_string(FREE_BLOCK).size(); j < mTableCellSize; j++){
            fileSystem.write(str, 1);
        }
    }

    for(int i = 0; i < mFileSize - mClusterSize - mNumberOfClusters * mTableCellSize; i++){
        fileSystem.write(str, 1);
    }
    fileSystem.close();
    return true;
}

int Commands::cp(std::vector<std::string> vectorOfCommands) {
    saveFileSystemParameters();
    getNumberFromFat(std::stoi(vectorOfCommands[1]));
    return 0;
}

int Commands::mv(std::vector<std::string> vectorOfCommands) {
    return 0;
}

bool Commands::rm(std::vector<std::string> vectorOfCommands) {
    return 0;
}

int Commands::mkdir(std::vector<std::string> vectorOfCommands) {

    return 0;
}

int Commands::rmdir(std::vector<std::string> vectorOfCommands) {
    return 0;
}

int Commands::ls(std::vector<std::string> vectorOfCommands) {
    return 0;
}

bool Commands::cat(std::vector<std::string> vectorOfCommands) {
    return false;
}

bool Commands::cd(std::vector<std::string> vectorOfCommands) {
    return false;
}

std::string Commands::pwd(std::vector<std::string> vectorOfCommands) {
    saveFileSystemParameters();
    return std::string();
}

bool Commands::info(std::vector<std::string> vectorOfCommands) {
    return false;
}

int Commands::incp(std::vector<std::string> vectorOfCommands) {
    return 0;
}

bool Commands::outcp(std::vector<std::string> vectorOfCommands) {
    return false;
}

bool Commands::load(std::vector<std::string> vectorOfCommands) {
    return false;
}

void Commands::saveFileSystemParameters() {
    std::ifstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);

    char data[1];
    std::string output;

    do{
        fileSystem.read(data, 1);
        output += *data;
    }while(*data != 0x00);


    if(!mFileSize){
        mFileSize = std::stoi(output);
    }

    output = "";

    do{
        fileSystem.read(data, 1);
        output += *data;
    }while(*data != 0x00);

    mClusterSize = std::stoi(output);

    mNumberOfClusters = mFileSize/mClusterSize;
    mTableCellSize = std::to_string(mNumberOfClusters).size() + 1;

    output = "";

    do{
        fileSystem.read(data, 1);
        output += *data;
    }while(*data != 0x00);

    mStartClusterOfData = std::stoi(output);

    fileSystem.close();
}

int Commands::getNumberFromFat(int cluster){
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    std::cout << mTableCellSize << std::endl;
    std::cout << mClusterSize << std::endl;
    fileSystem.seekp(mClusterSize + cluster * mTableCellSize);
    char data[1];

    std::string output;

    do{
        fileSystem.read(data, 1);
        output += *data;
    }while(*data != 0x00);

    fileSystem.close();
    return std::stoi(output);
}



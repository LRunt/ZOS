//
// Created by Lenovo on 12.11.2022.
//

#include "Commands.h"

const int LAST_BLOCK = -1;
const int FREE_BLOCK = -2;
const char DIRECTORY = 'A';
const char IS_FILE = 'S';
const int NAME_OF_FILE_LENGTH = 11;

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
    for(int i = std::to_string(LAST_BLOCK).size(); i < mTableCellSize; i++){
        fileSystem.write(str, 1);
    }

    fileSystem.write(&std::to_string(LAST_BLOCK)[0], std::to_string(LAST_BLOCK).size());
    for(int i = std::to_string(LAST_BLOCK).size(); i < mTableCellSize; i++){
        fileSystem.write(str, 1);
    }

    //data
    for(int i = 0; i < mNumberOfClusters - std::ceil(numberOfClustersForTable) - 2; i++){
        fileSystem.write(&std::to_string(FREE_BLOCK)[0], std::to_string(FREE_BLOCK).size());
        for(int j = std::to_string(FREE_BLOCK).size(); j < mTableCellSize; j++){
            fileSystem.write(str, 1);
        }
    }

    for(int i = 0; i < mFileSize - mClusterSize - mNumberOfClusters * mTableCellSize; i++){
        fileSystem.write(str, 1);
    }
    //root file
    writeFileToTheCluster(mStartClusterOfData, "root", true, -1);

    fileSystem.close();

    if(!mActualCluster){
        mActualCluster = mStartClusterOfData;
    }

    return true;
}

int Commands::cp(std::vector<std::string> vectorOfCommands) {
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

/**
 * Method initialize data of the file system
 */
void Commands::saveFileSystemParameters() {
    std::ifstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    char data[1];
    std::string output;

    do{
        fileSystem.read(data, 1);
        output += *data;
    }while(*data != 0x00);

    mFileSize = std::stoi(output);

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

/**
 * Method reads value of cluster from FAT table
 * @param cluster cluster what value we want
 * @return number, -1 end of block, -2 empty cluster, other is address of next cluster
 */
int Commands::getNumberFromFat(int cluster){
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
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

/**
 * Method rewrites value in the FAT table
 * @param cluster cluster what value we will change
 * @param tableNumber value what will be in the table
 * @return true - it was successful, false - it failed
 */
bool Commands::rewriteTableCell(int cluster, int tableNumber){
    std::string tempString = std::to_string(tableNumber);
    char const* numberArray = tempString.c_str();

    std::fstream fileSystem;
    fileSystem.open(mFileSystemName, std::ios::out | std::ios::in | std::ios::binary);
    fileSystem.seekp(mClusterSize + cluster * mTableCellSize);

    for(int i = 0; i < tempString.size(); i++){
        fileSystem.put(numberArray[i]);
    }

    fileSystem.close();

    return true;
}

/**
 * Method writes a file information into the cluster
 * @param cluster the cluster into which the file will be written
 * @param fileName name of the file
 * @param isDirectory if the file is directory or not
 * @param directoryCluster the cluster where is the directory stored
 */
void Commands::writeFileToTheCluster(int cluster, std::string fileName, bool isDirectory, int directoryCluster){
    char data[1];
    std::fstream fileSystem;
    fileSystem.open(mFileSystemName, std::ios::out | std::ios::in | std::ios::binary);
    int p = 0;
    do{
        fileSystem.seekp(mClusterSize * cluster + p * (NAME_OF_FILE_LENGTH + 1 + mTableCellSize));
        fileSystem.read(data, 1);
        p++;
    } while (*data != 0x00);

    fileSystem.seekp(mClusterSize * cluster + (p-1) * (NAME_OF_FILE_LENGTH + 1 + mTableCellSize));

    for(char i : fileName){
        fileSystem.put(i);
    }
    for(int i = fileName.size(); i < NAME_OF_FILE_LENGTH; i++){
        fileSystem.put(0x00);
    }
    fileSystem.put(isDirectory);
    for(char character : std::to_string(directoryCluster)){
        fileSystem.put(character);
    }

    for(int i = std::to_string(directoryCluster).size(); i < mTableCellSize; i++){
        fileSystem.put(0x00);
    }

    fileSystem.close();
}

/**
 * Method returns index of first free cluster
 * @return index of first free cluster
 */
int Commands::getFreeCluster(){
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    fileSystem.seekp(mClusterSize);

    int size = std::to_string(mNumberOfClusters).size() + 1;
    std::string input(size, ' ');

    int i = -1;
    do{
        i++;
        fileSystem.read(&input[0], size);
    }while(input[0] != '-' || input[1] != '2');

    return i;
}

/**
 * Method returns number of free clusters
 * @return number of free clusters
 */
int Commands::getNumberOfFreeClusters(){
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    fileSystem.seekp(mClusterSize);

    int size = std::to_string(mNumberOfClusters).size() + 1;
    std::string input(size, ' ');
    int numberOfFreeClusters = 0;

    for(int i = 0; i < mNumberOfClusters; i++){
        fileSystem.read(&input[0], size);
        if(input[0] != '-' || input[1] != '2'){
            numberOfFreeClusters++;
        }
    }

    return numberOfFreeClusters;
}



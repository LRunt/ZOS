//
// Created by Lenovo on 12.11.2022.
//

#include "Commands.h"

const int LAST_BLOCK = -1;
const int FREE_BLOCK = -2;
const int NAME_OF_FILE_LENGTH = 11;
const int EMPTY_FILE_SIZE = 0;
const int BOTH = 0;
const int DIRECTORY = 1;
const int FILE_TYPE = 2;

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
 * Method splits string by slash
 * @param absolutePath absolute path of the file
 * @return vector of files
 */
std::vector<std::string> splitBySlash(std::string absolutePath){
    std::istringstream iss(absolutePath);
    std::vector<std::string> files;
    std::string token;
    while (std::getline(iss, token, '/')) {
        if (!token.empty())
            files.push_back(token);
    }
    return files;
}

std::string deleteSlash(std::string fileName){
    fileName.erase(remove(fileName.begin(), fileName.end(), '/'), fileName.end());
    return fileName;
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

    mLengthOfFile = NAME_OF_FILE_LENGTH + 1 + std::to_string(mFileSize).size() + mTableCellSize; //Name of file - file type - file size - cluster
    //root file
    writeFileToTheCluster(mStartClusterOfData, "root", true, EMPTY_FILE_SIZE, -1);

    fileSystem.close();

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

/**
 * Method make a new file if file not exist
 * @param vectorOfCommands commands from command line
 * @return 0 - File was created, 1 - wrong number of parameters, 2 - file already exist
 */
int Commands::mkdir(std::vector<std::string> vectorOfCommands) {
    if(mActualCluster == -1){
        saveFileSystemParameters();
    }
    if(vectorOfCommands.size() != 2){
        return 1;
    }else if(fileExist(deleteSlash(vectorOfCommands[1]))){
        return 2;
    }
    int fileCluster = getFreeCluster();
    writeFileToTheCluster(mActualCluster, deleteSlash(vectorOfCommands[1]), true, EMPTY_FILE_SIZE, fileCluster);
    writeFileToTheCluster(fileCluster, deleteSlash(vectorOfCommands[1]), true, EMPTY_FILE_SIZE, mActualCluster);
    rewriteTableCell(fileCluster, LAST_BLOCK);
    return 0;
}

int Commands::rmdir(std::vector<std::string> vectorOfCommands) {
    return 0;
}

/**
 * Method prints the contents of actual directory or directory from path
 * @param vectorOfCommands command form command line
 * @return return code 0 - all is ok, 1 - error
 */
int Commands::ls(std::vector<std::string> vectorOfCommands) {
    if(mActualCluster == -1){
        saveFileSystemParameters();
    }
    if(vectorOfCommands.size() == 1){ //ls
        printAllFiles(mActualCluster);
    }else if(vectorOfCommands.size() == 2){ //ls a1
        int cluster = absolutePathClusterNumber(splitBySlash(vectorOfCommands[1]), DIRECTORY);
        if(cluster == -1){
            std::cout << "PATH NOT FOUND" << std::endl;
        }else{
            printAllFiles(cluster);
        }
    }else{
        return 1;
    }
    return 0;
}

bool Commands::cat(std::vector<std::string> vectorOfCommands) {
    return false;
}

/**
 * Method change a destination where I am
 * @param vectorOfCommands commands form command line
 * @return 0 - command succeed, 1 - wrong number of parameters, 2 - file not found
 */
int Commands::cd(std::vector<std::string> vectorOfCommands) {
    if(mActualCluster == -1){
        saveFileSystemParameters();
    }
    if(vectorOfCommands.size() != 2){
        return 1;
    }
    int fileCluster;
    if(vectorOfCommands[1][0] == '/'){
        fileCluster = absolutePathClusterNumber(splitBySlash(vectorOfCommands[1]), DIRECTORY);
    }else{
        fileCluster = getDirectoryCluster(vectorOfCommands[1], mActualCluster);
    }
    if(fileCluster != -1){
        mActualCluster = fileCluster;
    }else{
        return 2;
    }
    return 0;
}

std::string Commands::pwd(std::vector<std::string> vectorOfCommands) {
    std::string path;
    if(mActualCluster == -1){
        saveFileSystemParameters();
    }
    if(vectorOfCommands.size() != 1){
        return "COMMAND NOT FOUND";
    }
    std::vector<std::string> files;
    int actualDirectory = mActualCluster;

    while(actualDirectory != -1){
        files.push_back(getDirectoryName(actualDirectory));
        actualDirectory = getParentCluster(actualDirectory);
    }

    std::reverse(files.begin(), files.end());

    for(auto & file : files){
        path += file + '/';
    }

    return path.substr(0, path.length()-1);
}

int Commands::info(std::vector<std::string> vectorOfCommands) {
    std::string output;

    if(mActualCluster == -1){
        saveFileSystemParameters();
    }
    if(vectorOfCommands.size() != 2){
        return 1;
    }
    int cluster;
    if(vectorOfCommands[1][0] == '/'){
        cluster = absolutePathClusterNumber(splitBySlash(vectorOfCommands[1]), BOTH);
    }else{
        cluster = getCluster(vectorOfCommands[1], mActualCluster);
    }
    if(cluster == -1){
        return 2;
    }
    output += vectorOfCommands[1] + " ";
    while(cluster != -1){
        output += std::to_string(cluster) + ", ";
        cluster = getNumberFromFat(cluster);
    }
    std::cout << output.substr(0, output.size() - 2) << std::endl;
    return 0;
}

/**
 * Method loads file from hard drive and copy it to the fileSystem
 * @param vectorOfCommands commands form command line
 * @return 0 - successfully copied, 1 - wrong number of parameters, 2 - file on hard drive not found, 3 - path in file system not found, 4 - not enough space
 */
int Commands::incp(std::vector<std::string> vectorOfCommands) {
    if(mActualCluster == -1){
        saveFileSystemParameters();
    }
    if(vectorOfCommands.size() != 3){
        return 1;
    }
    //file on hard drive exists
    std::fstream input(vectorOfCommands[1], std::ios::in | std::ios::binary);
    if(!input){
        return 2;
    }
    //path in file system exists
    std::replace(vectorOfCommands[1].begin(), vectorOfCommands[1].end(), '\\', '/');
    std::vector<std::string> absolutePath = splitBySlash(vectorOfCommands[1]);
    int cluster = absolutePathClusterNumber(splitBySlash(vectorOfCommands[2]), DIRECTORY);
    if(cluster == -1){
        return 3;
    }
    //testing if file fits into the file system
    int fileSize = std::filesystem::file_size(vectorOfCommands[1]);
    int numberOfCluster = std::ceil(fileSize/(double)mClusterSize);
    if(numberOfCluster > getNumberOfFreeClusters()){
        return 4;
    }
    char data[1];
    int newClusterData;
    std::fstream fileSystem;
    fileSystem.open(mFileSystemName, std::ios::out | std::ios::in | std::ios::binary);
    int clusterOfData = getFreeCluster();
    writeFileToTheCluster(cluster, absolutePath[absolutePath.size() - 1], false, fileSize, clusterOfData);
    rewriteTableCell(clusterOfData, LAST_BLOCK);
    fileSystem.seekp(mClusterSize * clusterOfData);
    for(int i = 0; i < fileSize; i++){
        input.read(data, 1);
        fileSystem.put(*data);
        if(fileSystem.tellg()%mClusterSize == 0){
            newClusterData = getFreeCluster();
            rewriteTableCell(clusterOfData, newClusterData);
            clusterOfData = newClusterData;
            rewriteTableCell(clusterOfData, LAST_BLOCK);
            fileSystem.seekp(mClusterSize * clusterOfData);
        }
    }
    fileSystem.close();
    input.close();
    return 0;
}

int Commands::outcp(std::vector<std::string> vectorOfCommands) {
    if(mActualCluster == -1){
        saveFileSystemParameters();
    }
    if(vectorOfCommands.size() != 3){
        return 1;
    }
    int cluster;
    if(vectorOfCommands[1][0] == '/'){
        cluster = absolutePathClusterNumber(splitBySlash(vectorOfCommands[1]), FILE_TYPE);
    }else{
        cluster = getFileCluster(vectorOfCommands[1], mActualCluster);
    }
    if(cluster == -1){
        return 2;
    }
    std::filesystem::path filepath = std::string(vectorOfCommands[2]);
    if(!std::filesystem::is_directory(filepath.parent_path())){
        return 3;
    }

    return 0;
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
    if(mActualCluster == -1){
        mActualCluster = mStartClusterOfData;
    }

    mLengthOfFile = NAME_OF_FILE_LENGTH + 1 + std::to_string(mFileSize).size() + mTableCellSize; //Name of file - file type - file size - cluster
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

    for(int i = tempString.size(); i < mTableCellSize; i++){
        fileSystem.put(0x00);
    }

    fileSystem.close();

    return true;
}

/**
 * Method writes a file information into the cluster
 * @param cluster the cluster into which the file will be written
 * @param fileName name of the file
 * @param isDirectory if the file is directory or not
 * @param size the size of the file
 * @param directoryCluster the cluster where is the directory stored
 */
void Commands::writeFileToTheCluster(int cluster, std::string fileName, bool isDirectory, int size, int directoryCluster){
    char data[1];
    std::fstream fileSystem;
    fileSystem.open(mFileSystemName, std::ios::out | std::ios::in | std::ios::binary);
    int p = 0;
    do{
        fileSystem.seekp(mClusterSize * cluster + p * mLengthOfFile);
        fileSystem.read(data, 1);
        p++;
    } while (*data != 0x00);

    fileSystem.seekp(mClusterSize * cluster + (p-1) * mLengthOfFile);

    //filename
    for(char i : fileName){
        fileSystem.put(i);
    }
    for(int i = fileName.size(); i < NAME_OF_FILE_LENGTH; i++){
        fileSystem.put(0x00);
    }

    //isDictionary
    fileSystem.put(isDirectory);

    //size of the file
    for(char character: std::to_string(size)){
        fileSystem.put(character);
    }
    for(int i = std::to_string(size).size(); i < std::to_string(mFileSize).size(); i++){
        fileSystem.put(0x00);
    }

    //directory cluster
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

/**
 * Method detects whether a folder with the same name already exists
 * @param fileName name of the file
 * @return true - folder exist, false - folder not exist
 */
bool Commands::fileExist(const std::string& fileName){
    char data[NAME_OF_FILE_LENGTH];
    int fileInformationLength = mLengthOfFile;
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    int i = 1;
    std::string nameOfTheFile;

    do{
        fileSystem.seekp(mClusterSize * mActualCluster + (fileInformationLength * i));
        fileSystem.read(data, NAME_OF_FILE_LENGTH);
        nameOfTheFile = "";
        int j = 0;
        while(data[j] != 0x00){
            nameOfTheFile += data[j];
            j++;
        }
        if(nameOfTheFile == fileName){
            fileSystem.close();
            return true;
        }
        i++;
    }while(!nameOfTheFile.empty());

    fileSystem.close();

    return false;
}

/**
 * Method returns if the file is dictionary or not
 * @param fileName name of the file
 * @return -1 - if directory not exist or cluster where is the directory
 */
int Commands::getDirectoryCluster(const std::string& fileName, int cluster){
    char data[NAME_OF_FILE_LENGTH];
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    int i = 1;
    std::string nameOfTheFile;

    do{
        fileSystem.seekp(mClusterSize * cluster + (mLengthOfFile * i));
        fileSystem.read(data, NAME_OF_FILE_LENGTH);
        nameOfTheFile = "";
        int j = 0;
        while(data[j] != 0x00){
            nameOfTheFile += data[j];
            j++;
        }
        if(nameOfTheFile == fileName){
            fileSystem.read(data, 1);
            if(*data == 0x01){
                fileSystem.read(data, std::to_string(mFileSize).size());
                fileSystem.read(data, mTableCellSize);
                j = 0;
                nameOfTheFile = "";
                while(data[j] != 0x00){
                    nameOfTheFile += data[j];
                    j++;
                }
                return std::stoi(nameOfTheFile);
            }else{
                fileSystem.close();
                return -1;
            }
        }
        i++;
    }while(!nameOfTheFile.empty());

    fileSystem.close();

    return -1;
}

/**
 * Method returns if the file is file or not
 * @param fileName name of the file
 * @return -1 - if file not exist or cluster where is the directory
 */
int Commands::getFileCluster(const std::string& fileName, int cluster){
    char data[NAME_OF_FILE_LENGTH];
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    int i = 1;
    std::string nameOfTheFile;

    do{
        fileSystem.seekp(mClusterSize * cluster + (mLengthOfFile * i));
        fileSystem.read(data, NAME_OF_FILE_LENGTH);
        nameOfTheFile = "";
        int j = 0;
        while(data[j] != 0x00){
            nameOfTheFile += data[j];
            j++;
        }
        if(nameOfTheFile == fileName){
            fileSystem.read(data, 1);
            if(*data == 0x00){
                fileSystem.read(data, std::to_string(mFileSize).size());
                fileSystem.read(data, mTableCellSize);
                j = 0;
                nameOfTheFile = "";
                while(data[j] != 0x00){
                    nameOfTheFile += data[j];
                    j++;
                }
                return std::stoi(nameOfTheFile);
            }else{
                fileSystem.close();
                return -1;
            }
        }
        i++;
    }while(!nameOfTheFile.empty());

    fileSystem.close();

    return -1;
}

/**
 * Method returns cluster of the file
 * @param fileName name of the file
 * @return cluster of file, -1 if file not exists
 */
int Commands::getCluster(const std::string& fileName, int cluster){
    char data[NAME_OF_FILE_LENGTH];
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    int i = 1;
    std::string nameOfTheFile;

    do{
        std::cout << "hehe" << std::endl;
        fileSystem.seekp(mClusterSize * cluster + (mLengthOfFile * i));
        fileSystem.read(data, NAME_OF_FILE_LENGTH);
        nameOfTheFile = "";
        int j = 0;
        while(data[j] != 0x00){
            nameOfTheFile += data[j];
            j++;
        }
        if(nameOfTheFile == fileName){
            fileSystem.read(data, 1);
            fileSystem.read(data, std::to_string(mFileSize).size());
            fileSystem.read(data, mTableCellSize);
            j = 0;
            nameOfTheFile = "";
            while(data[j] != 0x00){
                nameOfTheFile += data[j];
                j++;
            }
            return std::stoi(nameOfTheFile);
        }
        i++;
    }while(!nameOfTheFile.empty());

    fileSystem.close();

    return -1;
}

/**
 * Method returns parent cluster of the dictionary
 * @param cluster cluster where we are
 * @return parent cluster
 */
int Commands::getParentCluster(int cluster){
    char data[mTableCellSize];
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    fileSystem.seekp(mClusterSize * cluster + NAME_OF_FILE_LENGTH + 1 + std::to_string(mFileSize).size());
    fileSystem.read(data, mTableCellSize);

    std::string parentDirectoryCluster;
    int i = 0;
    while(data[i] != 0x00){
        parentDirectoryCluster += data[i];
        i++;
    }

    fileSystem.close();
    return std::stoi(parentDirectoryCluster);
}

/**
 * Method returns directory name
 * @param cluster cluster of directory
 * @return directory name
 */
std::string Commands::getDirectoryName(int cluster){
    char data[NAME_OF_FILE_LENGTH];
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    fileSystem.seekp(mClusterSize * cluster);
    fileSystem.read(data, NAME_OF_FILE_LENGTH);

    std::string directoryName;
    int i = 0;
    while(data[i] != 0x00){
        directoryName += data[i];
        i++;
    }

    fileSystem.close();
    return directoryName;
}

/**
 * Method returns a number of cluster from absolute path
 * @param vectorOfFiles parsed directories
 * @param type 0 - both (file and directory), 1 - directory, 2- file
 * @return -1 file not exist or number of a cluster
 */
int Commands::absolutePathClusterNumber(const std::vector<std::string>& vectorOfFiles, int type){
    int cluster = mStartClusterOfData;
    for(int i = 0; i < vectorOfFiles.size() - 1; i++){
        cluster = getDirectoryCluster(vectorOfFiles[i], cluster);
        if(cluster == -1){
            return -1;
        }
    }
    if(type == BOTH){
        cluster = getCluster(vectorOfFiles[vectorOfFiles.size()-1], cluster);
        if(cluster == -1){
            return -1;
        }
    }else if(type == DIRECTORY){
        cluster = getDirectoryCluster(vectorOfFiles[vectorOfFiles.size()-1], cluster);
        if(cluster == -1){
            return -1;
        }
    }else{
        cluster = getFileCluster(vectorOfFiles[vectorOfFiles.size()-1], cluster);
        if(cluster == -1){
            return -1;
        }
    }
    return cluster;
}

/**
 * Method prints all files in the cluster
 * @param cluster number of cluster
 */
void Commands::printAllFiles(int cluster){
    char data[mLengthOfFile];
    std::fstream fileSystem(mFileSystemName, std::ios::in | std::ios::binary);
    fileSystem.seekp(mClusterSize * cluster + mLengthOfFile);
    fileSystem.read(data, NAME_OF_FILE_LENGTH);
    std::string fileName;
    while(data[0] != 0x00){
        int i = 0;
        while(data[i] != 0x00){
            fileName += data[i];
            i++;
        }
        fileSystem.read(data, 1 + std::to_string(mFileSize).size() + mTableCellSize);
        if(data[0] == 0x01){
            std::cout << "DIR: ";
        }else{
            std::cout << "FILE: ";
        }
        std::cout << fileName << std::endl;
        fileName = "";
        fileSystem.read(data, NAME_OF_FILE_LENGTH);
    }
    fileSystem.close();
}

#include <iostream>

#include "Parser.h"

int main(int argc, char** argv) {

    if(argc != 1){
        std::cerr << "Error: Wrong number of parameters!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string input;
    int loop = 1;

    Parser prs;
    std::vector<std::string> myList;

    std::cin >> input;

    myList = prs.parseCommand(input);

    std::cout << myList[0] << std::endl;

    while(loop){
        std::cin >> input;

        /*for(auto & itr : myList){
            std::cout << itr << std::endl;
        }*/

        if(input == "quit"){
            loop = 0;
        }
    }

    return EXIT_SUCCESS;
}

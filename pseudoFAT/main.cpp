/**
 * The main class of pseudoFAT system
 * Created by Lukas Runt on 11.11.2022.
 */

#include <iostream>

#include "Parser.h"

int main(int argc, char** argv) {

    if(argc != 2){
        std::cerr << "Error: Wrong number of parameters!" << std::endl;
        return EXIT_FAILURE;
    }

    std::string input;
    int loop = 1;

    auto* prs = new Parser(argv[1]);
    std::vector<std::string> myList;

    while(loop){

        std::cout << "~$";

        std::getline(std::cin, input);

        loop = prs->loadCommand(input);

    }

    return EXIT_SUCCESS;
}

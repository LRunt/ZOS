//
// Created by Lenovo on 12.11.2022.
//

#ifndef PSEUDOFAT_COMMANDS_H
#define PSEUDOFAT_COMMANDS_H

#include <string>
#include <iostream>
#include "FAT.h"


class Commands {

    public:
        Commands() = default;

        bool format(const std::string& command);


    private:
        FAT::description mDescription;

};


#endif //PSEUDOFAT_COMMANDS_H

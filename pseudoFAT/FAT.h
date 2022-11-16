//
// Created by Lenovo on 12.11.2022.
//

#include <string>
#include <iostream>

#ifndef PSEUDOFAT_FAT_H
#define PSEUDOFAT_FAT_H


class FAT {
    public:
        struct description{
            int disk_size;
            int data_block_size;
            int number_of_data_blocks;
            std::string name_of_data_file;
        };

    private:
};


#endif //PSEUDOFAT_FAT_H

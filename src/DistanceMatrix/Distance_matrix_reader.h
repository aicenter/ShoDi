//
// Created by david on 2023-07-20.
//

#pragma once


#include <utility>
#include <memory>
#include <string>

#include "../constants.h"

class Distance_matrix_reader {
    virtual std::pair<std::unique_ptr<dist_t[]>, unsigned> read_matrix(const std::string& file_path) = 0;
};


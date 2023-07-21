//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 27.07.20.
//

#ifndef SHORTEST_PATHS_ERROR_H
#define SHORTEST_PATHS_ERROR_H

#include <stdexcept>



class input_error : public std::runtime_error {
public:
    explicit input_error(const std::string &arg) : std::runtime_error(arg) {}

    explicit input_error(const char* message) : std::runtime_error(message) {}
};

class not_implemented_error : public std::runtime_error {
public:
    explicit not_implemented_error(const std::string& arg) : std::runtime_error(arg) {}

    explicit not_implemented_error(const char* message) : std::runtime_error(message) {}
};

#endif //SHORTEST_PATHS_ERROR_H

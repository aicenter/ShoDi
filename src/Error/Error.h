//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 27.07.20.
//

#ifndef SHORTEST_PATHS_ERROR_H
#define SHORTEST_PATHS_ERROR_H

#include <stdexcept>

using namespace std;

class input_error : public runtime_error {
public:
    explicit input_error(const string &arg) : runtime_error(arg) {}

    explicit input_error(const char *string) : runtime_error(string) {}
};

class not_implemented_error : public runtime_error {
public:
    explicit not_implemented_error(const string &arg) : runtime_error(arg) {}

    explicit not_implemented_error(const char *string) : runtime_error(string) {}
};

#endif //SHORTEST_PATHS_ERROR_H

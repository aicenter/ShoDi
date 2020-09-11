/******************************************************************************
* File:             constants.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)
* Created:          07/29/20 
* Description:      typedefs, constants, etc
*****************************************************************************/

#ifndef CONSTANTS_H_8OFFCGXZ
#define CONSTANTS_H_8OFFCGXZ

#include <type_traits>

typedef unsigned int dist_t;

template<typename Base, typename T>
inline bool instanceof(const T&) {
    return std::is_base_of<Base, T>::value;
}

#endif /* end of include guard: CONSTANTS_H_8OFFCGXZ */

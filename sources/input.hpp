#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>

#include "manager.hpp"

class input
{
public:
    static std::vector<Manager> readManagers(const std::string&);
};

#endif


#include <fstream>

#include "input.hpp"
#include "manager.hpp"
#include "../libraries/json.hpp"

using json=nlohmann::json;

std::vector<Manager>input::readManagers(const std::string& fileName)
{
    std::vector<Manager> managerList;
    json jsonData;
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "Could not open file \"" << fileName << "\"" << std::endl;
        return managerList;
    }
    try
    {
        file>>jsonData;
    }
    catch (json::parse_error& e)
    {
        std::cerr<<"Error reading json file"<<e.what()<<"\n";
        file.close();
        return managerList;
    }

    file.close();
    for (const auto& atomObject:jsonData)
    {
        std::string name_temporary = atomObject.at("name").get<std::string>();
        std::string nationality_temporary = atomObject.at("nationality").get<std::string>();
        std::string league_temporary = atomObject.at("league").get<std::string>();
        Manager manager_temporary(name_temporary, nationality_temporary, league_temporary);
        managerList.push_back(manager_temporary);
    }
    return managerList;
}

#include "Utility/utility.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <limits>

using namespace geopter;

void Utility::split_path(const std::string &full_path, std::string &folder_path, std::string &file_base, std::string &ext)
{
    file_base = std::filesystem::path(full_path).stem().u8string();
    folder_path = std::filesystem::path(full_path).parent_path().u8string();
    ext = std::filesystem::path(full_path).extension().u8string();
}

std::vector<std::string> Utility::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::string item;
    for (auto ch: s) {
        if (ch == delim) {
            if (!item.empty()){
                elems.push_back(item);
            }
            item.clear();
        }
        else {
            item += ch;
        }
    }
    if (!item.empty()){
        elems.push_back(item);
    }

    return elems;
}


bool Utility::starts_with(const std::string &s, const std::string &prefix)
{
    auto size = prefix.size();
    if (s.size() < size) {
        return false;
    }
    return std::equal(std::begin(prefix), std::end(prefix), std::begin(s));
}

bool Utility::contains(const std::string &s, const std::string &item)
{
    if(s.find(item) != std::string::npos){
        return true;
    }else{
        return false;
    }
}


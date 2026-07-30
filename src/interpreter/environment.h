#pragma once
#include <string>
#include <unordered_map>

class Environment
{
public:
    void define(const std::string& name, int value);
    int get(const std::string& name);
    void assign(const std::string& name, int value);

private:
    std::unordered_map<std::string, int> values;
};

#include "environment.h"
#include <stdexcept>

void Environment::define(const std::string& name, int value)
{
    values[name] = value;
}

int Environment::get(const std::string& name)
{
    auto it = values.find(name);
    if (it == values.end())
    {
        throw std::runtime_error("Undefined variable '" + name + "'");
    }
    return it->second;
}

void Environment::assign(const std::string& name, int value)
{
    auto it = values.find(name);
    if (it == values.end())
    {
        throw std::runtime_error("Undefined variable '" + name + "'");
    }
    it->second = value;
}

#include "environment.h"
#include <stdexcept>

void Environment::define(const std::string& name, int value)
{
    values[name] = value;
}

int Environment::get(const std::string& name)
{
    auto it = values.find(name);
    if (it != values.end())
    {
        return it->second;
    }
    if (parent != nullptr)
    {
        return parent->get(name);
    }
    throw std::runtime_error("Undefined variable '" + name + "'");
}

void Environment::assign(const std::string& name, int value)
{
    auto it = values.find(name);
    if (it != values.end())
    {
        it->second = value;
        return;
    }
    if (parent != nullptr)
    {
        parent->assign(name, value);
        return;
    }
    throw std::runtime_error("Undefined variable '" + name + "'");
}

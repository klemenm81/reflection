#pragma once

#include "Reflectable.h"

#include <string>
#include <vector>
#include <map>

struct ExampleStruct : public Reflectable<ExampleStruct> {
    int mainValue;
    std::map<std::string, std::vector<int>> values;
};




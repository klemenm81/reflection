#pragma once

#include "../libjobject/Reflectable.h"
#include <string>

struct ParseStruct : public Reflectable<ParseStruct> {
	bool is_new = false;
	std::string name;
	std::string address;
};
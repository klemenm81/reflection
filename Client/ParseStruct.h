#pragma once

#include "../libjobject/Reflectable.h"
#include <string>

struct ParseStruct : public Reflectable<ParseStruct> {
	bool is_new = false;
	std::string name;
	std::optional<std::string> address;
	int age;

	std::string help_is_new() {
		return "Specify this option, if you want to mark the entry as new";
	}

	std::string help_name() {
		return "Name of the entry";
	}

	std::string help_address() {
		return "Address of the entry";
	}

	std::string help_age() {
		return "Age of the entry";
	}
};
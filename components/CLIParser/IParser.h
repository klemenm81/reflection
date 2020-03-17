#pragma once

#include "../../libjobject/Object.h"

class IParser {
public:
	virtual void Parse(int argc, char** argv, Object& cliStructure) = 0;
};
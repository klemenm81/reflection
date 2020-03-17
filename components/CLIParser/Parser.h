#pragma once

#include "IParser.h"

#include "../../libjobject/Object.h"
#include "../../libjobject/Class.h"
#include "../../libjobject/Reflectable.h"
#include <vector>
#include <string>

class Parser : public IParser, public Reflectable<Parser> {
protected:
	void PrintUsage(Object& cliStructure);

public:
	void Parse(int argc, char** argv, Object& cliStructure);
};

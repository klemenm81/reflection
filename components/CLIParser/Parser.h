#pragma once

#include "IParser.h"

#include "../../libjobject/Object.h"
#include "../../libjobject/Class.h"
#include "../../libjobject/Reflectable.h"
#include <vector>
#include <string>

class Parser : public IParser, public Reflectable<Parser> {
protected:
	void printUsage(Object& cliStructure);

public:
	void parse(int argc, char** argv, Object& cliStructure);
};

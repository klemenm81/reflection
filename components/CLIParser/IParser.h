#pragma once

#include "../../libjobject/Object.h"
#include "../../libjobject/exceptions/Exception.h"

class IParser {
public:
	virtual void parse(int argc, char** argv, Object& cliStructure) = 0;
	virtual void printUsage(Object& cliStructure) = 0;
};

class IParseException : public Exception {
};
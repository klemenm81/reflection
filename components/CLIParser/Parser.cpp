#pragma once

#include "Parser.h"

#include "../../libjobject/Object.h"
#include <vector>
#include <string>

void Parser::PrintUsage(Object& cliStructure) {
}

void Parser::Parse(int argc, char** argv, Object& cliStructure) {
	std::vector<Field> fields = cliStructure.GetClass().GetFields();

	for (Field field : fields) {
		bool optionFound = false;
		for (int iArg = 0; iArg < argc; iArg++) {
			char* option = argv[iArg];
			if (option[0] == '-' && std::string(option + 1) == field.GetName()) {
				if (iArg + 1 < argc) {
					field.Deserialize(cliStructure, argv[iArg + 1]);
					optionFound = true;
					break;
				}
				else {
					PrintUsage(cliStructure);
					return;
				}
			}
		}

		if (!optionFound) {
			PrintUsage(cliStructure);
			return;
		}
	}
}


#pragma once

#include "Parser.h"

#include "../../libjobject/Object.h"
#include <vector>
#include <string>

void Parser::printUsage(Object& cliStructure) {
}

void Parser::parse(int argc, char** argv, Object& cliStructure) {
	std::vector<Field> fields = cliStructure.getClass().getFields();

	for (Field field : fields) {
		bool optionFound = false;
		for (int iArg = 1; iArg < argc; iArg++) {
			char* option = argv[iArg];
			if (option[0] == '-' && std::string(option + 1) == field.getName()) {
				if (field.isType<bool>()) {
					field.set<bool>(cliStructure, true);
					optionFound = true;
					break;
				}
				else {
					if (iArg + 1 < argc) {
						field.deserialize(cliStructure, argv[iArg + 1]);
						optionFound = true;
						break;
					}
					else {
						printUsage(cliStructure);
						return;
					}
				}
			}
		}

		if (!optionFound && !field.isType<bool>()) {
			printUsage(cliStructure);
			return;
		}
	}
}


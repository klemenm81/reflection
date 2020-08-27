#pragma once

#include "Parser.h"

#include "../../libjobject/Object.h"
#include <vector>
#include <string>
#include <iostream>

#include "ParseException.h"

void Parser::printUsage(Object& cliStructure) {
	std::vector<Field> fields = cliStructure.getClass().getFields();
	std::cout << "Usage:" << std::endl;
	for (Field field : fields) {
		std::cout << "\t";
		if (field.isOptional() || field.isType<bool>()) {
			std::cout << "[";
		}
		std::cout << "-" << field.getName();
		if (!field.isType<bool>()) {
			std::cout << " <" << field.getName() << ">";
		}
		if (field.isOptional() || field.isType<bool>()) {
			std::cout << "]";
		}

		std::cout << "\t\t\t";

		try {
			std::string methodName = "help_";
			methodName += field.getName();
			Method helpMethod = cliStructure.getClass().getMethod(methodName.c_str());
			std::string helpMsg = helpMethod.invoke<std::string>(cliStructure);
			std::cout << " " << helpMsg;
		}
		catch (const Exception&) {
		}

		std::cout << std::endl;
	}
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
						field.fromString(cliStructure, argv[iArg + 1]);
						optionFound = true;
						break;
					}
					else {
						std::string error = "Failed to parse option ";
						error += field.getName();
						throw ParseException(error.c_str());
					}
				}
			}
		}

		if (!optionFound && (!(field.isType<bool>() || field.isOptional()))) {
			std::string error = "Option ";
			error += field.getName();
			error += " not specified";
			throw ParseException(error.c_str());
		}
	}
}


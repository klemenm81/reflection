#pragma once

#include "Exception.h"
#include <string>

class MethodsWithNArgumentsNotMatchingInputArguments : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodsWithNArgumentsNotMatchingInputArguments(const char *methodName, size_t nArguments) : m_errorMsg(
		std::string("Methods ") +
		std::string(methodName) +
		std::string(" with ") +
		std::to_string(nArguments) +
		std::string(" arguments not matching with input arguments.")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
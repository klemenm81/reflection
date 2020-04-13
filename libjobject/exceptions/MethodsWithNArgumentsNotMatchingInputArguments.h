#pragma once

#include "Exception.h"
#include <string>

class MethodsWithNArgumentsNotMatchingInputArguments : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodsWithNArgumentsNotMatchingInputArguments(size_t nArguments) : m_errorMsg(
		std::string("Methods with ") +
		std::to_string(nArguments) +
		std::string(" arguments not matching with input arguments.")
	) {
	}

	MethodsWithNArgumentsNotMatchingInputArguments(size_t nArguments, const Exception& other) : m_errorMsg(
		std::string("Methods with ") +
		std::to_string(nArguments) +
		std::string(" arguments not matching with input arguments: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
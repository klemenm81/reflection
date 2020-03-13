#pragma once

#include "Exception.h"
#include <string>

class MethodWithNArgumentsNotFound : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodWithNArgumentsNotFound(size_t nArguments) : m_errorMsg(
		std::string("Method with ") +
		std::to_string(nArguments) +
		std::string(" arguments not found.")
	) {
	}

	MethodWithNArgumentsNotFound(size_t nArguments, const Exception& other) : m_errorMsg(
		std::string("Method with ") +
		std::to_string(nArguments) +
		std::string(" arguments not found: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
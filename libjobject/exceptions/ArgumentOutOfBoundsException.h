#pragma once

#include "Exception.h"
#include "../Qualifier.h"
#include <string>

class ArgumentOutOfBoundsException : public Exception {
private:
	std::string m_errorMsg;
public:
	ArgumentOutOfBoundsException(size_t iArg, size_t nArgs) : m_errorMsg(
		std::string("Method accepts ") + 
		std::to_string(nArgs) +
		std::string(" arguments. Requested argument ") + 
		std::to_string(iArg) + 
		std::string(" is out of bounds.")
	) {
	}

	ArgumentOutOfBoundsException(size_t iArg, size_t nArgs, const Exception& other) : m_errorMsg(
		std::string("Method accepts ") +
		std::to_string(nArgs) +
		std::string(" arguments. Requested argument ") +
		std::to_string(iArg) +
		std::string(" is out of bounds: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
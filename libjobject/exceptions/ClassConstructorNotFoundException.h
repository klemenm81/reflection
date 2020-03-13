#pragma once

#include "Exception.h"
#include <string>

class ClassConstructorNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	ClassConstructorNotFoundException(const char *argsName) : m_errorMsg(
		std::string("Class constructor with args ") +
		std::string(argsName) +
		std::string(" not found.")
	) {
	}

	ClassConstructorNotFoundException(const char * argsName, const Exception& other) : m_errorMsg(
		std::string("Class constructor with args ") +
		std::string(argsName) +
		std::string(" not found: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
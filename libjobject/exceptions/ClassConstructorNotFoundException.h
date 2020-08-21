#pragma once

#include "Exception.h"
#include <string>

class ClassConstructorNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	ClassConstructorNotFoundException(const char *argsName, const char *className) : m_errorMsg(
		std::string("Class constructor with args ") +
		std::string(argsName) +
		std::string(" not found for class ") +
		std::string(className) +
		std::string(".")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
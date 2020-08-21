#pragma once

#include "Exception.h"
#include <string>

class MethodNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodNotFoundException(const char *methodName, const char *className) : m_errorMsg(
		std::string("Method ") +
		std::string(methodName) +
		std::string(" not found in class ") +
		std::string(className) + 
		std::string(".")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
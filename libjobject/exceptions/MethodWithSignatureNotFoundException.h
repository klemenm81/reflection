#pragma once

#include "Exception.h"
#include <string>

class MethodWithSignatureNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodWithSignatureNotFoundException(const char *methodName, const char* argsName) : m_errorMsg(
		std::string("Method ") + 
		std::string(methodName) +
		std::string("(") +
		std::string(argsName) +
		std::string(") not found.") 
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
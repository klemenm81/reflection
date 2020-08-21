#pragma once

#include "Exception.h"
#include "../Qualifier.h"
#include <string>



class MethodWithQualifierNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodWithQualifierNotFoundException(const char *methodName, const char *argsName, Qualifier qualifier) : m_errorMsg(
		std::string("Method ") +
		std::string(methodName) +
		std::string("(") +
		std::string(argsName) +
		std::string(") ") +
		std::string(" with qualifier ") +
		std::to_string(qualifier) +
		std::string(" not found.")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
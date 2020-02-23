#pragma once

#include "Exception.h"
#include <string>

class MethodNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodNotFoundException(const char *name) : m_errorMsg(
		std::string("Method ") +
		std::string(name) +
		std::string(" not found.")
	) {
	}

	MethodNotFoundException(const char *name, const Exception& other) : m_errorMsg(
		std::string("Method") +
		std::string(name) +
		std::string(" not found: \n") +
		std::string(other.GetMessage())
	) {
	}

	const char* GetMessage() const {
		return m_errorMsg.c_str();
	}
};
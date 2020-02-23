#pragma once

#include "Exception.h"
#include <string>

class MethodWithSignatureNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodWithSignatureNotFoundException(const char* argsName) : m_errorMsg(
		std::string("Method with signature ") + 
		std::string(argsName) +
		std::string(" not found.")
	) {
	}

	MethodWithSignatureNotFoundException(const char* argsName, const Exception& other) : m_errorMsg(
		std::string("Method with signature ") + 
		std::string(argsName) +
		std::string(" not found: \n") + 
		std::string(other.GetMessage())
	) {
	}

	const char* GetMessage() const {
		return m_errorMsg.c_str();
	}
};
#pragma once

#include "Exception.h"
#include <string>

class MethodContainingSignatureNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodContainingSignatureNotFoundException(const char* argsName) : m_errorMsg(
		std::string("Method containing signature ") +
		std::string(argsName) +
		std::string(" not found.")
	) {
	}

	MethodContainingSignatureNotFoundException(const char* argsName, const Exception& other) : m_errorMsg(
		std::string("Method containing signature ") +
		std::string(argsName) +
		std::string(" not found: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
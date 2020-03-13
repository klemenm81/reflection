#pragma once

#include "Exception.h"
#include <string>

class ClassNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	ClassNotFoundException(const char *name) : m_errorMsg(
		std::string("Class ") +
		std::string(name) +
		std::string(" not found.")
	) {
	}

	ClassNotFoundException(const char *name, const Exception& other) : m_errorMsg(
		std::string("Class ") +
		std::string(name) +
		std::string(" not found: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
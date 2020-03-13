#pragma once

#include "Exception.h"
#include <string>

class FieldNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	FieldNotFoundException(const char *name) : m_errorMsg(
		std::string("Field ") +
		std::string(name) +
		std::string(" not found.")
	) {
	}

	FieldNotFoundException(const char *name, const Exception& other) : m_errorMsg(
		std::string("Field ") +
		std::string(name) +
		std::string(" not found: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
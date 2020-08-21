#pragma once

#include "Exception.h"
#include <string>

class FieldNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	FieldNotFoundException(const char *fieldName, const char *className) : m_errorMsg(
		std::string("Field ") +
		std::string(fieldName) +
		std::string(" not found in class ") +
		std::string(className) + 
		std::string(".")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
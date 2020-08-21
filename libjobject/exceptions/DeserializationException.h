#pragma once

#include "Exception.h"
#include <string>

class DeserializationException : public Exception {
private:
	std::string m_errorMsg;
public:
	DeserializationException(const char* name) : m_errorMsg(
		std::string("Failed to deserialize ") +
		std::string(name) +
		std::string(".")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
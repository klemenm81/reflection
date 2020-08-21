#pragma once

#include "Exception.h"
#include <string>

class SerializationException : public Exception {
private:
	std::string m_errorMsg;
public:
	SerializationException(const char* name) : m_errorMsg(
		std::string("Failed to serialize ") +
		std::string(name) +
		std::string(".")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
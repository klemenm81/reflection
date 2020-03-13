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

	SerializationException(const char* name, const Exception& other) : m_errorMsg(
		std::string("Failed to serialize ") +
		std::string(name) +
		std::string(": \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
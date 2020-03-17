#pragma once

#include "Exception.h"
#include <string>

class MarshallingException : public Exception {
private:
	std::string m_errorMsg;
public:
	MarshallingException(const char* name) : m_errorMsg(name) {
	}

	MarshallingException(const char* name, const Exception& other) : m_errorMsg(
		std::string(name) +
		std::string(": \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
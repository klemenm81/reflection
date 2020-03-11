#pragma once

#include "Exception.h"
#include <string>

class CastNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	CastNotFoundException(const char *name) : m_errorMsg(
		std::string("Cast ") +
		std::string(name) +
		std::string(" not found.")
	) {
	}

	CastNotFoundException(const char *name, const Exception& other) : m_errorMsg(
		std::string("Cast ") +
		std::string(name) +
		std::string(" not found: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
#pragma once

#include "Exception.h"
#include <string>

class CastNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	CastNotFoundException(const char *castName, const char *className) : m_errorMsg(
		std::string("Cast ") +
		std::string(castName) +
		std::string(" not found for class ") +
		std::string(className) +
		std::string(".")
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
#pragma once

#include "Exception.h"
#include <string>

class InternalErrorException : public Exception {
private:
	std::string m_errorMsg;
public:
	InternalErrorException(const char* errorMsg) : m_errorMsg(errorMsg) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
#pragma once

#include "IParser.h"
#include <string>

class ParseException : public IParseException {
private:
	std::string m_msg;

	const char* Message() const {
		return m_msg.c_str();
	}

public:
	ParseException(const char* msg) : m_msg(msg) {
	}
};
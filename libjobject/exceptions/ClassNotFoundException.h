#pragma once

#include "Exception.h"
#include <string>

class ClassNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	ClassNotFoundException(const char *name, const char *libraryName) : m_errorMsg(
		std::string("Class ") +
		std::string(name) +
		std::string(" not found.")
	) {
		m_errorMsg = std::string(libraryName).empty() ?
			std::string("Class ") +
			std::string(name) +
			std::string(" not found in executing image.") :
			std::string("Class ") +
			std::string(name) +
			std::string(" not found in library ") +
			std::string(libraryName) +
			std::string(".");
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
#pragma once

#include "Exception.h"
#include "../Qualifier.h"
#include <string>



class MethodWithQualifierNotFoundException : public Exception {
private:
	std::string m_errorMsg;
public:
	MethodWithQualifierNotFoundException(Qualifier qualifier) : m_errorMsg(
		std::string("Method with qualifier ") +
		std::to_string(qualifier) +
		std::string(" not found.")
	) {
	}

	MethodWithQualifierNotFoundException(Qualifier qualifier, const Exception& other) : m_errorMsg(
		std::string("Method with qualifier ") +
		std::to_string(qualifier) +
		std::string(" not found: \n") +
		std::string(other.Message())
	) {
	}

	const char* Message() const {
		return m_errorMsg.c_str();
	}
};
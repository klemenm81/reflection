#pragma once

#include <map>
#include <string>

#include "IMethod2.h"
#include "exceptions/MethodWithSignatureNotFoundException.h"
#include "exceptions/MethodWithQualifierNotFoundException.h"
 
class CMethodQualifiers {
private:
	std::map<Qualifier, IMethod2 *> m_qualifiedMethods;

public:
	void AddMethod(IMethod2& method) {
		m_qualifiedMethods[method.GetQualifier()] = &method;
	}

	IMethod2 &GetMethod(Qualifier qualifier) {
		IMethod2* method = (m_qualifiedMethods.find(qualifier) != m_qualifiedMethods.end()) ?
			m_qualifiedMethods[qualifier] :
			nullptr;

		if (method == nullptr) {
			throw MethodWithQualifierNotFoundException(qualifier);
		}

		return *m_qualifiedMethods[qualifier];
	}
};


class IMethodOverloads {
private:
	std::map<std::string, CMethodQualifiers *> m_overloadedMethods;

	void AddMethod(IMethod2& method) {
		CMethodQualifiers* methodQualifiers = (m_overloadedMethods.find(method.GetArgsSignature()) != m_overloadedMethods.end()) ?
			m_overloadedMethods[method.GetArgsSignature()] :
			nullptr;

		if (methodQualifiers == nullptr) {
			methodQualifiers = new CMethodQualifiers();
			m_overloadedMethods[method.GetArgsSignature()] = methodQualifiers;
		}

		methodQualifiers->AddMethod(method);
	}

	IMethod& GetMethod(const char* argsSignature, const char *argsName, Qualifier qualifier) {
		CMethodQualifiers* methodQualifiers = (m_overloadedMethods.find(argsSignature) != m_overloadedMethods.end()) ?
			m_overloadedMethods[argsSignature] :
			nullptr;

		if (methodQualifiers == nullptr) {
			throw MethodWithSignatureNotFoundException(argsName);
		}

		methodQualifiers->GetMethod(qualifier);
	}
};

#pragma once

#include <map>
#include <string>

#include "IAdaptor.h"
#include "Qualifier.h"
#include "Object.h"

#include "exceptions/MethodWithSignatureNotFoundException.h"
#include "exceptions/MethodWithQualifierNotFoundException.h"
#include "exceptions/MethodContainingSignatureNotFoundException.h"
#include "exceptions/InternalErrorException.h"

class IMethod2 {
public:
	virtual const char* GetName() = 0;
	virtual size_t GetNArgs() = 0;
	virtual std::byte* GetArgBuffer(size_t iArg) = 0;
	virtual const char* GetArgsSignature() = 0;
	virtual const char* GetArgsName() = 0;
	virtual const char* GetRetValSignature() = 0;
	virtual const char* GetRetValName() = 0;
	virtual Qualifier GetQualifier() = 0;
	virtual IAdaptor* Invoke(Object &obj, IAdaptor **args) = 0;
	virtual IAdaptor* Invoke(const Object& obj, IAdaptor** args) = 0;
	virtual IAdaptor* Invoke(volatile Object& obj, IAdaptor** args) = 0;
	virtual IAdaptor* Invoke(const volatile Object& obj, IAdaptor** args) = 0;
	virtual IAdaptor* Invoke(Object&& obj, IAdaptor** args) = 0;
	virtual IAdaptor* Invoke(const Object&& obj, IAdaptor** args) = 0;
	virtual IAdaptor* Invoke(volatile Object&& obj, IAdaptor** args) = 0;
	virtual IAdaptor* Invoke(const volatile Object&& obj, IAdaptor** args) = 0;

	virtual Json::Value InvokeMarshalled(Object& obj, Json::Value args) {
		throw;
	}
	virtual ~IMethod2() {
	}
};

class IMethodQualifiers {
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

	IMethod2& GetAnyMethod() {
		for (auto it = m_qualifiedMethods.begin(); it != m_qualifiedMethods.end(); it++) {
			if (it->second != nullptr) {
				return *it->second;
			}
		}

		throw InternalErrorException("There should be at least one method with specific signature!!!");
	}
};


class IMethodOverloads {
private:
	std::map<std::string, IMethodQualifiers *> m_overloadedMethods;
	std::map<size_t, std::vector<IMethod2*>> m_methodOverloadMapByNArgs;

public:
	void AddMethod(IMethod2& method) {
		IMethodQualifiers* methodQualifiers = (m_overloadedMethods.find(method.GetArgsSignature()) != m_overloadedMethods.end()) ?
			m_overloadedMethods[method.GetArgsSignature()] :
			nullptr;

		if (methodQualifiers == nullptr) {
			methodQualifiers = new IMethodQualifiers();
			m_overloadedMethods[method.GetArgsSignature()] = methodQualifiers;
		}

		methodQualifiers->AddMethod(method);

		m_methodOverloadMapByNArgs[method.GetNArgs()].push_back(&method);
	}

	IMethod2& GetMethod(const char* argsSignature, const char *argsName, Qualifier qualifier) {
		IMethodQualifiers* methodQualifiers = (m_overloadedMethods.find(argsSignature) != m_overloadedMethods.end()) ?
			m_overloadedMethods[argsSignature] :
			nullptr;

		if (methodQualifiers == nullptr) {
			throw MethodWithSignatureNotFoundException(argsName);
		}

		return methodQualifiers->GetMethod(qualifier);
	}

	IMethod2& GetMethodContainingSignature(const char* argsSignature, const char* argsName) {
		for (auto it = m_overloadedMethods.begin(); it != m_overloadedMethods.end(); it++) {
			if (!strncmp(it->first.c_str(), argsSignature, strlen(argsSignature))) {
				IMethodQualifiers& methodQualifiers = *(it->second);
				return methodQualifiers.GetAnyMethod();
			}
		}

		throw MethodContainingSignatureNotFoundException(argsName);
	}

	IMethod2** GetMethodsByNArgs(size_t nArgs, size_t& nMethods) {
		std::vector<IMethod2*>& methods = m_methodOverloadMapByNArgs[nArgs];
		nMethods = methods.size();
		return methods.data();
	}
};

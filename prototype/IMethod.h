#pragma once

#include <map>
#include <string>

#include "IAdaptor.h"
#include "Qualifier.h"
#include "Object.h"

#include "exceptions/MethodWithSignatureNotFoundException.h"
#include "exceptions/MethodWithQualifierNotFoundException.h"
#include "exceptions/MethodContainingSignatureNotFoundException.h"
#include "exceptions/MethodWithNArgumentsNotFound.h"
#include "exceptions/InternalErrorException.h"

class IMethodInvoker {
public:
	virtual const char* GetName() const = 0;
	virtual size_t GetNArgs() const = 0;
	virtual std::byte* GetArgBuffer(size_t iArg) const = 0;
	virtual const char* GetArgsSignature() const = 0;
	virtual const char* GetArgsName() const = 0;
	virtual const char* GetRetValSignature() const = 0;
	virtual const char* GetRetValName() const = 0;
	virtual Qualifier GetQualifier() const = 0;
	virtual IAdaptor* Invoke(Object &obj, IAdaptor **args) const = 0;
	virtual IAdaptor* Invoke(const Object& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* Invoke(volatile Object& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* Invoke(const volatile Object& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* Invoke(Object&& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* Invoke(const Object&& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* Invoke(volatile Object&& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* Invoke(const volatile Object&& obj, IAdaptor** args) const = 0;

	virtual Json::Value InvokeMarshalled(Object& obj, Json::Value args) const {
		throw;
	}
	virtual ~IMethodInvoker() {
	}
};


class IMethod {
private:
	std::string m_name;
	std::map<std::string, std::map<Qualifier, IMethodInvoker*>> m_methodOverloadMap;
	std::map<size_t, std::vector<IMethodInvoker*>> m_methodOverloadMapByNArgs;

public:
	IMethod(const char* name) : m_name(name) {
	}

	const char* GetName() const {
		return m_name.c_str();
	}

	void AddMethod(IMethodInvoker& methodInvoker) {
		std::map<Qualifier, IMethodInvoker*> &methodInvokers = m_methodOverloadMap[methodInvoker.GetArgsSignature()];
		methodInvokers[methodInvoker.GetQualifier()] = &methodInvoker;
		m_methodOverloadMapByNArgs[methodInvoker.GetNArgs()].push_back(&methodInvoker);
	}

	const IMethodInvoker& GetMethod(const char* argsSignature, const char *argsName, Qualifier qualifier) const {
		auto methodInvokers = m_methodOverloadMap.find(argsSignature);
		if (methodInvokers != m_methodOverloadMap.end()) {
			auto invoker = methodInvokers->second.find(qualifier);
			if (invoker != methodInvokers->second.end()) {
				return *(invoker->second);
			}
			else {
				throw MethodWithQualifierNotFoundException(qualifier);
			}
		}
		else {
			throw MethodWithSignatureNotFoundException(argsName);
		}
	}

	const IMethodInvoker& GetMethodContainingSignature(const char* argsSignature, const char* argsName) const {
		for (auto it = m_methodOverloadMap.begin(); it != m_methodOverloadMap.end(); it++) {
			if (!strncmp(it->first.c_str(), argsSignature, strlen(argsSignature))) {
				const std::map<Qualifier, IMethodInvoker*>& methodInvokers = it->second;
				for (auto it = methodInvokers.begin(); it != methodInvokers.end(); it++) {
					if (it->second != nullptr) {
						return *it->second;
					}
				}
			}
		}

		throw MethodContainingSignatureNotFoundException(argsName);
	}

	IMethodInvoker* const* GetMethodsByNArgs(size_t nArgs, size_t& nMethods) const {
		auto methods = m_methodOverloadMapByNArgs.find(nArgs);
		if (methods != m_methodOverloadMapByNArgs.end()) {
			nMethods = methods->second.size();
			return methods->second.data();
		}
		else {
			throw MethodWithNArgumentsNotFound(nArgs);
		}
	}
};

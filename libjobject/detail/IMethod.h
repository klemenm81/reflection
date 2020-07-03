#pragma once

#include "IAdaptor.h"
#include "../Qualifier.h"
#include "../Object.h"
#include "../exceptions/MethodWithSignatureNotFoundException.h"
#include "../exceptions/MethodWithQualifierNotFoundException.h"
#include "../exceptions/MethodContainingSignatureNotFoundException.h"
#include "../exceptions/MethodWithNArgumentsNotFound.h"
#include "../exceptions/InternalErrorException.h"
#include <map>
#include <string>
#include <cstddef>

class IMethodInvoker {
public:
	virtual const char* getName() const = 0;
	virtual size_t getNArgs() const = 0;
	virtual std::byte* getArgBuffer(size_t iArg) const = 0;
	virtual const char* getArgsSignature() const = 0;
	virtual const char* getArgsName() const = 0;
	virtual const char* getRetValSignature() const = 0;
	virtual const char* getRetValName() const = 0;
	virtual Qualifier getQualifier() const = 0;
	virtual IAdaptor* invoke(Object &obj, IAdaptor **args) const = 0;
	virtual IAdaptor* invoke(const Object& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* invoke(volatile Object& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* invoke(const volatile Object& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* invoke(Object&& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* invoke(const Object&& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* invoke(volatile Object&& obj, IAdaptor** args) const = 0;
	virtual IAdaptor* invoke(const volatile Object&& obj, IAdaptor** args) const = 0;
	virtual Json::Value invokeMarshalled(Object& obj, Json::Value args) const = 0; 
	virtual Json::Value invokeMarshalled(const Object& obj, Json::Value args) const = 0; 
	virtual Json::Value invokeMarshalled(volatile Object& obj, Json::Value args) const = 0;
	virtual Json::Value invokeMarshalled(const volatile Object& obj, Json::Value args) const = 0;
	virtual Json::Value invokeMarshalled(Object&& obj, Json::Value args) const = 0;
	virtual Json::Value invokeMarshalled(const Object&& obj, Json::Value args) const = 0;
	virtual Json::Value invokeMarshalled(volatile Object&& obj, Json::Value args) const = 0;
	virtual Json::Value invokeMarshalled(const volatile Object&& obj, Json::Value args) const = 0;
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

	const char* getName() const {
		return m_name.c_str();
	}

	void addMethod(IMethodInvoker& methodInvoker) {
		std::map<Qualifier, IMethodInvoker*> &methodInvokers = m_methodOverloadMap[methodInvoker.getArgsSignature()];
		methodInvokers[methodInvoker.getQualifier()] = &methodInvoker;
		m_methodOverloadMapByNArgs[methodInvoker.getNArgs()].push_back(&methodInvoker);
	}

	const IMethodInvoker& getMethod(const char* argsSignature, const char *argsName, Qualifier qualifier) const {
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

	const IMethodInvoker& getMethodContainingSignature(const char* argsSignature, const char* argsName) const {
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

	IMethodInvoker* const* getMethodsByNArgs(size_t nArgs, size_t& nMethods) const {
		auto methods = m_methodOverloadMapByNArgs.find(nArgs);
		if (methods != m_methodOverloadMapByNArgs.end()) {
			nMethods = methods->second.size();
			return methods->second.data();
		}
		else {
			throw MethodWithNArgumentsNotFound(m_name.c_str(), nArgs);
		}
	}
};

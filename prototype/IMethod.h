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

class IMethodInvoker {
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
	virtual ~IMethodInvoker() {
	}
};


class IMethod {
private:
	std::map<std::string, std::map<Qualifier, IMethodInvoker*>> m_methodOverloadMap;
	std::map<size_t, std::vector<IMethodInvoker*>> m_methodOverloadMapByNArgs;

public:
	void AddMethod(IMethodInvoker& methodInvoker) {
		std::map<Qualifier, IMethodInvoker*> &methodInvokers = m_methodOverloadMap[methodInvoker.GetArgsSignature()];
		methodInvokers[methodInvoker.GetQualifier()] = &methodInvoker;
		m_methodOverloadMapByNArgs[methodInvoker.GetNArgs()].push_back(&methodInvoker);
	}

	IMethodInvoker& GetMethod(const char* argsSignature, const char *argsName, Qualifier qualifier) {
		std::map<Qualifier, IMethodInvoker*>& methodInvokers = m_methodOverloadMap.find(argsSignature) != m_methodOverloadMap.end() ?
			m_methodOverloadMap[argsSignature] : 
			throw MethodWithSignatureNotFoundException(argsName);
		IMethodInvoker& invoker =  methodInvokers.find(qualifier) != methodInvokers.end() ? 
			*methodInvokers[qualifier] :
			throw MethodWithQualifierNotFoundException(qualifier);
		return invoker;
	}

	IMethodInvoker& GetMethodContainingSignature(const char* argsSignature, const char* argsName) {
		for (auto it = m_methodOverloadMap.begin(); it != m_methodOverloadMap.end(); it++) {
			if (!strncmp(it->first.c_str(), argsSignature, strlen(argsSignature))) {
				std::map<Qualifier, IMethodInvoker*>& methodInvokers = it->second;
				for (auto it = methodInvokers.begin(); it != methodInvokers.end(); it++) {
					if (it->second != nullptr) {
						return *it->second;
					}
				}
			}
		}

		throw MethodContainingSignatureNotFoundException(argsName);
	}

	IMethodInvoker** GetMethodsByNArgs(size_t nArgs, size_t& nMethods) {
		std::vector<IMethodInvoker*>& methods = m_methodOverloadMapByNArgs[nArgs];
		nMethods = methods.size();
		return methods.data();
	}
};

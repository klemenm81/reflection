#pragma once

#include "IAdaptor.h"
#include "Qualifier.h"
#include "Object.h"

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

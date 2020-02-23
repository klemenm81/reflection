#pragma once


#include "IMethod.h"
#include "IAdaptor.h"
#include "Qualifier.h"

class IMethod2 : public IMethod {
public:
	virtual const char* GetName() = 0;
	virtual std::byte* GetArgBuffer(size_t iArg) = 0;
	virtual const char* GetArgsSignature() = 0;
	virtual const char* GetArgsName() = 0;
	virtual const char* GetRetValSignature() = 0;
	virtual const char* GetRetValName() = 0;
	virtual Qualifier GetQualifier() = 0;
	virtual IAdaptor* Invoke(Object &obj, IAdaptor **args) { throw; };
	virtual IAdaptor* Invoke(const Object& obj, IAdaptor** args) { throw; };
	virtual IAdaptor* Invoke(volatile Object& obj, IAdaptor** args) { throw; };
	virtual IAdaptor* Invoke(volatile const Object& obj, IAdaptor** args) { throw; };
	virtual IAdaptor* Invoke(Object&& obj, IAdaptor** args) { throw; };
	virtual IAdaptor* Invoke(const Object&& obj, IAdaptor** args) { throw; };
	virtual IAdaptor* Invoke(volatile Object&& obj, IAdaptor** args) { throw; };
	virtual IAdaptor* Invoke(volatile const Object&& obj, IAdaptor** args) { throw; };
	virtual ~IMethod2() {
	}
};

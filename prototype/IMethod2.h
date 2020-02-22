#pragma once


#include "IMethod.h"
#include "IAdaptor.h"

class IMethod2 : public IMethod {
public:
	virtual std::byte* GetArgBuffer(size_t iArg) = 0;
	virtual IAdaptor* Invoke(IAdaptor& object, IAdaptor **args) { throw; };
	virtual ~IMethod2() {
	}
};

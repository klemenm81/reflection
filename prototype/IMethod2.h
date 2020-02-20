#pragma once


#include "IMethod.h"
#include "IAdaptor.h"

class IMethod2 : public IMethod {
public:
	virtual IAdaptor& Invoke(IAdaptor& object, IAdaptor **args) { throw; };
	virtual ~IMethod2() {
	}
};

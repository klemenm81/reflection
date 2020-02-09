#pragma once


#include "IMethod.h"
#include "Adaptor2.h"

class IMethod2 : public IMethod {
public:
	virtual void Invoke(IAdaptor& retVal, IAdaptor& adaptor, std::vector<Adaptor2> &&args) { throw; };
	virtual ~IMethod2() {
	}
};

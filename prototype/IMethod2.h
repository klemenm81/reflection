#pragma once


#include "IMethod.h"
#include "Adaptor.h"

class IMethod2 : public IMethod {
public:
	virtual IAdaptor& Invoke(IAdaptor& object, std::vector<Adaptor>& args) { throw; };
	virtual IAdaptor& Invoke(IAdaptor& object, std::vector<Adaptor> &&args) { throw; };
	virtual ~IMethod2() {
	}
};

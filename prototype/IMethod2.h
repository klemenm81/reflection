#pragma once


#include "IMethod.h"
#include "Adaptor2.h"

class IMethod2 : public IMethod {
public:
	virtual IAdaptor& Invoke(std::byte *retValBuffer, IAdaptor& object, std::vector<IAdaptor*> &&args) { throw; };
	virtual ~IMethod2() {
	}
};

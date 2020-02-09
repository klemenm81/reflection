#pragma once


#include "IMethod.h"
#include "Adaptor.h"

class IMethod2 : public IMethod {
public:
	virtual IAdaptor& Invoke(std::byte *retValBuffer, IAdaptor& object, std::vector<Adaptor> &&args) { throw; };
	virtual ~IMethod2() {
	}
};

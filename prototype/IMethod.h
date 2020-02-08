#pragma once

#include <vector>

class IAdaptor;
class IReflectable;

class IMethod {
public:
	virtual IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) { throw; };
	virtual ~IMethod() {
	}
};

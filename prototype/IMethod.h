#pragma once

#include <vector>

class IAdaptor;
class Object;

class IMethod {
public:
	virtual IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) { throw; };
	virtual ~IMethod() {
	}
};

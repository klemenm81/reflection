#pragma once

#include <vector>

class IAdaptor;
class IReflectable;

class IMethod {
public:
	virtual IAdaptor& Invoke(IReflectable &obj, std::vector<IAdaptor*> args) = 0;
	virtual ~IMethod() {
	}
};

#pragma once

#include <vector>
#include "IAdaptor.h"
#include "IReflectable.h"

class IMethod {
public:
	virtual IAdaptor& Invoke(IReflectable &obj, std::vector<IAdaptor*> args) = 0;
	virtual ~IMethod() {
	}
};

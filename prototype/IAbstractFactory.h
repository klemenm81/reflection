#pragma once

#include "IConstructor.h"

class IAbstractFactory {
public:
	virtual IConstructor& GetInstantiator(const char* className, const char* argsSignature, const char* argsName) = 0;
	virtual ~IAbstractFactory() {
	}
};

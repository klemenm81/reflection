#pragma once

#include "IInstantiator.h"

class IAbstractFactory {
public:
	virtual IInstantiator& GetInstantiator(const char* className, const char* argsSignature, const char* argsName) = 0;
	virtual ~IAbstractFactory() {
	}
};

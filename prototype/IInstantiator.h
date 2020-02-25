#pragma once

#include "Object.h"

class IInstantiator {
public:
	virtual Object& Instantiate() = 0;
	virtual ~IInstantiator() {
	}
};

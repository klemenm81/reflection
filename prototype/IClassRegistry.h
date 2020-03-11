#pragma once

#include "IClass.h"

class IClassRegistry {
public:
	virtual IClass& GetClass(const char *name) = 0;
};

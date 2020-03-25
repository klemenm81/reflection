#pragma once

#include "IClass.h"

class IClassRegistry {
public:
	virtual const IClass& getClass(const char *name) const = 0;
	virtual IClass* const* getClasses(size_t& nClasses) const = 0;
};

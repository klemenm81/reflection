#pragma once

#include "IClass.h"

class IClassRegistry {
public:
	virtual const IClass& GetClass(const char *name) const = 0;
	virtual IClass* const* GetClasses(size_t& nClasses) const = 0;
};

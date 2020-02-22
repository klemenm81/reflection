#pragma once

#include "IInstantiator.h"

template <typename Class>
class CInstantiator : public IInstantiator {
public:
	Object& Instantiate() {
		return *new Class();
	}
};

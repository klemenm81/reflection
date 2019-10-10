#pragma once

#include "IInstantiator.h"

template <typename Class>
class CInstantiator : public IInstantiator {
public:
	IReflectable& Instantiate() {
		return *new Class();
	}
};

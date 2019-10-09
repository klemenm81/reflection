#pragma once

#include "IReflectable.h"
#include "CClass.h"

template <typename Class>
class Reflectable : public IReflectable {
public:
	IClass& GetClass() {
		static CClass<Class> reflection;
		return reflection;
	}
};

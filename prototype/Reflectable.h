#pragma once

#include "IReflectable.h"
#include "CClass.h"

#define FULL_ACCESS friend class CClass<ReflectedClass>;
#define PUBLIC_ACCESS

template <typename Class>
class Reflectable : public IReflectable {
protected:
	typedef Class ReflectedClass;
public:
	IClass& GetClass() {
		static CClass<Class> reflection;
		return reflection;
	}
};

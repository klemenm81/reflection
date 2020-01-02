#pragma once

#include "IReflectable.h"
#include "CClass.h"

#define REFLECT_FULL_ACCESS											\
	friend void CClass<ReflectedClass>::Register<ReflectedClass>(	\
		std::map<std::string, IField*>&,							\
		std::map<std::string, IMethod*>&);

#define REFLECT_PUBLIC_ACCESS

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

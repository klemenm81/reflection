#pragma once

#include "Object.h"
#include "CClass.h"

#define REFLECT_FULL_ACCESS											\
	friend void CClass<ReflectedClass>::Register<ReflectedClass>(	\
		std::map<std::string, IField*>&,							\
		std::map<std::string, IMethod*>&,							\
		std::map<std::string, CMethodOverloads*>&);

#define REFLECT_PUBLIC_ACCESS

template <typename ClassType>
class Reflectable : public Object {
protected:
	typedef ClassType ReflectedClass;
public:
	Class GetClass() {
		static CClass<ClassType> reflection;
		return reflection;
	}
};

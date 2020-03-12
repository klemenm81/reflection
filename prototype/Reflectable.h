#pragma once

#include "Object.h"
#include "CClass.h"
#include "Class.h"

#define REFLECT_FULL_ACCESS											\
	friend void CClass<ReflectedClass>::Register<ReflectedClass>();

#define REFLECT_PUBLIC_ACCESS

template <typename ClassType>
class Reflectable : public Object {
protected:
	typedef ClassType ReflectedClass;
public:
	Class &GetClass() const {
		static CClass<ClassType> reflection;
		static Class clasz(reflection);
		return clasz;
	}
};

#pragma once

#include "detail/CClass.h"
#include "Object.h"
#include "Class.h"

#define REFLECT_FULL_ACCESS											\
	friend void CClass<ReflectedClass>::registerMetadata<ReflectedClass>();

#define REFLECT_PUBLIC_ACCESS

template <typename ClassType>
class Reflectable : public Object {
protected:
	typedef ClassType ReflectedClass;
public:
	Class &getClass() const {
		static CClass<ClassType> reflection;
		static Class clasz(reflection);
		return clasz;
	}
};

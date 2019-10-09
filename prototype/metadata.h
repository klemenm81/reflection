#pragma once

#include "CField.h"
#include "CMethod2.h"

#define DEFINE_CLASS_START(Class)															\
std::map<std::string, IField*> CClass<Class>::m_fields;										\
std::map<std::string, IMethod*> CClass<Class>::m_methods;									\
template<>																					\
CClass<Class>::CClass() {

#define DEFINE_METHOD(Method, ...)															\
m_methods[#Method] = new CMethod2<ReflectedClass, __VA_ARGS__>(&ReflectedClass::Method);	

#define DEFINE_FIELD(Type, Field)															\
m_fields[#Field] = new CField<ReflectedClass, Type>(&ReflectedClass::Field);

#define DEFINE_CLASS_END																	\
}

#pragma once

#include "CAbstractFactory.h"
#include "CInstantiator.h"
#include "CClass.h"
#include "CField.h"
#include "CMethod.h"

#define REFLECT_CLASS_START(Class)																		\
std::map<std::string, IField*> CClass<Class>::m_fields;													\
std::map<std::string, IMethod*> CClass<Class>::m_methods;												\
template<>																								\
CClass<Class>::CClass() {

template <typename ReflectedClass, typename Return, typename... Args>
IMethod* newMethod(Return(ReflectedClass::* method)(Args...)) {
	return new CMethod<ReflectedClass, decltype((Return(ReflectedClass::*)(Args...))method), Args...>(method);
}

template <typename ReflectedClass, typename Return, typename... Args>
IMethod* newMethod(Return(ReflectedClass::* method)(Args...) const) {
	return new CMethod<ReflectedClass, decltype((Return(ReflectedClass::*)(Args...) const)method), Args...>(method);
}

#define REFLECT_METHOD(Method)																			\
m_methods[#Method] = new CMethod<																		\
	ReflectedClass,																						\
	decltype(&ReflectedClass::Method)>(																	\
	&ReflectedClass::Method																				\
);	
/*
#define REFLECT_METHOD_OVERLOAD(Method, Return, ...)				    								\
m_methods[#Method] = new CMethod<																		\
	ReflectedClass,																						\
	decltype((Return(ReflectedClass::*)(__VA_ARGS__))&ReflectedClass::Method),							\
	__VA_ARGS__>(																						\
	&ReflectedClass::Method																				\
);
*/

#define REFLECT_METHOD_OVERLOAD(Method, Return, ...)  \
m_methods[#Method] = newMethod<ReflectedClass, Return, __VA_ARGS__>(&ReflectedClass::Method);

#define REFLECT_FIELD(Field)																			\
m_fields[#Field] = new CField<ReflectedClass, decltype(&ReflectedClass::Field)>(						\
	&ReflectedClass::Field																				\
);

#define REFLECT_CLASS_END																				\
}

#define REFLECT_FACTORY_START																			\
extern "C" __declspec(dllexport) IReflectable& AbstractFactory(const char* name) {						\
	static CAbstractFactory abstractFactory;															\
	return abstractFactory.CreateInstance(name);														\
}																										\
std::map<std::string, IInstantiator*> CAbstractFactory::m_instantiators;								\
CAbstractFactory::CAbstractFactory() {

#define REFLECT_CLASS(Class)																			\
m_instantiators[#Class] = new CInstantiator<Class>();

#define REFLECT_FACTORY_END																				\
}
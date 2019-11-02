#pragma once

#include "CAbstractFactory.h"
#include "CInstantiator.h"
#include "CClass.h"
#include "CField.h"
#include "CMethod.h"

template <typename ReflectedClass, typename Return, typename... Args>
IMethod* newMethod(Return(ReflectedClass::* method)(Args...)) {
	return new CMethod<ReflectedClass, decltype((Return(ReflectedClass::*)(Args...))method)>(method);
}

template <typename ReflectedClass, typename Return, typename... Args>
IMethod* newMethod(Return(ReflectedClass::* method)(Args...) const) {
	return new CMethod<ReflectedClass, decltype((Return(ReflectedClass::*)(Args...) const)method)>(method);
}

template <typename ReflectedClass, typename Return, typename... Args>
IMethod* newMethod(Return(ReflectedClass::* method)(Args...) noexcept) {
	return new CMethod<ReflectedClass, decltype((Return(ReflectedClass::*)(Args...) noexcept)method)>(method);
}

template <typename ReflectedClass, typename Return, typename... Args>
IMethod* newMethod(Return(ReflectedClass::* method)(Args...) const noexcept) {
	return new CMethod<ReflectedClass, decltype((Return(ReflectedClass::*)(Args...) const noexcept)method)>(method);
}

template <typename ReflectedClass, typename Method>
IMethod* newMethod(Method method) {
	return new CMethod<ReflectedClass, decltype(method)>(method);
}

template <typename ReflectedClass, typename Field>
IField* newField(Field field) {
	return new CField<ReflectedClass, decltype(field)>(field);
}

#define REFLECT_CLASS_START(Class)																		\
std::map<std::string, IField*> CClass<Class>::m_fields;													\
std::map<std::string, IMethod*> CClass<Class>::m_methods;												\
template<>																								\
CClass<Class>::CClass() {

#define REFLECT_METHOD(Method, ...)																		\
m_methods[#Method] = newMethod<ReflectedClass, ##__VA_ARGS__>(&ReflectedClass::Method);

#define REFLECT_FIELD(Field)																			\
m_fields[#Field] = newField<ReflectedClass>(&ReflectedClass::Field);

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
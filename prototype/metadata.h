#pragma once

#include "CAbstractFactory.h"
#include "CInstantiator.h"
#include "CClass.h"
#include "CField.h"
#include "CMethod.h"

#define REFLECT_CLASS_START(Class)																	\
std::map<std::string, IField*> CClass<Class>::m_fields;												\
std::map<std::string, IMethod*> CClass<Class>::m_methods;											\
template<>																							\
CClass<Class>::CClass() {

#define REFLECT_METHOD(Return, Method, ...)															\
m_methods[#Method] = new CMethod<ReflectedClass, Return, __VA_ARGS__>(&ReflectedClass::Method);	

#define REFLECT_FIELD(Type, Field)																	\
m_fields[#Field] = new CField<ReflectedClass, Type>(&ReflectedClass::Field);

#define REFLECT_CLASS_END																			\
}

#define REFLECT_FACTORY_START																		\
extern "C" __declspec(dllexport) IReflectable& AbstractFactory(const char* name) {					\
	static CAbstractFactory abstractFactory;														\
	return abstractFactory.CreateInstance(name);													\
}																									\
std::map<std::string, IInstantiator*> CAbstractFactory::m_instantiators;							\
CAbstractFactory::CAbstractFactory() {

#define REFLECT_CLASS(Class)																		\
m_instantiators[#Class] = new CInstantiator<Class>();

#define REFLECT_FACTORY_END																			\
}
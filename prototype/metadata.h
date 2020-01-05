#pragma once

#include "CAbstractFactory.h"
#include "CInstantiator.h"
#include "CClass.h"
#include "CField.h"
#include "CMethod.h"
#include "macros.h"

template<typename I>
struct nothing {};

template <class... Ts>
constexpr bool inline_sfinae(Ts&&...) {
	return false;
}

template <class Ts, class Lambda>
constexpr auto inline_sfinae(nothing<Ts>&&, Lambda lambda) -> decltype(lambda(std::declval<Ts>()), bool{}) {
	return true;
}

template <typename ReflectedClass, typename Method>
IMethod* newMethod(Method method) {
	return new CMethod<ReflectedClass, Method>(method);
}

template <typename ReflectedClass, typename Field>
IField* newField(Field field) {
	return new CField<ReflectedClass, Field>(field);
}

#define REFLECT_CLASS_START(Class)												\
template<>																		\
template <typename ReflectedClass>												\
void CClass<Class>::Register(													\
	std::map<std::string, IField*>& m_fields,									\
	std::map<std::string, IMethod*>& m_methods)									\
{																				

#define REFLECT_CLASS_END(Class)												\
}																				\
template<>																		\
CClass<Class>::CClass() {														\
	Register<Class>(m_fields, m_methods);										\
}	

#define REFLECT_FIELD(Field)																			\
m_fields[#Field] = newField<ReflectedClass>(&ReflectedClass::Field);

#define REFLECT_METHOD_NO_OVERLOAD(Method)																\
m_methods[#Method] = newMethod<ReflectedClass>(&ReflectedClass::Method);

#define REFLECT_METHOD_OVERLOAD_NO_CVREF(Method, Return, ...)											\
	if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) ->								\
	decltype(static_cast<Return(decltype(v)::*)(__VA_ARGS__)>(&decltype(v)::Method), bool{}) { return false; })) {				\
		m_methods[#Method] =							\
			newMethod<ReflectedClass>(static_cast<Return(ReflectedClass::*)(__VA_ARGS__)>(				\
				&ReflectedClass::Method));																\
	}

#define REFLECT_METHOD_OVERLOAD_CVREF(Method, CvRef, Return, ...)										\
if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) ->									\
	decltype(static_cast<Return(decltype(v)::*)(__VA_ARGS__) CvRef>(&decltype(v)::Method), bool{}) { return false; })) {		\
		m_methods[#Method] =					\
			newMethod<ReflectedClass>(static_cast<Return(ReflectedClass::*)(__VA_ARGS__) CvRef>(		\
				&ReflectedClass::Method));																\
	}

#define REFLECT_METHOD_OVERLOAD(Method, Return, ...)									\
	REFLECT_METHOD_OVERLOAD_NO_CVREF(Method, Return, __VA_ARGS__)						\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, const, Return, __VA_ARGS__)					\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, const volatile, Return, __VA_ARGS__)			\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, volatile, Return, __VA_ARGS__)				\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, &, Return, __VA_ARGS__)						\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, const &, Return, __VA_ARGS__)					\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, const volatile &, Return, __VA_ARGS__)		\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, volatile &, Return, __VA_ARGS__)				\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, &&, Return, __VA_ARGS__)						\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, const &&, Return, __VA_ARGS__)				\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, const volatile &&, Return, __VA_ARGS__)		\
	REFLECT_METHOD_OVERLOAD_CVREF(Method, volatile &&, Return, __VA_ARGS__)

#define REFLECT_METHOD_0(Method, Return, ...) REFLECT_METHOD_OVERLOAD(Method, Return, ##__VA_ARGS__)	
#define REFLECT_METHOD_1(Method) REFLECT_METHOD_NO_OVERLOAD(Method)
#define REFLECT_METHOD(Method, ...)	CAT_FCN(REFLECT_METHOD_, ISEMPTY(__VA_ARGS__), Method, ##__VA_ARGS__)

#define REFLECT_FACTORY_START																			\
extern "C" _declspec(dllexport) IReflectable& AbstractFactory(const char* name) {						\
	static CAbstractFactory abstractFactory;															\
	return abstractFactory.CreateInstance(name);														\
}																										\
std::map<std::string, IInstantiator*> CAbstractFactory::m_instantiators;								\
CAbstractFactory::CAbstractFactory() {

#define REFLECT_CLASS(Class)																			\
m_instantiators[#Class] = new CInstantiator<Class>();

#define REFLECT_FACTORY_END																				\
}


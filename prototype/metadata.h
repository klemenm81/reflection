#pragma once

#include "CAbstractFactory.h"
#include "CConstructor.h"
#include "CClass.h"
#include "CField.h"
#include "CMethodInvoker.h"

#ifdef _WIN32
    #define EXPORT_API _declspec(dllexport)
#else
    #define EXPORT_API __attribute__ ((visibility("default")))
#endif

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

template <typename ReflectedClass, typename Field>
IField& newField(const char* name, Field field) {
	return *new CField<ReflectedClass, Field>(name, field);
}

template <typename ReflectedClass, typename Method>
IMethodInvoker& newMethod(const char *name, Method method) {
	return *new CMethodInvoker<ReflectedClass, Method>(name, method);
}

#define REFLECT_CLASS_START(Class)												\
template<>																		\
template <typename ReflectedClass>												\
void CClass<Class>::Register(													\
	std::map<std::string, IField*>& m_fields,									\
	std::map<std::string, IMethod*>& m_methods,									\
	std::map<std::string, IConstructor*>& m_constructors)						\
{																				

#define REFLECT_CLASS_END(Class)												\
}																				\
template<>																		\
CClass<Class>::CClass() {														\
	Register<Class>(m_fields,  m_methods, m_constructors);						\
}	

#define REFLECT_FIELD(Field)																			\
	AddField(newField<ReflectedClass>(#Field, &ReflectedClass::Field));

#define REFLECT_METHOD_NO_OVERLOAD(Method)																\
	AddMethod(newMethod<ReflectedClass>(#Method, &ReflectedClass::Method));

#define REFLECT_METHOD_OVERLOAD_NO_CVREF(Method, Return, ...)														\
	if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) ->											\
	decltype(static_cast<Return(decltype(v)::*)(__VA_ARGS__)>(&decltype(v)::Method), bool{}) { return false; })) {	\
		AddMethod(																									\
			newMethod<ReflectedClass>(#Method, static_cast<Return(ReflectedClass::*)(__VA_ARGS__)>(				\
				&ReflectedClass::Method)));																			\
	}

#define REFLECT_METHOD_OVERLOAD_CVREF(Method, CvRef, Return, ...)															\
	if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) ->													\
	decltype(static_cast<Return(decltype(v)::*)(__VA_ARGS__) CvRef>(&decltype(v)::Method), bool{}) { return false; })) {	\
		AddMethod(																											\
			newMethod<ReflectedClass>(#Method, static_cast<Return(ReflectedClass::*)(__VA_ARGS__) CvRef>(					\
				&ReflectedClass::Method)));																					\
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

#define CAT2_IMPL(_1, _2) _1 ## _2
#define CAT2(_1, _2) CAT2_IMPL(_1, _2)

#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define NARG16(...) _ARG16(__VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0)

#define REFLECT_METHOD(Method, ...) \
	CAT2(REFLECT_METHOD_, NARG16(dummy, ##__VA_ARGS__))(Method, ##__VA_ARGS__)

#define REFLECT_METHOD_1(Method) REFLECT_METHOD_NO_OVERLOAD(Method)
#define REFLECT_METHOD_2(Method, Return, ...) REFLECT_METHOD_OVERLOAD(Method, Return, ##__VA_ARGS__)	

#define REFLECT_FACTORY_START																			\
extern "C" EXPORT_API IAbstractFactory& AbstractFactory() {												\
	static CAbstractFactory abstractFactory;															\
	return abstractFactory;																				\
}																										\
std::map<std::string, std::map<std::string, IConstructor*>> CAbstractFactory::m_instantiators;			\
CAbstractFactory::CAbstractFactory() {

#define REFLECT_CLASS(Class, ...)																		\
	AddInstantiator<Class, ##__VA_ARGS__>(#Class);

#define REFLECT_FACTORY_END																				\
}


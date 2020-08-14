#pragma once

#include "detail/CConstructor.h"
#include "detail/CClass.h"
#include "detail/CCast.h"
#include "detail/CField.h"
#include "detail/CMethodInvoker.h"
#include "detail/CClassRegistry.h"
#include "TypeInfo.h"

#ifdef _WIN32
    #define EXPORT_API _declspec(dllexport)
#else
    #define EXPORT_API __attribute__ ((visibility("default")))
#endif

template<typename ...Args>
struct parameter_pack_counter {
	constexpr static int getN() {
		return sizeof...(Args);
	}
};

template <typename T, typename... Ts>
using parameter_pack_conjunction = std::conjunction<std::is_same<T, Ts>...>;

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

template <typename Class, typename... Casts>
std::vector<ICast*> newCasts() {
	std::vector<ICast*> ret;
	int dummy[] = { (ret.push_back(
		new CCast<Class, Casts>(std::to_string(TypeInfo<Casts>::getUniqueId()).c_str(), TypeInfo<Casts>::getName())), 0)... };
	return ret;
}

template <typename ReflectedClass, typename Field>
IField& newField(const char* name, Field field) {
	return *new CField<ReflectedClass, Field>(name, field);
}

template <typename ReflectedClass, typename Method>
IMethodInvoker& newMethod(const char *name, Method method) {
	return *new CMethodInvoker<ReflectedClass, Method>(name, method);
}

template <typename ReflectedClass, typename... Args>
IConstructor& newConstructor() {
	return *new CConstructor<ReflectedClass, Args...>();
}


#define REFLECT_TYPE_START(Class, ...)											\
template<>																		\
CClass<Class>::CClass() : m_name(#Class) {										\
	registerMetadata<Class>();													\
}																				\
template<>																		\
template <typename ReflectedClass>												\
void CClass<Class>::registerMetadata()											\
{																				\
	addCasts(newCasts<Class, Class, ##__VA_ARGS__>());							\
	if constexpr (std::is_default_constructible<Class>::value){					\
		addConstructor(newConstructor<Class>());								\
	}																			\
	if constexpr (std::is_copy_constructible<Class>::value) {					\
		addConstructor(newConstructor<Class, const Class &>());					\
	}																			\
	if constexpr (std::is_move_constructible<Class>::value) {					\
		addConstructor(newConstructor<Class, Class &&>());						\
	}

#define REFLECT_TYPE_END														\
}												


#define REFLECT_FIELD(Field)																			\
	addField(newField<ReflectedClass>(#Field, &ReflectedClass::Field));

#define REFLECT_METHOD_NO_OVERLOAD(Method)																\
	addMethodInvoker(newMethod<ReflectedClass>(#Method, &ReflectedClass::Method));

#define REFLECT_METHOD_OVERLOAD_CHECK_NO_CVREF(Method, Return, ...)																\
	inline_sfinae(nothing<ReflectedClass>{}, [](auto v) ->																		\
		decltype(static_cast<Return(decltype(v)::*)(__VA_ARGS__)>(&decltype(v)::Method), bool{}) { return false; })

#define REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, CvRef, Return, ...)															\
	inline_sfinae(nothing<ReflectedClass>{}, [](auto v) ->																		\
		decltype(static_cast<Return(decltype(v)::*)(__VA_ARGS__) CvRef>(&decltype(v)::Method), bool{}) { return false; })

#define REFLECT_METHOD_OVERLOAD_NO_CVREF(Method, Return, ...)														\
	if constexpr (REFLECT_METHOD_OVERLOAD_CHECK_NO_CVREF(Method, Return, ##__VA_ARGS__)) {							\
		addMethodInvoker(																							\
			newMethod<ReflectedClass>(#Method, static_cast<Return(ReflectedClass::*)(__VA_ARGS__)>(					\
				&ReflectedClass::Method)));																			\
	}

#define REFLECT_METHOD_OVERLOAD_CVREF(Method, CvRef, Return, ...)													\
	if constexpr (REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, CvRef, Return, ##__VA_ARGS__)) {						\
		addMethodInvoker(																							\
			newMethod<ReflectedClass>(#Method, static_cast<Return(ReflectedClass::*)(__VA_ARGS__) CvRef>(			\
				&ReflectedClass::Method)));																			\
	}

#define REFLECT_METHOD_OVERLOAD_CHECK(Method, Return, ...)	\
	static_assert(																							\
		REFLECT_METHOD_OVERLOAD_CHECK_NO_CVREF(Method, Return, __VA_ARGS__) ||								\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, const, Return, __VA_ARGS__) ||							\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, const volatile, Return, __VA_ARGS__) ||					\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, volatile, Return, __VA_ARGS__) ||						\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, &, Return, __VA_ARGS__) ||								\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, const&, Return, __VA_ARGS__) ||							\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, const volatile&, Return, __VA_ARGS__) ||				\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, volatile&, Return, __VA_ARGS__) ||						\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, &&, Return, __VA_ARGS__) ||								\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, const&&, Return, __VA_ARGS__) ||						\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, const volatile&&, Return, __VA_ARGS__) ||				\
		REFLECT_METHOD_OVERLOAD_CHECK_CVREF(Method, volatile&&, Return, __VA_ARGS__),						\
		"The specified method does not exist."																\
	);

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
	REFLECT_METHOD_OVERLOAD_CVREF(Method, volatile &&, Return, __VA_ARGS__)				\
	REFLECT_METHOD_OVERLOAD_CHECK(Method, Return, __VA_ARGS__)

#define CAT2_IMPL(_1, _2) _1 ## _2
#define CAT2(_1, _2) CAT2_IMPL(_1, _2)

#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define NARG16(...) _ARG16(__VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0)

#define REFLECT_METHOD(Method, ...) \
	CAT2(REFLECT_METHOD_, NARG16(dummy, ##__VA_ARGS__))(Method, ##__VA_ARGS__)

#define REFLECT_METHOD_1(Method) REFLECT_METHOD_NO_OVERLOAD(Method)
#define REFLECT_METHOD_2(Method, Return, ...) REFLECT_METHOD_OVERLOAD(Method, Return, ##__VA_ARGS__)

#define REFLECT_CONSTRUCTOR(Class, ...)																				\
	static_assert(																									\
		parameter_pack_counter<__VA_ARGS__>::getN() != 0,															\
		"Default constructor does not have to be specified. It gets registered automatically if it exists."			\
	);																												\
	static_assert(																									\
		!(parameter_pack_counter<__VA_ARGS__>::getN() == 1 &&														\
		parameter_pack_conjunction<const Class &, ##__VA_ARGS__>::value),											\
		"Copy constructor does not have to be specified. It gets registered automatically if it exists."			\
	);																												\
	static_assert(																									\
		!(parameter_pack_counter<__VA_ARGS__>::getN() == 1 &&														\
		parameter_pack_conjunction<Class &&, ##__VA_ARGS__>::value),												\
		"Move constructor does not have to be specified. It gets registered automatically if it exists."			\
	);																												\
	static_assert(std::is_constructible<Class, ##__VA_ARGS__>::value, "The specified constructor does not exist.");	\
	addConstructor(newConstructor<Class, ##__VA_ARGS__>());												

#define REFLECT_CLASS_REGISTRY_START											\
extern "C" EXPORT_API const IClassRegistry& ClassRegistry() {					\
	static const CClassRegistry classRegistry;									\
	return classRegistry;														\
}																				\
CClassRegistry::CClassRegistry() {

#define REFLECT_CLASS_REGISTRY_END												\
}

#define REFLECT_REGISTER_CLASS(Class)											\
	addClass(*new CClass<Class>());




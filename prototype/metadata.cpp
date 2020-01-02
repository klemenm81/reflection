#include "metadata.h"
#include "test.h"

REFLECT_CLASS_START(Test)
	REFLECT_FIELD(a)
	REFLECT_FIELD(myString)
	REFLECT_FIELD(ptrString)
	REFLECT_METHOD(Foo1)
	REFLECT_METHOD(Foo2)
	REFLECT_METHOD_OVERLOAD(FooOverloaded, int, int)
	REFLECT_METHOD_OVERLOAD(FooOverloaded, int, float)
	REFLECT_METHOD_OVERLOAD(FooOverloaded, int, std::vector<int>)
	REFLECT_METHOD(FooConst)
	REFLECT_METHOD(Bar1)
	REFLECT_METHOD(Bar2)
	REFLECT_METHOD(Bar3)
	REFLECT_METHOD(Bar4)
	REFLECT_METHOD(Bar5)
	REFLECT_METHOD(Bar6)
	REFLECT_METHOD(Bar7)
	REFLECT_METHOD(Bar8)
	REFLECT_METHOD(Bar9)
	REFLECT_METHOD(Bar10)
	REFLECT_METHOD(Bar11)
	REFLECT_METHOD(Bar12)
	REFLECT_METHOD(Bar13)
	REFLECT_METHOD(Bar14)
	REFLECT_METHOD(Bar15)
	REFLECT_METHOD(Bar16)
	REFLECT_METHOD(Bar17)
	REFLECT_METHOD(Bar18)
	REFLECT_METHOD(Bar19)
	REFLECT_METHOD(Bar20)
	REFLECT_METHOD(Bar21)
	REFLECT_METHOD(Bar22)
	REFLECT_METHOD(Bar23)
	REFLECT_METHOD(Bar24)

	REFLECT_METHOD_OVERLOAD(Overload, void, int)
	REFLECT_METHOD_OVERLOAD(OverloadEx, void, int)

	/*
	if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) -> 
		decltype(static_cast<void(decltype(v)::*)(int)>(&decltype(v)::Overload)) {})) {
		m_methods["Overload"] = 
			newMethod<Test>(static_cast<void(ReflectedClass::*)(int)>(&ReflectedClass::Overload));
	}


	if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) -> 
		decltype(static_cast<void(decltype(v)::*)(int) const>(&decltype(v)::Overload)) {})) {
		m_methods["Overload"] = 
			newMethod<Test>(static_cast<void(ReflectedClass::*)(int) const>(&ReflectedClass::Overload));
	}

	if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) -> 
		decltype(static_cast<void(decltype(v)::*)(int) const volatile>(&decltype(v)::Overload)) {})) {
		m_methods["Overload"] = 
			newMethod<Test>(static_cast<void(ReflectedClass::*)(int) const volatile>(&ReflectedClass::Overload));
	}

	if constexpr (inline_sfinae(nothing<ReflectedClass>{}, [](auto v) -> 
		decltype(static_cast<void(decltype(v)::*)(int) volatile>(&decltype(v)::Overload)) {})) {
		m_methods["Overload"] = 
			newMethod<Test>(static_cast<void(ReflectedClass::*)(int) volatile>(&ReflectedClass::Overload));
	}
	*/

REFLECT_CLASS_END(Test)

REFLECT_CLASS_START(TestDerived)
	REFLECT_METHOD(FooOverloaded)
REFLECT_CLASS_END(TestDerived)

REFLECT_FACTORY_START
	REFLECT_CLASS(Test)
REFLECT_FACTORY_END

#include "metadata.h"
#include "test.h"

REFLECT_CLASS_START(Test)
	REFLECT_FIELD(int, a)
	REFLECT_FIELD(std::string, myString)
	REFLECT_FIELD(const wchar_t *, ptrString)
	REFLECT_METHOD(void, Foo1, std::wstring&, float)
	REFLECT_METHOD(int, Foo2, const wchar_t *)
	REFLECT_METHOD(std::string, FooConst)
REFLECT_CLASS_END

REFLECT_FACTORY_START
	REFLECT_CLASS(Test)
REFLECT_FACTORY_END

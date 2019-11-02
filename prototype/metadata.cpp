#include "metadata.h"
#include "test.h"

REFLECT_CLASS_START(Test)
	REFLECT_FIELD(a)
	REFLECT_FIELD(myString)
	REFLECT_FIELD(ptrString)
	REFLECT_METHOD(Foo1)
	REFLECT_METHOD(Foo2)
	REFLECT_METHOD(FooOverloaded, int, int)
	REFLECT_METHOD(FooOverloaded, int, float)
	REFLECT_METHOD(FooOverloaded, int, std::vector<int>)
	REFLECT_METHOD(FooConst)
REFLECT_CLASS_END

REFLECT_FACTORY_START
	REFLECT_CLASS(Test)
REFLECT_FACTORY_END

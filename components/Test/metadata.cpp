#include "../../libjobject/Metadata.h"
#include "test.h"


REFLECT_TYPE_START(Test)
	//REFLECT_CONSTRUCTOR(Test)
	REFLECT_CONSTRUCTOR(Test, int)
	REFLECT_CONSTRUCTOR(Test, const char *)
	REFLECT_CONSTRUCTOR(Test, int, std::vector<int>)

	//REFLECT_CONSTRUCTOR(Test, const Test &)
	//REFLECT_CONSTRUCTOR(Test, Test&&)

	REFLECT_FIELD(a)
	REFLECT_FIELD(myString)
	REFLECT_FIELD(ptrString)
	REFLECT_FIELD(vec1)

	REFLECT_METHOD(foo1)
	REFLECT_METHOD(foo2)

	REFLECT_METHOD(fooOverloaded, int, int)
	REFLECT_METHOD(fooOverloaded, int, float)
	REFLECT_METHOD(fooOverloaded, int, std::vector<int>, int)
	REFLECT_METHOD(fooConst)
	REFLECT_METHOD(bar1)
	REFLECT_METHOD(bar2)
	REFLECT_METHOD(bar3)
	REFLECT_METHOD(bar4)
	REFLECT_METHOD(bar5)
	REFLECT_METHOD(bar6)
	REFLECT_METHOD(bar7)
	REFLECT_METHOD(bar8)

	REFLECT_METHOD(fcn1)
	REFLECT_METHOD(fcn2)
	REFLECT_METHOD(fcn3)
	REFLECT_METHOD(fcn4)
	REFLECT_METHOD(fcn5)
	REFLECT_METHOD(fcn6)
	REFLECT_METHOD(fcn7)
	REFLECT_METHOD(fcn8)

	REFLECT_METHOD(fcn9)
	REFLECT_METHOD(fcn10)
	REFLECT_METHOD(fcn11)
	REFLECT_METHOD(fcn12)
	REFLECT_METHOD(fcn13)
	REFLECT_METHOD(fcn14)
	REFLECT_METHOD(fcn15)
	REFLECT_METHOD(fcn16)

	REFLECT_METHOD(fcn17)
	REFLECT_METHOD(fcn18)
	REFLECT_METHOD(fcn19)
	REFLECT_METHOD(fcn20)
	REFLECT_METHOD(fcn21)
	REFLECT_METHOD(fcn22)
	REFLECT_METHOD(fcn23)
	REFLECT_METHOD(fcn24)

	REFLECT_METHOD(bar9)
	REFLECT_METHOD(bar10)
	REFLECT_METHOD(bar11)
	REFLECT_METHOD(bar12)
	REFLECT_METHOD(bar13)
	REFLECT_METHOD(bar14)
	REFLECT_METHOD(bar15)
	REFLECT_METHOD(bar16)
	REFLECT_METHOD(bar17, void)
	REFLECT_METHOD(bar18)
	REFLECT_METHOD(bar19)
	REFLECT_METHOD(bar20)
	REFLECT_METHOD(bar21)
	REFLECT_METHOD(bar22)
	REFLECT_METHOD(bar23)
	REFLECT_METHOD(bar24)
	REFLECT_METHOD(overload, void, int)
	REFLECT_METHOD(overloadEx, void, int) 
REFLECT_TYPE_END(Test)

REFLECT_TYPE_START(TestDerived, Test)
	REFLECT_METHOD(fooOverloaded)
	REFLECT_METHOD(fooMarshal1)
REFLECT_TYPE_END(TestDerived)

REFLECT_CLASS_REGISTRY_START
	REFLECT_REGISTER_CLASS(Test)
	REFLECT_REGISTER_CLASS(TestDerived)
REFLECT_CLASS_REGISTRY_END

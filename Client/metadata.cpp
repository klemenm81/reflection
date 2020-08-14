#include "../libjobject/Metadata.h"
#include "ParseStruct.h"
#include "MyRestServer.h"

REFLECT_TYPE_START(ParseStruct)
	REFLECT_FIELD(is_new)
	REFLECT_FIELD(name)
	REFLECT_FIELD(address)
REFLECT_TYPE_END

REFLECT_TYPE_START(MyRestServer)
	REFLECT_METHOD(GET_method1)
	REFLECT_METHOD(GET_method2)
	REFLECT_METHOD(GET_method3)
REFLECT_TYPE_END
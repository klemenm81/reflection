#include "../libjobject/Metadata.h"
#include "ParseStruct.h"
#include "MyRestServer.h"

#include "example.h"

REFLECT_TYPE_START(ParseStruct)
	FIELD(is_new)
	FIELD(name)
	FIELD(address)
REFLECT_TYPE_END

REFLECT_TYPE_START(MyRestServer)
	METHOD(GET_method1)
	METHOD(GET_method2)
	METHOD(GET_method3)
REFLECT_TYPE_END

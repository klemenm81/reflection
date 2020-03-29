#include "../libjobject/Metadata.h"
#include "ParseStruct.h"

REFLECT_TYPE_START(ParseStruct)
	REFLECT_FIELD(is_new)
	REFLECT_FIELD(name)
	REFLECT_FIELD(address)
REFLECT_TYPE_END(ParseStruct)
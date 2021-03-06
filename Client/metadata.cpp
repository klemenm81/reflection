#include "../libjobject/Metadata.h"
#include "ParseStruct.h"

#include "example.h"

REFLECT_TYPE_START(ParseStruct)
	FIELD(is_new)
	FIELD(name)
	FIELD(address)
	FIELD(age)
	METHOD(help_is_new)
	METHOD(help_name)
	METHOD(help_address)
	METHOD(help_age)
REFLECT_TYPE_END

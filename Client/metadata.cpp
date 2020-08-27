#include "../libjobject/Metadata.h"
#include "ParseStruct.h"
#include "MyRestServer.h"

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

REFLECT_TYPE_START(FilesystemItem)
	FIELD(fileName)
	FIELD(type)
	FIELD(size)
REFLECT_TYPE_END

REFLECT_TYPE_START(MyRestServer)
	METHOD(POST_changeDir)
	METHOD(GET_listDir)
REFLECT_TYPE_END

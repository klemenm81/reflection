#include "../../libjobject/Metadata.h"
#include "RESTController.h"

REFLECT_TYPE_START(RESTController, IRESTController)
REFLECT_CONSTRUCTOR(RESTController, std::wstring, Object&)
REFLECT_TYPE_END(RESTController)

REFLECT_CLASS_REGISTRY_START
	REFLECT_REGISTER_CLASS(RESTController)
REFLECT_CLASS_REGISTRY_END
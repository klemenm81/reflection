#include "../../libjobject/Metadata.h"
#include "RESTController.h"

REFLECT_TYPE_START(RESTController, IRESTController)
	CONSTRUCTOR(RESTController, std::wstring, Object&)
REFLECT_TYPE_END

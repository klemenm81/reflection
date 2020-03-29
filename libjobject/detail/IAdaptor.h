#pragma once

#include "../../libjson/include/json/json.h"
#include "../Indirection.h"

class IAdaptor {
public:
	virtual Indirection getIndirection() const = 0;
	virtual const char* getSignature() const = 0;
	virtual const char* getName() const = 0;
	virtual Json::Value marshall() const = 0;
	virtual ~IAdaptor() {
	}
};

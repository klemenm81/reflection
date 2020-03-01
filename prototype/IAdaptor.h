#pragma once

#include "json/json.h"

class IAdaptor {
public:
	virtual const char* GetSignature() = 0;
	virtual const char* GetName() = 0;
	virtual Json::Value Serialize() = 0;
	virtual ~IAdaptor() {
	}
};

#pragma once

#include "../../libjson/include/json/json.h"

class IAdaptor {
public:
	virtual const char* GetSignature() const = 0;
	virtual const char* GetName() const = 0;
	virtual Json::Value Serialize() const = 0;
	virtual ~IAdaptor() {
	}
};

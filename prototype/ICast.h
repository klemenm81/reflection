#pragma once

#include "IAdaptor.h"
#include "Object.h"

class ICast {
public:
	virtual const char* GetSignature() = 0;
	virtual const char* GetName() = 0;
	virtual IAdaptor* CastClass(Object& obj) = 0;
};

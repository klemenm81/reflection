#pragma once

#include "IAdaptor.h"
#include "../Object.h"

class ICast {
public:
	virtual const char* GetSignature() const = 0;
	virtual const char* GetName() const = 0;
	virtual IAdaptor* CastClass(Object& obj) const = 0;
};

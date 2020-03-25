#pragma once

#include "IAdaptor.h"
#include "../Object.h"

class ICast {
public:
	virtual const char* getSignature() const = 0;
	virtual const char* getName() const = 0;
	virtual IAdaptor* castClass(Object& obj) const = 0;
};

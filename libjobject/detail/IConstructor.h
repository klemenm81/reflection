#pragma once

#include "IAdaptor.h"
#include "../Object.h"

#include <cstddef>

class IConstructor {
public:
	virtual size_t getNArgs() const = 0;
	virtual std::byte* getArgBuffer(size_t iArg) const = 0;
	virtual const char* getArgsSignature() const = 0;
	virtual const char* getArgsName() const = 0;
	virtual Object& newInstance(IAdaptor** args) const = 0;
	virtual ~IConstructor() {
	}
};

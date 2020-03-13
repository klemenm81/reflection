#pragma once

#include "IAdaptor.h"
#include "../Object.h"

#include <cstddef>

class IConstructor {
public:
	virtual size_t GetNArgs() const = 0;
	virtual std::byte* GetArgBuffer(size_t iArg) const = 0;
	virtual const char* GetArgsSignature() const = 0;
	virtual const char* GetArgsName() const = 0;
	virtual Object& NewInstance(IAdaptor** args) const = 0;
	virtual ~IConstructor() {
	}
};

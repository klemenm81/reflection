#pragma once

#include "Object.h"
#include "IAdaptor.h"

class IInstantiator {
public:
	virtual size_t GetNArgs() = 0;
	virtual std::byte* GetArgBuffer(size_t iArg) = 0;
	virtual const char* GetArgsSignature() = 0;
	virtual const char* GetArgsName() = 0;
	virtual Object& Instantiate(IAdaptor** args) = 0;
	virtual ~IInstantiator() {
	}
};

#pragma once

#include "ICast.h"
#include "IField.h"
#include "IMethod.h"
#include "IConstructor.h"

class IClass {
public:
	virtual const char* getName() const = 0;
	virtual const ICast& getCast(const char* signature, const char* name) const = 0;
	virtual const IField& getField(const char* name) const = 0;
	virtual IField* const* getFields(size_t& nFields) const = 0;
	virtual const IMethod& getMethod(const char* name) const = 0;
	virtual IMethod* const* getMethods(size_t& nMethods) const = 0;
	virtual const IConstructor& getConstructor(const char* argsSignature, const char* argsName) const = 0;
	virtual IConstructor* const* getConstructors(size_t& nConstructors) const = 0;
	virtual ~IClass() {
	}
};

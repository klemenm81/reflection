#pragma once

#include <string>

#include "ICast.h"
#include "IField.h"
#include "IMethod.h"
#include "IConstructor.h"

class IClass {
public:
	virtual const char* GetName() const = 0;
	virtual const ICast& GetCast(const char* signature, const char* name) const = 0;
	virtual const IField& GetField(const char* name) const = 0;
	virtual IField* const* GetFields(size_t& nFields) const = 0;
	virtual const IMethod& GetMethod(const char* name) const = 0;
	virtual IMethod* const* GetMethods(size_t& nMethods) const = 0;
	virtual const IConstructor& GetConstructor(const char* argsSignature, const char* argsName) const = 0;
	virtual IConstructor* const* GetConstructors(size_t& nConstructors) const = 0;
	virtual ~IClass() {
	}
};

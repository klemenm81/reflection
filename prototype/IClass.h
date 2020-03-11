#pragma once

#include <string>

#include "IField.h"
#include "IMethod.h"
#include "IConstructor.h"

class IClass {
public:
	virtual const char* GetName() = 0;
	virtual IField& GetField(const char* name) = 0;
	virtual IField** GetFields(size_t& nFields) = 0;
	virtual IMethod& GetMethod(const char* name) = 0;
	virtual IMethod** GetMethods(size_t& nMethods) = 0;
	virtual IConstructor& GetConstructor(const char* argsSignature, const char* argsName) = 0;
	virtual IConstructor** GetConstructors(size_t& nConstructors) = 0;
	virtual ~IClass() {
	}
};

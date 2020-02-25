#pragma once

#include <string>

#include "IField.h"
#include "IMethodOverloads.h"

class IClass {
public:
	virtual IField& GetField(const char *name) = 0;
	virtual IMethodOverloads& GetMethodOverloads(const char* name) = 0;
	virtual ~IClass() {
	}
};

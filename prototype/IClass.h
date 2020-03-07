#pragma once

#include <string>

#include "IField.h"
#include "IMethod.h"

class IClass {
public:
	virtual IField& GetField(const char *name) = 0;
	virtual IMethod& GetMethod(const char* name) = 0;
	virtual ~IClass() {
	}
};

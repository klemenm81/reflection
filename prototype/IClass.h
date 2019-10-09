#pragma once

#include <string>
#include "IField.h"
#include "IMethod.h"

class IClass {
public:
	virtual IField& GetField(std::string name) = 0;
	virtual IMethod& GetMethod(std::string name) = 0;
	virtual ~IClass() {
	}
};

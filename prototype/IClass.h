#pragma once

#include <string>

class IField;
class IMethod;

class IClass {
public:
	virtual IField& GetField(const char *name) = 0;
	virtual IMethod& GetMethod(const char *name) = 0;
	virtual IMethod& GetConstMethod(const char *name) = 0;
	virtual ~IClass() {
	}
};

#pragma once

#include <string>

class IField;
class IMethod;
class IMethodOverloads;

class IClass {
public:
	virtual IField& GetField(const char *name) = 0;
	virtual IMethod& GetMethod(const char *name) = 0;
	virtual IMethodOverloads& GetMethodOverloads(const char* name) = 0;
	virtual ~IClass() {
	}
};

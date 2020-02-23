#pragma once

#include <string>

class IField;
class IMethod;
class CMethodOverloads;

class IClass {
public:
	virtual IField& GetField(const char *name) = 0;
	virtual IMethod& GetMethod(const char *name) = 0;
	virtual CMethodOverloads& GetMethodOverloads(const char* name) = 0;
	virtual ~IClass() {
	}
};

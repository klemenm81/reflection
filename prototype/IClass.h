#pragma once

#include <string>

class IField;
class IMethod;

class IClass {
public:
	virtual IField& GetField(std::string name) = 0;
	virtual IMethod& GetMethod(std::string name) = 0;
	virtual ~IClass() {
	}
};

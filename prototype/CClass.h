#pragma once

#include "IClass.h"
#include "IField.h"
#include "IMethod.h"
#include <string>
#include <map>

template<typename Class>
class CClass : public IClass {
private:
	static std::map<std::string, IField*> m_fields;
	static std::map<std::string, IMethod*> m_methods;

public:
	typedef Class ReflectedClass;

	IField& GetField(std::string name) {
		return *m_fields[name];
	}

	IMethod& GetMethod(std::string name) {
		return *m_methods[name];
	}

	CClass();
};

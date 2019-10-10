#pragma once

#include "IClass.h"
#include <string>
#include <map>

#include "IField.h"
#include "IMethod.h"

template<typename Class>
class CClass : public IClass {
private:
	static std::map<std::string, IField*> m_fields;
	static std::map<std::string, IMethod*> m_methods;
	typedef Class ReflectedClass;

public:
	IField& GetField(std::string name) {
		return *m_fields[name];
	}

	IMethod& GetMethod(std::string name) {
		return *m_methods[name];
	}

	CClass();
};

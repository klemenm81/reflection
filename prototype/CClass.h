#pragma once

#include "IClass.h"
#include <string>
#include <map>

#include "IField.h"
#include "IMethod.h"

template<typename Class>
class CClass : public IClass {
private:
	std::map<std::string, IField*> m_fields;
	std::map<std::string, IMethod*> m_methods;
	std::map<std::string, IMethod*> m_constMethods;
	typedef Class ReflectedClass;

public:
	IField& GetField(const char *name) {
		return *m_fields[name];
	}

	IMethod& GetMethod(const char *name) {
		return *m_methods[name];
	}

	IMethod& GetConstMethod(const char *name) {
		return *m_constMethods[name];
	}
	
	template <typename ReflectedClass>
	void Register(
		std::map<std::string, IField*>& m_fields,
		std::map<std::string, IMethod*>& m_methods,
		std::map<std::string, IMethod*>& m_constMethods);
	
	CClass();
};

#pragma once

#include "IClass.h"
#include <string>
#include <map>

#include "IField.h"
#include "IMethod.h"
#include "IMethodOverloads.h"

#include "exceptions/MethodNotFoundException.h"

template<typename Class>
class CClass : public IClass {
private:
	std::map<std::string, IField*> m_fields;
	std::map<std::string, IMethod*> m_methods;
	std::map<std::string, IMethodOverloads *> m_methodOverloads;
	typedef Class ReflectedClass;

public:
	IField& GetField(const char *name) {
		return *m_fields[name];
	}

	IMethod& GetMethod(const char *name) {
		return *m_methods[name];
	}

	IMethodOverloads& GetMethodOverloads(const char *name) {
		IMethodOverloads* methodOverloads = (m_methodOverloads.find(name) != m_methodOverloads.end()) ?
			m_methodOverloads[name] :
			nullptr;

		if (methodOverloads == nullptr) {
			throw MethodNotFoundException(name);
		}

		return *m_methodOverloads[name];
	}
	
	template <typename ReflectedClass>
	void Register(
		std::map<std::string, IField*>& m_fields,
		std::map<std::string, IMethod*>& m_methods,
		std::map<std::string, IMethodOverloads*> &m_methodOverloads);
	
	CClass();
};

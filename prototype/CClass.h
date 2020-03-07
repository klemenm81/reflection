#pragma once

#include "IClass.h"
#include <string>
#include <map>

#include "IField.h"

#include "exceptions/MethodNotFoundException.h"

template<typename Class>
class CClass : public IClass {
private:
	std::map<std::string, IField*> m_fields;
	std::map<std::string, IMethod *> m_methodOverloads;
	typedef Class ReflectedClass;

public:
	IField& GetField(const char *name) {
		return *m_fields[name];
	}

	IMethod& GetMethodOverloads(const char *name) {
		IMethod* methodOverloads = (m_methodOverloads.find(name) != m_methodOverloads.end()) ?
			m_methodOverloads[name] :
			nullptr;

		if (methodOverloads == nullptr) {
			throw MethodNotFoundException(name);
		}

		return *m_methodOverloads[name];
	}

	void AddMethod(IMethodInvoker& method) {
		IMethod* methodOverloads = (m_methodOverloads.find(method.GetName()) != m_methodOverloads.end()) ?
			m_methodOverloads[method.GetName()] :
			nullptr;

		if (methodOverloads == nullptr) {
			methodOverloads = new IMethod();
			m_methodOverloads[method.GetName()] = methodOverloads;
		}

		methodOverloads->AddMethod(method);
	}
	
	template <typename ReflectedClass>
	void Register(
		std::map<std::string, IField*>& m_fields,
		std::map<std::string, IMethod*> &m_methodOverloads);
	
	CClass();
};

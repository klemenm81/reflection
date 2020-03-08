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
	std::map<std::string, IMethod *> m_methods;
	typedef Class ReflectedClass;

public:
	IField& GetField(const char *name) {
		return *m_fields[name];
	}

	IMethod& GetMethod(const char *name) {
		IMethod* methodOverloads = (m_methods.find(name) != m_methods.end()) ?
			m_methods[name] :
			nullptr;

		if (methodOverloads == nullptr) {
			throw MethodNotFoundException(name);
		}

		return *m_methods[name];
	}

	void AddMethod(IMethodInvoker& method) {
		IMethod* methodOverloads = (m_methods.find(method.GetName()) != m_methods.end()) ?
			m_methods[method.GetName()] :
			nullptr;

		if (methodOverloads == nullptr) {
			methodOverloads = new IMethod();
			m_methods[method.GetName()] = methodOverloads;
		}

		methodOverloads->AddMethod(method);
	}
	
	template <typename ReflectedClass>
	void Register(
		std::map<std::string, IField*>& m_fields,
		std::map<std::string, IMethod*>& m_methods);
	
	CClass();
};

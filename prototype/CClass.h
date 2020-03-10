#pragma once

#include "IClass.h"
#include <string>
#include <map>

#include "IField.h"
#include "IMethod.h"
#include "IConstructor.h"

#include "exceptions/FieldNotFoundException.h"
#include "exceptions/MethodNotFoundException.h"
#include "exceptions/ClassConstructorNotFoundException.h"

template<typename Class>
class CClass : public IClass {
private:
	std::string m_name;
	std::map<std::string, IField*> m_fields;
	std::map<std::string, IMethod*> m_methods;
	std::map<std::string, IConstructor*> m_constructors;

	typedef Class ReflectedClass;

public:
	const char* GetName() {
		return m_name.c_str();
	}

	void AddField(IField& field) {
		m_fields[field.GetName()] = &field;
	}

	IField& GetField(const char *name) {
		IField& field = (m_fields.find(name) != m_fields.end()) ?
			*m_fields[name] :
			throw FieldNotFoundException(name);
		return field;
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

	IMethod& GetMethod(const char *name) {
		IMethod& method = (m_methods.find(name) != m_methods.end()) ?
			*m_methods[name] :
			throw MethodNotFoundException(name);
		return method;
	}

	void AddConstructor(IConstructor& constructor) {
		m_constructors[constructor.GetArgsSignature()] = &constructor;
	}

	IConstructor& GetConstructor(const char* argsSignature, const char* argsName) {
		IConstructor& constructor = (m_constructors.find(argsSignature) != m_constructors.end()) ?
			*m_constructors[argsSignature] :
			throw ClassConstructorNotFoundException(argsName);
		return constructor;
	}
	
	template <typename ReflectedClass>
	void Register(
		std::map<std::string, IField*>& m_fields,
		std::map<std::string, IMethod*>& m_methods,
		std::map<std::string, IConstructor*>& m_constructors);
	
	CClass();
};

#pragma once

#include "IClass.h"
#include <string>
#include <map>

#include "ICast.h"
#include "IField.h"
#include "IMethod.h"
#include "IConstructor.h"

#include "exceptions/CastNotFoundException.h"
#include "exceptions/FieldNotFoundException.h"
#include "exceptions/MethodNotFoundException.h"
#include "exceptions/ClassConstructorNotFoundException.h"

template<typename Class>
class CClass : public IClass {
private:
	std::string m_name;
	std::map<std::string, ICast*> m_castMap;
	std::map<std::string, IField*> m_fieldMap;
	std::map<std::string, IMethod*> m_methodMap;
	std::map<std::string, IConstructor*> m_constructorMap;
	std::vector<ICast*> m_castVector;
	std::vector<IField*> m_fieldVector;
	std::vector<IMethod*> m_methodVector;
	std::vector<IConstructor*> m_constructorVector;

	typedef Class ReflectedClass;

public:
	const char* GetName() {
		return m_name.c_str();
	}

	void AddCast(ICast& cast) {
		m_castMap[cast.GetSignature()] = &cast;
		m_castVector.push_back(&cast);
	}

	ICast& GetCast(const char* signature) {
		ICast& cast = (m_castMap.find(signature) != m_castMap.end()) ?
			*m_castMap[signature] :
			throw CastNotFoundException(signature);
		return cast;
	}

	void AddField(IField& field) {
		m_fieldMap[field.GetName()] = &field;
		m_fieldVector.push_back(&field);
	}

	IField& GetField(const char *name) {
		IField& field = (m_fieldMap.find(name) != m_fieldMap.end()) ?
			*m_fieldMap[name] :
			throw FieldNotFoundException(name);
		return field;
	}

	IField** GetFields(size_t& nFields) {
		nFields = m_fieldVector.size();
		return m_fieldVector.data();
	}

	void AddMethod(IMethodInvoker& method) {
		IMethod* methodOverloads = (m_methodMap.find(method.GetName()) != m_methodMap.end()) ?
			m_methodMap[method.GetName()] :
			nullptr;

		if (methodOverloads == nullptr) {
			methodOverloads = new IMethod();
			m_methodMap[method.GetName()] = methodOverloads;
			m_methodVector.push_back(methodOverloads);
		}

		methodOverloads->AddMethod(method);
	}

	IMethod& GetMethod(const char *name) {
		IMethod& method = (m_methodMap.find(name) != m_methodMap.end()) ?
			*m_methodMap[name] :
			throw MethodNotFoundException(name);
		return method;
	}

	IMethod** GetMethods(size_t& nMethods) {
		nMethods = m_methodVector.size();
		return m_methodVector.data();
	}

	void AddConstructor(IConstructor& constructor) {
		m_constructorMap[constructor.GetArgsSignature()] = &constructor;
		m_constructorVector.push_back(&constructor);
	}

	IConstructor& GetConstructor(const char* argsSignature, const char* argsName) {
		IConstructor& constructor = (m_constructorMap.find(argsSignature) != m_constructorMap.end()) ?
			*m_constructorMap[argsSignature] :
			throw ClassConstructorNotFoundException(argsName);
		return constructor;
	}

	IConstructor** GetConstructors(size_t& nConstructors) {
		nConstructors = m_constructorVector.size();
		return m_constructorVector.data();
	}
	
	template <typename ReflectedClass>
	void Register();
	
	CClass();
};

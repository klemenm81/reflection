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
	const char* GetName() const {
		return m_name.c_str();
	}

	void AddCasts(std::vector<ICast *> casts) {
		for (ICast* cast : casts) {
			m_castMap[cast->GetSignature()] = cast;
			m_castVector.push_back(cast);
		}
	}

	const ICast& GetCast(const char* signature, const char *name) const {
		auto cast = m_castMap.find(signature);
		if (cast != m_castMap.end()) {
			return *(cast->second);
		}
		else {
			throw CastNotFoundException(name);
		}
	}

	void AddField(IField& field) {
		m_fieldMap[field.GetName()] = &field;
		m_fieldVector.push_back(&field);
	}

	const IField& GetField(const char *name) const {
		auto field = m_fieldMap.find(name);
		if (field != m_fieldMap.end()) {
			return *(field->second);
		}
		else {
			throw FieldNotFoundException(name);
		}
	}

	IField* const* GetFields(size_t& nFields) const {
		nFields = m_fieldVector.size();
		return m_fieldVector.data();
	}

	void AddMethodInvoker(IMethodInvoker& method) {
		IMethod* methodOverloads = (m_methodMap.find(method.GetName()) != m_methodMap.end()) ?
			m_methodMap[method.GetName()] :
			nullptr;

		if (methodOverloads == nullptr) {
			methodOverloads = new IMethod(method.GetName());
			m_methodMap[method.GetName()] = methodOverloads;
			m_methodVector.push_back(methodOverloads);
		}

		methodOverloads->AddMethod(method);
	}

	const IMethod& GetMethod(const char *name) const {
		auto method = m_methodMap.find(name);
		if (method != m_methodMap.end()) {
			return *(method->second);
		}
		else {
			throw MethodNotFoundException(name);
		}
	}

	IMethod* const* GetMethods(size_t& nMethods) const {
		nMethods = m_methodVector.size();
		return m_methodVector.data();
	}

	void AddConstructor(IConstructor& constructor) {
		m_constructorMap[constructor.GetArgsSignature()] = &constructor;
		m_constructorVector.push_back(&constructor);
	}

	const IConstructor& GetConstructor(const char* argsSignature, const char* argsName) const {
		auto constructor = m_constructorMap.find(argsSignature);
		if (constructor != m_constructorMap.end()) {
			return *(constructor->second);
		}
		else {
			throw ClassConstructorNotFoundException(argsName);
		}
	}

	IConstructor* const* GetConstructors(size_t& nConstructors) const {
		nConstructors = m_constructorVector.size();
		return m_constructorVector.data();
	}
	
	template <typename ReflectedClass>
	void Register();
	
	CClass();
};

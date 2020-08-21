#pragma once

#include "IClass.h"
#include "ICast.h"
#include "IField.h"
#include "IMethod.h"
#include "IConstructor.h"
#include "../exceptions/CastNotFoundException.h"
#include "../exceptions/FieldNotFoundException.h"
#include "../exceptions/MethodNotFoundException.h"
#include "../exceptions/ClassConstructorNotFoundException.h"
#include <string>
#include <map>

template<typename Class>
class CClass : public IClass {
private:
	std::string m_name;
	std::map<std::string, ICast*> m_castMap;
	std::map<std::string, IField*> m_fieldMap;
	std::map<std::string, IMethod*> m_methodMap;
	std::map<size_t, IConstructor*> m_constructorMap;
	std::vector<ICast*> m_castVector;
	std::vector<IField*> m_fieldVector;
	std::vector<IMethod*> m_methodVector;
	std::vector<IConstructor*> m_constructorVector;

	typedef Class ReflectedClass;

public:
	const char* getName() const {
		return m_name.c_str();
	}

	void addCasts(std::vector<ICast *> casts) {
		for (ICast* cast : casts) {
			m_castMap[cast->getSignature()] = cast;
			m_castVector.push_back(cast);
		}
	}

	const ICast& getCast(const char* signature, const char *name) const {
		auto cast = m_castMap.find(signature);
		if (cast != m_castMap.end()) {
			return *(cast->second);
		}
		else {
			throw CastNotFoundException(name, m_name.c_str());
		}
	}

	void addField(IField& field) {
		m_fieldMap[field.getName()] = &field;
		m_fieldVector.push_back(&field);
	}

	const IField& getField(const char *name) const {
		auto field = m_fieldMap.find(name);
		if (field != m_fieldMap.end()) {
			return *(field->second);
		}
		else {
			throw FieldNotFoundException(name, m_name.c_str());
		}
	}

	IField* const* getFields(size_t& nFields) const {
		nFields = m_fieldVector.size();
		return m_fieldVector.data();
	}

	void addMethodInvoker(IMethodInvoker& method) {
		IMethod* methodOverloads = (m_methodMap.find(method.getName()) != m_methodMap.end()) ?
			m_methodMap[method.getName()] :
			nullptr;

		if (methodOverloads == nullptr) {
			std::string fullMethodName = m_name + std::string("::") + std::string(method.getName());
			methodOverloads = new IMethod(fullMethodName.c_str());
			m_methodMap[method.getName()] = methodOverloads;
			m_methodVector.push_back(methodOverloads);
		}

		methodOverloads->addMethod(method);
	}

	const IMethod& getMethod(const char *name) const {
		auto method = m_methodMap.find(name);
		if (method != m_methodMap.end()) {
			return *(method->second);
		}
		else {
			throw MethodNotFoundException(name, m_name.c_str());
		}
	}

	IMethod* const* getMethods(size_t& nMethods) const {
		nMethods = m_methodVector.size();
		return m_methodVector.data();
	}

	void addConstructor(IConstructor& constructor) {
		m_constructorMap[constructor.getArgsSignature()] = &constructor;
		m_constructorVector.push_back(&constructor);
	}

	const IConstructor& getConstructor(size_t argsSignature, const char* argsName) const {
		auto constructor = m_constructorMap.find(argsSignature);
		if (constructor != m_constructorMap.end()) {
			return *(constructor->second);
		}
		else {
			throw ClassConstructorNotFoundException(argsName, m_name.c_str());
		}
	}

	IConstructor* const* getConstructors(size_t& nConstructors) const {
		nConstructors = m_constructorVector.size();
		return m_constructorVector.data();
	}
	
	template <typename ReflectedClass>
	void registerMetadata();
	
	CClass();
};

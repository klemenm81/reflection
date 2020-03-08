#pragma once

#include <map>
#include <string>

#include "IAbstractFactory.h"
#include "CConstructor.h"
#include "Object.h"
#include "exceptions/ClassConstructorNotFoundException.h"
#include "exceptions/ClassNotFoundException.h"

class CAbstractFactory : public IAbstractFactory {
private:
	static std::map<std::string, std::map<std::string, IConstructor*>> m_instantiators;

public:
	CAbstractFactory();

	template <typename Class, typename... Args>
	void AddInstantiator(const char *className) {
		IConstructor *instantiator = new CConstructor<Class, Args...>();
		m_instantiators[className][instantiator->GetArgsSignature()] = instantiator;
	}

	IConstructor& GetInstantiator(const char* className, const char* argsSignature, const char* argsName) {
		std::map<std::string, IConstructor*>& classInstantiators = m_instantiators.find(className) != m_instantiators.end() ?
			m_instantiators[className] :
			throw ClassNotFoundException(className);
		IConstructor* instantiator = classInstantiators.find(argsSignature) != classInstantiators.end() ?
			classInstantiators[argsSignature] :
			throw ClassConstructorNotFoundException(argsName);
		return *instantiator;
	}
};
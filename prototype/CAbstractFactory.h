#pragma once

#include <map>
#include <string>

#include "IAbstractFactory.h"
#include "CInstantiator.h"
#include "Object.h"
#include "exceptions/ClassConstructorNotFoundException.h"
#include "exceptions/ClassNotFoundException.h"

class CAbstractFactory : public IAbstractFactory {
private:
	static std::map<std::string, std::map<std::string, IInstantiator*>> m_instantiators;

public:
	CAbstractFactory();

	template <typename Class, typename... Args>
	void AddInstantiator(const char *className) {
		IInstantiator *instantiator = new CInstantiator<Class, Args...>();
		m_instantiators[className][instantiator->GetArgsSignature()] = instantiator;
	}

	IInstantiator& GetInstantiator(const char* className, const char* argsSignature, const char* argsName) {
		std::map<std::string, IInstantiator*>& classInstantiators = m_instantiators.find(className) != m_instantiators.end() ?
			m_instantiators[className] :
			throw ClassNotFoundException(className);
		IInstantiator* instantiator = classInstantiators.find(argsSignature) != classInstantiators.end() ?
			classInstantiators[argsSignature] :
			throw ClassConstructorNotFoundException(argsName);
		return *instantiator;
	}
};
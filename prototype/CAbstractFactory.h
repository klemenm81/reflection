#pragma once

#include <map>
#include <string>

#include "IInstantiator.h"
#include "IReflectable.h"

class CAbstractFactory {
private:
	static std::map<std::string, IInstantiator*> m_instantiators;

public:
	CAbstractFactory();

	IReflectable& CreateInstance(const char* className) {
		return(m_instantiators[className]->Instantiate());
	}
};
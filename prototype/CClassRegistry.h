#pragma once

#include "IClassRegistry.h"
#include "IClass.h"

#include "exceptions/ClassNotFoundException.h"

#include <map>
#include <string>

class CClassRegistry : public IClassRegistry {
private:
	std::map<std::string, IClass*> m_classes;
	
public:
	CClassRegistry();

	void AddClass(IClass& clasz) {
		m_classes[clasz.GetName()] = &clasz;
	}

	IClass& GetClass(const char* name) {
		IClass& clasz = (m_classes.find(name) != m_classes.end()) ?
			*m_classes[name] :
			throw ClassNotFoundException(name);
		return clasz;
	}
};

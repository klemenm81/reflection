#pragma once

#include "IClassRegistry.h"
#include "IClass.h"

#include "exceptions/ClassNotFoundException.h"

#include <map>
#include <string>

class CClassRegistry : public IClassRegistry {
private:
	std::map<std::string, IClass*> m_classMap;
	std::vector<IClass*> m_classList;
	
public:
	CClassRegistry();

	void AddClass(IClass& clasz) {
		m_classMap[clasz.GetName()] = &clasz;
		m_classList.push_back(&clasz);
	}

	IClass& GetClass(const char* name) {
		IClass& clasz = (m_classMap.find(name) != m_classMap.end()) ?
			*m_classMap[name] :
			throw ClassNotFoundException(name);
		return clasz;
	}

	IClass** GetClasses(size_t &nClasses) {
		nClasses = m_classList.size();
		return m_classList.data();
	}
};

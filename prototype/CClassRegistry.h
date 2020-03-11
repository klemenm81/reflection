#pragma once

#include "IClassRegistry.h"
#include "IClass.h"

#include "exceptions/ClassNotFoundException.h"

#include <map>
#include <string>

class CClassRegistry : public IClassRegistry {
private:
	std::map<std::string, IClass*> m_classMap;
	std::vector<IClass*> m_classVector;
	
public:
	CClassRegistry();

	void AddClass(IClass& clasz) {
		m_classMap[clasz.GetName()] = &clasz;
		m_classVector.push_back(&clasz);
	}

	IClass& GetClass(const char* name) {
		IClass& clasz = (m_classMap.find(name) != m_classMap.end()) ?
			*m_classMap[name] :
			throw ClassNotFoundException(name);
		return clasz;
	}

	IClass** GetClasses(size_t &nClasses) {
		nClasses = m_classVector.size();
		return m_classVector.data();
	}
};

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
		auto clasz = m_classMap.find(name);
		if (clasz != m_classMap.end()) {
			return *(clasz->second);
		}
		else {
			throw ClassNotFoundException(name);
		}
	}

	IClass** GetClasses(size_t &nClasses) {
		nClasses = m_classVector.size();
		return m_classVector.data();
	}
};

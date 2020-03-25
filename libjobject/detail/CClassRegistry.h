#pragma once

#include "IClassRegistry.h"
#include "IClass.h"
#include "../exceptions/ClassNotFoundException.h"
#include <map>
#include <string>

class CClassRegistry : public IClassRegistry {
private:
	std::map<std::string, IClass*> m_classMap;
	std::vector<IClass*> m_classVector;
	
public:
	CClassRegistry();

	void addClass(IClass& clasz) {
		m_classMap[clasz.getName()] = &clasz;
		m_classVector.push_back(&clasz);
	}

	const IClass& getClass(const char* name) const {
		auto clasz = m_classMap.find(name);
		if (clasz != m_classMap.end()) {
			return *(clasz->second);
		}
		else {
			throw ClassNotFoundException(name);
		}
	}

	IClass* const* getClasses(size_t &nClasses) const {
		nClasses = m_classVector.size();
		return m_classVector.data();
	}
};

#pragma once

#include "IClassRegistry.h"
#include "Class.h"

class ClassRegistry {
private:
	IClassRegistry& m_classRegistry;

public:
	ClassRegistry(IClassRegistry& classRegistry) : m_classRegistry(classRegistry) {
	}

	ClassRegistry(ClassRegistry&& other) noexcept : m_classRegistry(other.m_classRegistry) {
	}

	ClassRegistry(const ClassRegistry&) = delete;
	ClassRegistry& operator=(const ClassRegistry&) = delete;

	Class GetClass(const char* name) {
		return m_classRegistry.GetClass(name);
	}

	std::vector<Class> GetClasses() {
		std::vector<Class> ret;
		size_t nClasses = 0;
		IClass** classes = m_classRegistry.GetClasses(nClasses);
		for (size_t iClass = 0; iClass < nClasses; iClass++) {
			ret.push_back(Class(*classes[iClass]));
		}
		return ret;
	}
};

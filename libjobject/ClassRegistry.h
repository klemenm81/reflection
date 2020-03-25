#pragma once

#include "detail/IClassRegistry.h"
#include "Class.h"

class ClassRegistry {
private:
	const IClassRegistry& m_classRegistry;

public:
	ClassRegistry(const IClassRegistry& classRegistry) : m_classRegistry(classRegistry) {
	}

	ClassRegistry(ClassRegistry&& other) noexcept : m_classRegistry(other.m_classRegistry) {
	}

	ClassRegistry(const ClassRegistry& other) : m_classRegistry(other.m_classRegistry) {
	}

	ClassRegistry& operator=(ClassRegistry&&) = delete;
	ClassRegistry& operator=(const ClassRegistry&) = delete;

	Class getClass(const char* name) const {
		return m_classRegistry.GetClass(name);
	}

	std::vector<Class> getClasses() const {
		std::vector<Class> ret;
		size_t nClasses = 0;
		IClass* const* classes = m_classRegistry.GetClasses(nClasses);
		for (size_t iClass = 0; iClass < nClasses; iClass++) {
			ret.push_back(Class(*classes[iClass]));
		}
		return ret;
	}
};

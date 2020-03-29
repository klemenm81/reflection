#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "detail/IClassRegistry.h"
#include "Class.h"

class ClassRegistry {
private:
	const IClassRegistry& m_classRegistry;

public:
	static ClassRegistry GetClassRegistry(const char* name) {
#ifdef _WIN32
		HMODULE hModule = LoadLibraryA(name);
		const IClassRegistry& (*ClassRegistryFcn)() =
			(const IClassRegistry & (*)())GetProcAddress(hModule, "ClassRegistry");
#else
		void* hModule = dlopen(NULL, RTLD_NOW | RTLD_LOCAL);
		if (hModule == NULL) {
			perror("dlopen");
			exit(1);
		}
		const IClassRegistry& (*ClassRegistryFcn)() =
			(const IClassRegistry & (*)())dlsym(hModule, "ClassRegistry");
		if (ClassRegistryFcn == NULL) {
			printf("ClassRegistry not found\n");
			exit(1);
		}
#endif
		ClassRegistry classRegistry = ClassRegistryFcn();
		return classRegistry;
	}

	ClassRegistry(const IClassRegistry& classRegistry) : m_classRegistry(classRegistry) {
	}

	ClassRegistry(ClassRegistry&& other) noexcept : m_classRegistry(other.m_classRegistry) {
	}

	ClassRegistry(const ClassRegistry& other) : m_classRegistry(other.m_classRegistry) {
	}

	ClassRegistry& operator=(ClassRegistry&&) = delete;
	ClassRegistry& operator=(const ClassRegistry&) = delete;

	Class getClass(const char* name) const {
		return m_classRegistry.getClass(name);
	}

	std::vector<Class> getClasses() const {
		std::vector<Class> ret;
		size_t nClasses = 0;
		IClass* const* classes = m_classRegistry.getClasses(nClasses);
		for (size_t iClass = 0; iClass < nClasses; iClass++) {
			ret.push_back(Class(*classes[iClass]));
		}
		return ret;
	}
};

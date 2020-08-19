#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "detail/IClassRegistry.h"
#include "Class.h"
#include "exceptions/ClassNotFoundException.h"
#include <atomic>


class ClassRegistry {
private:
	static std::atomic<int> m_counter;
	std::string m_libraryName;
	std::map<std::string, Class> m_classes;
	void *m_libraryHandle;
	bool m_isLibrary;

public:
	void Initialize();
	void Uninitialize();
/*
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
*/
/*	ClassRegistry() {
		Initialize();
	}
*/
	ClassRegistry() {
		Initialize();
	}

	ClassRegistry(std::string libraryName) : m_libraryName(libraryName) {
		Initialize();
	}

	ClassRegistry(const ClassRegistry& other) : m_classes(other.m_classes) {
		Initialize();
	}

	~ClassRegistry() {
		Uninitialize();
	}

	ClassRegistry& operator=(ClassRegistry&&) = delete;
	ClassRegistry& operator=(const ClassRegistry&) = delete;

	Class getClass(const char* name) const {
		auto it = m_classes.find(name);
		if (it != m_classes.end()) {
			return it->second;
		}
		throw ClassNotFoundException(name);
	}

	std::vector<Class> getClasses() const {
		std::vector<Class> ret;		
		for (auto it = m_classes.begin(); it != m_classes.end(); it++) {
			ret.push_back(it->second);
		}
		return ret;
	}
};

#include "ClassRegistry.h"
#include "exceptions/InternalErrorException.h"

#include <windows.h>
#include <tchar.h>
#include <dbghelp.h>
#include <stdio.h>

BOOL CALLBACK MyEnumSymbolsCallback(SYMBOL_INFO* pSymInfo, ULONG SymbolSize, PVOID UserContext) {
	std::vector<std::string>& classFactoryNames = 
		*(reinterpret_cast<std::vector<std::string>*>(UserContext));
	classFactoryNames.push_back(pSymInfo->Name);
	return TRUE;
}

std::atomic<int> ClassRegistry::m_counter = 0;

void ClassRegistry::Initialize() {
	if (m_counter == 0) {
		if (!::SymInitialize(
			GetCurrentProcess(),
			NULL,
			FALSE
		)) {
			std::string error =
				std::string("SymInitialize() failed. Error code: ") +
				std::to_string(::GetLastError());
			throw InternalErrorException(error.c_str());
		}

		m_counter++;
	}

	DWORD64 modBase;
	if ((modBase = ::SymLoadModuleEx(
		GetCurrentProcess(),
		NULL,
		m_libraryName.c_str(),
		NULL,
		0,
		0,
		NULL,
		SLMFLAG_NO_SYMBOLS
	)) == 0) {
		std::string error =
			std::string("SymLoadModuleEx() failed. Error code: ") +
			std::to_string(::GetLastError());
		throw InternalErrorException(error.c_str());
	}

	std::vector<std::string> classFactoryNames;

	if (!::SymEnumSymbols(
		GetCurrentProcess(),
		modBase,
		"*",
		MyEnumSymbolsCallback,
		reinterpret_cast<void*>(&classFactoryNames)
	)) {
		std::string error =
			std::string("SymEnumSymbols() failed. Error code: ") +
			std::to_string(::GetLastError());
		throw InternalErrorException(error.c_str());
	}

	if (!::SymUnloadModule(GetCurrentProcess(), modBase)) {
		std::string error =
			std::string("SymUnloadModule() failed. Error code: ") +
			std::to_string(::GetLastError());
		throw InternalErrorException(error.c_str());
	}

	m_libraryHandle = LoadLibraryA(m_libraryName.c_str());
	if (m_libraryHandle == nullptr) {
		std::string error =
			std::string("LoadLibraryA() failed. Error code: ") +
			std::to_string(::GetLastError());
		throw InternalErrorException(error.c_str());
	}
	
	for (std::string classFactoryName : classFactoryNames) {
		if (classFactoryName.compare(0, std::string("Factory_").length(), "Factory_") == 0) {
			const IClass& (*ClassFactoryFcn)() =
				(const IClass & (*)())GetProcAddress(reinterpret_cast<HMODULE>(m_libraryHandle), classFactoryName.c_str());
			m_classes.emplace(classFactoryName.substr(std::string("Factory_").length()), Class(ClassFactoryFcn()));
		}
	}
}

void ClassRegistry::Uninitialize() {
	FreeLibrary(reinterpret_cast<HMODULE>(m_libraryHandle));
	m_counter--;
	if (m_counter == 0) {
		::SymCleanup(GetCurrentProcess());
	}
}
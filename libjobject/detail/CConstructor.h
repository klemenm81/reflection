#pragma once

#include "CAdaptor.h"
#include "IConstructor.h"
#include "../Object.h"
#include "../TypeInfo.h"
#include "../Reflectable.h"


template <typename Class, typename... Args>
class CConstructor : public IConstructor {
public:
	static size_t GetNArgs() {
		return sizeof...(Args);
	}

	template <size_t... Index>
	static size_t GetArgsSignature(std::index_sequence<Index...>) {
		if constexpr (sizeof...(Args) > 0) {
			static const size_t argsSignature = ((TypeInfo<Args>::getUniqueId() << Index) + ...);
			return argsSignature;
		}
		else {
			return 0;
		}
	}

	static const char* GetArgsName() {
		if constexpr (sizeof...(Args) > 0) {
			static const std::string name = ((std::string(";") + std::string(TypeInfo<Args>::getName())) + ...);
			return name.c_str() + 1;
		}
		else {
			static const std::string name = ";";
			return name.c_str() + 1;
		}
	}

	template <std::size_t... Index>
	static Object& NewInstance(IAdaptor** args, std::index_sequence<Index...>) {
		return static_cast<Reflectable<Class>&>(*new Class(static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...));
	}

	static Object& NewInstance(IAdaptor** args) {
		return NewInstance(args, std::index_sequence_for<Args...>{});
	}

	size_t getNArgs() const {
		return GetNArgs();
	}

	size_t getArgsSignature() const {
		return GetArgsSignature(std::index_sequence_for<Args...>{});
	}

	const char* getArgsName() const {
		return GetArgsName();
	}

	Object& newInstance(IAdaptor **args) const {
		return NewInstance(args);
	}
};

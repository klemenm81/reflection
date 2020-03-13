#pragma once

#include "CAdaptor.h"
#include "IConstructor.h"
#include "../Object.h"
#include "../Reflectable.h"
#include "../exceptions/ArgumentOutOfBoundsException.h"


template <typename Class, typename... Args>
class CConstructor : public IConstructor {
public:
	static constexpr size_t ArgsSize() {
		if constexpr (sizeof...(Args) > 0) {
			return (sizeof(CAdaptor<Args>) + ...);
		}
		else {
			return 0;
		}
	}

	template <size_t... Index>
	static constexpr size_t ArgOffset(int iArg, std::index_sequence<Index...>) {
		return (((Index < iArg) ? sizeof(CAdaptor<Args>) : 0) + ...);
	}

	template <size_t... Index>
	static std::byte* GetArgBuffer(size_t iArg, std::index_sequence<Index...>) {
		if constexpr (sizeof...(Args) > 0) {
			static thread_local std::byte argsBuffer[ArgsSize()];
			static size_t offsets[sizeof...(Args)] = { ArgOffset(Index, std::index_sequence_for<Args...>{})... };
			return argsBuffer + offsets[iArg];
		}
		else {
			throw ArgumentOutOfBoundsException(iArg, sizeof...(Args));
		}
	}

	size_t GetNArgs() const {
		return sizeof...(Args);
	}

	std::byte* GetArgBuffer(size_t iArg) const {
		return GetArgBuffer(iArg, std::index_sequence_for<Args...>{});
	}

	const char* GetArgsSignature() const {
		if constexpr (sizeof...(Args) > 0) {
			static const std::string signature = ((std::string(";") + std::to_string(typeid(Args).hash_code())) + ...);
			return signature.c_str() + 1;
		}
		else {
			static const std::string signature = ";";
			return signature.c_str() + 1;
		}
	}

	const char* GetArgsName() const {
		if constexpr (sizeof...(Args) > 0) {
			static const std::string name = ((std::string(";") + std::string(typeid(Args).name())) + ...);
			return name.c_str() + 1;
		}
		else {
			static const std::string name = ";";
			return name.c_str() + 1;
		}
	}

	template <std::size_t... Index>
	Object& NewInstance(IAdaptor **args, std::index_sequence<Index...>) const {
		return static_cast<Reflectable<Class> &>(*new Class(static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...));
	}

	Object& NewInstance(IAdaptor **args) const {
		return NewInstance(args, std::index_sequence_for<Args...>{});
	}
};

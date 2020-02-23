#pragma once

#include "CAdaptor.h"
#include "IMethod2.h"

#include <string>
#include <typeinfo>

template <typename Class, typename Return, typename... Args>
class CMethodBase2 : public IMethod2 {
protected:
	CMethodBase2(const char* name) : m_name(name) {
	}

	static constexpr size_t ArgsSize() {
		return (sizeof(CAdaptor<Args>) + ...);
	}

	template <size_t... Index>
	static constexpr size_t ArgOffset(int iArg, std::index_sequence<Index...>) {
		return (((Index < iArg) ? sizeof(CAdaptor<Args>) : 0) + ...);
	}

	template <size_t... Index>
	static std::byte *GetArgBuffer(size_t iArg, std::index_sequence<Index...>) {
		static thread_local std::byte argsBuffer[ArgsSize()];
		static size_t offsets[sizeof...(Args)] = { ArgOffset(Index, std::index_sequence_for<Args...>{})... };
		return argsBuffer + offsets[iArg];
	}

	const char* GetName() {
		return m_name.c_str();
	}

	std::byte* GetArgBuffer(size_t iArg) {
		return GetArgBuffer(iArg, std::index_sequence_for<Args...>{});
	}

	const char* GetArgsSignature() {
		static const std::string signature = (sizeof...(Args) > 0) ? 
			((std::string(";") + std::to_string(typeid(Args).hash_code())) + ...) : 
			";";
		return signature.c_str() + 1;
	}

	const char* GetArgsName() {
		static const std::string name = (sizeof...(Args) > 0) ?
			((std::string(";") + std::string(typeid(Args).name())) + ...) :
			";";
		return name.c_str() + 1;
	}

	const char *GetRetValSignature() {
		static const std::string signature = std::to_string(typeid(Return).hash_code());
		return signature.c_str();
	}

	const char *GetRetValName() {
		static const std::string name = typeid(Return).name();
		return name.c_str();
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, Object& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&>(object).*method)(
				static_cast<CAdaptor<Args> &>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<Class&>(object).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method>
	IAdaptor* Invoke(Method method, Object& object, IAdaptor** args) {
		return Invoke(method, object, args, std::index_sequence_for<Args...>{});
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, const Object& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const Class&>(object).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const Class&>(object).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method>
	IAdaptor* Invoke(Method method, const Object& object, IAdaptor** args) {
		return Invoke(method, object, args, std::index_sequence_for<Args...>{});
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, Object&& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&&>(object).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<Class&&>(object).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method>
	IAdaptor* Invoke(Method method, Object&& object, IAdaptor** args) {
		return Invoke(method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

private:
	std::string m_name;
};

template <typename Class, typename Method>
class CMethod2;

template <typename Class, typename Return, typename... Args>
class CMethod2<Class, Return(Class::*)(Args...)> : public CMethodBase2<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...);
	
public:
	constexpr CMethod2(const char *name, Return(Class::* method)(Args...)) : 
		CMethodBase2<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return LValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor **args) {
		return CMethodBase2<Class, Return, Args...>::Invoke(m_method, object, args);
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod2<Class, Return(Class::*)(Args...) const> : public CMethodBase2<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const;

public:
	constexpr CMethod2(const char *name, Return(Class::* method)(Args...) const) : 
		CMethodBase2<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodBase2<Class, Return, Args...>::Invoke(m_method, object, args);
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodBase2<Class, Return, Args...>::Invoke(m_method, object, args);
	}
};



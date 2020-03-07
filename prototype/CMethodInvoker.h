#pragma once

#include "CAdaptor.h"
#include "IMethod.h"

#include <string>
#include <typeinfo>

#include "exceptions/ArgumentOutOfBoundsException.h"

#include "Object.h"
#include "Reflectable.h"

#include "json/json.h"

template <typename Class, typename Return, typename... Args>
class CMethodInvokerBase : public IMethodInvoker {
protected:
	CMethodInvokerBase(const char* name) : m_name(name) {
	}

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
	static std::byte *GetArgBuffer(size_t iArg, std::index_sequence<Index...>) {
		if constexpr (sizeof...(Args) > 0) {
			static thread_local std::byte argsBuffer[ArgsSize()];
			static size_t offsets[sizeof...(Args)] = { ArgOffset(Index, std::index_sequence_for<Args...>{})... };
			return argsBuffer + offsets[iArg];
		}
		else {
			throw ArgumentOutOfBoundsException(iArg, sizeof...(Args));
		}
	}

	const char* GetName() {
		return m_name.c_str();
	}

	size_t GetNArgs() {
		return sizeof...(Args);
	}

	std::byte* GetArgBuffer(size_t iArg) {
		return GetArgBuffer(iArg, std::index_sequence_for<Args...>{});
	}

	const char* GetArgsSignature() {
		if constexpr (sizeof...(Args) > 0) {
			static const std::string signature = ((std::string(";") + std::to_string(typeid(Args).hash_code())) + ...);
			return signature.c_str() + 1;
		}
		else {
			static const std::string signature = ";";
			return signature.c_str() + 1;
		}
	}

	const char* GetArgsName() {
		if constexpr (sizeof...(Args) > 0) {
			static const std::string name = ((std::string(";") + std::string(typeid(Args).name())) + ...);
			return name.c_str() + 1;
		}
		else {
			static const std::string name = ";";
			return name.c_str() + 1;
		}
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
	Json::Value Invoke(Method method, Object& object, Json::Value args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&>(static_cast<Reflectable<Class> &>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).GetValue()...
			);
			return CAdaptor<void>().Serialize();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<Class&>(static_cast<Reflectable<Class>&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).GetValue()...
				)
			).Serialize();
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, Object& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&>(static_cast<Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args> &>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<Class&>(static_cast<Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, const Object& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const Class&>(static_cast<const Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const Class&>(static_cast<const Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, volatile Object& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<volatile Class&>(static_cast<volatile Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<volatile Class&>(static_cast<volatile Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, const volatile Object& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const volatile Class&>(static_cast<const volatile Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const volatile Class&>(static_cast<const volatile Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, Object&& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&&>(static_cast<Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<Class&&>(static_cast<Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, const Object&& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const Class&&>(static_cast<const Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const Class&&>(static_cast<const Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, volatile Object&& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<volatile Class&&>(static_cast<volatile Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<volatile Class&&>(static_cast<volatile Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* Invoke(Method method, const volatile Object&& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const volatile Class&&>(static_cast<const volatile Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const volatile Class&&>(static_cast<const volatile Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
		}
	}

private:
	std::string m_name;
};

template <typename Class, typename Method>
class CMethodInvoker;

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...)> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...);
	
public:
	constexpr CMethodInvoker(const char *name, Return(Class::* method)(Args...)) : 
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return LValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor **args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value InvokeMarshalled(Object& object, Json::Value args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const;

public:
	constexpr CMethodInvoker(const char *name, Return(Class::* method)(Args...) const) : 
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) volatile> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) volatile) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return VolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const volatile> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const volatile;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const volatile) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstVolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return LValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) volatile noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) volatile noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return VolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const volatile noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const volatile noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const volatile noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstVolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};





/********************************/




template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) &> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) &;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) &) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return LValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const &> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const &;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const &) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) volatile &> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile &;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) volatile &) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return VolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const volatile &> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const volatile &;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const volatile &) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstVolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) & noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) & noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) & noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return LValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const & noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const & noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const & noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) volatile & noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile & noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) volatile & noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return VolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const volatile & noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const volatile & noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const volatile & noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstVolatileLValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};



/***********************************/


template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) &&> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) &&;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) &&) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return RValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const &&> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const &&;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const &&) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstRValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) volatile &&> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile &&;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) volatile &&) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return VolatileRValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const volatile &&> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const volatile &&;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const volatile &&) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstVolatileRValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) && noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) && noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) && noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return RValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const && noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const && noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const && noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstRValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) volatile && noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile && noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) volatile && noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return VolatileRValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return nullptr;
	}
};

template <typename Class, typename Return, typename... Args>
class CMethodInvoker<Class, Return(Class::*)(Args...) const volatile && noexcept> : public CMethodInvokerBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const volatile && noexcept;

public:
	constexpr CMethodInvoker(const char* name, Return(Class::* method)(Args...) const volatile && noexcept) :
		CMethodInvokerBase<Class, Return, Args...>(name), m_method(method) {
	}

	Qualifier GetQualifier() {
		return ConstVolatileRValueRef;
	}

	IAdaptor* Invoke(Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(const volatile Object& object, IAdaptor** args) {
		return nullptr;
	}

	IAdaptor* Invoke(Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(volatile Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* Invoke(const volatile Object&& object, IAdaptor** args) {
		return CMethodInvokerBase<Class, Return, Args...>::Invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}
};

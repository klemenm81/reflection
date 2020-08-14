#pragma once

#include "CAdaptor.h"
#include "IMethod.h"
#include "../Object.h"
#include "../Reflectable.h"
#include "../exceptions/ArgumentOutOfBoundsException.h"
#include "../../libjson/include/json/json.h"
#include "../TypeInfo.h"
#include <string>
#include <cstddef>

template <typename Class, typename Return, typename... Args>
class CMethodInvokerBase : public IMethodInvoker {
private:
	std::string m_name;

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

	const char* getName() const {
		return m_name.c_str();
	}

	size_t getNArgs() const {
		return sizeof...(Args);
	}

	std::byte* getArgBuffer(size_t iArg) const {
		return GetArgBuffer(iArg, std::index_sequence_for<Args...>{});
	}

	size_t getArgsSignature() const {
		if constexpr (sizeof...(Args) > 0) {
			static const size_t argsSignature = (TypeInfo<Args>::getUniqueId() - ...);
			return argsSignature;
		}
		else {
			return 0;
		}
	}

	const char* getArgsName() const {
		if constexpr (sizeof...(Args) > 0) {
			static const std::string name = ((std::string(";") + std::string(TypeInfo<Args>::getName())) + ...);
			return name.c_str() + 1;
		}
		else {
			static const std::string name = ";";
			return name.c_str() + 1;
		}
	}

	const char *getRetValSignature() const {
		static const std::string signature = std::to_string(TypeInfo<Return>::getUniqueId());
		return signature.c_str();
	}

	const char *getRetValName() const {
		static const std::string name = TypeInfo<Return>::getName();
		return name.c_str();
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, Object& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&>(static_cast<Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args> &>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<Class&>(static_cast<Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, const Object& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const Class&>(static_cast<const Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const Class&>(static_cast<const Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, volatile Object& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<volatile Class&>(static_cast<volatile Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<volatile Class&>(static_cast<volatile Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, const volatile Object& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const volatile Class&>(static_cast<const volatile Reflectable<Class> &>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const volatile Class&>(static_cast<const volatile Reflectable<Class>&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, Object&& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&&>(static_cast<Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<Class&&>(static_cast<Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, const Object&& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const Class&&>(static_cast<const Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const Class&&>(static_cast<const Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, volatile Object&& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<volatile Class&&>(static_cast<volatile Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<volatile Class&&>(static_cast<volatile Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* invoke(Method method, const volatile Object&& object, IAdaptor** args, std::index_sequence<Index...>) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const volatile Class&&>(static_cast<const volatile Reflectable<Class>&&>(object)).*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
			);
			return new(retValBuffer) CAdaptor<void>();
		}
		else {
			return new(retValBuffer) CAdaptor<Return>(
				(static_cast<const volatile Class&&>(static_cast<const volatile Reflectable<Class>&&>(object)).*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).getValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, Object& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&>(static_cast<Reflectable<Class>&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<Class&>(static_cast<Reflectable<Class>&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, const Object& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const Class&>(static_cast<const Reflectable<Class>&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<const Class&>(static_cast<const Reflectable<Class>&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, volatile Object& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<volatile Class&>(static_cast<volatile Reflectable<Class>&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<volatile Class&>(static_cast<volatile Reflectable<Class>&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, const volatile Object& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const volatile Class&>(static_cast<const volatile Reflectable<Class>&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<const volatile Class&>(static_cast<const volatile Reflectable<Class>&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, Object&& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&&>(static_cast<Reflectable<Class>&&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<Class&&>(static_cast<Reflectable<Class>&&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, const Object&& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const Class&&>(static_cast<const Reflectable<Class>&&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<const Class&&>(static_cast<const Reflectable<Class>&&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, volatile Object&& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<volatile Class&&>(static_cast<volatile Reflectable<Class>&&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<volatile Class&&>(static_cast<volatile Reflectable<Class>&&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}

	template<typename Method, std::size_t... Index>
	Json::Value invoke(Method method, const volatile Object&& object, Json::Value args, std::index_sequence<Index...>) const {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<const volatile Class&&>(static_cast<const volatile Reflectable<Class>&&>(object)).*method)(
				CAdaptor<Args>(args[(int)Index]).getValue()...
			);
			return CAdaptor<void>().marshall();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<const volatile Class&&>(static_cast<const volatile Reflectable<Class>&&>(object)).*method)(
					CAdaptor<Args>(args[(int)Index]).getValue()...
				)
			).marshall();
		}
	}
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

	Qualifier getQualifier() const {
		return LValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor **args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return VolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstVolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return LValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return VolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstVolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return LValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return VolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstVolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return LValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return VolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstVolatileLValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, object, args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return RValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstRValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return VolatileRValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstVolatileRValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
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

	Qualifier getQualifier() const {
		return RValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstRValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return VolatileRValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
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

	Qualifier getQualifier() const {
		return ConstVolatileRValueRef;
	}

	IAdaptor* invoke(Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(const volatile Object& object, IAdaptor** args) const {
		return nullptr;
	}

	IAdaptor* invoke(Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(volatile Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	IAdaptor* invoke(const volatile Object&& object, IAdaptor** args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(const volatile Object& object, Json::Value args) const {
		return nullptr;
	}

	Json::Value invokeMarshalled(Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(volatile Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}

	Json::Value invokeMarshalled(const volatile Object&& object, Json::Value args) const {
		return CMethodInvokerBase<Class, Return, Args...>::invoke(m_method, std::move(object), args, std::index_sequence_for<Args...>{});
	}
};

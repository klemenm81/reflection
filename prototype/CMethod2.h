#pragma once

#include "CAdaptor.h"
#include "IMethod2.h"

template <typename Class, typename Return, typename... Args>
class CMethodBase2 : public IMethod2 {
protected:
	static constexpr size_t size() {
		return (sizeof(CAdaptor<Args>) + ...);
	}

	template <size_t... Index>
	static constexpr size_t offset(int iArg, std::index_sequence<Index...>) {
		return (((Index < iArg) ? sizeof(CAdaptor<Args>) : 0) + ...);
	}

	template <size_t... Index>
	static std::byte *GetArgBuffer(size_t iArg, std::index_sequence<Index...>) {
		static thread_local std::byte argsBuffer[size()];
		static size_t offsets[sizeof...(Args)] = { offset(Index, std::index_sequence_for<Args...>{})... };
		return argsBuffer + offsets[iArg];
	}

	std::byte* GetArgBuffer(size_t iArg) {
		return GetArgBuffer(iArg, std::index_sequence_for<Args...>{});
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* InvokeLValue(Method method, IAdaptor& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		CAdaptor<Return>* retVal;

		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&>&>(object).GetValue().*method)(
				static_cast<CAdaptor<Args> &>(*args[Index]).GetValue()...
			);
			retVal = new(retValBuffer) CAdaptor<void>();
			return retVal;
		}
		else {
			retVal = new(retValBuffer) CAdaptor<Return>(
				(static_cast<CAdaptor<Class&>&>(object).GetValue().*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
			return retVal;
		}
	}

	template<typename Method>
	IAdaptor* InvokeLValue(Method method, IAdaptor& object, IAdaptor** args) {
		return(InvokeLValue(method, object, args, std::index_sequence_for<Args...>{}));
	}

	template<typename Method, std::size_t... Index>
	IAdaptor* InvokeRValue(Method method, IAdaptor& object, IAdaptor** args, std::index_sequence<Index...>) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		CAdaptor<Return>* retVal;

		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&&>&>(object).GetValue().*method)(
				static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
			);
			retVal = new(retValBuffer) CAdaptor<void>();
			return *retVal;
		}
		else {
			retVal = new(retValBuffer) CAdaptor<Return>(
				(static_cast<CAdaptor<Class&&>&>(object).GetValue().*method)(
					static_cast<CAdaptor<Args>&>(*args[Index]).GetValue()...
				)
			);
			return *retVal;
		}
	}

	template<typename Method>
	IAdaptor* InvokeRValue(Method method, IAdaptor& object, IAdaptor** args) {
		return(InvokeRValue(method, object, args, std::index_sequence_for<Args...>{}));
	}
};

template <typename Class, typename Method>
class CMethod2;

template <typename Class, typename Return, typename... Args>
class CMethod2<Class, Return(Class::*)(Args...)> : public CMethodBase2<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...);
	
public:
	constexpr CMethod2(Return(Class::* method)(Args...)) : m_method(method) {
	}

	IAdaptor* Invoke(IAdaptor& object, IAdaptor **args) {
		return CMethodBase2<Class, Return, Args...>::InvokeLValue(m_method, object, args);
	}
};



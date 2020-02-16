#pragma once

#include "Adaptor.h"
#include "IMethod2.h"

template <typename Class, typename Return, typename... Args>
class CMethodBase2 : public IMethod2 {
protected:
	template<typename Method, std::size_t... Index>
	IAdaptor& InvokeLValue(std::byte *retValBuffer, Method method, IAdaptor& object, std::vector<Adaptor> &args, std::index_sequence<Index...>) {
		CAdaptor<Return>* retVal;

		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&>&>(object).GetValue().*method)(
				args[Index].Get<Args>()...
			);
			retVal = new(retValBuffer) CAdaptor<void>();
			return *retVal;
		}
		else {
			retVal = new(retValBuffer) CAdaptor<Return>(
				(static_cast<CAdaptor<Class&>&>(object).GetValue().*method)(
					args[Index].Get<Args>()...
				)
			);
			return *retVal;
		}
	}

	template<typename Method>
	IAdaptor& InvokeLValue(std::byte *retValBuffer, Method method, IAdaptor& object, std::vector<Adaptor> &args) {
		return(InvokeLValue(retValBuffer, method, object, args, std::index_sequence_for<Args...>{}));
	}

	template<typename Method, std::size_t... Index>
	IAdaptor& InvokeRValue(std::byte *retValBuffer, Method method, IAdaptor& object, std::vector<Adaptor> &args, std::index_sequence<Index...>) {
		CAdaptor<Return>* retVal;

		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&&>&>(object).GetValue().*method)(
				args[Index].Get<Args>()...
			);
			retVal = new(retValBuffer) CAdaptor<void>();
			return *retVal;
		}
		else {
			retVal = new(retValBuffer) CAdaptor<Return>(
				(static_cast<CAdaptor<Class&&>&>(object).GetValue().*method)(
					args[Index].Get<Args>()...
				)
			);
			return *retVal;
		}
	}

	template<typename Method>
	IAdaptor& InvokeRValue(std::byte* retValBuffer, Method method, IAdaptor& object, std::vector<Adaptor> &args) {
		return(InvokeRValue(retValBuffer, method, object, args, std::index_sequence_for<Args...>{}));
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

	IAdaptor& Invoke(IAdaptor& object, std::vector<Adaptor>& args) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		return CMethodBase2<Class, Return, Args...>::InvokeLValue(retValBuffer, m_method, object, args);
	}

	IAdaptor& Invoke(std::byte *retValBuffer, IAdaptor& object, std::vector<Adaptor> &&args) {
		return CMethodBase2<Class, Return, Args...>::InvokeLValue(retValBuffer, m_method, object, args);
	}
};



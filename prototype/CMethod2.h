#pragma once

#include "Adaptor.h"
#include "IMethod2.h"

template <typename Class, typename Return, typename... Args>
class CMethodBase2 : public IMethod2 {
protected:
	template<typename Method, std::size_t... Index>
	IAdaptor&& InvokeLValue(Method method, IAdaptor& adaptor, std::vector<Adaptor2> &args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&>&>(adaptor).GetValue().*method)(
				args[Index].Get<Args>()...
			);
			return std::move(CAdaptor<Return>());
		}
		else {
			return std::move(CAdaptor<Return>(
				(static_cast<CAdaptor<Class&>&>(adaptor).GetValue().*method)(
					args[Index].Get<Args>()...
				)
			));
		}
	}

	template<typename Method>
	IAdaptor&& InvokeLValue(Method method, IAdaptor& adaptor, std::vector<Adaptor2> &args) {
		return(InvokeLValue(method, adaptor, args, std::index_sequence_for<Args...>{}));
	}

	template<typename Method, std::size_t... Index>
	IAdaptor&& InvokeRValue(Method method, IAdaptor& adaptor, std::vector<Adaptor2> &args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&&>&>(adaptor).GetValue().*method)(
				args[Index].Get<Args>()...
			);
			return CAdaptor<Return>();
		}
		else {
			return CAdaptor<Return>(
				(static_cast<CAdaptor<Class&&>&>(adaptor).GetValue().*method)(
					args[Index].Get<Args>()...
				)
			);
		}
	}

	template<typename Method>
	IAdaptor&& InvokeRValue(Method method, IAdaptor& adaptor, std::vector<Adaptor2> &args) {
		return(InvokeRValue(method, adaptor, args, std::index_sequence_for<Args...>{}));
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

	IAdaptor&& Invoke(IAdaptor& adaptor, std::vector<Adaptor2> &args) {
		return CMethodBase2<Class, Return, Args...>::InvokeLValue(m_method, adaptor, args);
	}
};



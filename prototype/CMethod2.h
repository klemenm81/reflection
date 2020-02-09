#pragma once

#include "Adaptor.h"
#include "IMethod2.h"

template <typename Class, typename Return, typename... Args>
class CMethodBase2 : public IMethod2 {
protected:
	template<typename Method, std::size_t... Index>
	void InvokeLValue(Method method, IAdaptor& retVal, IAdaptor& object, std::vector<Adaptor2> &&args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&>&>(object).GetValue().*method)(
				args[Index].Get<Args>()...
			);
		}
		else {
			static_cast<CAdaptor<Return>&>(retVal).SetValue(
				(static_cast<CAdaptor<Class&>&>(object).GetValue().*method)(
					args[Index].Get<Args>()...
				)
			);
		}
	}

	template<typename Method>
	void InvokeLValue(Method method, IAdaptor& retVal, IAdaptor& object, std::vector<Adaptor2> &&args) {
		return(InvokeLValue(method, retVal, object, std::move(args), std::index_sequence_for<Args...>{}));
	}

	template<typename Method, std::size_t... Index>
	void InvokeRValue(Method method, IAdaptor& retVal, IAdaptor& object, std::vector<Adaptor2> &&args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&&>&>(object).GetValue().*method)(
				args[Index].Get<Args>()...
			);
		}
		else {
			static_cast<CAdaptor<Return>&>(retVal).SetValue(
				(static_cast<CAdaptor<Class&&>&>(object).GetValue().*method)(
					args[Index].Get<Args>()...
				)
			);
		}
	}

	template<typename Method>
	void InvokeRValue(Method method, IAdaptor& retVal, IAdaptor& object, std::vector<Adaptor2> &&args) {
		return(InvokeRValue(method, retVal, object, args, std::index_sequence_for<Args...>{}));
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

	void Invoke(IAdaptor& retVal, IAdaptor& object, std::vector<Adaptor2> &&args) {
		return CMethodBase2<Class, Return, Args...>::InvokeLValue(m_method, retVal, object, std::move(args));
	}
};



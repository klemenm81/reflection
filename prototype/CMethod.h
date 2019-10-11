#pragma once

#include "Adaptor.h"
#include "IMethod.h"

template <typename Class, typename Return, typename... Args>
class CMethod : public IMethod {
private:
	Return(Class::*m_method)(Args...);
	Return(Class::*m_constMethod)(Args...) const;

	template<std::size_t... Index> inline
	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(m_method != nullptr) ?
				(static_cast<Class&>(obj).*m_method)(
					(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
				) :
				(static_cast<Class&>(obj).*m_constMethod)(
					(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
				);

			return *new CAdaptor<Return>();
		}
		else {
			return *new CAdaptor<Return>(
				(m_method != nullptr) ? 
					(static_cast<Class&>(obj).*m_method)(
						(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
					) :				
					(static_cast<Class&>(obj).*m_constMethod)(
						(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
					)
				);
		}
	}

public:
	CMethod(Return(Class::* method)(Args...)) : m_method(method), m_constMethod(nullptr) {
	}

	CMethod(Return(Class::* constMethod)(Args...) const) : m_method(nullptr), m_constMethod(constMethod) {
	}
	
	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return Invoke(obj, args, std::index_sequence_for<Args...>{});
	}
};

#pragma once

#include "Adaptor.h"
#include "IMethod.h"

template <typename Class, typename... Args>
class CMethod2 : public IMethod {
	void (Class::*m_method)(Args...);

public:
	CMethod2(void (Class::* method)(Args...)) : m_method(method) {
	}
	
	template<std::size_t... Index> inline
	IAdaptor& InvokeInternal(IReflectable& obj, std::vector<IAdaptor*> args, std::index_sequence<Index...>) {
		(static_cast<Class&>(obj).*m_method)(
			(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
		);
		
		return *new CAdaptor<void>();
	}
	
	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return InvokeInternal(obj, args, std::index_sequence_for<Args...>{});
	}
};

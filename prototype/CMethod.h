#pragma once

#include "Adaptor.h"
#include "IMethod.h"

template <typename Class, typename... Args>
class CMethod : public IMethod {
private:
	void (Class::*m_method)(Args...);

	template<std::size_t... Index> inline
	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args, std::index_sequence<Index...>) {
		(static_cast<Class&>(obj).*m_method)(
			(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
		);
		
		return *new CAdaptor<void>();
	}

public:
	CMethod(void (Class::* method)(Args...)) : m_method(method) {
	}
	
	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return Invoke(obj, args, std::index_sequence_for<Args...>{});
	}
};

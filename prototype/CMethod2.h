#pragma once

#include "Adaptor.h"
#include "IMethod.h"

template <typename Class, typename Arg1, typename Arg2>
class CMethod2 : public IMethod {
	void (Class::*m_method)(Arg1, Arg2);

public:
	CMethod2(void (Class::* method)(Arg1, Arg2)) : m_method(method) {

	}
	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		CAdaptor<Arg1>& adaptor1 = static_cast<CAdaptor<Arg1>&>(*args[0]);
		CAdaptor<Arg2>& adaptor2 = static_cast<CAdaptor<Arg2>&>(*args[1]);

		(static_cast<Class&>(obj).*m_method)(
			adaptor1.GetValue(), 
			adaptor2.GetValue()
		);

		return *new CAdaptor<void>();
	}
};

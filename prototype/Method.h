#pragma once

#include "Adaptor.h"
#include "IMethod.h"

class Method {
private:
	IMethod& m_method;
	std::vector<IAdaptor*> m_args;
	Adaptor m_retVal;

public:
	Method(IMethod& method) : m_method(method) {
	}

	template<typename Type>
	void PushArg(Type arg) {
		m_args.push_back(new CAdaptor<Type>(arg));
	}

	template<typename Type>
	Type GetArg(int iArg) {
		Adaptor adaptor = *m_args[iArg];
		return adaptor.Get<Type>();
	}

	template<typename Type>
	Type GetRetVal() {
		return m_retVal.Get<Type>();
	}

	void Invoke(IReflectable& obj) {
		m_retVal = m_method.Invoke(obj, m_args);
	}
};

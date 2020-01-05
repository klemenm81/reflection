#pragma once

#include "Adaptor.h"
#include "IMethod.h"

class Method {
private:
	IMethod& m_method;
	std::vector<IAdaptor*> m_args;
	Adaptor m_retVal;

public:
	explicit Method(IMethod& method) : m_method(method) {
	}

	~Method() {
		ClearArgs();
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

	template <typename Class>
	void Invoke(Class& obj) {
		Adaptor adaptor = *new CAdaptor<Class&>(obj);
		m_retVal = m_method.Invoke(adaptor, m_args);
	}

	template <typename Class>
	void Invoke(Class&& obj) {
		Adaptor adaptor = *new CAdaptor<Class&&>(std::forward<Class>(obj));
		m_retVal = m_method.Invoke(adaptor, m_args);
	}

	void ClearArgs() {
		for (auto it = m_args.begin(); it != m_args.end(); it++) {
			delete* it;
		}
		m_args.clear();
	}
};

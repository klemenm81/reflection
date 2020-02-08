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

	Method(Method&& other) noexcept : m_method(other.m_method) {
	}

	Method(const Method &) = delete;
	Method& operator=(const Method &) = delete;

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

	template <typename Return, typename Class, typename... Args>
	Return InvokeEasy(Class& obj, Args... args) {
		CAdaptor<Class&> adaptor(obj);

		std::vector<IAdaptor*> argsVec = {
			&CAdaptor<Args>(args)...
		};

		m_retVal = m_method.Invoke(adaptor, argsVec);

		if constexpr (!std::is_same<Return, void>()) {
			return m_retVal.Get<Return>();
		}
	}

	template <typename Class>
	void Invoke(Class&& obj) {
		Adaptor adaptor = *new CAdaptor<Class&&>(std::forward<Class>(obj));
		m_retVal = m_method.Invoke(adaptor, m_args);
	}

	template <typename Return, typename Class, typename... Args>
	Return InvokeEasy(Class&& obj, Args... args) {
		CAdaptor<Class&&> adaptor(std::forward<Class>(obj));

		std::vector<IAdaptor*> argsVec = {
			&CAdaptor<Args>(args)...
		};

		m_retVal = m_method.Invoke(adaptor, argsVec);

		if constexpr (!std::is_same<Return, void>()) {
			return m_retVal.Get<Return>();
		}
	}

	void ClearArgs() {
		for (auto it = m_args.begin(); it != m_args.end(); it++) {
			delete* it;
		}
		m_args.clear();
	}
};

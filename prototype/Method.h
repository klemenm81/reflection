#pragma once

#include "Adaptor.h"
#include "Adaptor2.h"
#include "IMethod.h"
#include "IMethod2.h"

#include <deque>



template <typename T>
void add_to_vector(std::vector<T>* vec) {}

template <typename T, typename... Args>
void add_to_vector(std::vector<T>* vec, T&& car, Args&&... cdr) {
	vec->push_back(std::forward<T>(car));
	add_to_vector<T>(vec, std::forward<Args>(cdr)...);
}

template <typename T, typename... Args>
std::vector<T> make_vector(Args&&... args) {
	std::vector<T> result;
	add_to_vector<T>(&result, std::forward<Args>(args)...);
	return result;
}



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
		CAdaptor<Return> retVal;

		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		method2.Invoke(retVal, adaptor, make_vector<Adaptor2> (CAdaptor<Args>(args)... ));

		if constexpr (!std::is_same<Return, void>()) {
			return(retVal.GetValue());
		}
	}

	template <typename Class>
	void Invoke(Class&& obj) {
		Adaptor adaptor = *new CAdaptor<Class&&>(std::forward<Class>(obj));
		m_retVal = m_method.Invoke(adaptor, m_args);
	}

	template <typename Return, typename Class, typename... Args>
	Return InvokeEasy(Class&& obj, Args... args) {
		ClearArgs();
		Adaptor adaptor = *new CAdaptor<Class&&>(std::forward<Class>(obj));
		PushArg<Args...>(args...);
		m_retVal = m_method.Invoke(adaptor, m_args);
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

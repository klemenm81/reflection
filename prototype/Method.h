#pragma once

#include "Adaptor.h"
#include "Adaptor2.h"
#include "IMethod.h"
#include "IMethod2.h"

template <typename T, typename... Args>
std::vector<T> make_vector(Args&&... args) {
	std::vector<T> result;
	int dummy[] = { (result.push_back(std::forward<Args>(args)), 0)... };
	return result;
}


class Method {
private:
	IMethod& m_method;

public:
	Method(IMethod& method) : m_method(method) {
	}

	Method(Method&& other) noexcept : m_method(other.m_method) {
	}

	Method(const Method &) = delete;
	Method& operator=(const Method &) = delete;

	template <typename Class>
	Adaptor Invoke(Class& obj, std::vector<IAdaptor *> &args) {
		CAdaptor<Class&> adaptor(obj);
		return(m_method.Invoke(adaptor, args));
	}

	template <typename Return, typename Class, typename... Args>
	Return InvokeEasy(Class& obj, Args... args) {
		std::byte retValBuffer[sizeof(CAdaptor<Return>)];
		CAdaptor<Class&> adaptor(obj);
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		IAdaptor &retVal = method2.Invoke(retValBuffer, adaptor, make_vector<Adaptor> (CAdaptor<Args>(args)... ));

		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return> &>(retVal).GetValue());
		}
	}
};

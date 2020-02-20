#pragma once

#include "StaticAdaptor.h"
#include "DynamicAdaptor.h"
#include "IMethod.h"
#include "IMethod2.h"

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
	StaticAdaptor Invoke(Class& obj, std::vector<IAdaptor *> &args) {
		CAdaptor<Class&> adaptor(obj);
		return(m_method.Invoke(adaptor, args));
	}
	
	StaticAdaptor InvokeNew(IReflectable& obj, std::vector<DynamicAdaptor> &args) {
		CAdaptor<IReflectable&> adaptor(obj);
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		IAdaptor& retVal = method2.Invoke(adaptor, BuildAdaptorVectorFromAdaptors(args).data());
		return(StaticAdaptor(retVal));
	}
	
	template <typename Return, typename... Args>
	Return InvokeNewInline(IReflectable& obj, Args... args) {
		CAdaptor<IReflectable&> adaptor(obj);
		IMethod2& method2 = static_cast<IMethod2&>(m_method);

		IAdaptor &retVal = method2.Invoke(adaptor, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return> &>(retVal).GetValue());
		}
	}

protected:
	template <typename... Adaptors>
	std::vector<IAdaptor *> BuildAdaptorVectorFromArgs(Adaptors&&... adaptors) {
		std::vector<IAdaptor *> result;
		if constexpr (sizeof...(Adaptors) > 0) {
			int dummy[] = { (result.push_back(&adaptors), 0)... };
		}
		return result;
	}

	std::vector<IAdaptor*> BuildAdaptorVectorFromAdaptors(const std::vector<DynamicAdaptor> &adaptors) {
		std::vector<IAdaptor*> result;
		for (auto it = adaptors.begin(); it != adaptors.end(); it++) {
			result.push_back(it->GetPtr());
		}
		return result;
	}
};

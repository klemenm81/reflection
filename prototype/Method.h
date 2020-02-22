#pragma once

#include "StaticAdaptor.h"
#include "DynamicAdaptor.h"
#include "IMethod.h"
#include "IMethod2.h"

class Method {
private:
	IMethod& m_method;
	std::vector<IAdaptor *> m_args;
	IAdaptor* m_retVal;

public:
	Method(IMethod& method) : m_method(method), m_retVal(nullptr) {
	}

	Method(Method&& other) noexcept : m_method(other.m_method), m_retVal(other.m_retVal) {
	}

	Method(const Method &) = delete;
	Method& operator=(const Method &) = delete;

	template <typename Type>
	void PushArg(Type value) {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		m_args.push_back(new(method2.GetArgBuffer(m_args.size())) CAdaptor<Type>(value));
	}

	template <typename Type>
	Type GetArg(int iArg) {
		CAdaptor<Type> *adaptor = static_cast<CAdaptor<Type>*>(m_args[iArg]);
		return adaptor->GetValue();
	}

	template <typename Type>
	Type GetRetVal() {
		CAdaptor<Type>* adaptor = static_cast<CAdaptor<Type>*>(m_retVal);
		return adaptor->GetValue();
	}

	void ClearArgs() {
		m_args.clear();
	}

	template <typename Class>
	StaticAdaptor Invoke(Class& obj, std::vector<IAdaptor *> &args) {
		CAdaptor<Class&> adaptor(obj);
		return(m_method.Invoke(adaptor, args));
	}
	
	void InvokeNew(Object& obj) {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		m_retVal = method2.Invoke(obj, m_args.data());		
	}

	void InvokeNew(const Object& obj) {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		m_retVal = method2.Invoke(obj, m_args.data());
	}
	
	template <typename Return, typename... Args>
	Return InvokeNewInline(Object& obj, Args... args) {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);

		IAdaptor *retVal = method2.Invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return> &>(*retVal).GetValue());
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
};

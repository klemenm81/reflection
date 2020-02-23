#pragma once

#include "StaticAdaptor.h"
#include "DynamicAdaptor.h"
#include "IMethod.h"
#include "IMethod2.h"
#include "IMethodOverloads.h"

class Method {
private:
	IMethodOverloads& m_method;
	std::vector<IAdaptor *> m_args;
	std::string m_argsSignature;
	std::string m_argsName;
	IAdaptor* m_retVal;

public:
	Method(IMethodOverloads& method) : m_method(method), m_retVal(nullptr) {
	}

	Method(Method&& other) noexcept : m_method(other.m_method), m_retVal(other.m_retVal) {
	}

	Method(const Method &) = delete;
	Method& operator=(const Method &) = delete;

	template <typename Type>
	void PushArg(Type value) {
		std::string argsSignature = m_argsSignature;
		std::string argsName = m_argsName;
		argsSignature += std::string(";") + std::to_string(typeid(Type).hash_code());
		argsName += std::string(";") + std::string(typeid(Type).name());
		IMethod2& method2 = m_method.GetMethodContainingSignature(argsSignature.c_str() + 1, argsName.c_str() + 1);
		m_args.push_back(new(method2.GetArgBuffer(m_args.size())) CAdaptor<Type>(value));
		m_argsSignature = argsSignature;
		m_argsName = argsName;
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
	/*
	const char* GetArgsSignature() {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		return method2.GetArgsSignature();
	}

	const char* GetArgsName() {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		return method2.GetArgsName();
	}

	const char* GetRetValSignature() {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		return method2.GetRetValSignature();
	}

	const char* GetRetValName() {
		IMethod2& method2 = static_cast<IMethod2&>(m_method);
		return method2.GetRetValName();
	}
	
	template <typename Class>
	StaticAdaptor Invoke(Class& obj, std::vector<IAdaptor *> &args) {
		CAdaptor<Class&> adaptor(obj);
		return(m_method.Invoke(adaptor, args));
	}
	*/
	void InvokeNew(Object& obj) {
		std::string argsSignature;
		std::string argsName;
		for (IAdaptor* arg : m_args) {
			argsSignature += std::string(";") + std::string(arg->GetSignature());
			argsName += std::string(";") + std::string(arg->GetName());
		}
		IMethod2& method2 = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		m_retVal = method2.Invoke(obj, m_args.data());		
	}

	void InvokeNew(const Object& obj) {
		std::string argsSignature;
		std::string argsName;
		for (IAdaptor* arg : m_args) {
			argsSignature += std::string(";") + std::string(arg->GetSignature());
			argsName += std::string(";") + std::string(arg->GetName());
		}

		IMethod2& method2 = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstLValueRef);
		m_retVal = method2.Invoke(obj, m_args.data());
	}
	
	template <typename Return, typename... Args>
	Return InvokeNewInline(Object& obj, Args... args) {
		std::string argsSignature = (sizeof...(Args) > 0) ?
			((std::string(";") + std::to_string(typeid(Args).hash_code())) + ...) :
			";";
		std::string argsName = (sizeof...(Args) > 0) ?
			((std::string(";") + std::string(typeid(Args).name())) + ...) :
			";";
		IMethod2& method2 = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = method2.Invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
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

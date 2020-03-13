#pragma once


#include "detail/IAdaptor.h"
#include "detail/IMethod.h"
#include "detail/CAdaptor.h"
#include "Object.h"
#include <vector>

class Method {
private:
	const IMethod& m_method;
	std::vector<IAdaptor *> m_args;
	std::string m_argsSignature;
	std::string m_argsName;
	IAdaptor* m_retVal;

public:
	Method(const IMethod& method) : m_method(method), m_retVal(nullptr) {
	}

	Method(Method&& other) noexcept : m_method(other.m_method), m_retVal(other.m_retVal) {
	}

	Method(const Method &) = delete;
	Method& operator=(const Method &) = delete;

	const char* GetName() {
		return m_method.GetName();
	}

	template <typename Type>
	void PushArg(Type value) {
		std::string argsSignature = m_argsSignature;
		std::string argsName = m_argsName;
		argsSignature += std::string(";") + std::to_string(typeid(Type).hash_code());
		argsName += std::string(";") + std::string(typeid(Type).name());
		const IMethodInvoker& methodInvoker = m_method.GetMethodContainingSignature(argsSignature.c_str() + 1, argsName.c_str() + 1);
		m_args.push_back(new(methodInvoker.GetArgBuffer(m_args.size())) CAdaptor<Type>(value));
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

	std::string GetArgsSignature() {
		std::string argsSignature;		
		if (m_args.empty()) {
			argsSignature = ";";
		}
		else {
			for (IAdaptor* arg : m_args) {
				argsSignature += std::string(";") + std::string(arg->GetSignature());
			}
		}
		return argsSignature;
	}

	std::string GetArgsName() {
		std::string argsName;
		if (m_args.empty()) {
			argsName = ";";
		}
		else {
			for (IAdaptor* arg : m_args) {
				argsName += std::string(";") + std::string(arg->GetName());
			}
		}
		return argsName;
	}

	void Invoke(Object& obj) {
		std::string argsSignature = GetArgsSignature();
		std::string argsName = GetArgsName();
		const IMethodInvoker& methodInvoker = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		m_retVal = methodInvoker.Invoke(obj, m_args.data());		
	}

	void Invoke(const Object& obj) {
		std::string argsSignature = GetArgsSignature();
		std::string argsName = GetArgsName();
		const IMethodInvoker& methodInvoker = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstLValueRef);
		m_retVal = methodInvoker.Invoke(obj, m_args.data());
	}

	void Invoke(Object&& obj) {
		std::string argsSignature = GetArgsSignature();
		std::string argsName = GetArgsName();
		const IMethodInvoker& methodInvoker = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, RValueRef);
		m_retVal = methodInvoker.Invoke(std::move(obj), m_args.data());
	}

	void Invoke(const Object&& obj) {
		std::string argsSignature = GetArgsSignature();
		std::string argsName = GetArgsName();
		const IMethodInvoker& methodInvoker = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstRValueRef);
		m_retVal = methodInvoker.Invoke(std::move(obj), m_args.data());
	}
	
	template <typename Return, typename... Args>
	Return InvokeInline(Object& obj, Args... args) {
		std::string argsSignature;
		if constexpr (sizeof...(Args) > 0) {
			argsSignature = ((std::string(";") + std::to_string(typeid(Args).hash_code())) + ...);
		}
		else {
			argsSignature = ";";
		}
		std::string argsName;
		if constexpr (sizeof...(Args) > 0) {
			argsName = ((std::string(";") + std::string(typeid(Args).name())) + ...);
		}
		else {
			argsName = ";";
		}
		const IMethodInvoker& methodInvoker = m_method.GetMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.Invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return> &>(*retVal).GetValue());
		}
	}

	std::string InvokeMarshalled(Object& obj, std::vector<std::string> args) {
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "\t";
		Json::Value jsonArgs;
		Json::Value jsonRetVal;

		int index = 0;
		for (std::string arg : args) {
			std::string errs;
			std::stringstream s(arg);
			Json::Value json;
			Json::parseFromStream(rbuilder, s, &json, &errs);
			jsonArgs.insert(index++, json);
		}

		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.GetMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->InvokeMarshalled(obj, jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}

		if (iMethod == nMethods) {
			throw;
		}

		return Json::writeString(wbuilder, jsonRetVal);
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

#pragma once


#include "detail/IAdaptor.h"
#include "detail/IMethod.h"
#include "detail/CAdaptor.h"
#include "exceptions/MethodsWithNArgumentsNotMatchingInputArguments.h"
#include "Object.h"
#include "TypeInfo.h"
#include <vector>

class Method {
private:
	const IMethod& m_method;
	std::vector<IAdaptor *> m_args;
	std::string m_argsSignature;
	std::string m_argsName;
	IAdaptor* m_retVal;

protected:
	template <typename... Adaptors>
	static std::vector<IAdaptor*> BuildAdaptorVectorFromArgs(Adaptors&&... adaptors) {
		std::vector<IAdaptor*> result;
		if constexpr (sizeof...(Adaptors) > 0) {
			int dummy[] = { (result.push_back(&adaptors), 0)... };
		}
		return result;
	}

	template <typename... Args>
	static std::string GetArgsSignature() {
		std::string argsSignature;
		if constexpr (sizeof...(Args) > 0) {
			argsSignature = ((std::string(";") + std::to_string(TypeInfo<Args>::getUniqueId())) + ...);
		}
		else {
			argsSignature = ";";
		}
		return argsSignature;
	}

	template <typename... Args>
	static std::string GetArgsName() {
		std::string argsName;
		if constexpr (sizeof...(Args) > 0) {
			argsName = ((std::string(";") + std::string(TypeInfo<Args>::getName())) + ...);
		}
		else {
			argsName = ";";
		}
		return argsName;
	}

	static Json::Value PackArgsToJson(std::vector<std::string> args) {
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		Json::Value jsonArgs;
		int index = 0;
		for (std::string arg : args) {
			std::string errs;
			std::stringstream s(arg);
			Json::Value json;
			Json::parseFromStream(rbuilder, s, &json, &errs);
			jsonArgs.insert(index++, json);
		}
		return jsonArgs;
	}

	static std::string UnpackRetvalFromJson(Json::Value jsonRetVal) {
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "\t";
		return Json::writeString(wbuilder, jsonRetVal);
	}

	std::string getArgsSignature() const {
		std::string argsSignature;
		if (m_args.empty()) {
			argsSignature = ";";
		}
		else {
			for (IAdaptor* arg : m_args) {
				argsSignature += std::string(";") + std::string(arg->getSignature());
			}
		}
		return argsSignature;
	}

	std::string getArgsName() const {
		std::string argsName;
		if (m_args.empty()) {
			argsName = ";";
		}
		else {
			for (IAdaptor* arg : m_args) {
				argsName += std::string(";") + std::string(arg->getName());
			}
		}
		return argsName;
	}

public:
	Method(const IMethod& method) : m_method(method), m_retVal(nullptr) {
	}

	Method(Method&& other) noexcept : m_method(other.m_method), m_retVal(nullptr) {
	}

	Method(const Method& other) : m_method(other.m_method), m_retVal(nullptr) {
	}

	Method& operator=(Method&&) = delete;
	Method& operator=(const Method &) = delete;

	const char* getName() const {
		return m_method.getName();
	}

	template <typename Type>
	void pushArg(Type value) {
		std::string argsSignature = m_argsSignature;
		std::string argsName = m_argsName;
		argsSignature += std::string(";") + std::to_string(TypeInfo<Type>::getUniqueId());
		argsName += std::string(";") + std::string(TypeInfo<Type>::getName());
		const IMethodInvoker& methodInvoker = m_method.getMethodContainingSignature(argsSignature.c_str() + 1, argsName.c_str() + 1);
		m_args.push_back(new(methodInvoker.getArgBuffer(m_args.size())) CAdaptor<Type>(value));
		m_argsSignature = argsSignature;
		m_argsName = argsName;
	}

	template <typename Type>
	Type getArg(int iArg) const {
		CAdaptor<Type> *adaptor = static_cast<CAdaptor<Type>*>(m_args[iArg]);
		return adaptor->getValue();
	}

	template <typename Type>
	Type getRetVal() const {
		CAdaptor<Type>* adaptor = static_cast<CAdaptor<Type>*>(m_retVal);
		return adaptor->getValue();
	}

	void clearArgs() {
		m_args.clear();
	}

	void invoke(Object& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		m_retVal = methodInvoker.invoke(obj, m_args.data());		
	}

	void invoke(const Object& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstLValueRef);
		m_retVal = methodInvoker.invoke(obj, m_args.data());
	}

	void invoke(volatile Object& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstLValueRef);
		m_retVal = methodInvoker.invoke(obj, m_args.data());
	}

	void invoke(const volatile Object& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstLValueRef);
		m_retVal = methodInvoker.invoke(obj, m_args.data());
	}

	void invoke(Object&& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, RValueRef);
		m_retVal = methodInvoker.invoke(std::move(obj), m_args.data());
	}

	void invoke(const Object&& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstRValueRef);
		m_retVal = methodInvoker.invoke(std::move(obj), m_args.data());
	}

	void invoke(volatile Object&& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstRValueRef);
		m_retVal = methodInvoker.invoke(std::move(obj), m_args.data());
	}

	void invoke(const volatile Object&& obj) {
		std::string argsSignature = getArgsSignature();
		std::string argsName = getArgsName();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, ConstRValueRef);
		m_retVal = methodInvoker.invoke(std::move(obj), m_args.data());
	}
	
	template <typename Return, typename... Args>
	Return invokeInline(Object& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return> &>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invokeInline(const Object& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invokeInline(volatile Object& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invokeInline(const volatile Object& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invokeInline(Object&& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(std::move(obj), BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invokeInline(const Object&& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(std::move(obj), BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invokeInline(volatile Object&& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(std::move(obj), BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invokeInline(const volatile Object&& obj, Args... args) const {
		std::string argsSignature = GetArgsSignature<Args...>();
		std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature.c_str() + 1, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(std::move(obj), BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	std::string invokeMarshalled(Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(obj, jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

	std::string invokeMarshalled(const Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(obj, jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

	std::string invokeMarshalled(volatile Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(obj, jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

	std::string invokeMarshalled(const volatile Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(obj, jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

	std::string invokeMarshalled(Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(std::move(obj), jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

	std::string invokeMarshalled(const Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(std::move(obj), jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

	std::string invokeMarshalled(volatile Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(std::move(obj), jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

	std::string invokeMarshalled(const volatile Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeMarshalled(std::move(obj), jsonArgs);
				break;
			}
			catch (Exception&) {
				continue;
			}
		}
		if (iMethod == nMethods) {
			throw MethodsWithNArgumentsNotMatchingInputArguments(args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

};

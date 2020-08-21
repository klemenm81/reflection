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

protected:
	template <typename... Adaptors>
	static std::vector<IAdaptor*> BuildAdaptorVectorFromArgs(Adaptors&&... adaptors) {
		std::vector<IAdaptor*> result;
		if constexpr (sizeof...(Adaptors) > 0) {
			int dummy[] = { (result.push_back(&adaptors), 0)... };
		}
		return result;
	}

	template <typename... Args, size_t... Index>
	static size_t GetArgsSignature(std::index_sequence<Index...>) {
		if constexpr (sizeof...(Args) > 0) {
			static const size_t argsSignature = ((TypeInfo<Args>::getUniqueId() << Index) + ...);
			return argsSignature;
		}
		else {
			 return 0;
		}
	}

	template <typename... Args>
	static std::string GetArgsName() {
		if constexpr (sizeof...(Args) > 0) {
			static const std::string argsName = ((std::string(";") + std::string(TypeInfo<Args>::getName())) + ...);
			return argsName;
		}
		else {
			static const std::string argsName = ";";
			return argsName;
		}
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

public:
	Method(const IMethod& method) : m_method(method) {
	}

	Method(Method&& other) noexcept : m_method(other.m_method) {
	}

	Method(const Method& other) : m_method(other.m_method) {
	}

	Method& operator=(Method&&) = delete;
	Method& operator=(const Method &) = delete;

	const char* getName() const {
		return m_method.getName();
	}

	template <typename Return, typename... Args>
	Return invoke(Object& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return> &>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invoke(const Object& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invoke(volatile Object& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invoke(const volatile Object& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invoke(Object&& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(std::move(obj), BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invoke(const Object&& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(std::move(obj), BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invoke(volatile Object&& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
		IAdaptor* retVal = methodInvoker.invoke(std::move(obj), BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename... Args>
	Return invoke(const volatile Object&& obj, Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = m_method.getMethod(argsSignature, argsName.c_str() + 1, LValueRef);
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
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
			throw MethodsWithNArgumentsNotMatchingInputArguments(m_method.getName(), args.size());
		}
		return UnpackRetvalFromJson(jsonRetVal);
	}

};

#pragma once


#include "detail/IAdaptor.h"
#include "detail/IMethod.h"
#include "detail/CAdaptor.h"
#include "exceptions/MethodsWithNArgumentsNotMatchingInputArguments.h"
#include "Object.h"
#include "TypeInfo.h"
#include <vector>
#include <array>

class Method {
private:
	const IMethod& m_method;

protected:
	template <size_t... Index, typename... Adaptors>
	static std::array<IAdaptor*, sizeof...(Adaptors)> BuildAdaptorArrayFromArgs(
		std::index_sequence<Index...>,
		Adaptors&&... adaptors
	) {
		if constexpr (sizeof...(Adaptors) > 0) {
			std::array<IAdaptor*, sizeof...(Adaptors)> result = { &adaptors... };
			return result;
		}
		else {
			std::array<IAdaptor*, 0> result = { };
			return result;
		}
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
			if (!Json::parseFromStream(rbuilder, s, &json, &errs)) {
				arg = "\"" + arg + "\"";
				std::stringstream s(arg);
				Json::parseFromStream(rbuilder, s, &json, &errs);
			}
			jsonArgs.insert(index++, json);
		}
		return jsonArgs;
	}

	static std::string UnpackRetvalFromJson(Json::Value jsonRetVal) {
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "\t";
		return Json::writeString(wbuilder, jsonRetVal);
	}

	template <typename Return>
	static Return InvokeReturn(IAdaptor* retVal) {
		if constexpr (!std::is_same<Return, void>()) {
			return(static_cast<CAdaptor<Return>&>(*retVal).getValue());
		}
	}

	template <typename Return, typename ObjectReference, typename... Args>
	static IAdaptor* Invoke(const IMethod& method, Qualifier q, ObjectReference obj, Args... args) {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IMethodInvoker& methodInvoker = method.getMethod(argsSignature, argsName.c_str() + 1, q);
		IAdaptor* retVal = methodInvoker.invoke(obj, BuildAdaptorArrayFromArgs(
			std::index_sequence_for<Args...>{},
			CAdaptor<Args>(args)...
		).data());
		return retVal;		
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
		IAdaptor *retVal = Invoke<Return, Object&, Args...>(m_method, LValueRef, obj, args...);
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&, Args...>(m_method, ConstLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&, Args...>(m_method, VolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			throw;
		}
		return InvokeReturn<Return>(retVal);
	}

	template <typename Return, typename... Args>
	Return invoke(const Object& obj, Args... args) const {
		IAdaptor* retVal = Invoke<Return, const Object&, Args...>(m_method, ConstLValueRef, obj, args...);
		if (retVal == nullptr) {
			retVal = Invoke<Return, const Object&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			throw;
		}
		return InvokeReturn<Return>(retVal);
	}

	template <typename Return, typename... Args>
	Return invoke(volatile Object& obj, Args... args) const {
		IAdaptor* retVal = Invoke<Return, volatile Object&, Args...>(m_method, VolatileLValueRef, obj, args...);
		if (retVal == nullptr) {
			retVal = Invoke<Return, volatile Object&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			throw;
		}
		return InvokeReturn<Return>(retVal);
	}

	template <typename Return, typename... Args>
	Return invoke(const volatile Object& obj, Args... args) const {
		IAdaptor* retVal = Invoke<Return, const volatile Object&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		if (retVal == nullptr) {
			throw;
		}
		return InvokeReturn<Return>(retVal);
	}

	template <typename Return, typename... Args>
	Return invoke(Object&& obj, Args... args) const {
		IAdaptor* retVal = Invoke<Return, Object&&, Args...>(m_method, RValueRef, obj, args...);
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&&, Args...>(m_method, ConstRValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&&, Args...>(m_method, VolatileRValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&&, Args...>(m_method, ConstVolatileRValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&&, Args...>(m_method, LValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&&, Args...>(m_method, ConstLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&&, Args...>(m_method, VolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, Object&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			throw;
		}
		return InvokeReturn<Return>(retVal);
	}

	template <typename Return, typename... Args>
	Return invoke(const Object&& obj, Args... args) const {
		IAdaptor* retVal = Invoke<Return, const Object&&, Args...>(m_method, ConstRValueRef, obj, args...);
		if (retVal == nullptr) {
			retVal = Invoke<Return, const Object&&, Args...>(m_method, ConstVolatileRValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, const Object&&, Args...>(m_method, ConstLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, const Object&&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			throw;
		}
		return InvokeReturn<Return>(retVal);
	}

	template <typename Return, typename... Args>
	Return invoke(volatile Object&& obj, Args... args) const {
		IAdaptor* retVal = Invoke<Return, volatile Object&&, Args...>(m_method, VolatileRValueRef, obj, args...);
		if (retVal == nullptr) {
			retVal = Invoke<Return, volatile Object&&, Args...>(m_method, ConstVolatileRValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, volatile Object&&, Args...>(m_method, VolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			retVal = Invoke<Return, volatile Object&&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			throw;
		}
		return InvokeReturn<Return>(retVal);
	}

	template <typename Return, typename... Args>
	Return invoke(const volatile Object&& obj, Args... args) const {
		IAdaptor* retVal = Invoke<Return, const volatile Object&&, Args...>(m_method, ConstVolatileRValueRef, obj, args...);
		if (retVal == nullptr) {
			retVal = Invoke<Return, const volatile Object&&, Args...>(m_method, ConstVolatileLValueRef, obj, args...);
		}
		if (retVal == nullptr) {
			throw;
		}

		return InvokeReturn<Return>(retVal);
	}

	std::string invokeSerialized(Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(obj, jsonArgs);
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

	std::string invokeSerialized(const Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(obj, jsonArgs);
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

	std::string invokeSerialized(volatile Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(obj, jsonArgs);
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

	std::string invokeSerialized(const volatile Object& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(obj, jsonArgs);
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

	std::string invokeSerialized(Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(std::move(obj), jsonArgs);
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

	std::string invokeSerialized(const Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(std::move(obj), jsonArgs);
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

	std::string invokeSerialized(volatile Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(std::move(obj), jsonArgs);
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

	std::string invokeSerialized(const volatile Object&& obj, std::vector<std::string> args) const {
		Json::Value jsonArgs = PackArgsToJson(args);
		Json::Value jsonRetVal;
		size_t nMethods = 0;
		size_t iMethod = 0;
		IMethodInvoker* const* methods = m_method.getMethodsByNArgs(args.size(), nMethods);
		for (iMethod = 0; iMethod < nMethods; iMethod++) {
			try {
				jsonRetVal = methods[iMethod]->invokeSerialized(std::move(obj), jsonArgs);
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

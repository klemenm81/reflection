#pragma once

#include "detail/IClass.h"
#include "Field.h"
#include "Method.h"
#include "Object.h"

class Class {
private:
	const IClass& m_class;

protected:
	template <typename... Adaptors>
	std::vector<IAdaptor*> BuildAdaptorVectorFromArgs(Adaptors&&... adaptors) {
		std::vector<IAdaptor*> result;
		if constexpr (sizeof...(Adaptors) > 0) {
			int dummy[] = { (result.push_back(&adaptors), 0)... };
		}
		return result;
	}

public:
	Class(const IClass& clasz) : m_class(clasz) {
	}

	Class(Class&& other) noexcept : m_class(other.m_class) {
	}

	Class(const Class& other) = delete;
	Class& operator=(const Class& other) = delete;

	const char* GetName() {
		return m_class.GetName();
	}

	template <typename Cast>
	Cast& Upcast(Object& obj) {
		const ICast& cast = m_class.GetCast(std::to_string(typeid(Cast).hash_code()).c_str(), typeid(Cast).name());
		CAdaptor<Cast&> *adaptor = static_cast<CAdaptor<Cast&> *>(cast.CastClass(obj));
		return adaptor->GetValue();
	}

	Field GetField(const char *name) {
		return m_class.GetField(name);
	}

	std::vector<Field> GetFields() {
		std::vector<Field> ret;
		size_t nFields = 0;
		const IField* const * fields = m_class.GetFields(nFields);
		for (size_t iField = 0; iField < nFields; iField++) {
			ret.push_back(Field(*fields[iField]));
		}
		return ret;
	}

	Method GetMethod(const char *name) {
		return m_class.GetMethod(name);
	}

	std::vector<Method> GetMethods() {
		std::vector<Method> ret;
		size_t nMethods = 0;
		IMethod* const* methods = m_class.GetMethods(nMethods);
		for (size_t iMethod = 0; iMethod < nMethods; iMethod++) {
			ret.push_back(Method(*methods[iMethod]));
		}
		return ret;
	}

	template <typename... Args>
	Object& NewInstance(Args... args) {
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
		const IConstructor& constructor = m_class.GetConstructor(argsSignature.c_str() + 1, argsName.c_str() + 1);
		return constructor.NewInstance(BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
	}
};

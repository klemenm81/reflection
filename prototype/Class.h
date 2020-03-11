#pragma once

#include "IClass.h"
#include "Field.h"
#include "Method.h"
#include "Object.h"

class Class {
private:
	IClass& m_class;

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
	Class(IClass& clasz) : m_class(clasz) {
	}

	Class(Class&& other) noexcept : m_class(other.m_class) {
	}

	Class& operator=(Class&& other) noexcept {
		m_class = other.m_class;
		return *this;
	}

	Class(const Class& other) : m_class(other.m_class) {
	}

	Class& operator=(const Class& other) {
		m_class = other.m_class;
		return *this;
	}

	template <typename Cast>
	Cast& Query(Object& obj) {
		ICast& cast = m_class.GetCast(std::to_string(typeid(Cast).hash_code()).c_str());
		CAdaptor<Cast&> *adaptor = static_cast<CAdaptor<Cast&> *>(cast.CastClass(obj));
		return adaptor->GetValue();
	}

	Field GetField(const char *name) {
		return m_class.GetField(name);
	}

	std::vector<Field> GetFields() {
		std::vector<Field> ret;
		size_t nFields = 0;
		IField** fields = m_class.GetFields(nFields);
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
		IMethod** methods = m_class.GetMethods(nMethods);
		for (size_t iMethod = 0; iMethod < nMethods; iMethod++) {
			ret.push_back(Method(*methods[iMethod]));
		}
		return ret;
	}

	template <typename... Args>
	Object& Instantiate(Args... args) {
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
		IConstructor& constructor = m_class.GetConstructor(argsSignature.c_str() + 1, argsName.c_str() + 1);
		return constructor.Instantiate(BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
	}
};

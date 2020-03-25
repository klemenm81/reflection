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
	static std::vector<IAdaptor*> BuildAdaptorVectorFromArgs(Adaptors&&... adaptors) {
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

	Class(const Class& other) : m_class(other.m_class) {
	}

	Class& operator=(Class&& other) = delete;
	Class& operator=(const Class& other) = delete;

	const char* getName() const {
		return m_class.getName();
	}

	template <typename Cast>
	Cast& upcast(Object& obj) const {
		const ICast& cast = m_class.getCast(std::to_string(typeid(Cast).hash_code()).c_str(), typeid(Cast).name());
		CAdaptor<Cast&> *adaptor = static_cast<CAdaptor<Cast&> *>(cast.castClass(obj));
		return adaptor->getValue();
	}

	Field getField(const char *name) const {
		return m_class.getField(name);
	}

	std::vector<Field> getFields() const {
		std::vector<Field> ret;
		size_t nFields = 0;
		const IField* const * fields = m_class.getFields(nFields);
		for (size_t iField = 0; iField < nFields; iField++) {
			ret.push_back(Field(*fields[iField]));
		}
		return ret;
	}

	Method getMethod(const char *name) const {
		return m_class.getMethod(name);
	}

	std::vector<Method> getMethods() const {
		std::vector<Method> ret;
		size_t nMethods = 0;
		IMethod* const* methods = m_class.getMethods(nMethods);
		for (size_t iMethod = 0; iMethod < nMethods; iMethod++) {
			ret.push_back(Method(*methods[iMethod]));
		}
		return ret;
	}

	template <typename... Args>
	Object* newInstance(Args... args) const {
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
		const IConstructor& constructor = m_class.getConstructor(argsSignature.c_str() + 1, argsName.c_str() + 1);
		return &constructor.newInstance(BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data());
	}
};

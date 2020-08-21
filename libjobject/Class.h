#pragma once

#include "detail/IClass.h"
#include "Field.h"
#include "Method.h"
#include "Object.h"
#include "TypeInfo.h"

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

	template <typename... Args, size_t... Index>
	static size_t GetArgsSignature(std::index_sequence<Index...>) {
		if constexpr (sizeof...(Args) > 0) {
			static const size_t argsSignature = ((TypeInfo<Args>::getUniqueId()<<Index) + ...);
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
		const ICast& cast = m_class.getCast(std::to_string(TypeInfo<Cast>::getUniqueId()).c_str(), TypeInfo<Cast>::getName());
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
	std::unique_ptr<Object> newInstance(Args... args) const {
		static const size_t argsSignature = GetArgsSignature<Args...>(std::index_sequence_for<Args...>{});
		static const std::string argsName = GetArgsName<Args...>();
		const IConstructor& constructor = m_class.getConstructor(argsSignature, argsName.c_str() + 1);
		return std::unique_ptr<Object>(&constructor.newInstance(BuildAdaptorVectorFromArgs(CAdaptor<Args>(args)...).data()));
	}
};

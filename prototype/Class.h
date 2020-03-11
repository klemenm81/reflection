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

	Field GetField(const char *name) {
		return m_class.GetField(name);
	}

	Method GetMethod(const char *name) {
		return m_class.GetMethod(name);
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

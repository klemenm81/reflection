#pragma once

#include <string>
#include <vector>
#include <forward_list>
#include <typeinfo>
#include "IAdaptor.h"

#include "Serialization.h"

template <typename Type>
class CAdaptor : public IAdaptor {
private:
	Type m_value;

public:                                 
	CAdaptor(const Type &value) : m_value(value) {
	}
	
	CAdaptor(Type&& value) : m_value(std::move(value)) {
	}

	CAdaptor(Json::Value value) : m_value(Serialization<Type>::Deserialize(value)) {
	}
	
	const Type &GetValue() {
		return m_value;
	}

	const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(Type).hash_code());
		return signature.c_str();
	}

	const char* GetName() {
		static const std::string name = typeid(Type).name();
		return name.c_str();
	}

	Json::Value Serialize() {
		return Serialization<Type>::Serialize(m_value);
	}
};

template <typename Type>
class CAdaptor<Type&> : public IAdaptor {
private:
	Type &m_value;

public:
	CAdaptor(Type &value) : m_value(value) {
	}

	Type& GetValue() {
		return m_value;
	}

	const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(Type&).hash_code());
		return signature.c_str();
	}

	const char* GetName() {
		static const std::string name = typeid(Type&).name();
		return name.c_str();
	}

	Json::Value Serialize() {
		return Serialization<Type>::Serialize(m_value);
	}
};

template <typename Type>
class CAdaptor<Type &&> : public IAdaptor {
private:
	Type &&m_value;

public:
	CAdaptor(Type &&value) : m_value(std::forward<Type>(value)) {
	}

	Type && GetValue() {
		return std::forward<Type>(m_value);
	}

	const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(Type&&).hash_code());
		return signature.c_str();
	}

	const char* GetName() {
		static const std::string name = typeid(Type&&).name();
		return name.c_str();
	}

	Json::Value Serialize() {
		return Serialization<Type>::Serialize(m_value);
	}
};

template <>
class CAdaptor<void> : public IAdaptor {
public:
	CAdaptor() {
	}

	const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(void).hash_code());
		return signature.c_str();
	}

	const char* GetName() {
		static const std::string name = typeid(void).name();
		return name.c_str();
	}

	Json::Value Serialize() {
		return Json::Value("");
	}
};
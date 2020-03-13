#pragma once


#include "IAdaptor.h"
#include "../Serialization.h"
#include <string>
#include <vector>
#include <forward_list>
#include <typeinfo>

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

	const char* GetSignature() const {
		static const std::string signature = std::to_string(typeid(Type).hash_code());
		return signature.c_str();
	}

	const char* GetName() const {
		static const std::string name = typeid(Type).name();
		return name.c_str();
	}

	Json::Value Serialize() const {
		return Serialization<Type>::Serialize(m_value);
	}
};

template <typename Type>
class CAdaptor<Type*> : public IAdaptor {
private:
	Type* m_value;

public:
	CAdaptor(Type *value) : m_value(value) {
	}

	CAdaptor(Json::Value value) {
		throw;
	}

	Type *GetValue() {
		return m_value;
	}

	const char* GetSignature() const {
		static const std::string signature = std::to_string(typeid(Type*).hash_code());
		return signature.c_str();
	}

	const char* GetName() const {
		static const std::string name = typeid(Type*).name();
		return name.c_str();
	}

	Json::Value Serialize() const {
		return Serialization<Type>::Serialize(*m_value);
	}
};


template <typename Type>
class CAdaptor<Type&> : public IAdaptor {
private:
	Type *m_value;

public:
	CAdaptor(Type &value) : m_value(&value) {
	}

	CAdaptor(Json::Value value) {
		throw;
	}

	Type& GetValue() {
		return *m_value;
	}

	const char* GetSignature() const {
		static const std::string signature = std::to_string(typeid(Type&).hash_code());
		return signature.c_str();
	}

	const char* GetName() const {
		static const std::string name = typeid(Type&).name();
		return name.c_str();
	}

	Json::Value Serialize() const {
		return Serialization<Type>::Serialize(*m_value);
	}
};

template <typename Type>
class CAdaptor<Type &&> : public IAdaptor {
private:
	Type &&m_value;

public:
	CAdaptor(Type &&value) : m_value(std::forward<Type>(value)) {
	}

	CAdaptor(Json::Value value) : m_value(Serialization<Type>::Deserialize(value)) {
	}

	Type && GetValue() {
		return std::forward<Type>(m_value);
	}

	const char* GetSignature() const {
		static const std::string signature = std::to_string(typeid(Type&&).hash_code());
		return signature.c_str();
	}

	const char* GetName() const {
		static const std::string name = typeid(Type&&).name();
		return name.c_str();
	}

	Json::Value Serialize() const {
		return Serialization<Type>::Serialize(m_value);
	}
};

template <>
class CAdaptor<void> : public IAdaptor {
public:
	CAdaptor() {
	}

	const char* GetSignature() const {
		static const std::string signature = std::to_string(typeid(void).hash_code());
		return signature.c_str();
	}

	const char* GetName() const {
		static const std::string name = typeid(void).name();
		return name.c_str();
	}

	Json::Value Serialize() const {
		return Json::Value("");
	}
};
#pragma once


#include "IAdaptor.h"
#include "../Serialization.h"
#include "../exceptions/MarshallingException.h"
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

	Json::Value Marshall() const {
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
		throw MarshallingException("Pointers cannot be unmarshalled");;
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

	Json::Value Marshall() const {
		throw MarshallingException("Pointers cannot be marshalled");
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
		throw MarshallingException("LValue references cannot be unmarshalled");;
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
	
	Json::Value Marshall() const {
		throw MarshallingException("LValue references cannot be marshalled");
	}
};

template <typename Type>
class CAdaptor<Type &&> : public IAdaptor {
private:
	Type &&m_value;

public:
	CAdaptor(Type &&value) : m_value(std::forward<Type>(value)) {
	}

	CAdaptor(Json::Value value) {
		MarshallingException("RValue references cannot be unmarshalled");
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

	Json::Value Marshall() const {
		throw MarshallingException("RValue references cannot be marshalled");
	}
};

template <>
class CAdaptor<void> : public IAdaptor {
public:
	CAdaptor() {
	}

	CAdaptor(Json::Value value) {
	}

	const char* GetSignature() const {
		static const std::string signature = std::to_string(typeid(void).hash_code());
		return signature.c_str();
	}

	const char* GetName() const {
		static const std::string name = typeid(void).name();
		return name.c_str();
	}

	Json::Value Marshall() const {
		return Json::Value("");
	}
};
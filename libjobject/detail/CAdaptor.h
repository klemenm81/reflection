#pragma once


#include "IAdaptor.h"
#include "../Serialization.h"
#include "../exceptions/MarshallingException.h"
#include <optional>
#include <string>
#include <vector>
#include <forward_list>
#include <typeinfo>

template <typename Type>
class CAdaptor : public IAdaptor {
private:
	Type m_value;

public: 
	static Indirection GetIndirection() {
		return Value;
	}

	static bool IsOptional() {
		return false;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(Type).hash_code());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = typeid(Type).name();
		return name.c_str();
	}

	CAdaptor(const Type &value) : m_value(value) {
	}
	
	CAdaptor(Type&& value) : m_value(std::move(value)) {
	}

	CAdaptor(Json::Value value) : m_value(Serialization<Type>::Deserialize(value)) {
	}
	
	const Type &getValue() {
		return m_value;
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		return Serialization<Type>::Serialize(m_value);
	}
};

template <typename Type>
class CAdaptor<Type*> : public IAdaptor {
private:
	Type* m_value;

public:
	static Indirection GetIndirection() {
		return Pointer;
	}

	static bool IsOptional() {
		return false;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(Type*).hash_code());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = typeid(Type*).name();
		return name.c_str();
	}

	CAdaptor(Type *value) : m_value(value) {
	}

	CAdaptor(Json::Value value) {
		throw MarshallingException("Pointers cannot be unmarshalled");;
	}

	Type *getValue() {
		return m_value;
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		throw MarshallingException("Pointers cannot be marshalled");
	}
};


template <typename Type>
class CAdaptor<Type&> : public IAdaptor {
private:
	Type *m_value;

public:
	static Indirection GetIndirection() {
		return LValueReference;
	}

	static bool IsOptional() {
		return false;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(Type&).hash_code());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = typeid(Type&).name();
		return name.c_str();
	}

	CAdaptor(Type &value) : m_value(&value) {
	}

	CAdaptor(Json::Value value) {
		throw MarshallingException("LValue references cannot be unmarshalled");;
	}

	Type& getValue() {
		return *m_value;
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		throw MarshallingException("LValue references cannot be marshalled");
	}
};

template <typename Type>
class CAdaptor<Type &&> : public IAdaptor {
private:
	Type &&m_value;

public:
	static Indirection GetIndirection() {
		return RValueReference;
	}

	static bool IsOptional() {
		return false;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(Type&&).hash_code());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = typeid(Type&&).name();
		return name.c_str();
	}

	CAdaptor(Type &&value) : m_value(std::forward<Type>(value)) {
	}

	CAdaptor(Json::Value value) {
		MarshallingException("RValue references cannot be unmarshalled");
	}

	Type&& getValue() {
		return std::forward<Type>(m_value);
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		throw MarshallingException("RValue references cannot be marshalled");
	}
};

template <typename Type>
class CAdaptor<std::optional<Type>> : public IAdaptor {
private:
	std::optional<Type> m_value;

public:
	static Indirection GetIndirection() {
		return Value;
	}

	static bool IsOptional() {
		return true;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(std::optional<Type>).hash_code());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = typeid(std::optional<Type>).name();
		return name.c_str();
	}

	CAdaptor(const std::optional<Type>& value) : m_value(value) {
	}

	CAdaptor(std::optional<Type>&& value) : m_value(std::move(value)) {
	}

	CAdaptor(Json::Value value) : m_value(Serialization<std::optional<Type>>::Deserialize(value)) {
	}

	const std::optional<Type>& getValue() {
		return m_value;
	}

	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}

	Json::Value marshall() const {
		return Serialization<std::optional<Type>>::Serialize(m_value);
	}
};


template <>
class CAdaptor<void> : public IAdaptor {
public:
	static Indirection GetIndirection() {
		return Value;
	}

	static bool IsOptional() {
		return false;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(typeid(void).hash_code());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = typeid(void).name();
		return name.c_str();
	}

	CAdaptor() {
	}

	CAdaptor(Json::Value value) {
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		return Json::Value("");
	}
};